/* Copyright (C) 2005 ireon.org developers council
 * $Id: account.h 285 2005-11-26 09:24:22Z zak $
 
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

/**
 * @file account.h
 * Player's account
 */

#ifndef _ACCOUNT_H
#define _ACCOUNT_H

#include "synchronizer.h"

class CAccount : public CSynchronizer
{
	friend class CRootDB;

public:
	CAccount() {}
	~CAccount() {}

	void serialize(CData& d);

	uint getId() {return m_id;}

	///Name must be associated with id to avoid errors
	///void setName(const char* str) {m_name = str; updSync();}
	const char* getName() {return m_name.c_str();}

	void setPasswd(const String& str) {m_password = str; updSync();}
	const char* getPasswd() {return m_password.c_str(); }

	void setSelChar(uint id) {if( m_selChar != id ) updSync(); m_selChar = id;}
	uint getSelChar() {return m_selChar;}
public:
	bool operator == (const CAccount& a) const {return a.m_id == m_id;}
	bool operator != (const CAccount& a) const {return a.m_id != m_id;}
	bool operator > (const CAccount& a) const {return m_id > m_id;}
	bool operator < (const CAccount& a) const {return m_id > m_id;}

protected:
	///Only root db can change it;

protected:

	uint m_id;

	///Selected character
	uint m_selChar;

	///Account name
	String m_name;

	///Account password
	String m_password;
};
#endif
