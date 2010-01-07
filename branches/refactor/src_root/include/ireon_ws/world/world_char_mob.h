/* Copyright (C) 2005 ireon.org developers council
 * $Id: world_char_mob.h 433 2005-12-20 20:19:15Z zak $

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
 * @file world_char_mob.h
 * Mob
 */
#ifndef _WORLD_CHAR_MOB_H
#define _WORLD_CHAR_MOB_H

#include "world/world_character.h"

struct ClientCharMobData;

class CWorldCharMob : public CWorldCharacter
{
	friend class CWorld;
public:

	CWorldCharMob();
	~CWorldCharMob();

	bool serialize(CData& d);

	uint getProtId();

	const char* getName();

	CWorldCharMob* mob() {return this;}

	void update(uint time);

	/** Compute characteristics when character's state changes
	 *  it can be when we wearing some cloth or cast affect etc
	*/
	void compute();

	/// Fill structure
	void toClientCharMobData(ClientCharMobData& data);

	const char* getName() const;
protected:

	void onEnterWorld();

	void onLeaveWorld();
protected:

	WorldMobProtPtr m_prototype;

	Vector2 m_startPos;
};

#endif
