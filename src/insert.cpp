#include "common.hpp"
#include <sstream>

int AMerge::perform_action_insert() {
	try {
		Stat status;
		cout << "Action INSERT selected" << endl;
		cout << "Scanning directories ..." << flush;
		foreach(std::string directory, _directories){
			scan_directory(status, directory);
		}
		cout << "done" << endl;
		cout << "Scanned " << status.get_num_files() << " files in " << status.get_num_dirs() << " directories" << endl;

		cout << "Sorting lexicographically ..." << flush;
		std::sort( status.begin(), status.end() );
		cout << "done" << endl;

		cout << "Checking output directory ..." << flush;
		if( check_directory( _out_dir, CHECK_CREATE ) == 1 ) {
			return 1;
		}
		cout << "done" << endl;

		cout << "Looking for last used number ..." << flush;
		{
			Stat temp;
			scan_directory(temp, _out_dir, SCAN_MODE_FILES); // Warning: not tracked files may be overwritten
			std::sort( temp.begin(), temp.end() );
			Stat::iterator it = temp.end();
			it--;
			std::stringstream stream;
			stream << fs::basename(*it);
			stream >> _start_number;
		}
		cout << "done" << endl;

		cout << "Beginning insert ..." << flush;
		copy_and_rename( status, _out_dir, _start_number );
		cout << "done" << endl;

	} catch (fs::filesystem_error) {
		cout << "failed" << endl;
		cout << "A filesystem operation has failed" << endl 
			<< "Please check whether you have sufficient rights" << endl;
		return 1;
	} catch(std::bad_alloc) {
		// Bad ... very bad
		std::cerr << "Out of Memory" << std::endl;
		return 1;
	}
	return 1;
}
