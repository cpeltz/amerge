#ifndef COMMON_HPP
#define COMMON_HPP

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

class Stat {
	int num_files;
	int num_dirs;
	std::vector< fs::path > paths;

 public:
	typedef std::vector< fs::path >::iterator iterator;
	typedef std::vector< fs::path >::const_iterator const_iterator;

	Stat() : num_files(0), num_dirs(0) {}

	virtual ~Stat(){}

	virtual void add( const fs::path &path ) {
		paths.push_back( path );
	}

	virtual iterator begin() {
		return paths.begin();
	}

	virtual iterator end() {
		return paths.end();
	}

	void inc_dirs() {
		num_dirs++;
	}

	void inc_files() {
		num_files++;
	}

	virtual int get_num_files() {
		return num_files;
	}

	virtual int get_num_dirs() {
		return num_dirs;
	}
};

//1 = scan recursively
//2 = add files
//4 = add dirs
//8 = ignore file extensions
typedef enum FLAGS {	SCAN_MODE_RECURSIVE = 1, 
						SCAN_MODE_FILES = 2,
						SCAN_MODE_DIRS = 4,
						SCAN_MODE_NOEXTENSION = 8,
						CHECK_CREATE = 1,
						CHECK_CLEAR = 2 };

void copy_and_rename( Stat &status, const fs::path &out_dir, int start_number );
void scan_directory( Stat &status, const fs::path &directory, int flags = SCAN_MODE_RECURSIVE | SCAN_MODE_FILES );
int check_directory( const fs::path &dir, int flags );
void remove( Stat &status );

#endif
