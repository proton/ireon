/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: world_char_player.h 610 2006-04-20 11:04:59Z zak $

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
 * @file world_char_player.h
 * Player's character
 */

#ifndef _WORLD_CHARPLAYER_H
#define _WORLD_CHARPLAYER_H

#include "synchronizer.h"
#include "world/world_character.h"

class CWSPlayerContext;

struct ClientCharPlayerData;
struct ClientOwnCharData;

struct WayPoint
{
	Vector2 m_pos;
	byte m_rotation;
	uint m_time;
};

struct WorldPosition
{
	Vector2 m_pos;
	byte m_rotation;
};


struct FightPos
{
	FightPos() {}
	FightPos(uint enemyId, bool closeCombat):
		m_enemyId(enemyId),
		m_closeCombat(closeCombat)
		{}
	uint m_enemyId;
	bool m_closeCombat;
};

class CWorldCharPlayer : public CWorldCharacter, public CSynchronizer
{
	friend class CWSPlayerContext;
	friend class CWorld;
public:

	CWorldCharPlayer();
	~CWorldCharPlayer();

	void serialize(CData& d);

	void update(uint time);

	void addWP(const WayPoint& to) {m_wayPoints.push_back(to);}
	void clearWP() {m_wayPoints.clear();}

	/** Compute characteristics when character's state changes
	 *  it can be when we wearing some cloth or cast affect etc
	*/
	void compute();

	/// Fill structure
	void toClientOwnCharData(ClientOwnCharData& data);
	void toClientCharPlayerData(ClientCharPlayerData& data);

	void setContext(CWSPlayerContext* context);
	CWSPlayerContext* getContext() {return m_context;}

	Vector2 getDifference() {return m_difference;}

	CWorldCharPlayer* player() {return this;}

	const char* getName() const {return m_name.c_str();}

	uint getAccountId() {return m_accountId;}
	void setAccountId(uint id) {m_accountId = id; updSync();}

	/// Initialization
	void init();

	/// Load from client own char data
	void fromClientOwnCharData(ClientOwnCharData& data);

	void die(const CharacterPtr& who);

protected:

	void onEnterWorld();

	void onLeaveWorld();
protected:

	/// Try to move
	void moveTo(const WayPoint& p);	

	void noticeCharOut(uint id);

	void noticeCharIn(const CharacterPtr& ch);

	void noticeCharMove(uint id, const Vector2& pos, byte rotation);

	void noticeCharFight(uint id, uint enemyId, bool closeCombat)
	{
		std::map<uint,FightPos>::iterator it = m_playersFight.find(id);
		if( it == m_playersFight.end() )
			m_playersFight.insert(std::pair<uint,FightPos>(id,FightPos(enemyId,closeCombat)));
		else
		{
			m_playersFight[id].m_enemyId = enemyId;
			m_playersFight[id].m_closeCombat = closeCombat;
		}
	}

	void noticeCharStopFight(uint id)
	{
		std::map<uint,FightPos>::iterator it = m_playersFight.find(id);
		// To avoid situation when we stop fight
		// before send information about start fight
		if( it != m_playersFight.end() )
			m_playersFight.erase(it);
		else
		m_playersStopFight.insert(id);
	}

	void noticeCharDied(uint id, uint enemyId);

	void noticeCharHit(uint ch, uint enemyId, byte percentHP, byte flags);
protected:
	/// Data
	uint m_accountId;

	String m_name;

	/// Position where we appear into world
	Vector2 m_startPos;

	/// Base characteristics
	uint m_baseMaxHP;
	uint m_baseMaxMana;

	uint m_baseStr;
	uint m_baseSta;
	uint m_baseInt;
	uint m_baseWis;
	uint m_baseCon;
	uint m_baseLuck;

protected:
	/// World variables

	uint m_sinceLastMove;

	CWSPlayerContext* m_context;

	std::deque<WayPoint> m_wayPoints;
	/// Difference between char coordinates at client and at server side
	Vector2 m_difference;
	/// Difference was too big so we will not move until begin new move
	bool m_blocked;

	std::set<uint> m_playersOut;
	std::map<uint,CharacterPtr> m_playersIn;
	std::map<uint,WorldPosition> m_playersPos;
	std::set<uint> m_playersStopFight;
	std::map<uint,FightPos> m_playersFight;
};
#endif
