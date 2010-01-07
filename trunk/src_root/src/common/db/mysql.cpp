/**
 * @file common/db/mysql.cpp
 * general mysql connection manager
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: mysql.cpp 780 2006-10-07 16:01:08Z zak $
 
 *  See LICENSE for details
 */


#include "common/common_stdafx.h"
#include "common/db/mysql.h"

#include <mysql.h>

CMySQLConnection* CMySQLConnection::m_singleton = NULL;

// ------------------------------------------------------------

bool CMySQLConnection::init(const char* db, const char *user, const char *passwd, const char* host, uint port)
{
	if (m_dbConnection.connected())
		return true;
	try
	{
		_log(_debug, _loc, gtc_("Connecting to db..."));
		m_dbConnection.connect(db, host, user, passwd, port);
		_log(_debug, _loc, gtc_("Done."));
		return true;
	}
	catch (mysqlpp::ConnectionFailed) 
	{
		_log(_error, _loc, gtc_("Connection failed, reason: %s."), m_dbConnection.error());
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

String CMySQLConnection::quoteString(const String &input)
{
	char* buf = new char[input.size() * 2 + 1];
	mysql_escape_string(buf, input.c_str(),
			static_cast<unsigned long>(input.size()));
	String output = String("'") + buf + String("'");
	delete[] buf;
	return output;
}
