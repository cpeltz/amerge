#include "../src/common.hpp"
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

void move( fs::path src, fs::path dest );

BOOST_AUTO_TEST_CASE( test_move ) {
	fs::create_directory(".test");
	fs::create_directory(".test/src");
	fs::create_directory(".test/dest");
	std::ofstream file( "file" );
	BOOST_CHECK( !file.fail() );
	file.close();
	fs::copy_file( "file", fs::path(".test") / "src" / "file1.jpg" );
	fs::copy_file( "file", fs::path(".test") / "src" / "file2.jpg" );
	fs::copy_file( "file", fs::path(".test") / "src" / "file3.jpg" );
	fs::copy_file( "file", fs::path(".test") / "src" / "file4.jpg" );
	fs::copy_file( "file", fs::path(".test") / "src" / "file5.jpg" );
	move( fs::path(".test/src/"), fs::path(".test/dest/") );
	BOOST_CHECK( fs::is_empty(fs::path(".test/src")) );
	BOOST_CHECK( !fs::is_empty(fs::path(".test/dest")) );
	BOOST_CHECK( fs::is_regular(fs::path(".test/dest/file3.jpg")) );
	fs::remove( fs::path(".test") / "dest" / "file1.jpg" );
	fs::remove( fs::path(".test") / "dest" / "file2.jpg" );
	fs::remove( fs::path(".test") / "dest" / "file3.jpg" );
	fs::remove( fs::path(".test") / "dest" / "file4.jpg" );
	fs::remove( fs::path(".test") / "dest" / "file5.jpg" );
	fs::remove( fs::path(".test") / "dest" );
	fs::remove( fs::path(".test") / "src" );
	fs::remove( fs::path(".test") );
	fs::remove( fs::path("file") );
}
