/* Copyright (C) 2005 ireon.org developers council

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
 * @file player_character.h
 * Player character's data
 */
#ifndef _PLAYER_CHARACTER_H
#define _PLAYER_CHARACTER_H

#include "world/server_character.h"

class CCharPlayerData;

class CPlayerCharacter : public CServerCharacter
{
public:
	CPlayerCharacter(const String& name);

	CharPlayerPtr data() {return m_data;}
protected:

	CharPlayerPtr m_data;
};

#endif