#include "amerge.hpp"
#include <iostream>
#include <boost/filesystem.hpp>

#define foreach BOOST_FOREACH

namespace fs = boost::filesystem;

using std::cout;
using std::endl;
using std::flush;

struct Merge{
	int num_files;
	int num_dirs;
	std::vector< fs::path > paths;
	Merge():num_files(0),num_dirs(0){}
};

bool has_valid_extension( const fs::path &path ) {
	std::string ending = path.leaf();
	std::string::size_type pos = ending.find_last_of( '.' );
	ending = ending.substr( pos + 1 );
	ending.to_lower();
	foreach( const char* ext, extensions ) {
		if(endling == ext) {
			return true;
		}
	}
	return false;
}

void scan_directory(Merge &status, const fs::path &directory) { // TODO Implement exists(), is_directory(), is_file() and has_valid_extension()
	if( !exists(directory) ) {
		cout << "ERROR: Directory " << directory << " does not exist!" << endl;
		return;
	}
	fs::directory_iterator dir_iter = directory.begin(), end_iter;
	for( ; dir_iter != end_iter; dir_iter++ ){
		if( is_directory(*dir_iter) ) {
			status.num_dirs++;
			scan_directory( *dir_iter );
		} else if ( is_file(*dir_iter) && has_valid_extension(*dir_iter) ) {
			status.num_files++;
			status.paths.push_back( *dir_iter );
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
	cout << "done" << endl;
	cout << "Scanned " << status.num_files << " files in " << status.num_dirs << "directories" << endl;
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
