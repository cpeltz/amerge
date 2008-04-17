#include "common.hpp"
#include <sstream>

int AMerge::perform_action_insert() {
	Stat status;
	cout << "Action INSERT selected" << endl;
	cout << "Scanning directories ..." << flush;
	foreach(std::string directory, _directories){
		status.scan_directory( directory );
	}
	cout << "Scanned " << status.get_num_files() << " files in " << status.get_num_dirs() << " directories" << endl;

	cout << "Sorting lexicographically ..." << flush;
	status.sort();

	cout << "Checking output directory ..." << flush;
	if( check_directory( _out_dir, CHECK_CREATE ) == 1 ) {
		return 1;
	}

	cout << "Looking for last used number ..." << flush;
	{
		Stat temp;
		temp.scan_directory( _out_dir, SCAN_MODE_FILES ); // Warning: not tracked files may be overwritten
		std::sort( temp.begin(), temp.end() );
		Stat::iterator it = temp.end();
		it--;
		std::stringstream stream;
		stream << fs::basename(*it);
		stream >> _start_number;
		_start_number++;
	}

	cout << "Beginning insert ..." << _start_number << flush;
	status.renumber( _out_dir, _start_number, (_auto_clear_src ? MOVE : COPY) );
	_auto_clear_src = false;

	return 0;
}
