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

void remove_direcotries( fs::path dir ) {
	Stat status;
	scan_directory( status, dir, SCAN_MODE_DIRS );
	std::sort( status.paths.begin(), status.paths.end() );
	std::reverse( status.paths.begin(), status.paths.end() );
	foreach( fs::path entry, status.paths ) {
		fs::remove( entry );
	}
}

int AMerge::perform_action_defrag() {
	foreach( fs::path dir, _directories ) {
		Stat status;
		scan_directory( status, dir );
		std::sort( status.paths.begin(), status.paths.end() );
		copy_and_rename( status, dir / ".amerge", _start_number );
		remove( status );
		move( dir / ".amerge", dir );
		remove_directories( dir );
	}
	return 1;
}

