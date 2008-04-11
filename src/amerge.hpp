#ifndef AMERGE_HPP
#define AMERGE_HPP

#include <vector>
#include <string>

static const char* extensions[] = {".jpg", ".jpeg", ".gif", ".bmp", ".png", ".tiff"};

typedef enum Action { MERGE, DEFRAG };


class AMerge {
	std::vector< std::string > _directories;

	void perform_action_merge();
	void perform_action_defrag();
 public:
	AMerge(std::vector< std::string > directories);
	~AMerge();

	void perform_action(Action action);
};

#endif 
