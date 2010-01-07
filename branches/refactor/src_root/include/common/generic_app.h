/**
 * @file generic_app.h
 * Contains generic application class
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

#ifndef _GENERIC_APP_H
#define _GENERIC_APP_H

#include "stdafx.h"
#include "common/xmlconfig.h"

class CGenericApp
{
public:
	CGenericApp();
	~CGenericApp();

protected:
	/// configuration options storage
	CXMLConfig* m_config;

	// some utility functions
	void exitProcess(byte status = 0);
	void sleep(uint msecs);

	void setAppName(const String& name);
	String getAppName();

	bool initI10N();
	virtual bool initLogSystem();
	bool printPIDToFile();
	bool loadXMLConfigFile(const char *filename, bool useAsDefault = false);

private:
	String	appName;

};

#endif
