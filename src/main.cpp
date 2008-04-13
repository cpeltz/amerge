#include <iostream>
#include <vector>
#include <string>

#include <boost/program_options.hpp>

#include "amerge.hpp"

namespace po = boost::program_options;

int main(int argc, char **argv) {
	int start_number;
	std::string out_dir, action;
	std::vector< std::string > dirs;
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce help message")
		("start-number,s", po::value<int>(&start_number)->default_value(0), "start numbering files from this offset on")
		("input-dir,i", po::value< std::vector<std::string> >(&dirs), "specify directories to work on")
		("output-dir,o", po::value< std::string >(&out_dir), "specify output directory")
		("action,a", po::value< std::string >(&action), "can be merge or defrag at the moment")
	;
	po::positional_options_description p;
	p.add("input-dir", -1);

	po::variables_map vm;
	po::store( po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm );
	po::notify(vm);

	if( vm.count("help") ) {
		std::cout << desc;
		return 0;
	} else if( !vm.count("action") ) {
		std::cout << "You need to specify an action" << std::endl;
		return 1;
	}

	AMerge app( dirs );
	app.set_start_number( start_number );
	app.set_output_directory( out_dir );

	if( action == "merge" ) {
		return app.perform_action( MERGE );
	} else if( action == "defrag" ) {
		return app.perform_action( DEFRAG );
	} 

	std::cout << "Unknown action" << std::endl;
	return 1;
}
