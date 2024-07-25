#pragma once

#include <string>
#include <vector>

using namespace std;

typedef struct{
	string inc_type;
	string inc_local;
	string inc_lib;
	string inc_root;
	string src_ext;
	string comp_cmd;
} profile;

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

