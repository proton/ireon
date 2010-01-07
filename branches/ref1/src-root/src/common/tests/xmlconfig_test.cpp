/**
 * @file xmlconfig_test.cpp
 * Tests for CXMLConfig class
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: xmlconfig_test.cpp 510 2006-02-26 21:09:40Z zak $

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


#include "xmlconfig.h"
#include "tinyxml/tinyxml.h"

//-----------------------------------------------------------------------

void testCXMLConfigConstructor()
{

	// just test that no exceptions occur
	// default
	{
		CXMLConfig		config;
	}
	// char *
	{
		CXMLConfig		config("<a>b</a>");
	}
	{
		CXMLConfig		config("");
	}
	// string
	{
		CXMLConfig		config(String("<a>b</a>"));
	}
	{
		CXMLConfig		config(String(""));
	}
}

//-----------------------------------------------------------------------

void testCXMLConfigLoad()
{
	// empty input
	{
		CXMLConfig		config;
		BOOST_CHECK ( config.load("") == false );
	}
	{
		CXMLConfig		config;
		BOOST_CHECK ( config.load("<b/>") == true );
	}
    { // load value + default
        CXMLConfig		config;
        StringVector	reqOptions;
        reqOptions += "/a";

        // check empty container
        BOOST_CHECK( config.getFirstValue("/a") == "" );
        BOOST_CHECK( config.getFirstDefaultValue("/a") == "" );
        BOOST_CHECK( !config.checkRequiredOptions(reqOptions) );

		// empty key, value, both
        BOOST_CHECK( config.getFirstValue("") == "" );
        config.appendValue("","value");
        BOOST_CHECK( config.getFirstValue("") == "value" );
        config.load("<key></key>");
        BOOST_CHECK( config.getFirstValue("key") == "" );

        // adding value
        config.load("<a>value</a>");
        BOOST_CHECK( config.getFirstValue("/a") == "value" );
        BOOST_CHECK( config.getFirstDefaultValue("/a") == "" );
        BOOST_CHECK( config.checkRequiredOptions(reqOptions) );

        // adding default
        config.load("<a>default</a>", true);
        BOOST_CHECK( config.getFirstValue("/a") == "value" );
        BOOST_CHECK( config.getFirstDefaultValue("/a") == "default" );
        BOOST_CHECK( config.checkRequiredOptions(reqOptions) );
    }
}

//-----------------------------------------------------------------------

void testCXMLConfigMultipleValues()
{
    { // load default + value
        CXMLConfig		config;

        // adding default
        config.load("<a>default</a>", true);
        BOOST_CHECK( config.getFirstValue("/a") == "default" );
        BOOST_CHECK( config.getFirstDefaultValue("/a") == "default" );

        // adding value
        config.load("<a>value</a>");
        BOOST_CHECK( config.getFirstValue("/a") == "value" );
        BOOST_CHECK( config.getFirstDefaultValue("/a") == "default" );

        // add values
        // nothing in should change in first values

        // adding default
        config.load("<a>new default</a>", true);
        BOOST_CHECK( config.getFirstValue("/a") == "value" );
        BOOST_CHECK( config.getFirstDefaultValue("/a") == "default" );

        // adding value
        config.load("<a>new value</a>");
        BOOST_CHECK( config.getFirstValue("/a") == "value" );
        BOOST_CHECK( config.getFirstDefaultValue("/a") == "default" );

        // now check all values
        StringVector val = config.getAllValuesForKey("/a");
        StringVector::iterator i = val.begin();
        BOOST_CHECK( *i == "value" );
        ++i;
        BOOST_CHECK( *i == "new value" );
        ++i;
        BOOST_CHECK( i == val.end() );

        // check default values too
        StringVector def = config.getAllDefaultValuesForKey("/a");
        i = def.begin();
        BOOST_CHECK( *i == "default" );
        ++i;
        BOOST_CHECK( *i == "new default" );
        ++i;
        BOOST_CHECK( i == def.end() );
    }
    {
        // multiple values, first defaults then values
        CXMLConfig		config;
        StringVector	reqOptions;
        reqOptions += "/a";

        {
            // check default values -- should be empty
            StringVector def = config.getAllDefaultValuesForKey("/a");
            StringVector::iterator i = def.begin();
            BOOST_CHECK( i == def.end() );
            BOOST_CHECK( config.getFirstDefaultValue("/a") == "" );
        }

        // adding default
        config.load("<a>default</a>", true);
        BOOST_CHECK( config.getFirstValue("/a") == "default" );
        BOOST_CHECK( config.getFirstDefaultValue("/a") == "default" );
        BOOST_CHECK( config.checkRequiredOptions(reqOptions) );
        config.load("<a>new default</a>", true);
        BOOST_CHECK( config.getFirstValue("/a") == "default" );
        BOOST_CHECK( config.getFirstDefaultValue("/a") == "default" );
        BOOST_CHECK( config.checkRequiredOptions(reqOptions) );

        {
            // now check all values -- should be defaults
            StringVector val = config.getAllValuesForKey("/a");
            StringVector::iterator i = val.begin();
            BOOST_CHECK( *i == "default" );
            ++i;
            BOOST_CHECK( *i == "new default" );
            ++i;
            BOOST_CHECK( i == val.end() );
        }

        // adding value
        config.load("<a>value</a>");
        BOOST_CHECK( config.getFirstValue("/a") == "value" );
        BOOST_CHECK( config.getFirstDefaultValue("/a") == "default" );
        BOOST_CHECK( config.checkRequiredOptions(reqOptions) );
        config.load("<a>new value</a>");
        BOOST_CHECK( config.getFirstValue("/a") == "value" );
        BOOST_CHECK( config.getFirstDefaultValue("/a") == "default" );
        BOOST_CHECK( config.checkRequiredOptions(reqOptions) );

        {
            // now check all values
            StringVector val = config.getAllValuesForKey("/a");
            StringVector::iterator i = val.begin();
            BOOST_CHECK( *i == "value" );
            ++i;
            BOOST_CHECK( *i == "new value" );
            ++i;
            BOOST_CHECK( i == val.end() );
        }
        {
            // check default values too
            StringVector def = config.getAllDefaultValuesForKey("/a");
            StringVector::iterator i = def.begin();
            BOOST_CHECK( *i == "default" );
            ++i;
            BOOST_CHECK( *i == "new default" );
            ++i;
            BOOST_CHECK( i == def.end() );
        }
    }

    {
        // multiple values, first values then defaults
        CXMLConfig		config;

        // adding value
        config.load("<a>value</a>");
        BOOST_CHECK( config.getFirstValue("/a") == "value" );
        BOOST_CHECK( config.getFirstDefaultValue("/a") == "" );
        config.load("<a>new value</a>");
        BOOST_CHECK( config.getFirstValue("/a") == "value" );
        BOOST_CHECK( config.getFirstDefaultValue("/a") == "" );

        {
            // now check all values
            StringVector val = config.getAllValuesForKey("/a");
            StringVector::iterator i = val.begin();
            BOOST_CHECK( *i == "value" );
            ++i;
            BOOST_CHECK( *i == "new value" );
            ++i;
            BOOST_CHECK( i == val.end() );
        }

        {
            // now check all default values -- should be empty
            StringVector val = config.getAllDefaultValuesForKey("/a");
            StringVector::iterator i = val.begin();
            BOOST_CHECK( i == val.end() );
        }

        // adding default
        config.load("<a>default</a>", true);
        BOOST_CHECK( config.getFirstValue("/a") == "value" );
        BOOST_CHECK( config.getFirstDefaultValue("/a") == "default" );
        config.load("<a>new default</a>", true);
        BOOST_CHECK( config.getFirstValue("/a") == "value" );
        BOOST_CHECK( config.getFirstDefaultValue("/a") == "default" );

        {
            // now check all values
            StringVector val = config.getAllValuesForKey("/a");
            StringVector::iterator i = val.begin();
            BOOST_CHECK( *i == "value" );
            ++i;
            BOOST_CHECK( *i == "new value" );
            ++i;
            BOOST_CHECK( i == val.end() );
        }
        {
            // check default values too
            StringVector def = config.getAllDefaultValuesForKey("/a");
            StringVector::iterator i = def.begin();
            BOOST_CHECK( *i == "default" );
            ++i;
            BOOST_CHECK( *i == "new default" );
            ++i;
            BOOST_CHECK( i == def.end() );
        }
    }
};

//-----------------------------------------------------------------------

void testCXMLConfigOverload()
{
    {
        // first value and default overload
        CXMLConfig		config;

        // adding value
        config.setFirstValue("/a", "value");
        BOOST_CHECK( config.getFirstValue("/a") == "value" );
        BOOST_CHECK( config.getFirstDefaultValue("/a") == "" );
        config.appendValue("/a", "some value");
        BOOST_CHECK( config.getFirstValue("/a") == "value" );
        BOOST_CHECK( config.getFirstDefaultValue("/a") == "" );
        config.setFirstValue("/a","new value");
        BOOST_CHECK( config.getFirstValue("/a") == "new value" );
        BOOST_CHECK( config.getFirstDefaultValue("/a") == "" );

        // adding default
        config.setFirstValue("/a", "default", true);
        BOOST_CHECK( config.getFirstValue("/a") == "new value" );
        BOOST_CHECK( config.getFirstDefaultValue("/a") == "default" );
        config.appendValue("/a", "some default", true);
        BOOST_CHECK( config.getFirstValue("/a") == "new value" );
        BOOST_CHECK( config.getFirstDefaultValue("/a") == "default" );
        config.setFirstValue("/a", "new default", true);
        BOOST_CHECK( config.getFirstValue("/a") == "new value" );
        BOOST_CHECK( config.getFirstDefaultValue("/a") == "new default" );
    }
}

//-----------------------------------------------------------------------

void testCXMLConfigPaths()
{
    {
        CXMLConfig		config;

        // adding value
        config.setFirstValue("/t/e/s/t/ key ", " value ");
        BOOST_CHECK( config.getFirstValue("/t/e/s/t/ key ") == " value " );
        BOOST_CHECK( config.getFirstDefaultValue("/t/e/s/t/ key ") == "" );
        config.appendValue("/t/e/s/t/ key ", "some value");
        BOOST_CHECK( config.getFirstValue("/t/e/s/t/ key ") == " value " );
        BOOST_CHECK( config.getFirstDefaultValue("/t/e/s/t/ key ") == "" );
        config.setFirstValue("/t/e/s/t/ key ","new value");
        BOOST_CHECK( config.getFirstValue("/t/e/s/t/ key ") == "new value" );
        BOOST_CHECK( config.getFirstDefaultValue("/t/e/s/t/ key ") == "" );
        {
            // now check all values
            StringVector val = config.getAllValuesForKey("/t/e/s/t/ key ");
            StringVector::iterator i = val.begin();
            BOOST_CHECK( *i == "new value" );
            ++i;
            BOOST_CHECK( *i == "some value" );
            ++i;
            BOOST_CHECK( i == val.end() );
        }

        // adding default
        config.setFirstValue("/t/e/s/t/ key ", "default", true);
        BOOST_CHECK( config.getFirstValue("/t/e/s/t/ key ") == "new value" );
        BOOST_CHECK( config.getFirstDefaultValue("/t/e/s/t/ key ") == "default" );
        config.appendValue("/t/e/s/t/ key ", "some default", true);
        BOOST_CHECK( config.getFirstValue("/t/e/s/t/ key ") == "new value" );
        BOOST_CHECK( config.getFirstDefaultValue("/t/e/s/t/ key ") == "default" );
        config.setFirstValue("/t/e/s/t/ key ", "new default", true);
        BOOST_CHECK( config.getFirstValue("/t/e/s/t/ key ") == "new value" );
        BOOST_CHECK( config.getFirstDefaultValue("/t/e/s/t/ key ") == "new default" );
        {
            // now check all values
            StringVector val = config.getAllDefaultValuesForKey("/t/e/s/t/ key ");
            StringVector::iterator i = val.begin();
            BOOST_CHECK( *i == "new default" );
            ++i;
            BOOST_CHECK( *i == "some default" );
            ++i;
            BOOST_CHECK( i == val.end() );
        }
	}
}

//-----------------------------------------------------------------------

void testCXMLConfigComplexFormatting()
{
    {
        CXMLConfig		config;

        config.load("<a>a<b/><c>c</c>a2<d> <e>\n \t\t e \n\te \t\n </e> </d></a>", false, "/test/");
        BOOST_CHECK( config.getFirstValue("/test/a") == "a" );
        {
            StringVector val = config.getAllValuesForKey("/test/a");
            StringVector::iterator i = val.begin();
            BOOST_CHECK( *i == "a" );
            ++i;
            BOOST_CHECK( *i == "a2" );
            ++i;
            BOOST_CHECK( i == val.end() );
        }
        BOOST_CHECK( config.getFirstValue("/test/a/b") == "" );
        BOOST_CHECK( config.getFirstValue("/test/a/c") == "c" );
        BOOST_CHECK( config.getFirstValue("/test/a/d") == "" );
        BOOST_CHECK( config.getFirstValue("/test/a/d/e") == "e e" );
	}
}

//-----------------------------------------------------------------------

void testCXMLConfigIncorrectFormatting()
{
    {
        CXMLConfig		config;

        BOOST_CHECK( config.load("<a>b") == false );
	}
}

//-----------------------------------------------------------------------

test_suite* init_unit_test_suite( int /*argc*/, char* /*argv*/[] ) {
	test_suite* test = BOOST_TEST_SUITE("CXMLConfig test");

	test->add( BOOST_TEST_CASE( &testCXMLConfigConstructor ) );
	test->add( BOOST_TEST_CASE( &testCXMLConfigLoad ) );
	test->add( BOOST_TEST_CASE( &testCXMLConfigMultipleValues ) );
	test->add( BOOST_TEST_CASE( &testCXMLConfigOverload ) );
	test->add( BOOST_TEST_CASE( &testCXMLConfigPaths ) );
	test->add( BOOST_TEST_CASE( &testCXMLConfigComplexFormatting ) );
	test->add( BOOST_TEST_CASE( &testCXMLConfigIncorrectFormatting ) );
	return test;
}
