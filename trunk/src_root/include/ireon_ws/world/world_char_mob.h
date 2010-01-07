/* Copyright (C) 2005 ireon.org developers council
 * $Id: world_char_mob.h 801 2006-11-07 15:44:53Z zak $

 *  See LICENSE for details
 */

/**
 * @file world_char_mob.h
 * Mob
 */
#ifndef _WORLD_CHAR_MOB_H
#define _WORLD_CHAR_MOB_H

#include "ireon_ws/world/world_character.h"

struct ClientCharMobData;
class TiXmlElement;
class CData;

class CWorldCharMob : public CWSCharacter
{
public:

	CWorldCharMob(uint id, const TiXmlElement* xmlspec);
	~CWorldCharMob();

	static void setDefaults(const TiXmlElement* xmlspec);

	bool serialize(CData& d);

	String getProtType();

	const char* getName();

	CWorldCharMob* mob() {return this;}

	void update(ushort time);
	void update(ushort time, Vector2 accel);

	/** Compute characteristics when character's state changes
	 *  it can be when we wearing some cloth or cast effect etc
	*/
	void compute();

	/// Fill structure
	void toClientCharMobData(ClientCharMobData& data);

	const char* getName() const;

	void die(const CharacterPtr& who);

	/// Repopulation
	void repop();

	void onLeaveWorld();

private:

	void onEnterWorld();

	WorldMobProtPtr m_prototype;
	Vector2 m_startPos;
	Vector2 m_speed;

	static Vector2 m_defaultOrigin;
	static int	   m_maxRandomDeviation;
};

// ------------------------------------------------

class CMobFlock: public CWSEntityBase
{
public:
	CMobFlock(uint id, TiXmlElement* xmlspec);
	void moveMobs(uint time);

private:
	/// flock origin, zone center
	Vector2 m_origin;
	/// zone radius
	uint m_zoneRadius;
	/// maximum flock radius
	uint m_flockRadius;
	/// centripetal force for mobs
	Real m_centripetalRate;
	Real m_roamRate;
	Real m_repulsionRadius;
	Real m_repulsionRate;
	Real m_codirectionalRate;

	/// current flock center
	Vector2 m_midpoint;

	std::list<CharMobPtr> m_mobs;

	/// returns vector from old midpoint to new
	Vector2 recalcMidpoint();
	void addMob(TiXmlElement* xmlspec);
	void setOptions(TiXmlElement* xmlspec);
	void initMobs();
};

#endif
