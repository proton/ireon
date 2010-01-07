/**
 * @file data_test.cpp
 * Tests for CConfig class
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: config_test.cpp 510 2006-02-26 21:09:40Z zak $

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
#include "config.h"


/* ---------------------------------------------------- */


void testCConfigLoad()
{
	{	// simple load instructions
		CConfig		config;
		
		{	// new
			char		buffer[60] = "a=b";
			CData		data(buffer, strlen(buffer), false);
			config.load(data);
			BOOST_CHECK( config.getSetting("a") == "b" );
			BOOST_CHECK( config.getSetting("c") == "" );
		}
		{	// append
			char		buffer[60] = "c=d\n\n";
			CData		data(buffer, strlen(buffer), false);
			config.load(data);
			BOOST_CHECK( config.getSetting("a") == "b" );
			BOOST_CHECK( config.getSetting("c") == "d" );
		}
		{	// overload
			char		buffer[60] = "c=b";
			CData		data(buffer, strlen(buffer), false);
			config.load(data);
			BOOST_CHECK( config.getSetting("a") == "b" );
			BOOST_CHECK( config.getSetting("c") == "b" );
		}
		{	// clear
			config.clear();
			BOOST_CHECK( config.getSetting("a") == "" );
			BOOST_CHECK( config.getSetting("c") == "" );

			char		buffer[60] = "c=b\n";
			CData		data(buffer, strlen(buffer), false);
			config.load(data);
			BOOST_CHECK( config.getSetting("a") == "" );
			BOOST_CHECK( config.getSetting("c") == "b" );
		}
		{	// empty key & value
			config.clear();
			char		buffer[60] = "c=\n=d";
			CData		data(buffer, strlen(buffer), false);
			config.load(data);
			BOOST_CHECK( config.getSetting("c") == "" );
			BOOST_CHECK( config.getSetting("") == "d" );
		}
		{	// empty everything :)
			config.clear();
			char		buffer[60] = "=";
			CData		data(buffer, strlen(buffer), false);
			config.load(data);
			BOOST_CHECK( config.getSetting("") == "" );
		}
		{	// load empty string
			config.clear();
			char		buffer[60] = "";
			CData		data(buffer, strlen(buffer), false);
			config.load(data);
			BOOST_CHECK( config.getSetting("") == "" );
		}
	}

	{	// heavy formatting
		CConfig		config;
		
		{	// new
			char		buffer[60] = "a=b\n\n\t\t#c=e\nc=\td";
			CData		data(buffer, strlen(buffer), false);
			config.load(data);
			BOOST_CHECK( config.getSetting("a") == "b" );
			BOOST_CHECK( config.getSetting("c") == "d" );
		}
		{	// append
			char		buffer[60] = "\n\n\t\te\t\t=\t\tf\n\n\t\t#g=i\n\tg\t=\th\n\n";
			CData		data(buffer, strlen(buffer), false);
			config.load(data);
			BOOST_CHECK( config.getSetting("a") == "b" );
			BOOST_CHECK( config.getSetting("c") == "d" );
			BOOST_CHECK( config.getSetting("e") == "f" );
			BOOST_CHECK( config.getSetting("g") == "h" );
		}
		{	// with spaces
			char		buffer[60] = "\n\ta b\t\t=\tc d\t\n\n\t\t#e f=g h\n\t e f \t = \t g h \n\n";
			CData		data(buffer, strlen(buffer), false);
			config.load(data);
			BOOST_CHECK( config.getSetting("a b") == "c d" );
			BOOST_CHECK( config.getSetting("a") == "b" );
			BOOST_CHECK( config.getSetting("e f") == "g h" );
		}
		{	// with multiple =
			char		buffer[60] = "\n\ta b c\t\t=\tc = d\t\n\n";
			CData		data(buffer, strlen(buffer), false);
			config.load(data);
			BOOST_CHECK( config.getSetting("a b c") == "c = d" );
		}
	}
}

//-----------------------------------------------------------------------

void testCConfigSections()
{
	{	
		CConfig		config;

		{	
			// get from empty section
			BOOST_CHECK( config.getSetting("a", "nonexistentsect") == "" );

			// get from empty key
			BOOST_CHECK( config.getSetting("a") == "" );
			
			// new
			config.setSetting("a", "b", "mysect");
			BOOST_CHECK( config.getSetting("a") == "" );
			BOOST_CHECK( config.getSetting("a", "mysect") == "b" );

			// overload
			config.setSetting("a", "c", "mysect");
			BOOST_CHECK( config.getSetting("a") == "" );
			BOOST_CHECK( config.getSetting("a", "mysect") == "c" );

			// get from empty section
			BOOST_CHECK( config.getSetting("a", "nonexistentsect") == "" );

			// preserve whitespaces
			config.setSetting(" a\tb ", "\t c \t", " my\tsect ");
			BOOST_CHECK( config.getSetting(" a\tb ") == "" );
			BOOST_CHECK( config.getSetting(" a\tb ", " my\tsect ") == "\t c \t" );
		}

	}
}

//-----------------------------------------------------------------------

void testCConfigConstructor()
{
	CConfig		config;
}

//-----------------------------------------------------------------------

test_suite* init_unit_test_suite( int /*argc*/, char* /*argv*/[] ) {
	test_suite* test = BOOST_TEST_SUITE("CConfig test");

	test->add( BOOST_TEST_CASE( &testCConfigConstructor ) );
	test->add( BOOST_TEST_CASE( &testCConfigLoad ) );
	test->add( BOOST_TEST_CASE( &testCConfigSections ) );
	return test;
}
