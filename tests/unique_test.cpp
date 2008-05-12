#include "../src/common.hpp"
#include "../src/unique.cpp"
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test_CRCTable ) {
	fs::create_directory(".test");
	std::ofstream file( "file" );
	BOOST_CHECK( !file.fail() );
	file << "Beer";
	file.close();
	fs::copy_file( "file", fs::path(".test") / "file1.jpg" );
	fs::copy_file( "file", fs::path(".test") / "file2.jpg" );
	fs::copy_file( "file", fs::path(".test") / "file3.jpg" );
	fs::copy_file( "file", fs::path(".test") / "file4.jpg" );
	fs::copy_file( "file", fs::path(".test") / "file5.jpg" );
	file.open( "file" );
	BOOST_CHECK( !file.fail() );
	file << "Kekse";
	file.close();
	fs::copy_file( "file", fs::path(".test") / "file6.jpg" );
	CRCTable table;
	table.scan_directory( fs::path(".test") );
	table.sort();
	table.create_crc();
	BOOST_CHECK( table.unique() == 4 );
	fs::remove( fs::path(".test/file1.jpg") );
	fs::remove( fs::path(".test/file6.jpg") );
	fs::remove( fs::path(".test") );
	fs::remove( fs::path("file") );
}
