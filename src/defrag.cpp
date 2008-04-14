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
		Stat status;
		cout << "Scanning directory ..." << flush;
		scan_directory( status, dir );
		cout << "done" << endl;
		cout << "Sorting lexicographically ..." << flush;
		std::sort( status.paths.begin(), status.paths.end() );
		cout << "done" << endl;
		fs::create_directory( dir / ".amerge" );
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
	return 1;
}

