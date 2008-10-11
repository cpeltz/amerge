#include "common.hpp"
#include <sys/stat.h>

/**
 * Helper function for to_lower
 * @param ch The character which should be converted to lowercase.
 * @return char The lowercase version of ch
 */
char _lower( char ch ) {
	return static_cast<char>( std::tolower( (int) ch ) );
}

/**
 * Converts a string directly to lowercase.
 *
 * @param str The string, which will be transformed to lowercase.
 * @return void
 */
void to_lower( std::string &str ) {
	std::transform( str.begin(), str.end(), str.begin(), _lower );
}

/**
 * Helperfunction for Stat::scan_directory. Checks whether or not the path has a valid extension (only files should be used)
 * @param path A Path to a file to be checked
 * @return bool TRUE if the File has a valid extension, FALSE otherwise
 */
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

void Stat::scan_directory( const fs::path &directory, int flags ) throw(RuntimeError*) {
	if( !fs::exists( directory ) ) {
		throw new RuntimeError("Directory " + directory.string() + " does not exist!" );
	}
	for( fs::directory_iterator dir_iter( directory ), end_iter; dir_iter != end_iter; dir_iter++ ) {
		if( fs::is_directory(dir_iter->path()) ) {
			inc_dirs();
			if( flags & SCAN_MODE_RECURSIVE ) {
				scan_directory( dir_iter->path(), flags );
			}
			if( flags & SCAN_MODE_DIRS ) {
				add( dir_iter->path() );
			}
		} else if( fs::is_regular(dir_iter->path()) && (has_valid_extension(dir_iter->path()) || (SCAN_MODE_NOEXTENSION & flags)) ) {
			inc_files();
			if( flags & SCAN_MODE_FILES ) {
				add( dir_iter->path() );
			}
		}
	}
}
void Stat::renumber( const fs::path &out_dir, int start_number, int flags ) const {
	std::string extension, number_str;
	foreach( fs::path src, *this ) {
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
		start_number++;
	}
}

int check_directory( const fs::path &dir, int flags ) {
	// Check whether the directory exists, is a directory, is empty and we have the proper rights
	if( !fs::exists(dir) ) {
		if( flags & CHECK_CREATE ) {
			fs::create_directory(dir); 
			return 0;
		} else {
			std::cerr << "ERROR: " << dir << " does not exist" << endl;
			return 1;
		}
	} else if( flags & CHECK_CLEAR ) {
		Stat stat;
		stat.scan_directory( dir , SCAN_MODE_RECURSIVE | SCAN_MODE_DIRS | SCAN_MODE_FILES );
		stat.sort();
		stat.remove();
	} else if( !fs::is_directory(dir) ) {
		std::cerr << "ERROR: " << dir << " is not a directory" << endl;
		return 1;
	} else if( !fs::is_empty(dir) && !(flags & CHECK_IGNORE_EMPTY) ) {
		std::cerr << "ERROR: " << dir << " is not empty" << endl;
		return 1;
	} else {
		fs::ofstream out;
		out.open( dir / "test" );	
		if( !out.is_open() ) {
			std::cerr << "ERROR: We do not have proper rights for " << dir << endl;
			return 1;
		}
		out.close();
		fs::remove( dir / "test" ); 
	}
	return 0;
}

void Stat::remove() const {
	foreach( fs::path entry, *this ) {
		fs::remove( entry );
	}
}
