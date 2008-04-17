#include "common.hpp"

AMerge::AMerge( std::vector< std::string > directories ) 
	: _directories(directories), _start_number(0), _out_dir(""), _auto_clear_src(false) {}

AMerge::~AMerge(){}

void AMerge::set_start_number( int start_number ) {
	_start_number = start_number;
}

void AMerge::set_output_directory( std::string &out_dir ) {
	_out_dir = out_dir;
}

void AMerge::set_auto_clear_src( bool auto_clear_src ) {
	_auto_clear_src = auto_clear_src;
}

int AMerge::perform_action( const std::string &action ) {
	int return_value = 1;

	try {
		if( action == "create" ) {
			if( !_out_dir.empty() && !_directories.empty() )
				return_value = perform_action_create();
			else
				std::cerr << "You need to specify a output directory and one or more input directories" << endl;
		} else if( action == "defrag" ) {
			if( _out_dir.empty() && _directories.size() == 1 && _auto_clear_src ) {
				cout << "clear-src flag ignored, because no output directory was specified" << endl;
				_auto_clear_src = false;
			}

			if( !_directories.empty() )
				return_value = perform_action_defrag();
			else
				cout << "You need to specify at least one input directory" << endl;
		} else if( action == "unique" ) {
			if( _auto_clear_src ) {
				cout << "clear-src flag ignored, action does not allow it" << endl;
				_auto_clear_src = false;
			}

			if( !_directories.empty() )
				return_value = perform_action_unique();
			else
				cout << "You need to specify at least one input directory" << endl;
		} else if( action == "insert" ) {
			if( _out_dir.empty() || _directories.empty())
				std::cerr << "You need to specify a output directory and one or more input directories" << endl;
			else
				return_value = perform_action_insert();
		} else {
			std::cerr << "Unknown action" << endl;
		}

		if( _auto_clear_src ) {
			this->clear_src();
		}

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

	return return_value;
}

void AMerge::clear_src() {
	cout << "Remove source files ..." << flush;
	foreach( fs::path dir, _directories ) {
		Stat status;
		status.scan_directory( dir, SCAN_MODE_RECURSIVE | SCAN_MODE_FILES | SCAN_MODE_DIRS );
		status.remove();
	}
}
