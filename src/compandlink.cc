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
#include <sys/wait.h>
#include <thread>
#include <vector>
#include <boost/algorithm/string/replace.hpp>
#include <srcstruct.hh>
#include <cstdlib>
#include <main.hh>
#include <subprocess_handle.hh>

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

vector<vector<string>> splitting_workload(vector<string> workload, int job_count){
	vector<vector<string>> ret = vector<vector<string>>(job_count);
	int current_basic = 0;
	int max_complete = workload.size();
	for(int i = 0; i < max_complete; i++){
		ret[current_basic].push_back(workload[i]);
		current_basic++;
		if(current_basic == job_count){
			current_basic = 0;
		}
	}
	return ret;
}

vector<string> localstate;

int compile_list(void){
	for(string srccompileli : localstate){
		for(profile prfl : profiles){
			std::filesystem::path path0 = std::filesystem::absolute(srccompileli);
			string filename0 = path0.filename().string();
			string extension0 = string(std::find(filename0.begin(), filename0.end(), '.'), filename0.end());
			if(extension0 == "." + prfl.src_ext){
				string compilecommand = boost::replace_all_copy(boost::replace_all_copy(boost::replace_all_copy(prfl.comp_cmd, "{input}", srccompileli), "{output}", replacerootdir(std::filesystem::relative(srccompileli).replace_extension(obj_ext).string(), obj_dir)), "{include}", inc_dir);
				int result = std::system(compilecommand.c_str());
				if(result == 0){
					cout << " - " << srccompileli << " " << "[SUCCESS]\n";
				}
				else{
					cout << " - " << srccompileli << " " << "[FAILURE] (program returned " << result << "; see above.)\n";
					exit(-1);
				}
				break;
			}
		}
	}
	return 0;
}

void recompile(vector<string> srccompilelist){
	cout << "Building " << proj_name << " in directory " << proj_root << ".\n";
	int job_count = thread::hardware_concurrency();
	vector<vector<string>> split_workload = splitting_workload(srccompilelist, job_count);
// 	cout << "total dispatchable data collection [ ";
// 	for(int i = 0; i < srccompilelist.size(); i++){
// 		cout << srccompilelist[i] << ", ";
// 	}
// 	cout << "];\n";
// 	for(int i = 0; i < split_workload.size(); i++){
// 		cout << "dispatched worker thread " << i << " [ ";
// 		for(int j = 0; j < split_workload[i].size(); j++){
// 			cout << split_workload[i][j] << ", ";
// 		}
// 		cout << "];\n";
// 	}
// 	while(true);
	vector<int *> sharelist;
	vector<int> pids;
	for(int i = 0; i < job_count; i++){
		sharelist.push_back((int *)subprocess_create_share(sizeof(sharelist[0][0])));
		localstate = split_workload[i];
		pids.push_back(subprocess_start(compile_list));
	}
	for(int i = 0; i < job_count; i++){
		waitpid(pids[i], NULL, 0);
		subprocess_delete_share(sharelist[i], sizeof(sharelist[0][0]));
	}
}

void relinkobj(vector<string> objfilelist){
	string ins = "";
	for(string objfile : objfilelist){
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
