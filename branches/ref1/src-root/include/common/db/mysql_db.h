/**
 * @file mysql_db.h
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

#ifndef _MYSQL_DB_H
#define _MYSQL_DB_H

#include "stdafx.h"
#include <mysql++.h>

class CMySQLConnection
{
	CMySQLConnection();

public:
	static CMySQLConnection* instance();

	bool init(const char* db, const char *user, const char *passwd, const char* host, uint port=3306);
	mysqlpp::Query getQuery();
	void flush();

	String getTableNamesPrefix();
	void setTableNamesPrefix(const String prefix);

	String quoteString(const String input);

private:
	static CMySQLConnection*	m_singleton;
	mysqlpp::Connection			m_dbConnection;
	String						m_tableNamesPrefix;
};

inline CMySQLConnection* CMySQLConnection::instance() 
{	
	if(!m_singleton) 
		m_singleton = new CMySQLConnection(); 
	return m_singleton;
}

#endif

