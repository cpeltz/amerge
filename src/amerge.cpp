#include "amerge.hpp"
#include <iostream>

#define foreach BOOST_FOREACH

using std::cout;
using std::endl;
using std::flush;

struct Merge{
	int num_files;
	int num_dirs;
	std::vector< std::string > paths;
	Merge():num_files(0),num_dirs(0){}
};

void scan_directory(Merge &status, std::string directory){
	std::vector< std::string > ls = get_directory(directory);
	foreach(std::string entry, ls){
		if(is_file(entry) && check_ending(entry)){
			status.num_files++;
			status.paths.push_back(directory + "/" + entry);
		} else if(is_dir(entry)) {
			scan_directory(status, directory + "/" + entry);
		}
	}
}

void AMerge::perform_action_merge(){
	Merge status;
	cout << "Action MERGE selected" << endl;
	cout << "Scanning directories ..." << flush;
	foreach(std::string directory, _directories){
		scan_directory(status, directory);
	}
}

void AMerge::perform_action_defrag(){}

AMerge::AMerge(std::vector< std::string > directories) : _directories(directories){}
AMerge::~AMerge(){}

void AMerge::perform_action(Action action){
	switch(action){
		case MERGE:
			perform_action_merge();
			break;
		case DEFRAG:
			perform_action_defrag();
			break;
		default:
			break;
	}
}
