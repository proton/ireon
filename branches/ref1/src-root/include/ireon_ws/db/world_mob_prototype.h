/**
 * @file world_mob_prototype.h
 * Mob prototype for world server
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: world_mob_prototype.h 522 2006-03-06 16:49:30Z zak $
 
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

#ifndef _WORLD_MOB_PROTOTYPE_H
#define _WORLD_MOB_PROTOTYPE_H

class TiXmlNode;

class CWorldMobPrototype
{
friend class CWorld;
protected:
	CWorldMobPrototype();
public:
	~CWorldMobPrototype();

	bool load(const String& resourceName);

	uint getId() const {return m_id;}

	const char* getName() const {return m_name.c_str();}

	float getVelocity() {return m_velocity;}
	byte getFightSpeed() {return m_fightSpeed;}

	uint getMaxHP() {return m_maxHP;}
	uint getMaxMana() {return m_maxMana;}

	uint getStr() {return m_str;}
	uint getSta() {return m_sta;}
	uint getInt() {return m_int;}
	uint getWis() {return m_wis;}
	uint getCon() {return m_con;}
	uint getLuck() {return m_luck;}
protected:

	bool processOption(TiXmlNode* option);
protected:

	// Identifier
	uint m_id;

	/// Mob's name
	String m_name;

	uint m_maxHP;
	uint m_maxMana;
	uint m_str;
	uint m_sta;
	uint m_int;
	uint m_wis;
	uint m_con;
	uint m_luck;

	/// Mob's velocity
	float m_velocity;

	/// Start position
	Vector2 m_startPos;

	/// Fight speed
	byte m_fightSpeed;
};

#endif
