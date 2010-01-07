/**
 * @file ireon_rs/main.cpp
 * Root server application bootstrapper
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: main.cpp 782 2006-10-09 18:51:33Z zak $

 * See LICENSE for details
 */


#include "ireon_rs/stdafx.h"
#include "ireon_rs/root_app.h"
#include "ireon_rs/version.h"

#include "common/xsd/common_xsd.h"

#pragma warning(push)
#pragma warning(disable:4100 4512)
#include "tclap/CmdLine.h"
#pragma warning(pop)
//#include <iostream>

#define APPLICATION_CLASS CRootApp
#include "common/os_signal_handlers.h"
#undef APPLICATION_CLASS

#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/ndc.h>

//#include <log4cxx/helpers/exception.h>
//#include <log4cxx/ndc.h>

int main(int argc, char** argv)
{
	log4cxx::BasicConfigurator::configure();
	log4cxx::LoggerPtr logger = log4cxx::Logger::getRootLogger();
	log4cxx::NDC::push("");

	byte exitStatus = EXIT_SUCCESS;
	try {  

		TCLAP::CmdLine cmd( String(
			"\nireon.org root server, version ") + PRODUCT_VERSION_STR + "\n" +
			"Copyright (C) 2005-2006 ireon.org developers council \n\n" +
			"This program is free software; you can redistribute it and/or " +
			"modify it under the terms of the GNU General Public License " +
			"as published by the Free Software Foundation; either version 2 " +
			"of the License, or (at your option) any later version. \n" +

			"This program is distributed in the hope that it will be useful, " +
			"but WITHOUT ANY WARRANTY; without even the implied warranty of " +
			"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the " +
			"GNU General Public License for more details. \n" +

			"You should have received a copy of the GNU General Public License " +
			"along with this program; if not, write to the Free Software " +
			"Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.", 
			' ', 
			String(PRODUCT_VERSION_STR) + " [" + __TIMESTAMP__ + "]");
		TCLAP::ValueArg<String> cfgArg("c","config","Configuration file path. Default: ../config/config.xml",false,"../config/config.xml","path", cmd);
		TCLAP::ValueArg<String> logCfgArg("l","log_config","Log4cxx configuration file path. Default: ../config/log4cxx.xml",false,"../config/log4cxx.xml","path", cmd);
		cmd.parse( argc, argv );

		log4cxx::xml::DOMConfigurator::configureAndWatch(logCfgArg.getValue()); 
		LOG4CXX_INFO(logger, "Starting ireon.org root server...");

		CRootApp app(cfgArg.getValue());
		SET_OS_SIGNAL_HANDLERS;
		app.go();

	} catch (TCLAP::ArgException &e)  // catch command line parser exceptions
	{ 
		LOG4CXX_FATAL(logger, e.error());
		exitStatus = EXIT_FAILURE;
	}
	catch (const xml_schema::exception& e)
	{
		LOG4CXX_FATAL(logger, "XML file parsing error. Details follow:");
		LOG4CXX_FATAL(logger, e.what());
		exitStatus = EXIT_FAILURE;
	}
	catch (CRootApp::EInitError&)
	{
		LOG4CXX_FATAL(logger, "Root server initialization error occurred!");
		exitStatus = EXIT_FAILURE;
	}

	LOG4CXX_INFO(logger, "ireon.org root server stopped.");
	return exitStatus;
}

