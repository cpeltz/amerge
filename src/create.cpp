#include "common.hpp"

int AMerge::perform_action_create() {
	try {
		Stat status;
		cout << "Action CREATE selected" << endl;
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

		cout << "Beginning create ..." << flush;
		// TODO Fortschrittsbalken
		status.renumber( _out_dir, _start_number, COPY );

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
	return 0;
}
