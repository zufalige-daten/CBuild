#pragma once

#include <string>
#include <vector>

using namespace std;

extern const int _EOF, _IDENT, _OP_EQ, _OP_OPEN, _OP_CLOSE, _STRING, _INCSTRING, _OP;

typedef struct{
	string text;
	int type;
	int linenumber;
} token;

extern vector<token> tokenize(string text);

