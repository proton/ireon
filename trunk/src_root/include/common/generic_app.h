/**
 * @file common/generic_app.h
 * Contains generic application class
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: generic_app.h 781 2006-10-07 19:52:08Z zak $

 *  See LICENSE for details
 */

#ifndef _GENERIC_APP_H
#define _GENERIC_APP_H

#include "common/common_stdafx.h"
//#include "common/xmlconfig.h"
//class CXMLConfig;

class CGenericApp
{
protected:
	CGenericApp() {};
	
	// some utility functions
	void exitProcess(byte status = 0);
	void sleep(uint msecs);

	bool initI10N(const char* textDomain);
	bool printPIDToFile(const char *fileName);

};

#endif
