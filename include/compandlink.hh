#pragma once

#include <string>
#include <vector>

using namespace std;

struct job_export{
	int error;
	string stderr;
};

extern vector<string> compilelistget(vector<vector<string>> relates);
extern void recompile(vector<string> srccompilelist);
extern void relinkobj(vector<string> objfilelist);
extern void test_splitting_workload(void);

