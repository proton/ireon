/**
 * @file accounts/user_account.cpp
 * user account
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

#include "ireon_rs/accounts/user_account.h"
#include "common/db/mysql_db.h"
#include "common/log/log.h"
#include "common/kernel/md5class.h"

#include <mysql++.h>

CUserAccount::CUserAccount():
m_id(0),
m_login(""),
m_passwdHash("")
{
}

// --------------------------------------------------------

UserAccountPointer CUserAccount::fetchAccount(const uint id)
{
	UserAccountPointer	acc(new CUserAccount);
	String	sqlQuery;

	char idbuf[20];
	sprintf(idbuf,"%u",id);

	sqlQuery = "select * from `" + CMySQLConnection::instance()->getTableNamesPrefix() + "accounts` where id=" + idbuf;
	acc->loadFromFirstReturnedRow(sqlQuery);
	return acc;
}

// --------------------------------------------------------

UserAccountPointer CUserAccount::fetchAccount(const String login)
{
	UserAccountPointer	acc(new CUserAccount);
	String	sqlQuery;

	sqlQuery = "select * from `" + CMySQLConnection::instance()->getTableNamesPrefix() + 
				"accounts` where `login`=" + CMySQLConnection::instance()->quoteString(login);
	try
	{
		acc->loadFromFirstReturnedRow(sqlQuery);
		return acc;
	}
	catch (mysqlpp::BadQuery)
	{
		acc.reset();
		return acc;
	}
}

// --------------------------------------------------------

void CUserAccount::loadFromFirstReturnedRow(const String sqlQuery)
{
	mysqlpp::Query		query = CMySQLConnection::instance()->getQuery();

	query << sqlQuery;

	mysqlpp::Result result = query.store();

	if (result)	{
		mysqlpp::Row row;
		row	= result.at(0);
		m_id = row["id"];
		m_login = row["login"];
		m_passwdHash = row["password"];
	}
}

// --------------------------------------------------------

bool CUserAccount::saveToDB()
{
	mysqlpp::Query		query = CMySQLConnection::instance()->getQuery();

	if (!m_id)
		query << "insert into `" << CMySQLConnection::instance()->getTableNamesPrefix() << 
					"accounts` (`id`,`login`,`password`) values(default, " << CMySQLConnection::instance()->quoteString(m_login) <<
					", " << CMySQLConnection::instance()->quoteString(m_passwdHash) << ")";
	else
		query << "replace into `" << CMySQLConnection::instance()->getTableNamesPrefix() << 
					"accounts` (`id`,`login`,`password`) values(" << m_id << ", " << CMySQLConnection::instance()->quoteString(m_login) << ", " <<
					CMySQLConnection::instance()->quoteString(m_passwdHash) << ")";

	return query.exec(query.str());
}

// --------------------------------------------------------

void CUserAccount::setLogin(const String login)
{
	if (m_id != 0)
		CLog::instance()->log(CLog::msgLvlWarn, _("Setting new login value for account %s, id=%u"),m_login.c_str(),m_id);
	m_login = login;
}

// --------------------------------------------------------

void CUserAccount::setPassword(const String password)
{
	if (m_id != 0)
		CLog::instance()->log(CLog::msgLvlWarn, _("Setting new password value for account %s, id=%u"),m_login.c_str(),m_id);
	m_passwdHash = getMD5Digest(password);
}

// --------------------------------------------------------

bool CUserAccount::passwordMatches(const String password)
{
	if (m_passwdHash != getMD5Digest(password))
		return false;
	return true;
}

// --------------------------------------------------------

String CUserAccount::getMD5Digest(const String plaintext)
{
	CMD5 md5(plaintext.c_str());
	return md5.getMD5Digest();
}

