/* Copyright (C) 2005 ireon.org developers council
 * $Id: character.h 433 2005-12-20 20:19:15Z zak $

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
 * @file character.h
 * Character
 */
#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "world/object.h"

struct WayPoint
{
	WayPoint(int64 pulse,Vector2 pos, Radian rotation) : m_pulse(pulse),m_pos(pos),m_rotation(rotation) {}
	int64 m_pulse;
	Vector2 m_pos;
	Radian m_rotation;
};

class CCharacter : public CObject
{
public:

	CCharacter(const String& name, const String& mesh);
	~CCharacter();

	virtual void update(Real time);

	void setMass(uint mass) {m_mass = mass;}
	uint getMass() {return m_mass;}

	void setWalkSpeed(Real speed);
	Real getWalkSpeed() {return m_walkSpeed;}

	void setFightSpeed(byte speed);
	byte getFightSpeed() {return m_fightSpeed;}

	virtual bool processCollision(OgreOde::Contact* contact);

	void lookAt(const Vector2& to);

	/// Jump
	virtual void jump();
	virtual void walk(Vector2 *direction = NULL);

	bool isWalking() {return m_walk || m_wayPoints.size(); }

	void addWayPoint(int64 pulse,Vector2 p, Radian rotation) {m_wayPoints.push_back(WayPoint(pulse,p,rotation));}
	void clearWayPoints() {m_wayPoints.clear();}

	/// Get closest animation pos where we can stop it
	Real getAnimStandPos();

	void walkAnimStep(const Real& time, bool toStand = false);

	void setRotation(Radian rot);
	Radian getRotation() {return m_rotation;}

	virtual uint getId() {return 0;}

	void setInfo(const WndPtr& info) {m_info = info;}
	WndPtr getInfo() {return m_info;}

	bool visible();

	void startFight(uint enemy, bool closeCombat);
	void stopFight();
protected:

	virtual void setCharData(ClientCharData* data);

protected:
	uint m_mass;

	/// Angular motor joint
	OgreOde::AngularMotorJoint* m_amj;
	OgreOde::TransformGeometry* m_transform;

	/// Moving speed
	Real m_walkSpeed;

	bool m_walk;
	bool m_jump;

	std::deque<WayPoint> m_wayPoints;

	AnimationState* m_walkAnim;
	Real m_animStandPos[2];
	/// Length of one step (in world units)
	Real m_stepLen;

	AnimationState* m_jumpAnim;
	Real m_jumpSpeed;

	AnimationState* m_hitAnim;
	Real m_hitSpeed;

	/// Character's position where we switched off gravity
	Vector3 m_noGravityPos;

	Radian m_rotation;
	
	/// Information window
	WndPtr m_info;

	/// Character with which we're fighting
	CharacterPtr m_fight;
	/// Id of our enemy, if client hasn't its data
	uint m_fightId;
	/// We're in close combat
	bool m_closeCombat;
	/// Fight speed
	byte m_fightSpeed;
	Real m_timeToNextHit;
};

#endif