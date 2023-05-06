#pragma once

#ifndef THETA_HPP
#define THETA_HPP

#include <string>
#include <vector>
#include <sstream>

using namespace std;

string cts(char chr){
	string tmp_string;
	tmp_string.append(1, chr);
	return tmp_string;
}

bool isalphadigit(char chr){
	return isalpha(chr) || isdigit(chr);
}

bool iswhitespace(char chr){
	return chr == ' ' || chr == '\t';
}

vector<string> split(const string &s, char delim) {
	vector<string> result;
	stringstream ss (s);
	string item;

	while (getline (ss, item, delim)) {
		result.push_back (item);
	}
	return result;
}

namespace Theta {
	int line = 1;
	enum TokenType {
		FILEEND = -1,
		OP = 0,
		NEWLINE = 1,
		IDENT = 2,
		STRING = 5,
		CHAR = 6,
		INT = 7,
		FLOAT = 8,
		INDENT = 9,
		EXDENT = 10,
		INCSTRING = 11,
	};
	class Token {
	public:
		TokenType Type;
		string Text;
		int Line;
		Token(TokenType type, string text) {
			Line = line;
			Type = type;
			Text = text;
		}
	};
	vector<Token> Tokenize(string incode, string commentstr, bool iscomment2sz, bool usenewline,
	bool useindent) {
		line = 1;
		vector<Token> tokens;
		int i = 0;
		int oldindent = 0;
		int newindent = 0;
		while (true) {
			if (incode[i-1] == '\n' && useindent){
				// coming from newline
				newindent = 0;
				while(incode[i] == '\t' || incode[i] == ' '){
					if(incode[i] == '\t'){
						newindent += 4; // tab = 4 spaces
					}
					else{
						newindent++;
					}
					i++;
				}
				newindent /= 4;
				if(newindent > oldindent){
					for(int i = 0; i < newindent-oldindent; i++){
						tokens.push_back(Token(TokenType::INDENT, ""));
					}
				}
				else if(oldindent > newindent){
					for(int i = 0; i < oldindent-newindent; i++){
						tokens.push_back(Token(TokenType::EXDENT, ""));
					}
				}
				oldindent = newindent;
			}
			if (i >= incode.size()){
				break;
			}
			if (incode[i] == '\n'){
				if (usenewline){
					tokens.push_back(Token(TokenType::NEWLINE, "\n"));
				}
				i++;
				line++;
			}
			else if (incode[i] == commentstr[0] && !iscomment2sz){
				while (incode[i] != '\n'){
					i++;
				}
			}
			else if (incode[i] == commentstr[0] && incode[i+1] == commentstr[1] && iscomment2sz){
				while (incode[i] != '\n'){
					i++;
				}
			}
			else if (iswhitespace(incode[i])){
				i++;
			}
			else if (isalpha(incode[i])){
				string outstr = "";
				while (isalphadigit(incode[i]) || incode[i] == '_' || incode[i] == '.'){
					outstr += cts(incode[i]);
					i++;
				}
				tokens.push_back(Token(TokenType::IDENT, outstr));
			}
			else if (incode[i] == '0' && toupper(incode[i+1]) == 'X'){
				i += 2;
				string outstr = "0x";
				while (isxdigit(incode[i])){
					outstr += cts(incode[i]);
					i++;
				}
				tokens.push_back(Token(TokenType::INT, outstr));
			}
			else if (isdigit(incode[i])){
				string outstr = "";
				while (isdigit(incode[i])){
					outstr += cts(incode[i]);
					i++;
				}
				if (incode[i] == '.'){
					outstr += ".";
					i++;
					while (isdigit(incode[i])){
						outstr += cts(incode[i]);
						i++;
					}
					tokens.push_back(Token(TokenType::FLOAT, outstr));
				}
				else{
					tokens.push_back(Token(TokenType::INT, outstr));
				}
			}
			else if (incode[i] == '"'){
				i++;
				string outstr = "";
				while (incode[i] != '"' && i != incode.size()){
					outstr += cts(incode[i]);
					i++;
				}
				i++;
				tokens.push_back(Token(TokenType::STRING, outstr));
			}
			else if (incode[i] == '<'){
				i++;
				string outstr = "";
				while (incode[i] != '>' && i != incode.size()){
					outstr += cts(incode[i]);
					i++;
				}
				i++;
				tokens.push_back(Token(TokenType::INCSTRING, outstr));
			}
			else if ((incode[i] == '+' && (incode[i+1] == '+' || incode[i+1] == '=')) ||
				(incode[i] == '-' && (incode[i+1] == '-' || incode[i+1] == '=')) ||
				(incode[i] == '*' && incode[i+1] == '=') || (incode[i] == '/' && incode[i+1] == '=') ||
				(incode[i] == '=' && incode[i+1] == '=') ||
				(incode[i] == '|' && incode[i+1] == '|') ||
				(incode[i] == '&' && incode[i+1] == '&') ||
				(incode[i] == '<' && (incode[i+1] == '=' || incode[i+1] == '<')) ||
				(incode[i] == '>' && (incode[i+1] == '=' || incode[i+1] == '>'))){
				string outstr = cts(incode[i]) + cts(incode[i+1]);
				i += 2;
				tokens.push_back(Token(TokenType::OP, outstr));
			}
			else if (incode[i] == '\''){
				i++;
				string outstr = "";
				while (incode[i] != '\'' && i != incode.size()){
					outstr += cts(incode[i]);
					i++;
				}
				i++;
				tokens.push_back(Token(TokenType::CHAR, outstr));
			}
			else{
				tokens.push_back(Token(TokenType::OP, cts(incode[i])));
				i++;
			}
		}
		tokens.push_back(Token(TokenType::FILEEND, ""));
		return tokens;
	}
}

#endif
