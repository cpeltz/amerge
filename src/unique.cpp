#include "common.hpp"
#include <map>
#include <list>
#include <boost/crc.hpp>
#include <boost/integer.hpp>

class CRCTable : public Stat {
	std::map< fs::path, boost::uint_t<32>::fast > table;

 public:
	virtual ~CRCTable(){}

	void add( const fs::path &path ) {
		Stat::add( path );
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
	CRCTable status;
	cout << "Action UNIQUE selected" << endl;
	cout << "Scanning directories ..." << flush;
	foreach(std::string directory, _directories){
		status.scan_directory( directory );
	}
	cout << "Scanned " << status.get_num_files() << " files in " << status.get_num_dirs() << " directories" << endl;

	cout << "Sorting lexicographically ..." << flush;
	status.sort();

	cout << "Create CRC-Table ..." << flush;
	status.create_crc();

	cout << "Find and remove duplicated entrys ..." << flush;
	unsigned int deleted = status.unique();
	cout << "Removed " << deleted << " files" << endl;

	return 0;
}
