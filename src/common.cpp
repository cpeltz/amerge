#include "common.hpp"
#include <sys/stat.h>

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

void scan_directory( Stat &status, const fs::path &directory, int flags ) { 
	if( !fs::exists( directory ) ) {
		std::cerr << "ERROR: Directory " << directory << " does not exist!" << endl;
		return;
	}
	fs::directory_iterator dir_iter( directory ), end_iter;
	for( ; dir_iter != end_iter; dir_iter++ ){
		if( fs::is_directory(dir_iter->path()) ) {
			status.num_dirs++;
			if( flags & SCAN_MODE_RECURSIVE ) {
				scan_directory( status, dir_iter->path() );
			}
			if( flags & SCAN_MODE_DIRS ) {
				status.paths.push_back( dir_iter->path() );
			}
		} else if( fs::is_regular(dir_iter->path()) && (has_valid_extension(dir_iter->path()) || (SCAN_MODE_NOEXTENSION & flags)) ) {
			status.num_files++;
			if( flags & SCAN_MODE_FILES ) {
				status.paths.push_back( dir_iter->path() );
			}
		}
	}
}

void copy_and_rename( Stat &status, const fs::path &out_dir, int start_number ) {
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
		if( chmod( dest.file_string().c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH ) ) { // non-fatal permission error
			std::cerr << "ERROR: Could not set permissions for " << dest << endl;
		}
//		cout << src << ", " << dest  << ";" << endl; // for debugging purposes
		start_number++;
	}
}

int check_directory( const fs::path &dir, int flags ) {
	// Check whether the directory exists, is a directory, is empty and we have the proper rights
	fs::ofstream out;
	out.open( dir / "test" );	
	if( !fs::exists(dir) ) {
		if( flags & CHECK_CREATE ) {
			fs::create_directory(dir); 
		} else {
			std::cerr << "ERROR: " << dir << " does not exist" << endl;
			return 1;
		}
	} else if( flags & CHECK_CLEAR ) {
		Stat stat;
		scan_directory( stat, dir / ".amerge", SCAN_MODE_RECURSIVE | SCAN_MODE_DIRS | SCAN_MODE_FILES );
		stat.paths.push_back( dir / ".amerge" );
		std::sort( stat.paths.begin(), stat.paths.end() );
		remove( stat );
	} else if( !fs::is_directory(dir) ) {
		std::cerr << "ERROR: " << dir << " is not a directory" << endl;
		return 1;
	} else if( fs::is_empty(dir) ) {
		std::cerr << "ERROR: " << dir << " is not empty" << endl;
		return 1;
	} else if( !out.is_open() ) {
		std::cerr << "ERROR: We do not have proper rights for " << dir << endl;
		return 1;
	}
	out.close();
	fs::remove( dir / "test" ); 
	return 0;
}

void remove( Stat &status ) {
	foreach( fs::path entry, status.paths ) {
		fs::remove( entry );
	}
}
