/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: world_character.h 610 2006-04-20 11:04:59Z zak $

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

#include "world/ws_world.h"
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
	void clearVisible();

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

	/* Notice all necessary characters about we're stop 
	 * fighting
	*/
	void noticeIStopFight();

	/* Update our enemy if we're not fighting
	*/
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
	bool inCloseBattleRange(const Vector2& pos) {return (getPos()-pos).squaredLength() <= CLOSE_COMBAT_DISTANCE * CLOSE_COMBAT_DISTANCE;}
	/// Can we hit somebody?
	bool canHit() {return CWorld::m_pulse >= m_nextHitPulse;}
	/*! Hit character in close combat
	   \return
	    -true if we hitted him
	    -false if not
	*/
	bool hit(const CharacterPtr& ch);

	virtual void die(const CharacterPtr& who);

	/// Can character walk to this position?
	bool canWalk(const Vector2& pos);

	uint getAc() {return m_ac;}
	uint getHitroll() {return m_hitroll;}
	uint getDamroll() {return m_damroll;}
	uint getArmor() {return m_armor;}

	uint getMaxHP() {return m_maxHP;}
	uint getHP() {return m_curHP;}
	void setHP(uint hp) {m_curHP = hp;}
	bool changeHP(int hp, const CharacterPtr& who);
	byte getPercentHP() {return m_curHP * 100 / m_maxHP;}

	uint getMaxMana() {return m_maxMana;}
	uint getMana() {return m_curMana;}
	void setMana(uint mana) {m_curMana = mana;}

	uint getStr() {return m_str;}
	uint getSta() {return m_sta;}
	uint getInt() {return m_int;}
	uint getWis() {return m_wis;}
	uint getCon() {return m_con;}
	uint getLuck() {return m_luck;}
protected:
	///Protected functions for operating with character
	void init();

protected:

	/** Notice character that another character went out from
	 *  its visible range
	*/
	virtual void noticeCharOut(uint id){}

	/** Notice character that another character went into
	 *  its visible range
	*/
	virtual void noticeCharIn(const CharacterPtr& ch) {}

	/** Notice character that another character changed
	 *  its position
	*/
	virtual void noticeCharMove(uint id, const Vector2& pos, byte rotation) {}

	/** Notice character that character started fight
	*/
	virtual void noticeCharFight(uint id, uint enemyId, bool closeCombat) {}

	/** Notice character that character stopped fight
	*/
	virtual void noticeCharStopFight(uint id) {}

	/** Notice character that character died
	*/
	virtual void noticeCharDied(uint id, uint enemyId) {}

	/** Notice character that one character hit another
	*/
	virtual void noticeCharHit(uint ch, uint enemyId, byte percentHP, byte flags) {}

	virtual void onLeaveWorld();

	virtual void onEnterWorld();
protected:
	///Data
	uint m_id;

	///----- Computable characteristics
	float m_velocity;
	byte m_fightSpeed;

	uint m_maxHP;
	uint m_maxMana;

	uint m_curHP;
	uint m_curMana;

	byte m_regHPModifier;
	byte m_regManaModifier;

	uint m_ac;
	uint m_armor;
	uint m_hitroll;
	uint m_damroll;

	uint m_str;
	uint m_sta;
	uint m_int;
	uint m_wis;
	uint m_con;
	uint m_luck;

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
