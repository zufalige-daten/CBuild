#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef WIN32
#include <unistd.h>
#endif
#ifdef WIN32
#define stat _stat
#endif

#include "Theta.hpp"

using namespace Theta;

vector<Token> toks;
int i = 0;

bool istype(TokenType type){
	return toks[i].Type == type;
}

bool isident(std::string text){
	return toks[i].Type == TokenType::IDENT && toks[i].Text == text;
}

bool isop(std::string text){
	return toks[i].Type == TokenType::OP && toks[i].Text == text;
}

void must(bool cond){
	if(!cond){
		std::cout << "cbuild - ERROR: config error" << std::endl;
	}
}

std::string src_dir;
std::string inc_dir;
std::string obj_dir;
std::string obj_main;
std::string out_bin;
std::string src_ext;
std::string obj_ext;
std::string inc_line_lib;
std::string inc_line_local;
std::string com_cmd;
std::string lin_cmd;

class fileentry{
public:
	std::string filename;
	std::vector<std::string> deps;
	public: fileentry(std::string fname, std::vector<std::string> ds){
		filename = fname;
		deps=ds;
	}
};

std::vector<fileentry> fileentries;

std::vector<std::string> listfiles(std::string dir){
	std::vector<std::string> ret;
	for(auto entry : std::filesystem::directory_iterator(dir)){
		ret.push_back(entry.path());
	}
	return ret;
}

bool ext_fname(std::string fname, std::string ext){
	ext = "." + ext;
	if(fname.length() < ext.length()){
		return false;
	}
	return fname.compare(fname.length() - ext.length(), ext.length(), ext) == 0;
}

bool endswith(std::string fname, std::string with){
	if(fname.length() < with.length()){
		return false;
	}
	return fname.compare(fname.length() - with.length(), with.length(), with) == 0;
}

std::string reverse_single_replace(std::string instr, std::string replace, std::string with){
	int rloc = instr.rfind(replace);
	char *retd = (char *)instr.c_str();
	retd[rloc] = 0;
	std::string ret = std::string(retd) + with;
	return ret;
}

std::string forward_single_replace(std::string instr, std::string replace, std::string with){
	int loc = instr.find(replace);
	std::string ret1 = instr;
	for(int x = 0; x < replace.length(); x++){
		ret1[loc+x] = ' ';
	}
	std::string ret2 = "";
	int x;
	for(x = 0; ret1[x] == ' '; x++);
	for(; x < ret1.length(); x++){
		ret2 += ret1[x];
	}
	std::string ret = with + ret2;
	return ret;
}

bool contains(std::vector<std::string> v, std::string x){
	return !(v.empty() || std::find(v.begin(), v.end(), x) == v.end());
}

int lmodtime(std::string fname){
	int stat_time = 0;
	struct stat resault;
	if(stat(fname.c_str(), &resault) == 0){
		stat_time = resault.st_mtime;
	}
	return stat_time;
}

std::string replaceall(std::string original, std::string from, std::string to){
	std::string results;
	std::string::const_iterator end = original.end();
	std::string::const_iterator current = original.begin();
	std::string::const_iterator next = std::search(current, end, from.begin(), from.end());
	while(next != end){
		results.append(current, next);
		results.append(to);
		current = next + from.size();
		next = std::search(current, end, from.begin(), from.end());
	}
	results.append(current, next);
	return results;
}

std::vector<std::string> get_src_deps(std::string src){
	std::vector<std::string> depsd;
	std::vector<Token> lib_pattern = Tokenize(inc_line_lib, "//", true, false, false);
	std::vector<Token> local_pattern = Tokenize(inc_line_local, "//", true, false, false);
	string line;
	ifstream filesrc(src);
	while(getline(filesrc, line)){
		std::vector<Token> line_pattern = Tokenize(line, "//", true, false, false);
		if(line_pattern.size() == lib_pattern.size()){
			bool status = true;
			std::string filenm_out;
			for(int m = 0; m < line_pattern.size(); m++){
				if(lib_pattern[m].Type == TokenType::INCSTRING &&
						line_pattern[m].Type == TokenType::INCSTRING){
					filenm_out = line_pattern[m].Text;
				}
				else{
					if((lib_pattern[m].Type != line_pattern[m].Type) ||
							(lib_pattern[m].Text != line_pattern[m].Text)){
						status = false;
					}
				}
			}
			if(status){
				if(std::filesystem::exists(inc_dir + filenm_out)){
					if(!contains(depsd, inc_dir + filenm_out)){
						depsd.push_back(inc_dir + filenm_out);
					}
					std::vector<std::string> sdeps = get_src_deps(inc_dir + filenm_out);
					for(std::string sdep : sdeps){
						if(!contains(depsd, sdep)){
							depsd.push_back(sdep);
						}
					}
				}
			}
		}
		if(line_pattern.size() == local_pattern.size()){
			bool status = true;
			std::string filenm_out;
			for(int m = 0; m < local_pattern.size(); m++){
				if(local_pattern[m].Type == TokenType::STRING &&
						line_pattern[m].Type == TokenType::STRING){
					filenm_out = line_pattern[m].Text;
				}
				else{
					if((local_pattern[m].Type != line_pattern[m].Type) ||
							(local_pattern[m].Text != line_pattern[m].Text)){
						status = false;
					}
				}
			}
			if(status){
				if(std::filesystem::exists(src_dir + filenm_out)){
					if(!contains(depsd, src_dir + filenm_out)){
						depsd.push_back(src_dir + filenm_out);
					}
					std::vector<std::string> sdeps = get_src_deps(src_dir + filenm_out);
					for(std::string sdep : sdeps){
						if(!contains(depsd, sdep)){
							depsd.push_back(sdep);
						}
					}
				}
			}
		}
	}
	return depsd;
}

int main(void){
	string config = "";
	string line;
	ifstream inconfig("cbuild.make");
	if(inconfig.is_open()){
		while(getline(inconfig, line)){
			config += line;
			config += "\n";
		}
	}
	else{
		std::cout << "cbuild - ERROR: 'cbuild.make' not found in project directory ./" << std::endl;
		return 0;
	}
	toks = Tokenize(config, "//", true, false, false);
	while(!istype(TokenType::FILEEND)){
		if(isident("source_dir")){
			i++;
			must(isop("="));
			i++;
			must(istype(TokenType::CHAR)||istype(TokenType::STRING));
			src_dir = toks[i].Text;
		}
		else if(isident("include_dir")){
			i++;
			must(isop("="));
			i++;
			must(istype(TokenType::CHAR)||istype(TokenType::STRING));
			inc_dir = toks[i].Text;
		}
		else if(isident("object_dir")){
			i++;
			must(isop("="));
			i++;
			must(istype(TokenType::CHAR)||istype(TokenType::STRING));
			obj_dir = toks[i].Text;
		}
		else if(isident("object_main")){
			i++;
			must(isop("="));
			i++;
			must(istype(TokenType::CHAR)||istype(TokenType::STRING));
			obj_main = toks[i].Text;
		}
		else if(isident("output_binary")){
			i++;
			must(isop("="));
			i++;
			must(istype(TokenType::CHAR)||istype(TokenType::STRING));
			out_bin = toks[i].Text;
		}
		else if(isident("source_ext")){
			i++;
			must(isop("="));
			i++;
			must(istype(TokenType::CHAR)||istype(TokenType::STRING));
			src_ext = toks[i].Text;
		}
		else if(isident("object_ext")){
			i++;
			must(isop("="));
			i++;
			must(istype(TokenType::CHAR)||istype(TokenType::STRING));
			obj_ext = toks[i].Text;
		}
		else if(isident("include_line_lib")){
			i++;
			must(isop("="));
			i++;
			must(istype(TokenType::CHAR)||istype(TokenType::STRING));
			inc_line_lib = toks[i].Text;
		}
		else if(isident("include_line_local")){
			i++;
			must(isop("="));
			i++;
			must(istype(TokenType::CHAR)||istype(TokenType::STRING));
			inc_line_local = toks[i].Text;
		}
		else if(isident("compiler_command")){
			i++;
			must(isop("="));
			i++;
			must(istype(TokenType::CHAR)||istype(TokenType::STRING));
			com_cmd = toks[i].Text;
		}
		else if(isident("linker_command")){
			i++;
			must(isop("="));
			i++;
			must(istype(TokenType::CHAR)||istype(TokenType::STRING));
			lin_cmd = toks[i].Text;
		}
		else{
			std::cout << "cbuild - ERROR: invalid parameter in config file '" << toks[i].Text << "'" << std::endl;
			return 0;
		}
		i++;
	}
	std::vector<std::string> srcfiles_init = listfiles(src_dir);
	std::vector<std::string> srcfiles;
	for(std::string srcfile : srcfiles_init){
		if(ext_fname(srcfile, src_ext)){
			srcfiles.push_back(srcfile);
		}
	}
	if(!endswith(src_dir, "/")){
		src_dir += "/";
	}
	if(!endswith(obj_dir, "/")){
		obj_dir += "/";
	}
	if(!endswith(inc_dir, "/")){
		inc_dir += "/";
	}
	std::vector<std::string> objfiles;
	for(std::string srcfile : srcfiles){
		objfiles.push_back(forward_single_replace(
					reverse_single_replace(srcfile, "." + src_ext, "." + obj_ext),
					src_dir, obj_dir));
	}
	for(int l = 0; l < objfiles.size(); l++){
		std::vector<std::string> deps;
		deps.push_back(srcfiles[l]);
		fileentries.push_back(fileentry(objfiles[l], deps));
	}
	for(int n = 0; n < srcfiles.size(); n++){
		std::vector<std::string> deps = get_src_deps(srcfiles[n]);
		for(int o = 0; o < deps.size(); o++){
			fileentries[n].deps.push_back(deps[o]);
		}
	}
	std::vector<std::string> tocom;
	for(fileentry entry : fileentries){
		int objlmodtime = lmodtime(entry.filename);
		for(int p = 0; p < entry.deps.size(); p++){
			int srclmodtime = lmodtime(entry.deps[p]);
			if(srclmodtime > objlmodtime && !contains(tocom, entry.deps[0])){
				tocom.push_back(entry.deps[0]);
			}
		}
	}
	if(tocom.size() == 0){
		std::cout << "cbuild - no changes." << std::endl;
		return 0;
	}
	for(std::string filenm : tocom){
		std::string inputd = filenm;
		std::string outputd = forward_single_replace(
				reverse_single_replace(filenm, "." + src_ext, "." + obj_ext),
				src_dir, obj_dir);
		std::string cmd = replaceall(replaceall(com_cmd, "{input}", inputd), "{output}", outputd);
		const char * cmd_sys = cmd.c_str();
		int status = system(cmd_sys);
		if(status == 0){
			std::cout << " - " << outputd << " [SUCCESS]" << std::endl;
		}
		else{
			std::cout << " - " << outputd << " [FAILED] (see above.)" << std::endl;
			return -1;
		}
	}
	std::string allobjs = obj_dir + obj_main + " ";
	for(fileentry entry : fileentries){
		if(entry.filename != obj_dir + obj_main){
			allobjs += entry.filename + " ";
		}
	}
	allobjs = reverse_single_replace(allobjs, " ", "");
	std::string cmd = replaceall(replaceall(lin_cmd, "{input}", allobjs), "{output}", out_bin);
	std::cout << cmd << "\n";
	const char * cmd_sys = cmd.c_str();
	int status = system(cmd_sys);
	if(status == 0){
		std::cout << " - " << out_bin << " [SUCCESS]" << std::endl;
	}
	else{
		std::cout << " - " << out_bin << " [FAILED] (see above.)" << std::endl;
		return -1;
	}
	return 0;
}
