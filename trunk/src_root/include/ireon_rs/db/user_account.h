/**
 * @file ireon_rs/db/user_account.h
 * user account for root server
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: user_account.h 726 2006-09-13 21:10:24Z zak $
 
 * See LICENSE for details
 */

#ifndef _USER_ACCOUNT_H_
#define _USER_ACCOUNT_H_

#include "ireon_rs/stdafx.h"

//class CUserAccount;
//#include "boost/shared_ptr.hpp"
//typedef boost::shared_ptr<CUserAccount> UserAccountPointer;

class EAccountError {};
class EAccountNotFound : public EAccountError {};

class CUserAccount
{
public:
	/// if(loadFromDB) loads account from db, throws EAccountNotFound, EDBError
	CUserAccount(const String &login, const String &password, bool loadFromDB = true);

	void saveToDB();

//	void setNewPassword(const String &newPassword) { m_passwdHash = CGenericApp::getSHA256Digest(newPassword); }

//	bool remove();

	uint getId() { return m_id; }
	String getLogin() { return m_login; }

private:
	uint	m_id;
	String	m_login;
	String	m_passwdHash;
};

#endif
