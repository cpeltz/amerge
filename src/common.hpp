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

struct Stat {
	int num_files;
	int num_dirs;
	std::vector< fs::path > paths;
	Stat() : num_files(0), num_dirs(0) {}
};

//0 = normal (recursive, don't add dirs)
//1 = not recursive, don't add dirs
//2 = not recursive, don't add files but dirs
enum FLAGS {	SCAN_MODE_RECURSIVE = 1, 
				SCAN_MODE_FILES = 2,
				SCAN_MODE_DIRS = 4 };

void copy_and_rename( Stat &status, fs::path &out_dir, int start_number );
void scan_directory( Stat &status, const fs::path &directory, FLAGS flags = SCAN_MODE_RECURSIVE | SCAN_MODE_FILES );

#endif
