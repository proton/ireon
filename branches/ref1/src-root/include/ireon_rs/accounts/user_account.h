/**
 * @file accounts/manager.h
 * user account for root server
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

#ifndef _USER_ACCOUNT_H_
#define _USER_ACCOUNT_H_

#include "stdafx.h"

class CUserAccount;
#include "boost/shared_ptr.hpp"
typedef boost::shared_ptr<CUserAccount> UserAccountPointer;

class CUserAccount
{
public:
	CUserAccount();

	// methods interacting with mysql
	static UserAccountPointer fetchAccount(const String login);
	/// account with this login must not exist, if new account
	bool saveToDB();
//	bool remove();
	// ------------------------------

	uint getId() { return m_id; }

	void setLogin(const String login);
	String getLogin() { return m_login; }

	void setPassword(const String password);

	bool passwordMatches(const String password);

private:
	uint	m_id;
	String	m_login;
	String	m_passwdHash;

	String getMD5Digest(String plaintext);

	// methods interacting with mysql
	static UserAccountPointer fetchAccount(const uint id);
	void loadFromFirstReturnedRow(const String sqlQuery);
	// ------------------------------

};

#endif
