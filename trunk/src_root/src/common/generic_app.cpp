/**
 * @file common/generic_app.cpp
 * generic application
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: generic_app.cpp 781 2006-10-07 19:52:08Z zak $

 * See LICENSE for details
 */

#include "common/common_stdafx.h"
#include "common/generic_app.h"
#include "common/miscutils.h"

#include <locale.h>

#ifdef __UNIX__
#include <unistd.h>
#include <stdlib.h>
#endif

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

bool CGenericApp::initI10N(const char* textDomain)
{
	//Gettext initialization
	setlocale( LC_ALL, "" );
	textdomain (textDomain);
	bindtextdomain ("common", "../../locale");
	bindtextdomain (textDomain, "../../locale");
	return true;
}

//-----------------------------------------------------------------------

bool CGenericApp::printPIDToFile(const char *fileName)
{
	return ireon::misc::printPIDToFile(fileName);
}

//-----------------------------------------------------------------------

