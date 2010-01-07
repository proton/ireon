/**
 * @file mysql_db.cpp
 * general mysql connection manager
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
#include "common/db/mysql_db.h"

#include <mysql++.h>
#include <mysql.h>

CMySQLConnection* CMySQLConnection::m_singleton = NULL;

CMySQLConnection::CMySQLConnection():
m_tableNamesPrefix("")
{
}

// ------------------------------------------------------------

bool CMySQLConnection::init(const char* db, const char *user, const char *passwd, const char* host, uint port)
{

	if (m_dbConnection.connected())
		return true;
	try
	{
		CLog::instance()->log(CLog::msgFlagMysql, CLog::msgLvlTrace,_("CMySQLConnection::init: Connecting to db..."));
		m_dbConnection.connect(db, host, user, passwd, port);
		CLog::instance()->log(CLog::msgFlagMysql, CLog::msgLvlTrace,_("Done."));
		return true;
	}
	catch (mysqlpp::ConnectionFailed) 
	{
		CLog::instance()->log(CLog::msgFlagMysql, CLog::msgLvlError,_("Connection failed, reason: %s\n"),_(m_dbConnection.error()));
		return false;
	}
}

// ------------------------------------------------------------

mysqlpp::Query CMySQLConnection::getQuery()
{
	assert(m_dbConnection.connected());
	return m_dbConnection.query();
}

// ------------------------------------------------------------

void CMySQLConnection::flush()
{

}

// ------------------------------------------------------------

void CMySQLConnection::setTableNamesPrefix(const String prefix)
{
	m_tableNamesPrefix = prefix;
}

// ------------------------------------------------------------

String CMySQLConnection::getTableNamesPrefix()
{
	return m_tableNamesPrefix;
}

// ------------------------------------------------------------

String CMySQLConnection::quoteString(const String input)
{
	char* buf = new char[input.size() * 2 + 1];
	mysql_escape_string(buf, input.c_str(),
			static_cast<unsigned long>(input.size()));
	String output = "'";
	output += buf;
	delete[] buf;
	output += "'";
	return output;
}
