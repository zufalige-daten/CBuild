#include <tuple>
#include <vector>
#include <string>

using namespace std;

class file_node{
public:
	string filename;
	bool is_modified;
	bool dependancies_updated;
	int this_index;
	vector<tuple<int, file_node *>> depends_on;
	vector<tuple<int, file_node *>> dependancies_of;

	file_node(int a_this_index, string a_filename, vector<tuple<int, file_node *>> a_depends_on){
		filename = a_filename;
		this_index = a_this_index;
		depends_on = a_depends_on;
		is_modified = false;
		dependancies_updated = false;
	}

	void update_dependancies(){
		if(dependancies_updated){
			return;
		}

		dependancies_updated = true;

		for(tuple<int, file_node *> dependancy : depends_on){
			get<1>(dependancy)->add_dependant(tuple(this_index, this));
		}
	}

	void add_dependant(tuple<int, file_node *> dependant){
		dependancies_of.push_back(dependant);
	}

	void mark_modified(){
		if(is_modified){
			return;
		}

		is_modified = true;

		for(tuple<int, file_node *> dependancy_of : dependancies_of){
			get<1>(dependancy_of)->mark_modified();
		}
	}
};

