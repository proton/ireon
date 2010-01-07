/* Copyright (C) 2005 ireon.org developers council
 * $Id: world_character.h 821 2006-11-21 21:00:36Z zak $

 *  See LICENSE for details
 */

/**
 * @file world_character.h
 * Character
 */

#ifndef _WORLD_CHARACTER_H
#define _WORLD_CHARACTER_H

//#include "ireon_ws/world/ws_world.h"
//#include "ireon_ws/world/ws_char_stats.h"
#include "common/common_stdafx.h"

#include "boost/shared_ptr.hpp"
class CWSEntityBase;
typedef boost::shared_ptr<CWSEntityBase> WSEntityPtr;

struct Position;

//class WSWorld;
class CWSEntityBase
{
//	friend class WSWorld;
public:
	class EComponentNotFound {};

	CWSEntityBase(uint id, Position * pos = NULL): id_(id), pos_(pos) {}
	virtual ~CWSEntityBase();
	uint getId() const {return id_;}

	void setPosition(Position * pos) { pos_ = pos; }
	Position * getPosition() { return pos_; }

	/// time in milliseconds
	virtual void update(ushort time) = 0;
	
	virtual uint getOverallSize() { return 1; }
	virtual void removeComponent(uint) {}
	virtual WSEntityPtr findComponentById(uint) { throw EComponentNotFound(); };

private:
	uint		id_;
	Position *	pos_;
};

class CWSCharacter: public CWSEntityBase
{
public:
	CWSCharacter(uint id): CWSEntityBase(id), heading_(0,0) {};

	void setHeading(Vector2 heading) { heading_ = heading; }

	/**
		starts or continues character movement
		moves character to destination position if it is possible
		@param pos character destination (new) coordinates in horizontal plane
		@param navTriangleId id of destination triangle in navigation mesh
	 */
	void updatePosition(Vector2 pos, uint navTriangleId) { }

	/**
		stops character movement
		moves character to destination position if it is possible
		can be used for one short move (i.e. without calling updatePosition)
		@param pos character destination (new) coordinates in horizontal plane
		@param navTriangleId id of destination triangle in navigation mesh
	*/
	void stopMovement(Vector2 pos, uint navTriangleId) { }

private:
	Vector2		heading_;
};

class CWSPlayerCharacter: public CWSCharacter
{
public:
	CWSPlayerCharacter(uint id): CWSCharacter(id) {};

	void update(ushort) {};
};

class CWSCharGroup: public CWSEntityBase
{
public:
	CWSCharGroup();

	void update(ushort time);

	void addComponent(WSEntityPtr component) {components_.push_back(component);}
	uint getOverallSize();
	void removeComponent(uint id);
	WSEntityPtr findComponentById(uint id);

private:
	std::list<WSEntityPtr> components_;

};

// ------------------------------------------------------------------

#if 0

class CWorldCharPlayer;
class CWorldCharMob;


class CWSCharacter: public CWSEntityBase
{
public:
	CWSCharacter(uint id);
	~CWSCharacter();

	virtual void update(ushort time);

	void toClientCharData(ClientCharData* data);

	/** Compute characteristics when character's state changes
	 *  it can be when we wearing some cloth or cast affect etc
	*/
	virtual void compute() {}

	float getVelocity(){return m_velocity;}

	/// Can this character see another?
	virtual bool seeCharacter(const CWSCharacter* ch, float& distance);

	/// Update visible lists
	void updatePosition();
	void updateVisible();
	void clearVisible();


	/// Returns pointer to CWorldCharPlayer if is a player
	virtual CWorldCharPlayer* player() {return NULL;}
	/// Returns pointer to CWorldCharMob if is a mob
	virtual CWorldCharMob* mob() {return NULL;}

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
	bool canHit() {return true;} //return WSWorld::m_pulse >= m_nextHitPulse;}
	/*! Hit character in close combat
	   \return
	    -true if we hitted him
	    -false if not
	*/
	bool hit(const CharacterPtr& ch);

	virtual void die(const CharacterPtr& who);

	/// Can character walk to this position?
	bool canWalk(const Vector2& pos);


	virtual void setPos(const Vector2& pos);
	Vector2 getPos() const {return m_pos;}

	virtual void setRotation(byte rot) {m_rotation = rot;}
	byte getRotation() const {return m_rotation;}

	bool inWorld() {return m_inWorld;}

	// CHECK THIS!!
	bool changeHP(int hp, const CharacterPtr& who);

protected:
	///Protected functions for operating with character
	void init();

	/** This function is called when character enters world
	*/
	virtual void onEnterWorld();

	/** This function is called when character leaves world
	*/
	virtual void onLeaveWorld();
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

protected:

	Vector2 m_pos;
	///Rotation
	byte m_rotation;

	/// Object is in world
	bool m_inWorld;

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
protected:
	CWSCharStats& getStats() {return m_stats;}

private:
	CWSCharStats	m_stats;
};

#endif

#endif
