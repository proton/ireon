/**
 * @file ireon_ws/db/player_characters_manager.h
 * Login magager for player-controlled characters.
 * Contains list of all player characters in game and
 * login information for incoming connections.
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: player_characters_manager.h 806 2006-11-08 14:02:44Z zak $

 * See LICENSE for details
 */

#pragma once

#include "ireon_ws/stdafx.h"
#include <set>
#include <map>

#include "boost/shared_ptr.hpp"
class CWSPlayerCharacter;
typedef boost::shared_ptr<CWSPlayerCharacter> WSPlayerCharPtr;

#include "common/log_mixin.h"

class PlayerCharactersManager : private CLogMixin
{
public:
	// exceptions
	class ECharacterIsInGame{};
	class EIncorrectLoginInfo{};
	class EBadCharacterName{};

	static PlayerCharactersManager* instance() {if (!singleton_) singleton_ = new PlayerCharactersManager(); return singleton_;}

	bool isCharInGame(ushort charid) { return charactersInGame_.find(charid) != charactersInGame_.end(); }

	/// registers new temporary login
	/// if createNew == true name must be set, otherwise it does not matter
	void registerNewLogin(ushort id, String token, bool createNew, String name);

	WSPlayerCharPtr characterLogin(ushort charId, String token);
	void characterLogout(WSPlayerCharPtr ch);

private:
	static const int LOGIN_TIMEOUT = 10000; // msecs

	void deleteExpiredLogins();

	struct LoginInfo
	{
		String token;
		bool createNew;
		String name;
		int64 expire; //< time when this login information will expire
	};
	std::map<ushort, LoginInfo>	logins_;
	std::set<ushort>		charactersInGame_;

	PlayerCharactersManager(): CLogMixin("secutiry.auth") {}
	static PlayerCharactersManager* singleton_;
};

