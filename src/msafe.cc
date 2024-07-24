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
 * This file is: "msafe.hpp".
*/

#include <iostream>
#include <vector>
#include <msafe.hh>

using namespace std;

template <typename T>
vector<T> setv(vector<T> data, int index, T value, int line){
	if(index >= data.size()){
		cout << "ERROR: Vector at line: '" << line << "' out of bounds index '" << index << "' upper bounds: '" << data.size() << "'." << "\n";
		exit(-1);
	}
	data[index] = value;
	return data;
}

template <typename T>
T getv(vector<T> data, int index, int line){
	if(index >= data.size()){
		cout << "ERROR: Vector at line: '" << line << "' out of bounds index '" << index << "' upper bounds: '" << data.size() << "'." << "\n";
		exit(-1);
	}
	return data[index];
}

