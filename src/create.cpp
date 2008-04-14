#include "common.hpp"

int check_output_directory( fs::path &out_dir ) {
	// Check whether the output directory exists, is a directory, is empty and we have the proper rights
	fs::ofstream out;
	out.open(out_dir / "test" );
	if( !fs::exists(out_dir) ) {
		fs::create_directory(out_dir);
	} else if( !fs::is_directory(out_dir) ) {
		std::cerr << "ERROR: " << out_dir << " is not a directory" << endl;
		return 1;
	} else if( fs::is_empty(out_dir) ) {
		std::cerr << "ERROR: " << out_dir << " is not empty" << endl;
		return 1;
	} else if( !out.is_open() ) {
		std::cerr << "ERROR: We do not have proper rights for " << out_dir << endl;
		return 1;
	}
	out.close();
	fs::remove( out_dir / "test" ); 
	return 0;
}

int AMerge::perform_action_create() {
	try {
		Stat status;
		cout << "Action CREATE selected" << endl;
		cout << "Scanning directories ..." << flush;
		foreach(std::string directory, _directories){
			scan_directory(status, directory);
		}
		cout << "done" << endl;
		cout << "Scanned " << status.num_files << " files in " << status.num_dirs << " directories" << endl;

		cout << "Sorting lexicographically ..." << flush;
		std::sort( status.paths.begin(), status.paths.end() );
		cout << "done" << endl;

		cout << "Checking output directory ..." << flush;
		if( check_output_directory( _out_dir ) == 1 ) {
			return 1;
		}
		cout << "done" << endl;

		cout << "Beginning create ..." << flush;
		// TODO Fortschrittsbalken
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
	return 0;
}
