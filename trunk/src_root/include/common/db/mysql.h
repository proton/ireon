/**
 * @file common/db/mysql.h
 * general mysql connection manager
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: mysql.h 780 2006-10-07 16:01:08Z zak $
 
 * See LICENSE for details
 */

#ifndef _MYSQL_DB_H
#define _MYSQL_DB_H

#include "common/common_stdafx.h"
#include "common/misc/mysqlpp_wrapper.h"
#include "common/log_mixin.h"

class CMySQLConnection: private CLogMixin
{
	CMySQLConnection(): CLogMixin("common.db") {};

public:
	static CMySQLConnection* instance();

	bool init(const char* db, const char *user, const char *passwd, const char* host, uint port=3306);
	mysqlpp::Query getQuery();
	void flush();

	String quoteString(const String &input);

private:
	static CMySQLConnection*	m_singleton;
	mysqlpp::Connection			m_dbConnection;
};

inline CMySQLConnection* CMySQLConnection::instance() 
{	
	if(!m_singleton) 
		m_singleton = new CMySQLConnection(); 
	return m_singleton;
}

#endif

