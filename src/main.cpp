#include <iostream>
#include <vector>
#include <string>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char **argv) {
	int start_number;
	std::vector< std::string > dirs;
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce help message")
		("start-number,s", po::value<int>(&start_number)->default_value(0), "start numbering files from this offset on")
		("input-dir,i", po::value< std::vector<std::string> >(&dirs), "specify directories to work on")
	;
	po::positional_options_description p;
	p.add("input-dir", -1);

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
	po::notify(vm);

	if(vm.count("help")) {
		std::cout << desc;
		return 0;
	}

	return 0;
}
