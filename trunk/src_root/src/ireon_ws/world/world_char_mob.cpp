/* Copyright (C) 2005 ireon.org developers council
 * $Id: world_char_mob.cpp 806 2006-11-08 14:02:44Z zak $

 *  See LICENSE for details
 */

/**
 * @file world_char_mob.cpp
 * Mob
 */

#include "ireon_ws/stdafx.h"
#include "common/db/client_char_data.h"
#include "ireon_ws/db/world_mob_prototype.h"
#include "ireon_ws/world/ws_world.h"
#include "ireon_ws/world/world_char_mob.h"

#include "tinyxml/tinyxml.h"
#include "common/data.h"

Vector2 CWorldCharMob::m_defaultOrigin = Vector2(0.0f, 0.0f);
int CWorldCharMob::m_maxRandomDeviation = 30;

const Real MAX_MOB_SPEED = 5.0f;


CWorldCharMob::CWorldCharMob(uint id, const TiXmlElement* xmlspec):
m_startPos(Vector2::ZERO),
m_speed(Vector2(0.0f, 0.0f)),
CWSCharacter(id)
{

	if (xmlspec)
	{
		if(xmlspec->Attribute("pos"))
		{
			StringVector vec = StringConverter::parseStringVector(xmlspec->Attribute("pos"));
			if (vec.size() == 2)
				m_startPos = Vector2(StringConverter::parseReal(vec[0]), StringConverter::parseReal(vec[1]));
			else
			{
				CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlWarn,__FUNCTION__, _("Incorrect position specification for mob %d, setting default..."), m_id);
				m_startPos = Vector2(m_defaultOrigin.x + (double) rand()/RAND_MAX * m_maxRandomDeviation - m_maxRandomDeviation/2,
										m_defaultOrigin.y + (double) rand()/RAND_MAX * m_maxRandomDeviation - m_maxRandomDeviation/2);
			}
		}
		else
	 		m_startPos = Vector2(m_defaultOrigin.x + (double) rand()/RAND_MAX * m_maxRandomDeviation - m_maxRandomDeviation/2,
									m_defaultOrigin.y + (double) rand()/RAND_MAX * m_maxRandomDeviation - m_maxRandomDeviation/2);
		String mob_type;
		if (xmlspec->Attribute("type"))
			mob_type = xmlspec->Attribute("type");
		else 
			mob_type = "demon";
		m_prototype = WSWorld::instance()->getMobPrototype(mob_type);
	}
};

CWorldCharMob::~CWorldCharMob()
{
};

void CWorldCharMob::setDefaults(const TiXmlElement* xmlspec)
{
	if ((xmlspec) && (xmlspec->Attribute("def_origin")))
	{
		StringVector vec = StringConverter::parseStringVector(xmlspec->Attribute("def_origin"));
		if (vec.size() == 2)
			m_defaultOrigin = Vector2(StringConverter::parseReal(vec[0]), StringConverter::parseReal(vec[1]));
		else
			CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlWarn,__FUNCTION__, _("Incorrect default origin for zone (see mobs.xml)."));
	}

	if ((xmlspec) && (xmlspec->Attribute("max_deviation")))
		m_maxRandomDeviation = StringConverter::parseInt(xmlspec->Attribute("max_deviation"));
}

bool CWorldCharMob::serialize(CData& data)
{
	if( data.store() )
	{
		data << m_id;
		data << m_prototype->getType();
		data << m_startPos;
	} else
	{
		data >> m_id;
		String id;
		data >> id;
		if( !(m_prototype = WSWorld::instance()->getMobPrototype(id)) )
		{
			CLog::instance()->log(CLog::msgLvlCritical,__FUNCTION__,"Can't find mob prototype %s for mob %d.\n",id,m_id);
			return false;
		};
		data >> m_startPos;
	}
	return true;
};

void CWorldCharMob::toClientCharMobData(ClientCharMobData& d)
{
//	d.m_id = m_id;
//	d.m_prototype = m_prototype->getType();
//	toClientCharData(&d);
};

const char* CWorldCharMob::getName() const
{
	return m_prototype->getName();
};

String CWorldCharMob::getProtType()
{
	return m_prototype->getType();
};


void CWorldCharMob::compute()
{
	m_velocity = m_prototype->getVelocity();
	m_fightSpeed = m_prototype->getFightSpeed();

	getStats().setMaxHP(m_prototype->getMaxHP());
/*
	m_maxMana = m_prototype->getMaxMana();

	m_str = m_prototype->getStr();
	m_sta = m_prototype->getSta();
	m_int = m_prototype->getInt();
	m_wis = m_prototype->getWis();
	m_con = m_prototype->getCon();
	m_luck = m_prototype->getLuck();
*/ 
	CWSCharacter::compute();
};

void CWorldCharMob::update(uint time)
{
// 	if( WSWorld::m_mobMovePulse && (rand() & 1) )
// 	{
		Real dirAngle = rand() * 2 * Math::PI / RAND_MAX;
		Vector2 dir(Math::Sin(dirAngle,true),Math::Cos(dirAngle,true));
		if	( dir.x > 0 && (getPos().x > WSWorld::instance()->getMaxX()-50)
				|| dir.x < 0 && (-getPos().x > WSWorld::instance()->getMaxX() - 50)
				)
			dir.x = -dir.x;
		if( dir.y > 0 && (getPos().y > WSWorld::instance()->getMaxZ() - 50)
				|| dir.y < 0 && (-getPos().y > WSWorld::instance()->getMaxZ() - 50)
			)
			dir.y = -dir.y;
		Vector2 pos = getPos() + dir * getVelocity() * 0.5f;
// 		if( canWalk(pos) )
// 			setPos(pos);
// 		updatePosition();
// 	};
	CWSCharacter::update(time);
};

void CWorldCharMob::update(uint time, Vector2 accel)
{
// 	if( WSWorld::m_mobMovePulse)
// 	{
		if (rand() > RAND_MAX/2)
		{
			m_speed += accel;
			if (m_speed.length() > MAX_MOB_SPEED)
				m_speed.normalise();
		}
	 	Vector2 pos = getPos() + m_speed * getVelocity() * 0.5f;
		if( canWalk(pos) )
			setPos(pos);
	 	updatePosition();
//	}
	CWSCharacter::update(time);
};

void CWorldCharMob::onEnterWorld()
{
	getStats().setMaxHP(m_prototype->getMaxHP());
// 	m_maxMana = m_prototype->getMaxMana();
	setPos(m_startPos);
	getStats().setHP(getStats().getMaxHP());
// 	setMana(getMaxMana());
	CWSCharacter::onEnterWorld();
};

void CWorldCharMob::onLeaveWorld()
{
	CWSCharacter::onLeaveWorld();
};

void CWorldCharMob::die(const CharacterPtr& who)
{
	CWSCharacter::die(who);
// 	CharMobPtr me = WSWorld::instance()->findCharMob(getId());
// 	if( me )
// 		WSWorld::instance()->addMobRepop(WSWorld::m_pulse + 200,me);
	WSWorld::instance()->removeEntity(getId());
};

void CWorldCharMob::repop()
{
};

// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------

CMobFlock::CMobFlock(uint id, TiXmlElement* xmlspec): 
m_midpoint(Vector2::ZERO), 
m_origin(Vector2(0.0f, 0.0f)),
m_zoneRadius(30),
m_flockRadius(7),
m_centripetalRate(2.0f),
m_roamRate(1.0f),
m_repulsionRadius(1.0f),
m_repulsionRate(4.0f),
m_codirectionalRate(1.0f),
CWSEntityBase(id)
{
	setOptions(xmlspec);
	TiXmlNode* childNode;
	for ( childNode = xmlspec->FirstChild(); childNode != 0; childNode = childNode->NextSibling() )
	{
		if ((childNode->Type() == TiXmlNode::ELEMENT) && !strcmp(childNode->Value(), "mob_group"))
		{
			int gsize = 0;
			if ((childNode) && (childNode->ToElement()->Attribute("group_size")))
				gsize = StringConverter::parseInt(childNode->ToElement()->Attribute("group_size"));
				if (gsize<0)
				{
					CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlDebug,__FUNCTION__, _("Incorrect mob group size"));
					continue;
				}
				for	(int i = 0; i<gsize; ++i)
				 	addMob(childNode->ToElement());
		}
		if ((childNode->Type() == TiXmlNode::ELEMENT) && !strcmp(childNode->Value(), "mob"))
		 	addMob(childNode->ToElement());
	}
	initMobs();
};

// ------------------------------------------------------

void CMobFlock::setOptions(TiXmlElement* xmlspec)
{
	if ((xmlspec) && (xmlspec->Attribute("origin")))
	{
		StringVector vec = StringConverter::parseStringVector(xmlspec->Attribute("origin"));
		if (vec.size() == 2)
			m_origin = Vector2(StringConverter::parseReal(vec[0]), StringConverter::parseReal(vec[1]));
		else
			CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlWarn,__FUNCTION__, _("Incorrect origin for flock."));
	}

	if ((xmlspec) && (xmlspec->Attribute("zone_radius")))
		m_zoneRadius = StringConverter::parseInt(xmlspec->Attribute("zone_radius"));
	if (m_zoneRadius < 5)
		m_zoneRadius = 5;

	if ((xmlspec) && (xmlspec->Attribute("zone_radius")))
		m_flockRadius = StringConverter::parseInt(xmlspec->Attribute("flock_radius"));
	if (m_flockRadius < 2)
		m_flockRadius = 2;

	if (m_flockRadius > m_zoneRadius * 0.7f)
		m_flockRadius = (int) m_zoneRadius * 0.7f;

	if ((xmlspec) && (xmlspec->Attribute("centripetal_rate")))
		m_centripetalRate = StringConverter::parseReal(xmlspec->Attribute("centripetal_rate"));
	if ((m_centripetalRate < 0.2f) || (m_centripetalRate > 20.0f))
		m_centripetalRate = 2.0f;

	if ((xmlspec) && (xmlspec->Attribute("roam_rate")))
		m_roamRate = StringConverter::parseReal(xmlspec->Attribute("roam_rate"));
	if ((m_roamRate < 0.2f) || (m_roamRate > 20.0f))
		m_roamRate = 1.0f;

	if ((xmlspec) && (xmlspec->Attribute("repulsion_radius")))
		m_repulsionRadius = StringConverter::parseReal(xmlspec->Attribute("repulsion_radius"));
	if ((m_repulsionRadius < 0.2f) || (m_repulsionRadius > 20.0f))
		m_repulsionRadius = 1.0f;

	if ((xmlspec) && (xmlspec->Attribute("repulsion_rate")))
		m_repulsionRate = StringConverter::parseReal(xmlspec->Attribute("repulsion_rate"));
	if ((m_repulsionRate < 0.2f) || (m_repulsionRate > 20.0f))
		m_repulsionRate = 4.0f;

	if ((xmlspec) && (xmlspec->Attribute("codirectional_rate")))
		m_codirectionalRate = StringConverter::parseReal(xmlspec->Attribute("codirectional_rate"));
	if ((m_codirectionalRate < 0.2f) || (m_codirectionalRate > 20.0f))
		m_codirectionalRate = 1.0f;
};

// ------------------------------------------------------

void CMobFlock::initMobs()
{
	std::list<CharMobPtr>::iterator mobIt;
	for( mobIt = m_mobs.begin(); mobIt != m_mobs.end(); ++mobIt )
		(*mobIt)->setPos(m_origin + Vector2((double) rand()/RAND_MAX * m_flockRadius - m_flockRadius/2, (double) rand()/RAND_MAX * m_flockRadius - m_flockRadius/2));
}

// ------------------------------------------------------

void CMobFlock::addMob(TiXmlElement* xmlspec)
{
//	CharMobPtr mob(new CWorldCharMob(xmlspec));
//	WSWorld::instance()->insertCharMob(mob);
	m_mobs.push_back(CharMobPtr(new CWorldCharMob(WSWorld::instance()->getNewEntityID(), xmlspec)));
};																				

// ------------------------------------------------------

const Real RANDOM_DEVIATION_FACTOR = 0.1f;

void CMobFlock::moveMobs(uint time)
{
	Vector2 flockDirection = recalcMidpoint();
	flockDirection.normalise();

	std::list<CharMobPtr>::iterator mobIt;
	for( mobIt = m_mobs.begin(); mobIt != m_mobs.end(); ++mobIt )
	{
		// rule 1
		// keep close to midpoint
		Vector2 force = m_midpoint - (*mobIt)->getPos();
		Real len = force.length();
		force = (force / m_flockRadius) * m_centripetalRate;
		if ((len < m_flockRadius * 0.7f) && (len > 0.3f))
		{
	 		force.x += (double) (rand() / RAND_MAX * m_roamRate - m_roamRate/2) / len;
	 		force.y += (double) (rand() / RAND_MAX * m_roamRate - m_roamRate/2) / len;
		}

		// rule 2
		// avoid collisions
		std::list<CharMobPtr>::iterator mobIt2;
		for( mobIt2 = m_mobs.begin(); mobIt2 != m_mobs.end(); ++mobIt2 )
		{
			Vector2 dist = (*mobIt)->getPos() - (*mobIt2)->getPos();
			if (!dist.isZeroLength() && (dist.length() < m_repulsionRadius))
				force += (dist / (dist.length() * m_mobs.size())) * m_repulsionRate;
		}

		// rule 3
		// stay aligned with flock direction
		force += (flockDirection * m_codirectionalRate);

		(*mobIt)->update(time, force.randomDeviant(RANDOM_DEVIATION_FACTOR));
	}
};

// ------------------------------------------------------

Vector2 CMobFlock::recalcMidpoint()
{
	Vector2 oldMidpoint = m_midpoint;
	m_midpoint.x = 0;
	m_midpoint.y = 0;
	std::list<CharMobPtr>::iterator mobIt;
	for( mobIt = m_mobs.begin(); mobIt != m_mobs.end(); ++mobIt )
		m_midpoint += (*mobIt)->getPos();
	if (m_mobs.size()>0)
		m_midpoint /= m_mobs.size();

	Vector2 radius = m_midpoint - m_origin;
	if (radius.length() > m_zoneRadius * 0.8f)
	{
		radius.normalise();
		m_midpoint = m_origin + radius * 0.8f * m_zoneRadius;
	}

	if (oldMidpoint == Vector2::ZERO)
		return Vector2(0.0f, 0.0f);
	return m_midpoint - oldMidpoint;
};


