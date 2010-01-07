/**
  * @file character.h
  *  Character
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: character.h 829 2006-11-30 18:39:37Z mip $
 
  *  See LICENSE for details
  */
#ifndef _DYNAMIC_H
#define _DYNAMIC_H
#include "common/world/generic_object/object.h"

//#include "boost/shared_ptr.hpp"

//class CWindow;
//typedef boost::shared_ptr<CWindow> WndPtr;
//class CCharacter;
//typedef boost::shared_ptr<CCharacter> CharacterPtr;

struct WayPoint
{
	WayPoint(int64 pulse,Vector2 pos, Radian rotation) : m_pulse(pulse),m_pos(pos),m_rotation(rotation) {}
 	int64 m_pulse;
 	Vector2 m_pos;
 	Radian m_rotation;
};
 
class CDynamicObject : public CObject
{
public:

	/*CCharacter(const String& name, const String& mesh);
	~CCharacter();*/

	CDynamicObject(CPrototype* proto);
	~CDynamicObject();
	/// this function must be INHERITED
	static CDynamicObject* productCallback(CPrototype* proto);

	uint getId();

	virtual void update(Real time);

	bool visible();

	void lookAt(const Vector2& to);
 	/// Jump
	virtual void jump();
	/** Check if player can walk to position. If not, in 'norm'
	function returns normal to impassable wall
	to correct move
	*/
	bool canWalk(const Vector3& to, Vector2& norm);
	/// Walk
	virtual void walk(Real time, Vector2 *direction = NULL);
 
	bool isWalking() {return m_walk || m_wayPoints.size(); }

	void walkAnimStep(const Real& time, bool toStand = false);

	Real getWalkSpeed() {return m_walkSpeed;}
	void setWalkSpeed(Real speed);

	void startFight(uint enemy, bool closeCombat);
	void stopFight();
	byte getFightSpeed();

	void die();
	bool isDead() {return m_dead;}

	/// Get closest animation pos where we can stop it
	Real getAnimStandPos();

	void addWayPoint(int64 pulse,Vector2 p, Radian rotation) {m_wayPoints.push_back(WayPoint(pulse,p,rotation));}
	void clearWayPoints() {m_wayPoints.clear();}

	void setRotation(Radian rot);
	Radian getRotation() {return m_rotation;}


	//void setInfo(const WndPtr& info) {m_info = info;}
	//WndPtr getInfo() {return m_info;}

	byte getPercentHP();
	void setPercentHP(byte percent);

	CDynamicObject* getEnemy() {return m_enemy;}


protected:

	//ClientCharData* m_data;

	uint m_mass;

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

	AnimationState* m_dieAnim;
	Real m_dieSpeed;

	Radian m_rotation;
	
	/// Information window
	//WndPtr m_info;

	/// Character with which we're fighting
	CDynamicObject* m_enemy;
	/// Id of our enemy, if client hasn't its data
	uint m_fightId;
	/// We're in close combat
	bool m_closeCombat;
	/// Time to next time we'll hit enemy
	Real m_timeToNextHit;

	/// Character is dead
	bool m_dead;
	Real m_timeToDisappear;

	CPrototype* proto_;
	/// -------------------  characteristics --------------------
	/// 
	byte percentHP_;
	byte fightSpeed_;

	
};

#endif
