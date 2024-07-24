#pragma once

#include <string>
#include <vector>

using namespace std;

extern vector<string> compilelistget(vector<vector<string>> relates);
extern void recompile(vector<string> srccompilelist);
extern void relinkobj(vector<string> objfilelist);

