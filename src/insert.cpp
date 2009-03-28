#include "common.hpp"
#include <sstream>

bool contains_directory( fs::path directory ) {
	fs::directory_iterator end_itr;
	for( fs::directory_iterator itr(directory); itr != end_itr; itr++ ) {
		if( is_directory( itr->status() ) ) {
			return true;
		}
	}
	return false;
}

void insert_in_sub_dirs( fs::path output_directory, int boundary, int start_number ) {
	Stat status;
	status.scan_directory( output_directory, SCAN_MODE_FILES );
	status.sort();

	Stat dirs;
	dirs.scan_directory( output_directory, SCAN_MODE_DIRS );
	dirs.sort();

	Stat::iterator it = dirs.end();
	it--;
	std::stringstream sstream;
	sstream << it->leaf();
	int dir_number;
	sstream >> dir_number;
	int until_boundary_reached = (boundary + dir_number) - start_number;
	fs::path target_dir = *it;
	for( Stat::iterator iter = status.begin(); iter != status.end(); iter++ ) {
		if( !until_boundary_reached ) {
			fs::path basename = fs::basename(*iter);
			fs::path branch = iter->branch_path();
			target_dir = branch / basename;
			fs::create_directory( branch / basename );
			until_boundary_reached = boundary;
		}
		fs::copy_file( *iter, target_dir / iter->leaf() );
		until_boundary_reached--;
	}
	status.remove();
}

int AMerge::perform_action_insert() {
	bool sub_dirs = false;
	Stat status;
	cout << "Action INSERT selected" << endl;
	cout << "Scanning directories ..." << endl;
	foreach(std::string directory, _directories){
		status.scan_directory( directory );
	}
	cout << "Scanned " << status.get_num_files() << " files in " << status.get_num_dirs() << " directories" << endl;

	cout << "Sorting lexicographically ..." << endl;
	status.sort();

	cout << "Checking output directory ..." << endl;
	{
		if( check_directory( _out_dir, CHECK_CREATE | CHECK_IGNORE_EMPTY ) == 1 ) {
			return 1;
		}
		sub_dirs = contains_directory( _out_dir );
	}

	cout << "Looking for last used number ..." << endl;
	{
		Stat temp;
		temp.scan_directory( _out_dir, SCAN_MODE_FILES | (sub_dirs ? SCAN_MODE_RECURSIVE : 0) ); // Warning: not tracked files may be overwritten
		std::sort( temp.begin(), temp.end() );
		Stat::iterator it = temp.end();
		it--;
		std::stringstream stream;
		stream << fs::basename(*it);
		stream >> _start_number;
		_start_number++;
	}

	cout << "Beginning insert ..." << _start_number << endl;
	status.renumber( _out_dir, _start_number, (_auto_clear_src ? MOVE : COPY) );
	if( sub_dirs ) {
		insert_in_sub_dirs( _out_dir, _sub_dir_boundary, _start_number );
	}
	_auto_clear_src = false; // TODO Needed?

	return 0;
}
