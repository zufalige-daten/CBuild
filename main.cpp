/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 3, 29 June 2007
 *
 * The following project "CBuild" is copywrighted by Gunraj Singh Mann and published under the GNU General
 * Public License.
 *
 * You may freely modify and distribute this source code and the binary executable file as long as you reference
 * "Gunraj Singh Mann" or "Zufalige Daten On Github" as the origional programmer of any source code taken from
 * this project or this project as a whole.
 *
 * This file is: "main.cpp".
*/

#define DEBUG true
#define DBG if(DEBUG){cout<<__LINE__<<": line number\n";fflush(stdout);}

#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
#include <source_location>

using namespace std;

typedef struct{
	string inc_type;
	string inc_local;
	string inc_lib;
	string inc_root;
	string src_ext;
	string comp_cmd;
} profile;

profile mprofile(string inc_type, string inc_local, string inc_lib, string inc_root, string src_ext, string comp_cmd){
	profile ret;
	ret.inc_type = inc_type;
	ret.inc_local = inc_local;
	ret.inc_lib = inc_lib;
	ret.inc_root = inc_root;
	ret.src_ext = src_ext;
	ret.comp_cmd = comp_cmd;
	return ret;
}

string proj_name = "";
string proj_root = "./";
string bin_out = "a.out";
string src_dir = "./";
string obj_dir = "./";
string inc_dir = "./";
string obj_ext = "o";
string obj_main = "main.o";
string link_cmd = "echo \"No object linker specified.\"";
vector<profile> profiles;

#include "compandlink.hpp"
#include "lexer.hpp"
#include "modchecks.hpp"
#include "msafe.hpp"
#include "parseapp.hpp"
#include "srcstruct.hpp"

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
	// parse arguments
	int argindex = 1;
	bool dobuild = true;
	for(; argindex < argc; argindex++){
		if(strcmp(argv[argindex], "-v") == 0){
			dobuild = false;
			cout << "cbuild: 3.0.0. vername: REWRITE001." << "\n";
		}
		else if(strcmp(argv[argindex], "-h") == 0){
			dobuild = false;
			cout << "cbuild [ARGUMENTS]" << "\n";
			cout << "[ARGUMENTS]" << "\n";
			cout << "\t-v: Display CBuild version." << "\n";
			cout << "\t-h: Display CBuild help." << "\n";
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
	// get the list of source files
	vector<string> srcfilelist = getlistsrcfilesdir();
	// for(string ent : srcfilelist){
	// 	cout << ent << "\n";
	// }
	// create a file relationship table with the source files and the headers / include files they use
	vector<vector<string>> urelates;
	vector<vector<string>> srcfilelist_arranged;
	for(profile prfl : profiles){
		vector<string> prfl_list;
		for(string srcfile : srcfilelist){
			if(filesystem::absolute(srcfile).extension().string() == "." + prfl.src_ext){
				prfl_list.push_back(srcfile);
			}
		}
		srcfilelist_arranged.push_back(prfl_list);
	}
	// for(vector<string> relate : srcfilelist_arranged){
	// 	for(string ent : relate){
	// 		cout << ent << "\n";
	// 	}
	// 	cout << "\n";
	// }
	for(int prfl_i = 0; prfl_i < profiles.size(); prfl_i++){
		vector<vector<string>> relates_temp = getsrcfilerecurrels(srcfilelist_arranged[prfl_i], profiles[prfl_i]);
		urelates.insert(urelates.end(), relates_temp.begin(), relates_temp.end());
	}
	vector<vector<string>> relates;
	
	// IMPORTANT filter
	for(int rel = 0; rel < urelates.size(); rel++){
		vector<string> tmp;
		
		for(int file = 0; file < urelates[rel].size(); file++){
			
			if(filesystem::exists(urelates[rel][file]) && !vhas(tmp, urelates[rel][file])){
				
				tmp.push_back(urelates[rel][file]);
				
			}
			
		}
		
		relates.push_back(tmp);
	}
	
	// for(vector<string> relate : relates){
		
	// 	cout << "Source File '" << relate[0] << "':\n";
		
	// 	relate.erase(relate.begin());
		
	// 	for(string rr : relate){
			
	// 		cout << "\t\tInclude '" << rr << "'\n";
			
	// 	}
		
	// }
	
	// create a list of source files that need to be compiled because their timespamps are greater than the timestamps of the object files relating to those source files or one of their dependancies changed
	vector<string> srccompilelist = compilelistget(relates);
	vector<string> objectfiles;
	for(string srcfileli : srcfilelist){
		objectfiles.push_back(obj_dir + "/" + filesystem::absolute(srcfileli).filename().replace_extension(obj_ext).string());
	}
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
