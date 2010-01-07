/* Copyright (C) 2005 ireon.org developers council
 * $Id: root_db.h 285 2005-11-26 09:24:22Z zak $
 
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

	/// Get cluster where we send new character
	uint getBirthCluster(ClientOwnCharData* ch);

	/// Get cluster wrere this character lives
	uint getCluster(uint);

	/// Find player by name
	RootCharPlayerData* findPlayer(String name);
	/// Find player by name
	RootCharPlayerData* findPlayer(uint id) {return (m_players.find(id) == m_players.end()) ? 0 : &m_players[id];}
	/// Find char player data, that is currently creating
	RootCharPlayerData* findCreating(uint id) {return (m_creating.find(id) != m_creating.end()) ? &m_creating[id] : 0; }


	byte addCharacter(RootCharPlayerData* ch, uint id);
	byte addCreatingCharacter(ClientOwnCharData* ch, uint acc);

	void deleteCharPlayer(uint id);
	void deleteCreating(uint id);

	void selectCharactersByAccount(uint id, std::map<uint, RootCharPlayerData> &chars);
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
public:

	std::vector<AccPtr> m_accounts;

protected:

	/// Map player's characters to clusters
	std::map<uint,RootCharPlayerData> m_players;
	/// Characters that creating now
	std::map<uint,RootCharPlayerData> m_creating;

	static CRootDB* m_singleton;

};

#endif
