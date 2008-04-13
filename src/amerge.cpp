#include "amerge.hpp"
#include <iostream>
#include <boost/foreach.hpp>
#include <boost/filesystem/fstream.hpp>
#include <cctype>
#include <algorithm>
#include <sstream>

#define foreach BOOST_FOREACH


using std::cout;
using std::endl;
using std::flush;

struct Merge {
	int num_files;
	int num_dirs;
	std::vector< fs::path > paths;
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
			scan_directory( status, dir_iter->path() );
		} else if ( fs::is_regular(dir_iter->path()) && has_valid_extension(dir_iter->path()) ) {
			status.num_files++;
			status.paths.push_back( dir_iter->path() );
		}
	}
}

void copy_and_rename( Merge &status, fs::path &out_dir, int start_number ) {
	std::string extension, number_str;
	foreach( fs::path src, status.paths ) {
		std::stringstream sstream;
		sstream.fill( '0' );
		sstream.width( 5 );
		extension = fs::extension( src );
		to_lower( extension );
		sstream << start_number;
		number_str = sstream.str();
		fs::path dest = out_dir / (number_str + extension);
		fs::copy_file( src, dest );
//		cout << src << ", " << dest  << ";" << endl; // for debugging purposes
		start_number++;
	}
}

int AMerge::perform_action_merge(){
	Merge status;
	cout << "Action MERGE selected" << endl;
	cout << "Scanning directories ..." << flush;
	foreach(std::string directory, _directories){
		scan_directory(status, directory);
	}
	cout << "done" << endl;
	cout << "Scanned " << status.num_files << " files in " << status.num_dirs << " directories" << endl;
	cout << "Sorting lexicographically ..." << flush;

	std::sort( status.paths.begin(), status.paths.end() );

	cout << "done" << endl;
	cout << "Beginning merge ..." << flush;
	// TODO Fortschrittsbalken
	
	// Check whether the output directory exists, is a directory, is empty and we have the proper rights
	{
	fs::ofstream out;
	out.open(_out_dir / "test" );
	if( !fs::exists(_out_dir) ) {
		fs::create_directory(_out_dir);
	} else if( !fs::is_directory(_out_dir) ) {
		std::cerr << "ERROR: " << _out_dir << " is not a directory" << endl;
		return 1;
	} else if( fs::is_empty(_out_dir) ) {
		std::cerr << "ERROR: " << _out_dir << " is not empty" << endl;
		return 1;
	} else if( !out.is_open() ) {
		std::cerr << "ERROR: We do not have proper rights for " << _out_dir << endl;
		return 1;
	}
	out.close();
	fs::remove( _out_dir / "test" ); 
	}

	copy_and_rename( status, _out_dir, _start_number );

	cout << "done" << endl;
	// TODO Achte auf Dateirechte
	return 0;
}

int AMerge::perform_action_defrag() {
	return 1;
}

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
		case MERGE:
			return perform_action_merge();
			break;
		case DEFRAG:
			return perform_action_defrag();
			break;
		default:
			break;
	}
	return 1;
}
