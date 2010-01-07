/**
 * @file generic_app.cpp
 * generic application
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id$

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

#include "stdafx.h"
#include "common/generic_app.h"
#include "common/file/file.h"
#include "common/xmlconfig.h"
#include "common/miscutils.h"

#ifdef __UNIX__
#include <unistd.h>
#include <stdlib.h>
#endif


CGenericApp::CGenericApp()
{
	setAppName("generic_app");
	m_config = new CXMLConfig;
}

//-----------------------------------------------------------------------

CGenericApp::~CGenericApp()
{
	delete m_config;
}

//-----------------------------------------------------------------------

void CGenericApp::exitProcess(byte status)
{
	#ifdef __WIN32__
	ExitProcess(status);
	#elif defined(__UNIX__)
	exit(status);
	#endif
}

//-----------------------------------------------------------------------

void CGenericApp::sleep(uint msecs)
{
	#ifdef __WIN32__
	Sleep(msecs);
	#elif defined __UNIX__
	usleep(msecs*1000);
	#endif
}

//-----------------------------------------------------------------------

void CGenericApp::setAppName(const String& name)
{
	appName = name;
}

//-----------------------------------------------------------------------

String CGenericApp::getAppName()
{
	return appName;
}

//-----------------------------------------------------------------------

bool CGenericApp::initI10N()
{
	//Gettext initialization
	setlocale( LC_ALL, "" );
	bindtextdomain( "ireon", "locale" );
	textdomain( "ireon" );
	return true;
}

//-----------------------------------------------------------------------

bool CGenericApp::initLogSystem()
{
	CLog::instance()->regListener(new CFileLogListener("../logs/log_common.txt"));
	CLog::instance()->regListener(new CFileLogListener("../logs/log_net.txt",CLog::msgFlagNetwork));
	assert(appName.length());
	CLog::instance()->regListener(new CScreenLogListener(appName.c_str()));
	return true;
}

//-----------------------------------------------------------------------

bool CGenericApp::printPIDToFile()
{
	String	pidfile = getAppName() + ".pid";
	return ireon::misc::printPIDToFile(pidfile.c_str());
}

//-----------------------------------------------------------------------

bool CGenericApp::loadXMLConfigFile(const char *filename, bool useAsDefault)
{
	// to be moved
	FilePtr f(new CFile);
	if( !f )
		return false;
	f->open(filename,"r");
	if( !f->isOpen() )
		return false;

	char* configFileContents;
	configFileContents = new char[f->getSize()];
	f->read(configFileContents,f->getSize());

	if (!m_config->load(configFileContents,useAsDefault,"/"))
		return false;

	return true;
}

