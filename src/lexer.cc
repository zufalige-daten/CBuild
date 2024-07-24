/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 3, 29 June 2007
 *
 * The following project "CBuild" is written by Gunraj Singh Mann and published under the GNU General
 * Public License.
 *
 * You may freely modify and distribute this source code and the binary executable file as long as you reference
 * "Gunraj Singh Mann" or "Zufalige Daten On Github" as the origional programmer of any source code taken from
 * this project or this project as a whole.
 *
 * This file is: "lexer.hpp".
*/

#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include <vector>
#include <lexer.hh>

using namespace std;

const int _EOF = -1;
const int _IDENT = 0;
const int _OP_EQ = 1;
const int _OP_OPEN = 2;
const int _OP_CLOSE = 3;
const int _STRING = 4;
const int _INCSTRING = 5;
const int _OP = 6;

vector<token> tokenize(string text){
	vector<token> ret;
	int index = 0;
	int ln = 1;
	while(index < (int)text.length()){
		if(text[index] == '\n'){
			ln++;
			index++;
		}
		else if(text[index] == ' ' || text[index] == '\t'){
			index++;
		}
		else if(text[index] == '/' && text[index + 1] == '/'){
			while(text[index] != '\n'){
				index++;
			}
		}
		else if(isalpha(text[index]) || text[index] == '_'){
			string text_z = "";
			while(isalnum(text[index]) || text[index] == '_'){
				text_z += text[index];
				index++;
			}
			ret.push_back({text_z, _IDENT, ln});
		}
		else if(text[index] == '='){
			ret.push_back({"=", _OP_EQ, ln});
			index++;
		}
		else if(text[index] == '{'){
			ret.push_back({"{", _OP_OPEN, ln});
			index++;
		}
		else if(text[index] == '}'){
			ret.push_back({"}", _OP_CLOSE, ln});
			index++;
		}
		else if(text[index] == '\''){
			string text_z = "";
			index++;
			while(text[index] != '\''){
				text_z += text[index];
				index++;
			}
			index++;
			ret.push_back({text_z, _STRING, ln});
		}
		else if(text[index] == '"'){
			string text_z = "";
			index++;
			while(text[index] != '"'){
				text_z += text[index];
				index++;
			}
			index++;
			ret.push_back({text_z, _STRING, ln});
		}
		else if(text[index] == '<'){
			string text_z = "";
			index++;
			while(text[index] != '>'){
				text_z += text[index];
				index++;
			}
			index++;
			ret.push_back({text_z, _INCSTRING, ln});
		}
		else{
			ret.push_back({string(1, text[index]), _OP, ln});
			index++;
		}
	}
	ret.push_back({"", _EOF, ln});
	return ret;
}

