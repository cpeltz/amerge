#include "common.hpp"

void remove( Stat &status ) {
	foreach( fs::path entry, status.paths ) {
		fs::remove( entry );
	}
}

void move( fs::path src, fs::path dest ) {
	Stat status;
	scan_directory( status, src, SCAN_MODE_FILES );
	foreach( fs::path file, status.paths ) {
		fs::rename( file, dest / file.leaf() );
	}
}

void remove_directories( fs::path dir ) {
	Stat status;
	scan_directory( status, dir, SCAN_MODE_DIRS );
	std::sort( status.paths.begin(), status.paths.end() );
	std::reverse( status.paths.begin(), status.paths.end() );
	foreach( fs::path entry, status.paths ) {
		fs::remove( entry );
	}
}

int AMerge::perform_action_defrag() {
	cout << "Action DEFRAG selected" << endl;
	foreach( fs::path dir, _directories ) {
		try {
			Stat status;
			cout << "Scanning directory ..." << flush;
			scan_directory( status, dir );
			cout << "done" << endl;
			cout << "Scanned " << status.num_files << " files in " << status.num_dirs << " directories" << endl;
			cout << "Sorting lexicographically ..." << flush;
			std::sort( status.paths.begin(), status.paths.end() );
			cout << "done" << endl;
			if( exists(dir / ".amerge") ) {
				cout << "Clean old temporary directory ..." << flush;
				Stat stat;
				scan_directory( stat, dir / ".amerge", SCAN_MODE_RECURSIVE | SCAN_MODE_DIRS | SCAN_MODE_FILES );
				stat.paths.push_back( dir / ".amerge" );
				std::sort( stat.paths.begin(), stat.paths.end() );
				remove( stat );
				cout << "done" << endl;
			}
			cout << "Create temporary directory ..." << flush;
			fs::create_directory( dir / ".amerge" );
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
	}
	return 0;
}

