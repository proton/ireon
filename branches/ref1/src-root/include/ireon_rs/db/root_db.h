/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: root_db.h 527 2006-03-09 17:44:32Z zak $
 
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
 * @file root_db.h
 * Player's database
 */

#ifndef _ROOT_DB
#define _ROOT_DB

struct ClientOwnCharData;

///Character's data, stored on root server
struct RootCharPlayerData
{

	void serialize(CData& d);

	///Cluster where char lives
	uint m_cluster;
	///Id of player's account
	uint m_account;
	///Character's name
	String m_name;

	uint m_id;
};

class CRootDB
{
	friend class CRootApp;

	CRootDB() {}

	static bool init();

	bool load();

	bool save();

public:

	static CRootDB* CRootDB::instance() {return m_singleton;}

public:
	/// Work with players

	/// Get cluster wrere this character lives
	uint getCluster(uint charId);

	/// Find player by name
	RootCharPlayerData* findCharacter(String name);
	/// Find player by name
	RootCharPlayerData* findCharacter(uint charId) {return (m_chars.find(charId) == m_chars.end()) ? 0 : &m_chars[charId];}
	uint getMaxCharacterId();

	byte addCharacter(RootCharPlayerData* ch);

	void deleteCharacter(uint id);

	void selectCharactersByAccount(uint accId, std::map<uint, RootCharPlayerData> &chars);

public:
	///Create, initialize and add to DB new account
	AccPtr createAccount(const String& name);

	///Deletes existing account
	bool deleteAccount(CAccount* acc);
	bool deleteAccount(uint m_id);
	bool deleteAccount(String name);

	///Find account
	AccPtr findAccount(uint m_id);
	AccPtr findAccount(String name);

	std::vector<AccPtr> m_accounts;

private:

	/// Map charid to char data
	std::map<uint,RootCharPlayerData> m_chars;

	static CRootDB* m_singleton;

};

#endif
