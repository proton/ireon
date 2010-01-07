/**
 * @file ireon_ws/db/player_characters_manager.cpp
 * Login magager for player-controlled characters.
 * Contains list of all player characters in game and 
 * login information for incoming connections.
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: player_characters_manager.cpp 806 2006-11-08 14:02:44Z zak $

 * See LICENSE for details
 */

#include "ireon_ws/db/player_characters_manager.h"
#include "common/miscutils.h"
#include "ireon_ws/world/ws_world.h"
#include "ireon_ws/world/world_character.h"
#include <list>
#include <limits>

PlayerCharactersManager * PlayerCharactersManager::singleton_;

void PlayerCharactersManager::registerNewLogin(ushort id, String token, bool createNew, String name)
{
	if (charactersInGame_.find(id) != charactersInGame_.end())
	{
		_log ( _error, _loc, _("Character with id %d is already in game."), id );
		throw PlayerCharactersManager::ECharacterIsInGame();
	}

	logins_[id].token = token;
	logins_[id].createNew = createNew;

/*
	// TODO check for correct character names here
	if ( createNew )
		if ( name != "" )
*/
	logins_[id].name = name;
/*
		else
		{
			_log ( _error, _loc, _("Name for new char %d is incorrect."), id );
			throw PlayerCharactersManager::EIncorrectLoginInfo();
		} 
*/
	logins_[id].expire = ireon::misc::getTime() + LOGIN_TIMEOUT;
}

// ----------------------------------------------------------------------

void PlayerCharactersManager::deleteExpiredLogins()
{
	int64 currentTime = ireon::misc::getTime();
	std::list<ushort> expiredList;
	for ( std::map<ushort, LoginInfo>::iterator it = logins_.begin(); it != logins_.end(); ++ it) 
		if ( it->second.expire < currentTime)
			expiredList.push_back(it->first);

	for ( std::list<ushort>::iterator it = expiredList.begin(); it != expiredList.end(); ++it )
		logins_.erase(*it);
}

// ----------------------------------------------------------------------

WSPlayerCharPtr PlayerCharactersManager::characterLogin(ushort charId, String token)
{
	deleteExpiredLogins();

	std::map<ushort, LoginInfo>::iterator it = logins_.find(charId);
	if ( (it == logins_.end()) || (it->second.token != token) )
	{
		_log ( _error, _loc, _("Incorrect token or missing login info for character %d."), charId );
		throw PlayerCharactersManager::EIncorrectLoginInfo();
	}

	WSPlayerCharPtr ch(new CWSPlayerCharacter(charId));
	if (charactersInGame_.find(charId) != charactersInGame_.end())
		_log ( _error, _loc, _("Character with id %d is already in game."), charId );
	else
	{
		charactersInGame_.insert(charId);
		WSWorld::instance()->addEntity(ch);
	}

	logins_.erase(it);
	return ch;
}
// ----------------------------------------------------------------------

void PlayerCharactersManager::characterLogout(WSPlayerCharPtr ch)
{
	assert(ch.get() && "PlayerCharactersManager::characterLogout: character pointer is NULL.");
	assert(ch->getId() <= std::numeric_limits<ushort>::max() && "PlayerCharactersManager::characterLogout: character id exceeds player id range. Deleting mob?");

	std::set<ushort>::iterator it = charactersInGame_.find(static_cast<ushort> (ch->getId()));
	if (it == charactersInGame_.end())
	{
		_log ( _error, _loc, _("Character %d was not found in charactersInGame set."), ch->getId() );
//		throw PlayerCharactersManager::EIncorrectLoginInfo();
	}
	charactersInGame_.erase(it);
	WSWorld::instance()->removeEntity(ch);
}

