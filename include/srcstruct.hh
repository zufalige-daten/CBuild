#pragma once

#include <vector>
#include <string>
#include <main.hh>

using namespace std;

extern vector<string> object_dirs;

extern vector<string> getlistsrcfilesdir();
string replacerootdir(string repath, string root_dir);
bool matchagainst(string text, string format, string &output);
vector<vector<string>> getsrcfilerecurrels(vector<string> srcfilelist, profile prfl);

