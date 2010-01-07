/**
 * @file ireon_client/main.cpp
 * Main project .cpp file.
 * Contains "main" function that does some initialization actions
 * and CtrlHandler function that handles Ctrl key presses (for win32).
 */


/* Copyright (C) 2005 ireon.org developers council
 * $Id: main.cpp 822 2006-11-22 13:34:49Z mip $

 *  See LICENSE for details
 */
#include "ireon_client/stdafx.h"

#include "ireon_client/client_app.h"

#pragma warning(push, 1)
#include <Ogre/main/OgreNoMemoryMacros.h>
#include "common/xsd/common_xsd.h"
#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/ndc.h>
#include <Ogre/main/OgreMemoryMacros.h>
#include "Ogre/main/Ogre.h"
#pragma warning(pop)

#define APPLICATION_CLASS CClientApp
#include "common/os_signal_handlers.h"
#undef APPLICATION_CLASS

#pragma warning(push)
#pragma warning(disable:4100 4512)
#include "tclap/CmdLine.h"
#pragma warning(pop)



/**
 * Main client procedure.
 * It creates application instance, initializes and runs it.
 * Also, for windows version it sets up handler for Ctrl key presses.
 * Most functionality is contained in application class.
 * @see CClientApp
 */
#ifdef __WIN32__
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char* argv[])
#endif
{
	//log4cxx::BasicConfigurator::configure();
	log4cxx::LoggerPtr logger = log4cxx::Logger::getRootLogger();
	log4cxx::NDC::push("");

	byte exitStatus = EXIT_SUCCESS;
	try {  
		TCLAP::CmdLine cmd( String(
				"\nireon.org player client, version ") + PRODUCT_VERSION_STR + "\n" +
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
				"Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.", ' ', PRODUCT_VERSION_STR);
		TCLAP::ValueArg<String> cfgArg("c","config","Configuration file path. Default: ../config/config.xml",false,"../config/config.xml","path", cmd);
		TCLAP::ValueArg<String> logCfgArg("l","log_config","Log4cxx configuration file path. Default: ../config/log4cxx.xml",false,"../config/log4cxx.xml","path", cmd);
		/*
		cmd.parse( argc, argv );
		*/
		log4cxx::xml::DOMConfigurator::configureAndWatch(logCfgArg.getValue()); 
		LOG4CXX_INFO(logger, "Starting ireon.org player client...");

		CClientApp app(cfgArg.getValue());

		if( !app.init() )
		{
			std::cerr << "Application initialization failed!" << std::endl;
			return 1;
		}
		SET_OS_SIGNAL_HANDLERS;
		app.go();
	}
	catch( Ogre::Exception& e )
	{
		LOG4CXX_ERROR(logger, e.getFullDescription().c_str());

		#ifdef __WIN32__
		MessageBox( NULL, e.getFullDescription().c_str(), "An OGRE exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#else
		std::cerr << "An OGRE exception has occured: " <<
		    e.getFullDescription().c_str() << std::endl;
		#endif
	}
	catch (TCLAP::ArgException &e)  // catch command line parser exceptions
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
	/*
	catch (CRootApp::EInitError&)
	{
		LOG4CXX_FATAL(logger, "Player Client initialization error occurred!");
		exitStatus = EXIT_FAILURE;
	}
*/
	LOG4CXX_INFO(logger, "ireon.org player client stopped.");
	return exitStatus;

}
