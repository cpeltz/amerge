#ifndef AMERGE_HPP
#define AMERGE_HPP

#include <vector>
#include <string>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

static const char* extensions[] = {".jpg", ".jpeg", ".gif", ".bmp", ".png", ".tiff"};

typedef enum Action { CREATE, DEFRAG };


class AMerge {
	std::vector< std::string > _directories;
	int _start_number;
	fs::path _out_dir;

	int perform_action_create();
	int perform_action_defrag();

 public:
	AMerge(std::vector< std::string > directories);
	~AMerge();
	
	void set_start_number(int start_number);
	void set_output_directory(std::string &out_dir);
	int perform_action(Action action);
};

#endif 
