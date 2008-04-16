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
			status.inc_dirs();
			if( flags & SCAN_MODE_RECURSIVE ) {
				scan_directory( status, dir_iter->path() );
			}
			if( flags & SCAN_MODE_DIRS ) {
				status.add( dir_iter->path() );
			}
		} else if( fs::is_regular(dir_iter->path()) && (has_valid_extension(dir_iter->path()) || (SCAN_MODE_NOEXTENSION & flags)) ) {
			status.inc_files();
			if( flags & SCAN_MODE_FILES ) {
				status.add( dir_iter->path() );
			}
		}
	}
}

void renumber( const Stat &status, const fs::path &out_dir, int start_number, int flags ) {
	std::string extension, number_str;
	foreach( fs::path src, status ) {
		std::stringstream sstream;
		sstream.fill( '0' );
		sstream.width( 5 );
		extension = fs::extension( src );
		to_lower( extension );
		sstream << start_number;
		number_str = sstream.str();
		fs::path dest = out_dir / (number_str + extension);

		if( flags & COPY )
			fs::copy_file( src, dest );
		else if( flags & MOVE )
			fs::rename( src, dest );

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
		scan_directory( stat, dir , SCAN_MODE_RECURSIVE | SCAN_MODE_DIRS | SCAN_MODE_FILES );
		stat.add( dir );
		std::sort( stat.begin(), stat.end() );
		remove( stat );
	} else if( !fs::is_directory(dir) ) {
		std::cerr << "ERROR: " << dir << " is not a directory" << endl;
		return 1;
	} else if( !fs::is_empty(dir) ) {
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

void remove( const Stat &status ) {
	foreach( fs::path entry, status ) {
		fs::remove( entry );
	}
}
