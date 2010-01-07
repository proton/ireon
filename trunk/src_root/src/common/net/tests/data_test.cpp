/**
  * @file data_test.cpp
  * Tests for CData class
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: data_test.cpp 623 2006-05-05 20:19:22Z mip $
 
  *  See LICENSE for details
  */
 
 
 #include "boost/test/unit_test.hpp"
 using boost::unit_test::test_suite;
 
 
 #include "stdafx.h"
 #include "net/data.h"
 
 void testCDataConstructor()
 {
 	{
 		CData data;
 
 		BOOST_CHECK( data.store() );
 		BOOST_CHECK( data.length() == 0 );
 //		BOOST_CHECK( data.data() == NULL );
 
 		data.reset(false);
 		BOOST_CHECK( !data.store() );
 		BOOST_CHECK( data.length() == 0 );
 //		BOOST_CHECK( data.data() == NULL );
 	}
 	{
 		CData data("asdfg", 5, true);
 		BOOST_CHECK( data.store() );
 		BOOST_CHECK( data.length() == 5 );
 		BOOST_CHECK( !strncmp(data.data(), "asdfg", 5) );
 	}
 	{
 		CData data("asdfg", 0, true);
 		BOOST_CHECK( data.store() );
 		BOOST_CHECK( data.length() == 0 );
 //		BOOST_CHECK( data.data() == NULL );
 	}
 }
 
 void testCDataWrite()
 {
 	{
 		CData data;
 
 		data.write("abc",0);
 		BOOST_CHECK( data.length() == 0 );
 //		BOOST_CHECK( data.data() == NULL );
 
 		data.write("abc",3);
 		BOOST_CHECK( data.length() == 3 );
 		BOOST_CHECK( !strncmp(data.data(), "abc", 3) );
 
 		data.reset(false);
 		BOOST_CHECK( data.length() == 0 );
 		BOOST_CHECK( !strncmp(data.data(), "abc", 3) );
 		BOOST_CHECK( !data.store() );
 
 		data.reset(true);
 		BOOST_CHECK( data.write("abcde",5) == 5 );
 		BOOST_CHECK( data.length() == 8 );
 		BOOST_CHECK( !strncmp(data.data(), "abcabcde", 8) );
 
 		// addending data
 		data.append("abcdefabcdefabcdefabcdefabcdef",30);
 		BOOST_CHECK( data.length() == 38 );
 		BOOST_CHECK( !strncmp(data.data(), "abcabcdeabcdefabcdefabcdefabcdefabcdef", 38) );
 
 	}
 }
 
 void testCDataRead()
 {
 	{
 		CData data("abcdefabcdefabcdefabcdefabcdef", 30, false);
 
 		char *str;
 		str = new char[50];
 		BOOST_CHECK( data.read(str, data.size()) == data.size());
 		BOOST_CHECK( !strncmp(str, "abcdefabcdefabcdefabcdefabcdef", data.size()) );
 		delete str;
 	}
 	{
 		// read less
 		CData data("abcdefabcdefabcdefabcdefabcdef", 30, false);
 
 		char *str;
 		str = new char[50];
 		BOOST_CHECK( data.read(str, 20) == 20);
 		BOOST_CHECK( !strncmp(str, "abcdefabcdefabcdefabcdefabcdef", 20) );
 		delete str;
 	}
 	{
 		// read more
 		CData data("abcdefabcdefabcdefabcdefabcdef", 30, false);
 
 		char *str;
 		str = new char[50];
 		BOOST_CHECK( data.read(str, 40) == data.size());
 		BOOST_CHECK( !strncmp(str, "abcdefabcdefabcdefabcdefabcdef", data.size()) );
 		delete str;
 	}
 	{
 		CData data("abcdefabcdefabcdefabcdefabcdef", 30, false);
 
 		char *str;
 		str = new char[50];
 		// read null
 		BOOST_CHECK( data.read(str, 0) == 0);
 		BOOST_CHECK( !strncmp(str, "abcdefabcdefabcdefabcdefabcdef", 0) );
 		// read some
 		BOOST_CHECK( data.read(str, 20) == 20);
 		BOOST_CHECK( !strncmp(str, "abcdefabcdefabcdefabcdefabcdef", 20) );
 		// and more
 		BOOST_CHECK( data.read(str, 20) == data.size()-20);
 		BOOST_CHECK( !strncmp(str, "cdefabcdef", data.size()-20) );
 		delete str;
 	}
 }
 
 void testCDataGetS()
 {
 	{
 		CData data("abcdefabcdefabcdefabcdefabcdef", 30, false);
 
 		std::string str;
 		data.getS(data.size(), str);
 		
 		BOOST_CHECK( str.length() == data.size() );
 		BOOST_CHECK( !strncmp(str.c_str(), "abcdefabcdefabcdefabcdefabcdef", data.size()) );
 	}
 	{
 		// read less
 		CData data("abcdefabcd\nefabcdefabcdefabcdef", 31, false);
 
 		std::string str;
 		data.getS(data.size(), str);
 
 		BOOST_CHECK( str.length() == 10 );
 		BOOST_CHECK( !strncmp(str.c_str(), "abcdefabcd\nefabcdefabcdefabcdef", 10) );
 	}
 	{
 		// read less
 		CData data("abcdefabcd\nefabcdefabcdefabcdef", 31, false);
 
 		std::string str;
 		data.getS(8, str);
 		
 		BOOST_CHECK( str.length() == 8 );
 		BOOST_CHECK( !strncmp(str.c_str(), "abcdefabcd\nefabcdefabcdefabcdef", 8) );
 	}
 	{
 		// read exact
 		CData data("abcdefabcd\nefabcdefabcdefabcdef", 31, false);
 
 		std::string str;
 		data.getS(11, str);
 
 		BOOST_CHECK( str.length() == 10 );
 		BOOST_CHECK( !strncmp(str.c_str(), "abcdefabcd\nefabcdefabcdefabcdef", 10) );
 	}
 	{
 		// really complex test
 		CData data("\nabcde\n\n\nfabcd\nasdfasdfsadfasdfghjklqwer", 40, false);
 
 		std::string str;
 		// first empty string
 		data.getS(data.size(), str, 32);
 		BOOST_CHECK( str.length() == 0 );
 
 		// normal string
 		data.getS(data.size(), str, 32);
 		BOOST_CHECK( str.length() == 5 );
 		BOOST_CHECK( !strncmp(str.c_str(), "abcde", 5) );
 
 		// get empty string, skip next empty string
 		data.getS(data.size(), str, 32);
 		BOOST_CHECK( str.length() == 0 );
 
 		// normal string
 		data.getS(data.size(), str, 32);
 		BOOST_CHECK( str.length() == 5 );
 		BOOST_CHECK( !strncmp(str.c_str(), "fabcd", 5) );
 
 		// part of string
 		data.getS(2, str, 32);
 		BOOST_CHECK( str.length() == 2 );
 		BOOST_CHECK( !strncmp(str.c_str(), "as", 2) );
 
 		// string to the end of data with small buffer
 		data.getS(40, str, 16);
 		BOOST_CHECK( str.length() == 23 );
 		BOOST_CHECK( !strncmp(str.c_str(), "dfasdfsadfasdfghjklqwer", 23) );
 
 	}
 	{
 		// test string exactly two times larger than buffer
 		CData data("asdfgasdfgasdfgasdfgasdfgasdfgas", 32, false);
 
 		std::string str;
 		data.getS(data.size(), str, 16);
 		BOOST_CHECK( str.length() == 32 );
 		BOOST_CHECK( !strncmp(str.c_str(), "asdfgasdfgasdfgasdfgasdfgasdfgas", 32) );
 	}
 	{
 		CData data("asdfgasdfgasdfgasdfgasdfgasdfgas\n", 33, false);
 
 		std::string str;
 		data.getS(data.size(), str, 16);
 		BOOST_CHECK( str.length() == 32 );
 		BOOST_CHECK( !strncmp(str.c_str(), "asdfgasdfgasdfgasdfgasdfgasdfgas", 32) );
 	}
 }
 
 void testCDataLength()
 {
 	{
 		CData data("abcdefabcdefabcdefabcdefabcdef", 30, false);
 
 		char *str;
 		str = new char[50];
 		BOOST_CHECK( data.length() == 0 );
 		// read null
 		BOOST_CHECK( data.read(str, 0) == 0);
 		BOOST_CHECK( data.length() == 0 );
 		// read some
 		BOOST_CHECK( data.read(str, 20) == 20);
 		BOOST_CHECK( data.length() == 20 );
 		// and more
 		BOOST_CHECK( data.read(str, 20) == data.size()-20);
 		BOOST_CHECK( data.length() == data.size());
 		delete str;
 	}
 }
 
 
 /* ---------------------------------------------------- */
 
 test_suite* init_unit_test_suite( int /*argc*/, char* /*argv*/[] ) {
 	test_suite* test = BOOST_TEST_SUITE("CData test");
 
 	test->add( BOOST_TEST_CASE( &testCDataConstructor ) );
 	test->add( BOOST_TEST_CASE( &testCDataWrite ));
 	test->add( BOOST_TEST_CASE( &testCDataRead ));
 	test->add( BOOST_TEST_CASE( &testCDataGetS ));
 	test->add( BOOST_TEST_CASE( &testCDataLength ));
 	return test;
 }
