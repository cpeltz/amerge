#include <vector>
#include <string>

const char* extensions[] = {"jpg", "jpeg", "gif", "bmp", "png", "tiff"};

typedef enum Action = { MERGE, DEFRAG };

class LogFacility;

class AMerge {
	std::vector< std::string > _directories
	LogFacility &log;

	void perform_action_merge();
	void perform_action_defrag();
 public:
	AMerge(LogFacility &log, std::vector< std::string > directories);
	~AMerge();

	void perform_action(Action action);
};
