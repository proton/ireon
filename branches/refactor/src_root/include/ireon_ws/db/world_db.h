/* Copyright (C) 2005 ireon.org developers council
 
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
 * @file player_db.h
 * Player's database
 */

#ifndef _WORLD_DB
#define _WORLD_DB

class CNetClient;

struct WaitLoginStruct
{
	String name;
	String password;
	CNetClient* context;
};

class CWorldDB
{
	friend class CWorldApp;

	CWorldDB() {}

	static bool init();

	bool load();

	bool save();

public:

	static CWorldDB* CWorldDB::instance() {return m_singleton;}

public:

	///Find account
	AccPtr findAccount(uint m_id);
	AccPtr findAccount(String name);

	AccPtr updateAccount(AccPtr acc);

	void removeAccount(uint id);

	void checkLogin(String name, String passwd, CNetClient* context);
	void removeWaitLogin(CNetClient* context);
public:
	/// Work with characters
	///Find character
	CharPlayerPtr findCharPlayer(uint m_id);
	CharPlayerPtr findCharPlayer(String name);

	byte updateCharPlayer(CharPlayerPtr ch);

	void removeCharPlayer(uint id);

protected:

	static CWorldDB* m_singleton;

	std::vector<AccPtr> m_accounts;

	std::map<uint,CharPlayerPtr> m_players;

public:
	std::vector<WaitLoginStruct> m_waitLogin;
};

#endif
