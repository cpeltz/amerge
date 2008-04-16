#include "common.hpp"

void move( fs::path src, fs::path dest ) {
	Stat status;
	scan_directory( status, src, SCAN_MODE_FILES );
	foreach( fs::path file, status ) {
		fs::rename( file, dest / file.leaf() );
	}
}

void remove_directories( fs::path dir ) {
	Stat status;
	scan_directory( status, dir, SCAN_MODE_DIRS );
	std::sort( status.begin(), status.end() );
	std::reverse( status.begin(), status.end() );
	remove( status );
}

int AMerge::perform_action_defrag() {
	try {
		cout << "Action DEFRAG selected" << endl;
		foreach( fs::path dir, _directories ) {
			Stat status;
			
			cout << "Scanning directory ..." << flush;
			scan_directory( status, dir );
			cout << "done" << endl;
			cout << "Scanned " << status.get_num_files() << " files in " << status.get_num_dirs() << " directories" << endl;
			
			cout << "Sorting lexicographically ..." << flush;
			std::sort( status.begin(), status.end() );
			cout << "done" << endl;
			
			cout << "Check, Clean and Create temporary directory ..." << flush;
			check_directory( dir / ".amerge", CHECK_CLEAR | CHECK_CREATE );			
			cout << "done" << endl;
			
			cout << "Copy files into temporary dir an rename ..." << flush;
			copy_and_rename( status, dir / ".amerge", _start_number );
			cout << "done" << endl;
			
			cout << "Remove old files ..." << flush;
			remove( status );
			cout << "done" << endl;
			
			cout << "Cleaning up ..." << flush;
			move( dir / ".amerge", dir );
			remove_directories( dir );
			cout << "done" << endl;
		}
	} catch(fs::filesystem_error) {
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

