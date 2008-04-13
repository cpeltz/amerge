#include "common.hpp"

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

void scan_directory( Stat &status, const fs::path &directory, FLAGS flags ) { 
	if( !fs::exists( directory ) ) {
		cout << "ERROR: Directory " << directory << " does not exist!" << endl;
		return;
	}
	fs::directory_iterator dir_iter( directory ), end_iter;
	for( ; dir_iter != end_iter; dir_iter++ ){
		if( fs::is_directory(dir_iter->path()) ) {
			status.num_dirs++;
			if( flags == SCAN_MODE_RECURSIVE ) {
				scan_directory( status, dir_iter->path() );
			}
			if( flags == SCAN_MODE_DIRS ) {
				status.paths.push_back( dir_iter->path() );
			}
		} else if ( fs::is_regular(dir_iter->path()) && has_valid_extension(dir_iter->path()) ) {
			status.num_files++;
			if( flags == SCAN_MODE_FILES ) {
				status.paths.push_back( dir_iter->path() );
			}
		}
	}
}

void copy_and_rename( Stat &status, fs::path &out_dir, int start_number ) {
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

