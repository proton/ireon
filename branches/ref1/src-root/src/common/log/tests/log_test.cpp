/**
 * @file data_test.cpp
 * Tests for CData class
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: log_test.cpp 510 2006-02-26 21:09:40Z zak $

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
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
