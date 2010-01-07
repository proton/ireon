/**
 * @file char_mob.h
 * Player's character
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: char_mob.h 510 2006-02-26 21:09:40Z zak $

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
#ifndef _CHAR_MOB_H
#define _CHAR_MOB_H

#include "world/character.h"

struct ClientCharMobData;

class CCharMob : public CCharacter
{
public:
	CCharMob(ClientCharMobData* data, const MobPrototypePtr& prot);
	~CCharMob();

	//void setData(ClientCharMobData* data);
	//ClientCharMobData* getData() {return m_data;}
protected:
	ClientCharMobData* mobData() {return (ClientCharMobData*)m_data;}
protected:

	MobPrototypePtr m_prot;
};

#endif