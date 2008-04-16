#include "common.hpp"
#include <map>
#include <list>
#include <boost/crc.hpp>
#include <functional>
#include <boost/integer.hpp>

typedef std::pair< fs::path, boost::uint_t<32>::fast > pair_type;

class marked_for_deletion : public std::unary_function<pair_type, bool > {
 public:
	result_type operator()(argument_type i) { 
		return i.second == 0 ? true : false;
	}
};
		  
class CRCTable : public Stat {

 public:
	std::map< fs::path, boost::uint_t<32>::fast > table;
	
	void add( const fs::path &path ) {
		paths.push_back( path );
		table.insert( std::make_pair( path, 0) );
	}
	
	void create_crc() {
		for( std::map< fs::path, boost::uint_t<32>::fast >::iterator it = table.begin(); it != table.end(); it++ ) {
			fs::ifstream in( it->first );
			void *buf = new char[file_size( it->first )];
			in.read( (char *)buf, file_size( it->first ) );
			boost::crc_32_type crc_computer;
			crc_computer.process_bytes( buf, file_size(it->first) );
			it->second = crc_computer();
		}
	}

	unsigned int unique() {
		for( std::map< fs::path, boost::uint_t<32>::fast >::iterator it = table.begin(); it != table.end(); it++ ) {
			if( it->second == 0 )
				continue;
			for( std::map< fs::path, boost::uint_t<32>::fast >::iterator sec = it; sec != table.end(); sec++ ) {
				if( sec->second == it->second && sec != it) {
					fs::remove( sec->first );
					sec->second = 0; // mark for delete
				}
			}
		}
		//std::remove_if( table.begin(), table.end(), marked_for_deletion() );
		unsigned int deleted = 0;
		for( std::map< fs::path, boost::uint_t<32>::fast >::iterator it = table.begin(); it != table.end(); it++ ) {
			if( it->second == 0 ) {
				table.erase( it );
				it--;
				deleted++;
			}
		}
		return deleted;
	}
};

int AMerge::perform_action_unique() {
	try {
		CRCTable status;
		cout << "Action UNIQUE selected" << endl;
		cout << "Scanning directories ..." << flush;
		foreach(std::string directory, _directories){
			scan_directory(status, directory);
		}
		cout << "done" << endl;
		cout << "Scanned " << status.num_files << " files in " << status.num_dirs << " directories" << endl;

		cout << "Sorting lexicographically ..." << flush;
		std::sort( status.paths.begin(), status.paths.end() );
		cout << "done" << endl;

		cout << "Create CRC-Table ..." << flush;
		status.create_crc();
		cout << "done!" << endl;

		cout << "Find and remove duplicated entrys ..." << flush;
		unsigned int deleted = status.unique();
		cout << "done" << endl;
		cout << "Removed " << deleted << " files" << endl;

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
	return 1;
}
