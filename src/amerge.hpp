#ifndef AMERGE_HPP
#define AMERGE_HPP

#include <vector>
#include <string>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

/**
 * Valid extensions array
 */
static const char* extensions[] = {".jpg", ".jpeg", ".gif", ".bmp", ".png", ".tiff"};

/**
 * enum for actions
 */
typedef enum Action { CREATE, DEFRAG, UNIQUE, INSERT };

/**
 * @brief The main application class. Responsible for calling the actions and handle excceptions.
 *
 * The class AMerge is used to configure the parameter for the actions and run the actions 
 * themselves. It also handles all known exceptions.
 */
class AMerge {
	/**
	 * Contains the input directories.
	 */
	std::vector< std::string > _directories;
	/**
	 * Contains the number for starting numbering.
	 */
	int _start_number;
	/**
	 * For actions that need a output directory this variable holds it.
	 */
	fs::path _out_dir;
	/**
	 * Flag for removing the input directories
	 */
	bool _auto_clear_src;

	/**
	 * @brief Performs the action create.
	 *
	 * The action create will take multiple input directories and one output directory. It will scan all input directories and renumbers them and copies them to the output directory. It also checks the _auto_clear_src variable and moves the files instead of copying them if it is set.
	 * @return int 0 on success, 1 otherwise
	 */
	int perform_action_create();
	/**
	 * @brief Performs the action defrag.
	 *
	 * This action takes multiple input directories and will process them. All files will be renumbered but will remain in their original directory. (this is used to close numbering gaps, introduces by the action unique)
	 * @return int 0 on success, 1 otherwise
	 */
	int perform_action_defrag();
	/**
	 * @brief Performs the action unique.
	 *
	 * The action unique takes multiple input directories an processes them. It builds a CRC Table and removes every duplicate.
	 * @return int 0 on success, 1 otherwise
	 */
	int perform_action_unique();
	/**
	 * @brief Performs the action insert.
	 *
	 * This action takes multiple input directories and a output directory and inserts the contents of the input direcoties into the output directory and renumbers them.
	 * @return int 0 on success, 1 otherwise
	 */
	int perform_action_insert();

	/**
	 * If _auto_clear_src is set, this removes the input directories.
	 */
	void clear_src();

 public:
 	/**
	 * Constructor
	 * @param directories A List with input directories
	 */
	AMerge( std::vector< std::string > directories );
	/**
	 * Destructor
	 */
	~AMerge();
	
	/**
	 * Sets the number where the numbering should start.
	 * @param start_number First number for numbering
	 * @return void
	 */
	void set_start_number( int start_number );
	/**
	 * Sets the output directory (not all actions need this)
	 * @param out_dir Output directory
	 * @return void
	 */
	void set_output_directory( const std::string &out_dir );
	/**
	 * Sets the input directory clear flag
	 * @param auto_clear_src New value for _auto_clear_src
	 * @return void
	 */
	void set_auto_clear_src( bool auto_clear_src );

	/**
	 * Performs a Action that corresponds to the string action.
	 * @param action A String describing the action
	 * @return int 0 an success, otherwise not 0 (mostly 1)
	 */
	int perform_action( const std::string &action );
};

#endif 
