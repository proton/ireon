/**
 * @file char_player.h
 * Player's character
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: char_player.h 433 2005-12-20 20:19:15Z zak $

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
#ifndef _CHAR_PLAYER_H
#define _CHAR_PLAYER_H

#include "world/character.h"

class CCharPlayer : public CCharacter
{
public:
	CCharPlayer(ClientCharPlayerData* data, const String& mesh);
	~CCharPlayer();

	//void setData(ClientCharPlayerData* data);
	//ClientCharPlayerData* getData() {return m_data;}

	uint getId();

	void update(Real time);
protected:

	ClientCharPlayerData* m_data;
};

class COwnCharPlayer : public CCharPlayer
{
public:
	COwnCharPlayer(ClientOwnCharData* data, const String& mesh);
	~COwnCharPlayer();

	//void setData(ClientOwnCharData* data);
	//ClientOwnCharData* getData() {return m_data;}

	void setDifference(const Vector2& dif);
	Vector2 getDifference() {return m_difference;}

	void update(Real time);

	void setPosition(const Vector3& position);
	void setPosition(const Vector2& position) {CObject::setPosition(position);}

	void walk(Vector2 *direction = 0);

	void rotate(Degree degree);

protected:

	ClientOwnCharData* ownData() {return (ClientOwnCharData*)m_data;}

protected:

	/// Time since we sent position to server last time
	Real m_sinceLastSendPos;
	/// We stopped, so nex time we move we'll send to server command 'begin move'
	bool m_stop;
	/// We rotated, but we won't spam, so send coordinates not more often then one in 100ms
	bool m_rotated;
	/// We not affected by change position command until start new move
	bool m_blocked;

	/// Characters' previous position (for impassable areas check)
	Vector3 m_prevPosition;

	/// Base velocity (may be changed due to difference in coordinates at client and at server
	Real m_velocity;
	/// Correction to velocity
	Real m_correction;
	/// Difference between server and client positions of own character
	Vector2 m_difference;
};

#endif