#ifndef COMMON_HPP
#define COMMON_HPP

#include "amerge.hpp"
#include <iostream>
#include <boost/foreach.hpp>
#include <boost/filesystem/fstream.hpp>
#include <cctype>
#include <algorithm>
#include <sstream>
#include <exception>

#define foreach BOOST_FOREACH

using std::cout;
using std::endl;
using std::flush;

class RuntimeError : public std::exception {
	std::string reason;

 public:
 	RuntimeError( std::string _reason = "" ) : reason(_reason) {}
	virtual ~RuntimeError() throw() {}
    virtual const char* what() const throw(){
		return reason.c_str();
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
						CHECK_CLEAR = 2,
						CHECK_IGNORE_EMPTY = 4,
						COPY = 1, //for renumber()
						MOVE = 2};//for renumber()

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

	virtual const_iterator begin() const {
		return paths.begin();
	}
	virtual const_iterator end() const {
		return paths.end();
	}

	void inc_dirs() {
		num_dirs++;
	}

	void inc_files() {
		num_files++;
	}

	virtual int get_num_files() const {
		return num_files;
	}

	virtual int get_num_dirs() const {
		return num_dirs;
	}

	virtual void sort() {
		std::sort( paths.begin(), paths.end() );
	}

	virtual void remove() const ;
	virtual void scan_directory( const fs::path &directory, int flags = SCAN_MODE_RECURSIVE | SCAN_MODE_FILES ) throw(RuntimeError);
	virtual void renumber( const fs::path &out_dir, int start_number, int flags ) const;
	virtual void clear() {
		num_dirs = 0;
		num_files = 0;
		paths.clear();
	}

	virtual std::size_t size() const {
		return paths.size();
	}
};

int check_directory( const fs::path &dir, int flags );

#endif
