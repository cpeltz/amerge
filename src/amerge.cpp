#include "amerge.hpp"
#include <iostream>
#include <boost/filesystem.hpp>
#include <cctype>
#include <algorithm>

#define foreach BOOST_FOREACH

namespace fs = boost::filesystem;

using std::cout;
using std::endl;
using std::flush;

struct Merge{
	int num_files;
	int num_dirs;
	std::vector< const fs::path > paths;
	Merge():num_files(0),num_dirs(0){}
};

char _lower( char ch ) {
	return static_cast<char>( std::tolower( (int) ch ) );
}

void to_lower( std::string &str ) {
	std::transform( str.begin(), str.end(), str.begin(), _lower );
}

bool has_valid_extension( const fs::path &path ) {
	std::string ending = fs::extension( path );
	to_lower( ending );
	foreach( const char* ext, extensions ) {
		if(ending == ext) {
			return true;
		}
	}
	return false;
}

void scan_directory(Merge &status, const fs::path &directory) { 
	if( !fs::exists( directory ) ) {
		cout << "ERROR: Directory " << directory << " does not exist!" << endl;
		return;
	}
	fs::directory_iterator dir_iter( directory ), end_iter;
	for( ; dir_iter != end_iter; dir_iter++ ){
		if( fs::is_directory(dir_iter->path()) ) {
			status.num_dirs++;
			scan_directory( dir_iter->path() );
		} else if ( fs::is_regular(dir_iter->path()) && has_valid_extension(dir_iter->path()) ) {
			status.num_files++;
			status.paths.push_back( dir_iter->path() );
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
	cout << "Beginning merge " << flush;
	// TODO Fortschrittsbalken
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
