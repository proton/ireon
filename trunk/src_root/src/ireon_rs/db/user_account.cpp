/**
 * @file ireon_rs/db//user_account.cpp
 * user account
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: user_account.cpp 778 2006-10-05 20:15:59Z zak $
 
 * See LICENSE for details
 */

#include "ireon_rs/stdafx.h"
#include "ireon_rs/db/user_account.h"
#include "common/db/mysql.h"
#include "common/miscutils.h"

#include "common/misc/mysqlpp_wrapper.h"

// --------------------------------------------------------

CUserAccount::CUserAccount(const String &login, const String &password, bool loadFromDB):
m_id(0),
m_login(login),
m_passwdHash(ireon::misc::getSHA256Digest(password))
{
	if (loadFromDB)
	{
		try
		{
			mysqlpp::Query	query = CMySQLConnection::instance()->getQuery();

			query << "select * from `accounts` where `login`=" << mysqlpp::quote << m_login << " and `password`=" << mysqlpp::quote << m_passwdHash;

			mysqlpp::Result result = query.store();

			if (result)	{
				mysqlpp::Row row;
				try
				{
					row	= result.at(0);
					m_id = row["id"];
				}
				catch (mysqlpp::EndOfResults&)
				{
					throw EAccountNotFound();
				}
				
				
			} else
				throw EDBError();
		}
		catch (mysqlpp::Exception&)
		{
			throw EDBError();
		}

	}
}

// --------------------------------------------------------

void CUserAccount::saveToDB()
{
	try
	{
		mysqlpp::Query		query = CMySQLConnection::instance()->getQuery();
		if (!m_id)
		{
			query << "insert into `accounts` (`id`,`login`,`password`) values(default, " << 
			mysqlpp::quote << m_login << ", " << mysqlpp::quote << m_passwdHash << ")";
			query.execute();
			mysqlpp::Query	idquery = CMySQLConnection::instance()->getQuery();
			idquery << "select * from `accounts` where `login`=" << mysqlpp::quote << m_login << " and `password`=" << mysqlpp::quote << m_passwdHash;
			mysqlpp::Result result = idquery.store();
			if (result.size())	{
				mysqlpp::Row row;
				row	= result.at(0);
				m_id = row["id"];
			} else
				throw EDBError();		
		}
		else
		{
			query << "replace into `accounts` (`id`,`login`,`password`) values(" << m_id << ", " << 
			mysqlpp::quote << m_login << ", " << mysqlpp::quote << m_passwdHash << ")";
			query.execute();
		}
	}
	catch (mysqlpp::Exception&)
	{
		throw EDBError();
	}
}

// --------------------------------------------------------

