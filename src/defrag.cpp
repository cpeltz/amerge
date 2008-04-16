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
	scan_directory( status, dir, SCAN_MODE_DIRS | SCAN_MODE_RECURSIVE );
	std::sort( status.begin(), status.end() );
	std::reverse( status.begin(), status.end() );
	remove( status );
}

int AMerge::perform_action_defrag() {
	try {
		cout << "Action DEFRAG selected" << endl;
		bool inplace = (!_out_dir.empty() && _directories.size() == 1) ?  false : true ;

		foreach( fs::path dir, _directories ) {
			Stat status;
			_out_dir = inplace ? (dir / ".amerge") : _out_dir;
			
			cout << "Scanning directory " << dir << " ..." << flush;
			scan_directory( status, dir );
			cout << "Scanned " << status.get_num_files() << " files in " << status.get_num_dirs() << " directories" << endl;
			
			cout << "Sorting lexicographically ..." << flush;
			status.sort();
			
			cout << "Check, Clean and Create temporary directory ..." << flush;
			check_directory( _out_dir, (inplace ? (CHECK_CLEAR | CHECK_CREATE) : CHECK_CREATE) );			
			
			cout << "Rename files ..." << flush;
			renumber( status, _out_dir, _start_number, (inplace ? MOVE : COPY) );
			
			if( inplace || _auto_clear_src ) {
				cout << "Remove old files ..." << flush;
				remove( status ) ;
			}
			
			if( inplace ) {
				cout << "Cleaning up ..." << flush;
				move( _out_dir, dir );
				remove_directories( dir );
			}
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

