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
 * This file is: "parseapp.hpp".
*/

/*
 * string proj_name = "";
 * string bin_out = "a.out";
 * string src_dir = "./";
 * string obj_dir = "./";
 * string inc_dir = "./";
 * string obj_ext = "o";
 * string obj_main = "main.o";
 * string link_cmd = "echo \"No object linker specified.\"";
 * vector<profile> profiles;
*/

// profile mprofile(string inc_type, string inc_local, string inc_lib, string src_ext, string comp_cmd)

#include <iostream>
#include <vector>
#include <string>

vector<token> configfiletoks;
int toki = 0;

void muststr(){
	if(configfiletoks[toki].type != _STRING){
		cout << "ERROR <config>: Expected a string at line " << configfiletoks[toki].linenumber << "." << "\n";
		exit(-1);
	}
}

void musteq(){
	if(configfiletoks[toki].type != _OP_EQ){
		cout << "ERROR <config>: Expected an equals at line " << configfiletoks[toki].linenumber << "." << "\n";
		exit(-1);
	}
	toki++;
}

void mustident(){
	if(configfiletoks[toki].type != _IDENT){
		cout << "ERROR <config>: Expected an identifier at line " << configfiletoks[toki].linenumber << "." << "\n";
		exit(-1);
	}
}

void parseapp(vector<token> configfiletoks_){
	configfiletoks = configfiletoks_;
	while(configfiletoks[toki].type != _EOF){
		if(configfiletoks[toki].type == _STRING){
			proj_name = configfiletoks[toki].text;
			toki++;
		}
		else if(configfiletoks[toki].type == _IDENT){
			string val = configfiletoks[toki].text;
			toki++;
			musteq();
			muststr();
			string text = configfiletoks[toki].text;
			int linenumber = configfiletoks[toki].linenumber;
			toki++;
			if(val == "project_root"){
				proj_root = text + "/";
			}
			else if(val == "binary_out"){
				bin_out = text;
			}
			else if(val == "source_dir"){
				src_dir = text;
			}
			else if(val == "object_dir"){
				obj_dir = text;
			}
			else if(val == "include_dir"){
				inc_dir = text;
			}
			else if(val == "object_ext"){
				obj_ext = text;
			}
			else if(val == "object_main"){
				obj_main = text;
			}
			else if(val == "linker_command"){
				link_cmd = text;
			}
			else{
				cout << "ERROR <config>: At line " << linenumber << " unknown configuration entry '" << val << "'." << "\n";
				exit(-1);
			}
		}
		else if(configfiletoks[toki].type == _OP_OPEN){
			toki++;
			string inc_type = "lib_local";
			string inc_local = "%s";
			string inc_lib = "%s";
			string inc_root = "%s";
			string src_ext = "txt";
			string com_cmd = "echo \"No source compiler specified.\"";
			while(configfiletoks[toki].type != _OP_CLOSE){
				mustident();
				string val = configfiletoks[toki].text;
				toki++;
				musteq();
				muststr();
				string text = configfiletoks[toki].text;
				int linenumber = configfiletoks[toki].linenumber;
				toki++;
				if(val == "include_type"){
					inc_type = text;
				}
				else if(val == "include_local"){
					inc_local = text;
				}
				else if(val == "include_lib"){
					inc_lib = text;
				}
				else if(val == "include_root"){
					inc_root = text;
				}
				else if(val == "source_ext"){
					src_ext = text;
				}
				else if(val == "compiler_command"){
					com_cmd = text;
				}
				else{
					cout << "ERROR <config>: At line " << linenumber << " unknown configuration entry '" << val << "'." << "\n";
					exit(-1);
				}
			}
			toki++;
			profiles.push_back(mprofile(inc_type, inc_local, inc_lib, inc_root, src_ext, com_cmd));
		}
		else{
			cout << "ERROR <config>: At line " << configfiletoks[toki].linenumber << " unknown configuration entry '" << configfiletoks[toki].text << "'." << "\n";
			exit(-1);
		}
	}
}

