#include <iostream>
#include <vector>
#include <string>

#include <boost/program_options.hpp>

#include "amerge.hpp"

const unsigned int VERSION_MAJOR = 1;
const unsigned int VERSION_MINOR = 1;
const unsigned int VERSION_PATCH = 0;

namespace po = boost::program_options;

int main(int argc, char **argv) {
	int start_number, sub_dir_boundary;
	std::string out_dir, action;
	std::vector< std::string > dirs;
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce help message")
		("version", "print version information")
		("start-number,s", po::value<int>(&start_number)->default_value(0), "start numbering files from this offset on")
		("input-dir,i", po::value< std::vector<std::string> >(&dirs), "specify directories to work on")
		("output-dir,o", po::value< std::string >(&out_dir), "specify output directory")
		("action,a", po::value< std::string >(&action), "can be create or defrag at the moment")
		("clear-src,c", "removes source files when action is completed")
		("create-sub-dirs,d", "automatical create sub-directories if there are to many files")
		("create-sub-dirs-boundary,b", po::value<int>(&sub_dir_boundary)->default_value(1000), "number of files per sub-directorie if --create-sub-dirs is enabled")
	;
	po::positional_options_description p;
	p.add("input-dir", -1);

	po::variables_map vm;
	po::store( po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm );
	po::notify(vm);

	AMerge app( dirs );
	app.set_start_number( start_number );
	app.set_output_directory( out_dir );

	if( vm.count("help") ) {
		std::cout << desc;
		return 0;
	} else if( vm.count("version") ) {
		std::cout << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << std::endl;
		return 0;
	} else if( !vm.count("action") ) {
		std::cout << "You need to specify an action" << std::endl;
		std::cout << desc;
		return 1;
	} else if( vm.count("clear-src") ) {
		app.set_auto_clear_src( true );	
	} else if( vm.count("create-sub-dirs") ) {
		app.set_create_sub_dirs( true );
		if( vm.count("create-sub-dirs-boundary") == 1 ) {
			app.set_sub_dir_boundary( sub_dir_boundary );
		}
	}

	return app.perform_action( action );
}
