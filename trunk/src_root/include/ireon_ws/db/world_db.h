/* Copyright (C) 2005 ireon.org developers council
 * $Id: world_db.h 806 2006-11-08 14:02:44Z zak $
 *  See LICENSE for details
 */

/**
 * @file player_db.h
 * Player's database
 */

#ifndef _WORLD_DB
#define _WORLD_DB

#include "boost/shared_ptr.hpp"
class CAccount;
typedef boost::shared_ptr<CAccount> AccPtr;
class CWorldCharPlayer;
typedef boost::shared_ptr<CWorldCharPlayer> CharPlayerPtr;

#include <map>

class CNetDataProcessor;

struct WaitLoginStruct
{
	String name;
	String password;
	CNetDataProcessor* context;
};

class CWorldDB
{
	friend class WSApp;

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

	void checkLogin(String name, String passwd, CNetDataProcessor* context);
	void removeWaitLogin(CNetDataProcessor* context);
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
