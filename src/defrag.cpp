#include "common.hpp"

void move( fs::path src, fs::path dest ) {
	Stat status;
	status.scan_directory( src, SCAN_MODE_FILES );
	foreach( fs::path file, status ) {
		fs::rename( file, dest / file.leaf() );
	}
}

void remove_directories( fs::path dir ) {
	Stat status;
	status.scan_directory( dir, SCAN_MODE_DIRS | SCAN_MODE_RECURSIVE );
	std::sort( status.begin(), status.end() );
	std::reverse( status.begin(), status.end() );
	status.remove();
}

int AMerge::perform_action_defrag() {
	cout << "Action DEFRAG selected" << endl;
	bool inplace = (!_out_dir.empty() && _directories.size() == 1) ?  false : true ;

	foreach( fs::path dir, _directories ) {
		Stat status;
		_out_dir = inplace ? (dir / ".amerge") : _out_dir;
		
		cout << "Scanning directory " << dir << " ..." << endl;
		status.scan_directory( dir );
		cout << "Scanned " << status.get_num_files() << " files in " << status.get_num_dirs() << " directories" << endl;
		
		cout << "Sorting lexicographically ..." << endl;
		status.sort();
		
		cout << "Check, Clean and Create temporary directory ..." << endl;
		check_directory( _out_dir, (inplace ? (CHECK_CLEAR | CHECK_CREATE) : CHECK_CREATE) );			
		
		cout << "Rename files ..." << endl;
		status.renumber( _out_dir, _start_number, (inplace ? MOVE : COPY) );
		
		if( inplace ) {
			cout << "Cleaning up ..." << endl;
			move( _out_dir, dir );
			remove_directories( dir );
		}
	}
	return 0;
}

