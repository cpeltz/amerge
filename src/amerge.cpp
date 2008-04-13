#include "common.hpp"

AMerge::AMerge(std::vector< std::string > directories) : _directories(directories){}
AMerge::~AMerge(){}

void AMerge::set_start_number(int start_number) {
	_start_number = start_number;
}

void AMerge::set_output_directory(std::string &out_dir) {
	_out_dir = out_dir;
}

int AMerge::perform_action(Action action){
	switch(action){
		case CREATE:
			return perform_action_create();
			break;
		case DEFRAG:
			return perform_action_defrag();
			break;
		default:
			break;
	}
	return 1;
}
