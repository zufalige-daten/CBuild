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
 * This file is: "srcstruct.hpp".
*/

#include <vector>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <boost/algorithm/string/trim.hpp>
#include "msafe.hpp"
#include <sys/stat.h>
#include <unistd.h>

using namespace boost::algorithm;
using namespace std;

vector<string> getlistsrcfilesdir(){
	vector<string> ret;
	for (const auto & entry : filesystem::directory_iterator(src_dir)){
		string tmp = filesystem::path(entry).string();
		for(profile prfl : profiles){
			if(filesystem::path(tmp).extension().string() == "." + prfl.src_ext){
				ret.push_back(tmp);
			}
		}
	}
	return ret;
}

string reason = "";

bool matchagainst(string text, string format, string &output){
	text = trim_copy(text);
	int h = 0;
	bool success = true;
	int minimum_format_size = 0;
	for(int i = 0; i < format.size(); i++){
		if(format[i] == '%'){
			if(format[i] == '%'){
				minimum_format_size++;
			}
			else if(format[i] == 's'){
				minimum_format_size++;
			}
		}
		else{
			minimum_format_size++;
		}
	}
	if(text.size() < minimum_format_size){
		output = "";
		return false;
	}
	for(int i = 0; i < text.size(); i++){
		if(format[h] == '%'){
			h++;
			if(format[h] == '%'){
				if(text[i] != '%'){
					output = "";
					success = false;
					break;
				}
				h++;
			}
			else if(format[h] == 's'){
				h++;
				if(!isalpha(text[i]) && text[i] != '_' && text[i] != '.' && text[i] != '/' && text[i] != '\\'){
					output = "";
					success = false;
					break;
				}
				while(isalnum(text[i]) || text[i] == '_' || text[i] == '.' || text[i] == '/' || text[i] == '\\'){
					output += text[i];
					i++;
				}
				i--;
			}
		}
		else{
			if(text[i] != format[h]){
				output = "";
				success = false;
				break;
			}
			h++;
		}
	}
	return success;
}

vector<vector<string>> getsrcfilerecurrels(vector<string> srcfilelist, profile prfl){
	vector<vector<string>> ret;
	for(std::string srcfile : srcfilelist){
		vector<string> entry;
		entry.push_back(srcfile);
		ifstream infile(srcfile);
		string output;
		string line;
		while(getline(infile, line)){
			output = "";
			if(matchagainst(line, prfl.inc_lib, output) && prfl.inc_type.find("lib") != string::npos){
				entry.push_back(inc_dir + "/" + output);
				vector<string> list__;
				list__.push_back(inc_dir + "/" + output);
				vector<vector<string>> ret_temp = getsrcfilerecurrels(list__, prfl);
				ret_temp[0].erase(ret_temp[0].begin());
				for(string ret_temp_single : ret_temp[0]){
					entry.push_back(ret_temp_single);
				}
			}
			else if(matchagainst(line, prfl.inc_local, output) && prfl.inc_type.find("local") != string::npos){
				entry.push_back(src_dir + "/" + output);
				vector<string> list__;
				list__.push_back(src_dir + "/" + output);
				vector<vector<string>> ret_temp = getsrcfilerecurrels(list__, prfl);
				ret_temp[0].erase(ret_temp[0].begin());
				for(string ret_temp_single : ret_temp[0]){
					entry.push_back(ret_temp_single);
				}
			}
			else if(matchagainst(line, prfl.inc_root, output) && prfl.inc_type.find("root") != string::npos){
				entry.push_back(output);
				vector<string> list__;
				list__.push_back(output);
				vector<vector<string>> ret_temp = getsrcfilerecurrels(list__, prfl);
				ret_temp[0].erase(ret_temp[0].begin());
				for(string ret_temp_single : ret_temp[0]){
					entry.push_back(ret_temp_single);
				}
			}
		}
		infile.close();
		ret.push_back(entry);
	}
	return ret;
}
