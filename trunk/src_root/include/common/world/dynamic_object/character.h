/**
 * @file char_player.h
 * Player's character
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: char_player.h 829 2006-11-30 18:39:37Z mip $

 *  See LICENSE for details
 */
#ifndef _CHAR_PLAYER_H
#define _CHAR_PLAYER_H

#include "common/world/dynamic_object/dynamic.h"

struct ClientCharPlayerData;
struct ClientOwnCharData;

class CCharacter : public CDynamicObject
{
public:
	CCharacter(CPrototype* proto);
	~CCharacter();
	static CCharacter* productCallback(CPrototype* proto);
	void update(Real time);
};


class COwnPlayer : public CCharacter
{
public:
	COwnPlayer(CPrototype* proto);
	~COwnPlayer();

	static COwnPlayer* productCallback(CPrototype* proto);

	void setDifference(const Vector2& dif);
	Vector2 getDifference() {return m_difference;}

	void update(Real time);

	void setPosition3D(const Vector3& position) {CCharacter::setPosition3D(position);};
	void setPosition2D(const Vector2& position) {CCharacter::setPosition2D(position);}

	void walk(Real time, Vector2 *direction = 0);

	void rotate(Degree degree);

protected:

	/// Time since we sent position to server last time
	Real m_sinceLastSendPos;
	/// We stopped, so nex time we move we'll send to server command 'begin move'
	bool m_stop;
	/// We rotated, but we won't spam, so send coordinates not more often then one in 100ms
	bool m_rotated;
	/// We not affected by change position command until start new move
	bool m_blocked;

	/// Base velocity (may be changed due to difference in coordinates at client and at server
	Real m_velocity;
	/// Correction to velocity
	Real m_correction;
	/// Difference between server and client positions of own character
	Vector2 m_difference;
};

#endif
