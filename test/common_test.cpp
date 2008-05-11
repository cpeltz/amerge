#include "../src/common.cpp"
#include "../src/defrag.cpp"
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test__lower ) {
	char a = 'A';
	BOOST_CHECK( 'a' == _lower(a));
}

BOOST_AUTO_TEST_CASE( test_to_lower ) {
	std::string upper = "HELLO!", lower = "hello!";
	to_lower( upper );
	BOOST_CHECK( lower == upper );
}

BOOST_AUTO_TEST_CASE( test_Stat_clear ) {
	Stat stat;
	BOOST_CHECK( stat.get_num_dirs() == 0 );
	BOOST_CHECK( stat.get_num_files() == 0 );
	stat.inc_dirs();
	BOOST_CHECK( stat.get_num_dirs() == 1 );
	BOOST_CHECK( stat.get_num_files() == 0 );
	stat.inc_files();
	BOOST_CHECK( stat.get_num_dirs() == 1 );
	BOOST_CHECK( stat.get_num_files() == 1 );
	stat.add( fs::path(".test") );
	BOOST_CHECK( stat.size() == 1 );
	stat.clear();
	BOOST_CHECK( stat.get_num_dirs() == 0 );
	BOOST_CHECK( stat.get_num_files() == 0 );
	BOOST_CHECK( stat.size() == 0 );
}

BOOST_AUTO_TEST_CASE( test_Stat_scan_directory ) {
	fs::create_directory( ".test" );
	Stat stat;
	stat.scan_directory( ".test" );
	BOOST_CHECK( stat.get_num_files() == 0 );
	BOOST_CHECK( stat.get_num_dirs() == 0 );
	fs::create_directory( ".test/one" );
	fs::create_directory( ".test/two" );
	fs::create_directory( ".test/one/kekse" );
	stat.scan_directory( ".test" );
	BOOST_CHECK( stat.get_num_files() == 0 );
	BOOST_CHECK( stat.get_num_dirs() == 3 );
	stat.clear();
	std::ofstream file( "file" );
	BOOST_CHECK( !file.fail() );
	file.close();
	fs::rename( "file", fs::path(".test") / "file" );
	stat.scan_directory( ".test", SCAN_MODE_NOEXTENSION | SCAN_MODE_RECURSIVE | SCAN_MODE_FILES );
	BOOST_CHECK( stat.get_num_files() == 1 );
	BOOST_CHECK( stat.get_num_dirs() == 3 );
	int num_entries = 0;
	foreach( fs::path entry, stat ) {
		num_entries++;
	}
	BOOST_CHECK( num_entries == 1 );
	num_entries = 0;
	stat.clear();
	stat.scan_directory( ".test", SCAN_MODE_RECURSIVE | SCAN_MODE_DIRS );
	foreach( fs::path entry, stat ) {
		num_entries++;
	}
	BOOST_CHECK( num_entries == 3 );
	fs::remove( ".test/one/kekse" );
	fs::remove( ".test/one" );
	fs::remove( ".test/two" );
	fs::remove( ".test/file" );
	fs::remove( ".test" );
}

BOOST_AUTO_TEST_CASE( test_remove_directories ) {
	fs::create_directory( ".test" );
	fs::create_directory( ".test/one" );
	fs::create_directory( ".test/one/kekse" );
	fs::create_directory( ".test/one/kekse/beer" );
	fs::create_directory( ".test/two" );
	remove_directories( ".test" );
	BOOST_CHECK( !fs::is_directory(".test/one") );
	BOOST_CHECK( !fs::is_directory(".test/two") );
	BOOST_CHECK( fs::is_directory(".test") );
	fs::remove( ".test" );
}

BOOST_AUTO_TEST_CASE( test_Stat_renumber ) {
	fs::create_directory( ".test" );
	fs::create_directory( ".test/in" );
	fs::create_directory( ".test/out" );
	std::ofstream file( "file.jpg" );
	BOOST_CHECK( !file.fail() );
	file.close();
	fs::copy_file( "file.jpg", fs::path(".test/in") / "Aone.jpg" );
	fs::copy_file( "file.jpg", fs::path(".test/in") / "Btwo.jpg" );
	fs::copy_file( "file.jpg", fs::path(".test/in") / "Cthr.jpg" );
	fs::copy_file( "file.jpg", fs::path(".test/in") / "Dfou.jpg" );
	fs::copy_file( "file.jpg", fs::path(".test/in") / "Efiv.jpg" );
	fs::remove( "file.jpg" );
	Stat status;
	status.scan_directory( fs::path(".test/in"), SCAN_MODE_FILES | SCAN_MODE_RECURSIVE );
	status.renumber( fs::path(".test/out"), 0, MOVE );
	BOOST_CHECK( fs::is_regular(".test/out/00000.jpg") );
	BOOST_CHECK( fs::is_regular(".test/out/00001.jpg") );
	BOOST_CHECK( fs::is_regular(".test/out/00002.jpg") );
	BOOST_CHECK( fs::is_regular(".test/out/00003.jpg") );
	BOOST_CHECK( fs::is_regular(".test/out/00004.jpg") );
	fs::remove( ".test/out/00000.jpg" );
	fs::remove( ".test/out/00001.jpg" );
	fs::remove( ".test/out/00002.jpg" );
	fs::remove( ".test/out/00003.jpg" );
	fs::remove( ".test/out/00004.jpg" );
	remove_directories( ".test" );
	fs::remove( ".test" );
}

BOOST_AUTO_TEST_CASE( test_check_directory ) {
	BOOST_CHECK( check_directory( fs::path(".test"), 0 ) != 0 );
	BOOST_CHECK( check_directory( fs::path(".test"), CHECK_CREATE ) == 0 );
	BOOST_CHECK( check_directory( fs::path(".test/"), 0 ) == 0 );
	std::ofstream file( "file.jpg" );
	BOOST_CHECK( !file.fail() );
	file.close();
	fs::rename( "file.jpg", fs::path(".test") / "file.jpg" );
	BOOST_CHECK( check_directory( fs::path(".test"), CHECK_CLEAR ) == 0 );
	BOOST_CHECK( !fs::exists(fs::path(".test/file.jpg")) );
	fs::remove( ".test" );
}
