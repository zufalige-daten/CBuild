/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 3, 29 June 2007
 *
 * The following project "CBuild" is written by Gunraj Singh Mann and published under the GNU General
 * Public License.
 *
 * This file is "compandlink.cc".
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <boost/algorithm/string/replace.hpp>
#include <srcstruct.hh>
#include <cstdlib>
#include <main.hh>

using namespace std;

vector<string> compilelistget(vector<vector<string>> relates){
	vector<string> srccompilelist;
	for(vector<string> relate : relates){
		string object_file = replacerootdir(std::filesystem::relative(relate[0]).replace_extension(obj_ext).string(), obj_dir);
		if(std::filesystem::exists(object_file)){
			uint64_t lmodified_obj = std::filesystem::last_write_time(object_file).time_since_epoch().count();
			for(int i = 0; i < relate.size(); i++){
				uint64_t lmodified_src = std::filesystem::last_write_time(relate[i]).time_since_epoch().count();
				if(lmodified_src > lmodified_obj){
					srccompilelist.push_back(relate[0]);
					break;
				}
			}
		}
		else{
			srccompilelist.push_back(relate[0]);
		}
	}
	return srccompilelist;
}

void recompile(vector<string> srccompilelist){
	cout << "Building " << proj_name << " in directory " << proj_root << ".\n";
	for(string srccompileli : srccompilelist){
		for(profile prfl : profiles){
			if(std::filesystem::absolute(srccompileli).extension().string() == "." + prfl.src_ext){
				string compilecommand = boost::replace_all_copy(boost::replace_all_copy(boost::replace_all_copy(prfl.comp_cmd, "{input}", srccompileli), "{output}", replacerootdir(std::filesystem::relative(srccompileli).replace_extension(obj_ext).string(), obj_dir)), "{include}", inc_dir);
				int result = std::system(compilecommand.c_str());
				cout << " - " << srccompileli << " ";
				if(result == 0){
					cout << "[SUCCESS]\n";
				}
				else{
					cout << "[FAILURE] (program returned " << result << "; see above.)\n";
					exit(-1);
				}
				break;
			}
		}
	}
}

void relinkobj(vector<string> objfilelist){
	string ins = obj_dir + "/" + obj_main + " ";
	for(string objfile : objfilelist){
		if(objfile != obj_dir + "/" + obj_main)
			ins += objfile + " ";
	}
	ins.pop_back();
	string linkcommand = boost::replace_all_copy(boost::replace_all_copy(link_cmd, "{input}", ins), "{output}", bin_out);
	cout << linkcommand + "\n";
	cout << " - " << bin_out << " ";
	int result = std::system(linkcommand.c_str());
	if(result == 0){
		cout << "SUCCESS\n";
	}
	else{
		cout << "[FAILURE] (program returned " << result << "; see above.)\n";
		exit(-1);
	}
}
