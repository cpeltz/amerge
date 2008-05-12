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

/**
 * RuntimeError is a classic Exception for unrecoverable errors during runtime.
 */
class RuntimeError : public std::exception {
	/**
	 * This string contains the reason, why this error was raised.
	 */
	std::string reason;

 public:
 	/**
	 * Constructor
	 * @param _reason The reason why the error is raised
	 */
 	RuntimeError( std::string _reason = "" ) : reason(_reason) {}
	/**
	 * Destructor
	 */
	virtual ~RuntimeError() throw() {}
	/**
	 * Returns a string containing the reason for the error.
	 * @return const char*
	 */
    virtual const char* what() const throw() {
		return reason.c_str();
	}

};

//1 = scan recursively
//2 = add files
//4 = add dirs
//8 = ignore file extensions
/**
 * This enum contains all flags for every function.
 */
typedef enum FLAGS {	SCAN_MODE_RECURSIVE = 1, 
						SCAN_MODE_FILES = 2,
						SCAN_MODE_DIRS = 4,
						SCAN_MODE_NOEXTENSION = 8,
						CHECK_CREATE = 1,
						CHECK_CLEAR = 2,
						CHECK_IGNORE_EMPTY = 4,
						COPY = 1, //for renumber()
						MOVE = 2};//for renumber()
/**
 * This Class is used to collect information an paths for actions and operate an them.
 */
class Stat {
	/**
	 * Contains the number of files processed
	 */
	int num_files;
	/**
	 * Contains the number of directories processed
	 */
	int num_dirs;
	/**
	 * Contains all added paths (usually set by scan_directory)
	 */
	std::vector< fs::path > paths;

 public:
	typedef std::vector< fs::path >::iterator iterator;
	typedef std::vector< fs::path >::const_iterator const_iterator;

	/**
	 * Constructor, initializes num_files and num_dirs
	 */
	Stat() : num_files(0), num_dirs(0) {}

	/**
	 * Destructor
	 */
	virtual ~Stat(){}

	/**
	 * Adds a path to its internal list.
	 * @param path The path to be added
	 * @return void
	 */
	virtual void add( const fs::path &path ) {
		paths.push_back( path );
	}

	/**
	 * Returns a iterator an the begin of the path holding container
	 * @return iterator A Iterator to the beginning
	 */
	virtual iterator begin() {
		return paths.begin();
	}

	/**
	 * Returns a iterator an the end of the path holding container
	 * @return iterator A Iterator to the ending
	 */
	virtual iterator end() {
		return paths.end();
	}

	/**
	 * Returns a constant iterator an the begin of the path holding container
	 * @return const_iterator A constant Iterator to the beginning
	 */
	virtual const_iterator begin() const {
		return paths.begin();
	}

	/**
	 * Returns a constant iterator an the end of the path holding container
	 * @return const_iterator A constant Iterator to the ending
	 */
	virtual const_iterator end() const {
		return paths.end();
	}

	/**
	 * Increases the num_dirs count by 1
	 * @return void
	 */
	void inc_dirs() {
		num_dirs++;
	}

	/**
	 * Increases the num_files count by 1
	 * @return void
	 */
	void inc_files() {
		num_files++;
	}

	/**
	 * Gets the current number of processed files.
	 * @return int Number of processed files
	 */
	virtual int get_num_files() const {
		return num_files;
	}

	/**
	 * Gets the current number of processed dirs.
	 * @return int Number of processed dirs
	 */
	virtual int get_num_dirs() const {
		return num_dirs;
	}

	/**
	 * Sorts the paths lexicographicly.
	 * @return void
	 */
	virtual void sort() {
		std::sort( paths.begin(), paths.end() );
	}

	/**
	 * Removes all directories and files contained.
	 * @return void
	 */
	virtual void remove() const ;
	/**
	 * @brief Scans a given directory.
	 *
	 * directory will be scanned and depending an the flags SCAN_MODE_FILES and SCAN_MODE_DIRS, files or dirs (or both) will be added to the path container. Depending on the SCAN_MODE_RECURSIVE flag the function will traverse recursivly through directory and with SCAN_MODE_NOEXTENSION the function can be told to ignore wheter or not the files have valid extensions.
	 * @param directory The directory which should be processed
	 * @param flags Flags for manipulating the behaviour. Can be a combination from SCAN_MODE_FILES, SCAN_MODE_DIRS, SCAN_MODE_RECURSIVE and SCAN_MODE_NOEXTENSION
	 * @return void
	 */
	virtual void scan_directory( const fs::path &directory, int flags = SCAN_MODE_RECURSIVE | SCAN_MODE_FILES ) throw(RuntimeError);
	/**
	 * @brief This function takes all files in the path container and rename them and puts them into out_dir
	 *
	 * This function is a most crucial function. It takes all saved files and numbers them beginning from start_number with a five diget width and copies or moves (depending on the flag MOVE or COPY for the last parameter) them to the specified out_dir.
	 * @param out_dir The directory to put the files into
	 * @param start_number The number from where to begin renumbering
	 * @param flags Can be COPY or MOVE. COPY forces that the files will be copied, MOVE forces the files to be moved.
	 * @return void
	 */
	virtual void renumber( const fs::path &out_dir, int start_number, int flags ) const;
	/**
	 * Clears the contents.
	 * @return void
	 */
	virtual void clear() {
		num_dirs = 0;
		num_files = 0;
		paths.clear();
	}

	/**
	 * Returns the number of saved paths.
	 * @return std::size_t 
	 */
	virtual std::size_t size() const {
		return paths.size();
	}
};

/**
 * @brief Checks for a correct directory. (usually a ouput directory)
 *
 * This function checks dir to be accessable, writable, empty and existing. It can be modified by the flags CHECK_CLEAR, CHECK_CREATE, CHECK_IGNORE_EMPTY. CHECK_CLEAR will clear the dir (removing all files etc). CHECK_CREATE will create the dir if it does not exists. With CHECK_IGNORE_EMPTY the function will not fail if the dir is not empty.
 * @param dir The directory to be checked
 * @param flags The modifiing flags. Can be a combination of CHECK_CLEAR, CHECK_CREATE and CHECK_IGNORE_EMPTY.
 * @return int 0 on success and 1 otherwise
 */
int check_directory( const fs::path &dir, int flags );

#endif
