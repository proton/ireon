/* Copyright (C) 2005 ireon.org developers council
 * $Id: world_character.h 433 2005-12-20 20:19:15Z zak $

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
 * @file world_character.h
 * Character
 */

#ifndef _WORLD_CHARACTER_H
#define _WORLD_CHARACTER_H

#include "world.h"
#include "world/world_object.h"

class CWorldCharacter: public CWorldObject
{
	friend class CWorld;
	friend class CWorldDB;
public:
	CWorldCharacter();
	~CWorldCharacter();


	void toClientCharData(ClientCharData* data);

	/** Compute characteristics when character's state changes
	 *  it can be when we wearing some cloth or cast affect etc
	*/
	virtual void compute();

	float getVelocity(){return m_velocity;}

	/// Can this character see another?
	virtual bool seeCharacter(const CWorldCharacter* ch, float& distance);

	/// Update visible lists
	void updatePosition();
	void updateVisible();

	virtual void setPos(const Vector2& pos);

	virtual void update(uint time);

	/// Returns pointer to CWorldCharPlayer if is a player
	virtual CWorldCharPlayer* player() {return NULL;}
	/// Returns pointer to CWorldCharMob if is a mob
	virtual CWorldCharMob* mob() {return NULL;}

	uint getId() const {return m_id;}

	virtual const char* getName() const {return "";}

	/* Start fight with this character (and it will start
	 * to fight with us if it isn't fighting
	*/
	bool startFight(const CharacterPtr& ch, bool closeCombat);
	/* Stop our fight. All characters that was in fight with
	 * us will stop fight too (or will start fight with another
	 * characters)
	*/
	void stopFight();
	void updateFightPos();

	CharacterPtr getEnemy() {return m_enemy;}
	bool isFighting() {return m_enemy.get() != NULL;}
	bool inCloseCombat() {return m_closeCombat;}
	void setCloseCobat(bool value) {m_closeCombat = value;}
	bool needCloseCombat() {return m_needCloseCombat;}
	void setNeedCloseCombat(bool value) {m_needCloseCombat = value;}

	/* Function to determine if this character can fight only in
	 * close combat
	*/
	bool attackIsClose();

	/// Are we in close battle range from character in this position?
	bool inCloseBattleRange(const Vector2& pos) {return (getPos()-pos).squaredLength() <= NEAR_FIGHT_DISTANCE * NEAR_FIGHT_DISTANCE;}
	/// Can we hit somebody?
	bool canHit() {return CWorld::m_pulse >= m_nextHitPulse;}
	/*! Hit character in close combat
	   \return
	    -true if we hitted him
	    -false if not
	*/
	bool hit(CharacterPtr ch);
protected:
	///Protected functions for operating with character
	void init();

protected:

	/** Notice character that another character went out from
	 *  its visible range
	*/
	virtual void noticeCharOut(uint id) {}

	/** Notice character that another character went into
	 *  its visible range
	*/
	virtual void noticeCharIn(const CharacterPtr& ch) {}

	/** Notice character that another character changed
	 *  its position
	*/
	virtual void noticeCharMove(uint id, const Vector2& pos, byte rotation) {}

	/** Notice character that another character started fight
	*/
	virtual void noticeCharFight(uint id, uint enemyId, bool closeCombat) {}

	/** Notice character that another character stopped fight
	*/
	virtual void noticeCharStopFight(uint id) {}

	virtual void onLeaveWorld();

	virtual void onEnterWorld();
protected:
	///Data
	uint m_id;

	///----- Computable characteristics
	float m_velocity;
	byte m_fightSpeed;

	/// Characters that are visible for this
	std::vector<CharacterPtr> m_iSee;
	/// Characters, that see this character
	std::vector<CharacterPtr> m_seeMe;

	/// Did i moved after last call of updatePosition()
	bool m_moved;

	/// Smart pointer to this character
	CharacterPtr m_ptr;

	/// Character with who we're fighting
	CharacterPtr m_enemy;
	/// Characters that fighting with us
	std::list<CharacterPtr> m_fightWithMe;
	/// We fight in close combat
	bool m_closeCombat;
	/** We want to fight in close combat (and start it when
	*   we'll be in appropriate distance
	*/
	bool m_needCloseCombat;
	/// Pulse when we can hit somebody next time
	int64 m_nextHitPulse;
};

#endif
