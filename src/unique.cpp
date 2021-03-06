#include "common.hpp"
#include <map>
#include <list>
#include <boost/crc.hpp>
#include <boost/integer.hpp>

/**
 * CRCTable extends Stat with the ability to compute CRC32 Checksums.
 */
class CRCTable : public Stat {
	/**
	 * The additional table with a path->CRC32 relation
	 */
	std::map< fs::path, boost::uint_t<32>::fast > table;

 public:
 	/**
	 * Destructor
	 */
	virtual ~CRCTable(){}

	/**
	 * Overwritten add methode to acknowledge the existence of table.
	 *
	 * @param path The Path to be added
	 */
	void add( const fs::path &path ) {
		Stat::add( path );
		table.insert( std::make_pair( path, 0) );
	}
	
	/**
	 * This function creates for every path saved in table the correct CRC Checksum
	 * @return void
	 */
	void create_crc() {
		for( std::map< fs::path, boost::uint_t<32>::fast >::iterator it = table.begin(); it != table.end(); it++ ) {
			fs::ifstream in( it->first );
            uint32_t buffer_size = file_size( it->first );
            char *buf = new char[buffer_size];
			in.read( (char *)buf, file_size( it->first ) );
			boost::crc_32_type crc_computer;
			crc_computer.process_bytes( buf, file_size(it->first) );
			it->second = crc_computer();
            delete[] buf;
		}
	}

	/**
	 * This function removes all duplicates.
	 * @return unsigned int The number of removed files.
	 */
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
        for( std::map< fs::path, boost::uint_t<32>::fast >::iterator it = table.begin(); it != table.end(); ) {
			if( it->second == 0 ) {
                table.erase( it++ );
				deleted++;
            } else {
                it++;
            }
		}
		return deleted;
	}
};

int AMerge::perform_action_unique() {
	CRCTable status;
	cout << "Action UNIQUE selected" << endl;
	cout << "Scanning directories ..." << endl;
	foreach(std::string directory, _directories){
		status.scan_directory( directory );
	}
	cout << "Scanned " << status.get_num_files() << " files in " << status.get_num_dirs() << " directories" << endl;

	cout << "Sorting lexicographically ..." << endl;
	status.sort();

	cout << "Create CRC-Table ..." << endl;
	status.create_crc();

	cout << "Find and remove duplicated entrys ..." << endl;
	unsigned int deleted = status.unique();
	cout << "Removed " << deleted << " files" << endl;

	return 0;
}
