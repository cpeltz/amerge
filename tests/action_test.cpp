#include "../src/common.hpp"
#include "../src/amerge.hpp"
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

void setup() {
	fs::create_directory( fs::path(".test") );	
	fs::create_directory( fs::path(".test") / "in" );	
	fs::create_directory( fs::path(".test") / "out" );
	fs::create_directory( fs::path(".test") / "out_defrag" );
	std::ofstream file( "file.jpg" );
	BOOST_CHECK( !file.fail() );
	file << "beer";
	file.close();
	fs::copy_file( fs::path("file.jpg"), fs::path(".test") / "in" / "file1.jpg" );
	fs::copy_file( fs::path("file.jpg"), fs::path(".test") / "in" / "file2.jpg" );
	fs::copy_file( fs::path("file.jpg"), fs::path(".test") / "in" / "file3.jpg" );
	fs::copy_file( fs::path("file.jpg"), fs::path(".test") / "in" / "file4.jpg" );
	fs::copy_file( fs::path("file.jpg"), fs::path(".test") / "out_defrag" / "00000.jpg" );
	fs::copy_file( fs::path("file.jpg"), fs::path(".test") / "out_defrag" / "00002.jpg" );
	fs::copy_file( fs::path("file.jpg"), fs::path(".test") / "out_defrag" / "00003.jpg" );
	fs::copy_file( fs::path("file.jpg"), fs::path(".test") / "out_defrag" / "00014.jpg" );
	file.open( "file.jpg" );
	file << "cookies";
	file.close();
	fs::copy_file( fs::path("file.jpg"), fs::path(".test") / "in" / "file5.jpg" );
	fs::copy_file( fs::path("file.jpg"), fs::path(".test") / "in" / "file6.jpg" );
	fs::copy_file( fs::path("file.jpg"), fs::path(".test") / "in" / "file7.jpg" );
	fs::copy_file( fs::path("file.jpg"), fs::path(".test") / "in" / "file8.jpg" );
	file.open( "file.jpg" );
	file << "This is not a JPEG file";
	file.close();
	fs::copy_file( fs::path("file.jpg"), fs::path(".test") / "in" / "file9.jpg" );
	fs::copy_file( fs::path("file.jpg"), fs::path(".test") / "in" / "file10.jpg" );
	fs::copy_file( fs::path("file.jpg"), fs::path(".test") / "in" / "file11.jpg" );
	fs::copy_file( fs::path("file.jpg"), fs::path(".test") / "in" / "file12.jpg" );
	fs::remove( fs::path("file.jpg") );
}

void cleanup() {
	Stat stat;
	stat.scan_directory( fs::path(".test"), SCAN_MODE_RECURSIVE | SCAN_MODE_FILES | SCAN_MODE_DIRS );
	stat.sort();
	std::reverse( stat.begin(), stat.end() );
	stat.remove();
	fs::remove( fs::path(".test") );
}

BOOST_AUTO_TEST_CASE( test_action_create ) {
	setup();
	std::vector< std::string > dirs;
	dirs.push_back( std::string(".test/") + "in" );
	AMerge amerge( dirs );
	amerge.set_output_directory( std::string(".test/") + "out" );
	BOOST_CHECK( amerge.perform_action( "create" ) == 0 );
	cleanup();
}

BOOST_AUTO_TEST_CASE( test_action_defrag ) {
	setup();
	std::vector< std::string > dirs;
	dirs.push_back( std::string(".test/") + "out_defrag" );
	AMerge amerge( dirs );
	BOOST_CHECK( amerge.perform_action( "defrag" ) == 0 );
	cleanup();
}

BOOST_AUTO_TEST_CASE( test_action_insert ) {
	setup();
	std::vector< std::string > dirs;
	dirs.push_back( std::string(".test/") + "in" );
	AMerge amerge( dirs );
	amerge.set_output_directory( std::string(".test/") + "out_defrag" );
	BOOST_CHECK( amerge.perform_action( "insert" ) == 0 );
	cleanup();
}

BOOST_AUTO_TEST_CASE( test_action_unique ) {
	setup();
	std::vector< std::string > dirs;
	dirs.push_back( std::string(".test/") + "in" );
	AMerge amerge( dirs );
	BOOST_CHECK( amerge.perform_action( "unique" ) == 0 );
	cleanup();
}
