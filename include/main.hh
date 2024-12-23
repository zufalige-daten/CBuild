#pragma once

#include <string>
#include <vector>

using namespace std;

class profile{
public:
	string inc_type;
	string inc_local;
	string inc_lib;
	string inc_root;
	string src_ext;
	string comp_cmd;
	inline bool operator==(const profile &other){
		return inc_type == other.inc_type && inc_local == other.inc_local && inc_lib == other.inc_lib && inc_root == other.inc_root && src_ext == other.src_ext && comp_cmd == other.comp_cmd;
	}
};

extern string proj_name;
extern string proj_root;
extern string bin_out;
extern string src_dir;
extern string obj_dir;
extern string inc_dir;
extern string obj_ext;
extern string obj_main;
extern string link_cmd;
extern vector<profile> profiles;

#define VERNAME "ULTIMATUM"
#define VERNUM "4.0.0"

