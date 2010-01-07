/**
  * @file data_test.cpp
  * Tests for CData class
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: log_test.cpp 623 2006-05-05 20:19:22Z mip $
 
  *  See LICENSE for details
  */
 
 
 #include "boost/test/unit_test.hpp"
 using boost::unit_test::test_suite;
 
 
 #include "stdafx.h"
 #include "net/data.h"
 
 void testCDataConstructor()
 {
 	{
 	}
 }
 
 /* ---------------------------------------------------- */
 
 test_suite* init_unit_test_suite( int /*argc*/, char* /*argv*/[] ) {
 	test_suite* test = BOOST_TEST_SUITE("CLog test");
 
 	CLog::instance()->regListener(new CTestLogListener);
 	CLog::instance()->regListener(new CTestLogListener);
 	CLog::instance()->regListener(new CTestLogListener);
 	CLog::instance()->regListener(new CTestLogListener);
 	CLog::instance()->regListener(new CTestLogListener);
 	CLog::instance()->regListener(new CTestLogListener);
 	CLog::instance()->regListener(new CTestLogListener);
 	CLog::instance()->close();
 //	CLog::instance()->regListener(lst);
 //	CLog::instance()->close();
 
 //	delete lst;
 
 //	test->add( BOOST_TEST_CASE( &testCDataConstructor ) );
 	return test;
 }
