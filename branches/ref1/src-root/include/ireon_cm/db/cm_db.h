/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: cm_db.h 528 2006-03-11 21:56:12Z mip $
 
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
 * @file cm_db.h
 * Cluster manager's database
 */

#ifndef _CM_DB_H
#define _CM_DB_H

struct ClientOwnCharData;
//class CCMWorldContext;
class CCMWSConnection;
struct WaitAccountStruct
{
	String name;
	CCMWSConnection* context;
	/*CCMWorldContext* context; TODO: mip*/
	int64 sync;
};

class CCMDB
{
	friend class CCMApp;

	CCMDB() {}

	static bool init();

	bool load();

	bool save();

public:

	static CCMDB* CCMDB::instance() {return m_singleton;}

public:


	///Find account
	AccPtr findAccount(uint m_id);
	AccPtr findAccount(String name);

	AccPtr updateAccount(AccPtr a);

	void removeAccount(uint id);

	//void removeWaitAccount(CCMWorldContext* context);
	void removeWaitAccount(CCMWSConnection* context);
	///Get world server for character
	uint getWorld(uint id);
	///Get birth world for character
	uint getBirthWorld(ClientOwnCharData* ch);

	byte newCharacter(ClientOwnCharData* ch, uint acc);
	void removeCharacter(uint id);
protected:

	static CCMDB* m_singleton;

	std::vector<AccPtr> m_accounts;

	/// Map player's characters to world servers
	std::map<uint,uint> m_players;

public:

	///Clients that wait for account data
	std::vector<WaitAccountStruct> m_waitAccount;
};

#endif
