#include "common.hpp"

int AMerge::perform_action_create() {
	Stat status;
	cout << "Action CREATE selected" << endl;
	cout << "Scanning directories ..." << endl;
	foreach(std::string directory, _directories){
		status.scan_directory( directory );
	}
	cout << "Scanned " << status.get_num_files() << " files in " << status.get_num_dirs() << " directories" << endl;

	cout << "Sorting lexicographically ..." << endl;
	status.sort();

	cout << "Checking output directory ..." << endl;
	if( check_directory( _out_dir, CHECK_CREATE ) == 1 ) {
		return 1;
	}

	cout << "Beginning create ..." << endl;
	// TODO Fortschrittsbalken
	status.renumber( _out_dir, _start_number, COPY );
	if( _create_sub_dirs ) {
		cout << "Create sub-directories ..." << endl;
		create_sub_dirs( _out_dir, _sub_dir_boundary );
	}

	return 0;
}
