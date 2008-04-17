#ifndef AMERGE_HPP
#define AMERGE_HPP

#include <vector>
#include <string>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

static const char* extensions[] = {".jpg", ".jpeg", ".gif", ".bmp", ".png", ".tiff"};

typedef enum Action { CREATE, DEFRAG, UNIQUE, INSERT };


class AMerge {
	std::vector< std::string > _directories;
	int _start_number;
	fs::path _out_dir;
	bool _auto_clear_src;

	int perform_action_create();
	int perform_action_defrag();
	int perform_action_unique();
	int perform_action_insert();

	void clear_src();

 public:
	AMerge( std::vector< std::string > directories );
	~AMerge();
	
	void set_start_number( int start_number );
	void set_output_directory( std::string &out_dir );
	void set_auto_clear_src( bool auto_clear_src );

	int perform_action( const std::string &action );
};

#endif 
