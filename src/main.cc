/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 3, 29 June 2007
 *
 * The following project "CBuild" is written by Gunraj Singh Mann and published under the GNU General
 * Public License.
 *
 * This file is "main.cc".
*/

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
#include <filesystem>
#include <source_location>
#include <main.hh>

using namespace std;

string proj_name = "";
string proj_root = "./";
string bin_out = "bin/a.out";
string src_dir = "src";
string obj_dir = "bin/obj";
string inc_dir = "include";
string obj_ext = "o";
string obj_main = "main.o";
string link_cmd = "ld -o {output} {input}";
vector<profile> profiles = {
	{"root", "", "", "%%include \"%s\"", "asm", "nasm -felf$(getconf LONG_BIT) -o {output} {input}"},
	{"lib local", "#include \"%s\"", "#include <%s>", "", "c", "gcc -c -o {output} {input} -I {include} -masm=intel"},
	{"lib local", "#include \"%s\"", "#include <%s>", "", "cc", "g++ -c -o {output} {input} -I {include} -masm=intel"},
	{"lib local", "#include \"%s\"", "#include <%s>", "", "cpp", "g++ -c -o {output} {input} -I {include} -masm=intel"},
	{"lib local", ".include \"%s\"", ".include \"%s\"", "", "s", "gcc -c -o {output} {input} -I {include} -masm=intel"},
};

#include <boost/filesystem.hpp>

using namespace boost;

#include <compandlink.hh>
#include <lexer.hh>
#include <msafe.hh>
#include <parseapp.hh>
#include <srcstruct.hh>

template <typename T>
bool vhas(vector<T> invec, T has){
	for(int i = 0; i < invec.size(); i++){
		if(invec[i] == has){
			return true;
		}
	}
	return false;
}

int main(int argc, char **argv){
	/* test_splitting_workload();
	return 0; */
	// parse arguments
	int argindex = 1;
	bool dobuild = true;
	bool doyes = true;
	for(; argindex < argc; argindex++){
		if(strcmp(argv[argindex], "-v") == 0){
			dobuild = false;
			cout << "cbuild: " << VERNUM << ". vername: " << VERNAME << "." << "\n";
		}
		else if(strcmp(argv[argindex], "clean") == 0){
			doyes = false;
		}
		else if(strcmp(argv[argindex], "-h") == 0){
			dobuild = false;
			cout << "cbuild [ARGUMENTS]" << "\n";
			cout << "[ARGUMENTS]" << "\n";
			cout << "\t-v: Display CBuild version." << "\n";
			cout << "\t-h: Display CBuild help." << "\n";
			cout << "\tclean: Clear project object directory of all intermidiate compiler objects." << "\n";
		}
		else if(strcmp(argv[argindex], "init") == 0){
			dobuild = false;
			string tmp_proj_name = string(std::filesystem::current_path().filename().c_str());
			std::filesystem::create_directory("bin");
			std::filesystem::create_directory("src");
			std::filesystem::create_directory("bin/obj");
			std::filesystem::create_directory("include");
			string config_file_base =
"'" + tmp_proj_name + "'\n"
"\n"
"binary_out = 'bin/" + tmp_proj_name + "'\n"
"\n"
;
			ofstream config_file_out("cbuild.make");
			config_file_out << config_file_base;
			config_file_out.close();
			cout << "Created a new project of the current directory called '" + tmp_proj_name + "' successfully.\n";
		}
		else{
			cout << "ERROR: Invalid argument: " << argv[argindex] << "." << "\n";
			cout << "Type cbuild -h for help information." << "\n";
			return -1;
		}
	}
	// a dependancy as in an include file / a file included by a source file and also including files included by such dependancies
	// if the argument warrents so exit the program before compiling or linking
	if(!dobuild){
		return 0;
	}
	// open the config file
	string configfiletext = "";
	string line;
	ifstream configfile("cbuild.make");
	// display error and exit the program if the config file failed to open
	if(!configfile.is_open()){
		cout << "ERROR: Config file 'cbuild.make' not found or you do not have the permissions to read this file." << "\n";
		return -1;
	}
	while(getline(configfile, line)){
		configfiletext += line + "\n";
	}
	configfile.close();
	// tokenize the config file
	vector<token> configfiletoks = tokenize(configfiletext);
	// int oldline = 0;
	// for(token tok : configfiletoks){
	// 	if(oldline != tok.linenumber){
	// 		cout << "Line Number: " << tok.linenumber << "\n";
	// 	}
	// 	cout << tok.type << "\t\t\t\t" << tok.text << "\n";
	// 	oldline = tok.linenumber;
	// }
	// parse and apply the config file tokens (by apply it is ment to set the corresponding variables to the specified values)
	parseapp(configfiletoks);
	chdir(proj_root.c_str());
	if(!doyes){
		std::filesystem::remove_all(obj_dir);
		std::filesystem::create_directory(obj_dir);
		cout << "Cleared intermiediate." << "\n";
		return 0;
	}
	// get the list of source files
	vector<string> srcfilelist = getlistsrcfilesdir();
	for(string dirsq : object_dirs){
		std::filesystem::create_directories(obj_dir + "/" + dirsq);
	}
	// for(string ent : srcfilelist){
	// 	cout << ent << "\n";
	// }
	// create a file relationship table with the source files and the headers / include files they use
	vector<vector<string>> urelates;
	vector<vector<string>> srcfilelist_arranged;
	for(string srcfile : srcfilelist){
		vector<string> prfl_list;
		for(profile prfl : profiles){
			if(std::filesystem::absolute(srcfile).extension().string() == "." + prfl.src_ext){
				prfl_list.push_back(srcfile);
				break;
			}
		}
		srcfilelist_arranged.push_back(prfl_list);
	}
	// cout << "start:\n";
	// for(vector<string> relate : srcfilelist_arranged){
	// 	for(string ent : relate){
	// 		cout << ent << "\n";
	// 	}
	// 	cout << "\n";
	// }
	// while(1);
	for(int prfl_i = 0; prfl_i < profiles.size(); prfl_i++){
		vector<vector<string>> relates_temp = getsrcfilerecurrels(srcfilelist_arranged[prfl_i], profiles[prfl_i]);
		urelates.insert(urelates.end(), relates_temp.begin(), relates_temp.end());
	}
	vector<vector<string>> relates;
	
	// IMPORTANT filter
	for(int rel = 0; rel < urelates.size(); rel++){
		vector<string> tmp;
		
		for(int file = 0; file < urelates[rel].size(); file++){
			
			if(std::filesystem::exists(urelates[rel][file]) && !vhas(tmp, urelates[rel][file])){
				
				tmp.push_back(urelates[rel][file]);
				
			}
			
		}
		
		relates.push_back(tmp);
	}

	// filter relates
	relates.erase(unique(relates.begin(), relates.end()), relates.end());
	srcfilelist.erase(unique(srcfilelist.begin(), srcfilelist.end()), srcfilelist.end());
	
	// for(vector<string> relate : relates){
		
	// 	cout << "Source File '" << relate[0] << "':\n";
		
	// 	relate.erase(relate.begin());
		
	// 	for(string rr : relate){
			
	// 		cout << "\t\tInclude '" << rr << "'\n";
			
	// 	}
		
	// }

	// return 0;
	
	// create a list of source files that need to be compiled because their timespamps are greater than the timestamps of the object files relating to those source files or one of their dependancies changed
	vector<string> srccompilelist = compilelistget(relates);
	// for(string h : srccompilelist){
	// 	cout << h << "\n";
	// }
	// return 0;
	vector<string> objectfiles;
	for(string srcfileli : srcfilelist){
		objectfiles.push_back(replacerootdir(std::filesystem::relative(srcfileli).replace_extension(obj_ext).string(), obj_dir));
		// cout << replacerootdir(filesystem::relative(srcfileli).replace_extension(obj_ext).string(), obj_dir) << "\n";
	}
	// return 0;
	if(srccompilelist.size() == 0){
		cout << "cbuild - no changes.\n";
		return 0;
	}
	// recompile the source files that need to be compiled due to a changed modified date
	recompile(srccompilelist);
	// link the object files if any of the object files have been changed in the recompilation step
	relinkobj(objectfiles);
	return 0;
}
