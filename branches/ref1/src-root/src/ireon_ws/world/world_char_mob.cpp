/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: world_char_mob.cpp 610 2006-04-20 11:04:59Z zak $

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
 * @file world_char_mob.cpp
 * Mob
 */

#include "stdafx.h"
#include "db/client_char_data.h"
#include "db/world_mob_prototype.h"
#include "world/ws_world.h"
#include "world/world_char_mob.h"

CWorldCharMob::CWorldCharMob():
m_startPos(Vector2::ZERO)
{
};

CWorldCharMob::~CWorldCharMob()
{
};

bool CWorldCharMob::serialize(CData& data)
{
	if( data.store() )
	{
		data << m_id;
		data.wrt(m_prototype->getId());
		data << m_startPos;
	} else
	{
		data >> m_id;
		uint id;
		data >> id;
		if( !(m_prototype = CWorld::instance()->getMobPrototype(id)) )
		{
			CLog::instance()->log(CLog::msgLvlCritical,"Can't find mob prototype %d for mob %d.\n",id,m_id);
			return false;
		};
		data >> m_startPos;
	}
	return true;
};

void CWorldCharMob::toClientCharMobData(ClientCharMobData& d)
{
	d.m_id = m_id;
	d.m_prototype = m_prototype->getId();
	toClientCharData(&d);
};

const char* CWorldCharMob::getName() const
{
	return m_prototype->getName();
};

uint CWorldCharMob::getProtId()
{
	return m_prototype->getId();
};


void CWorldCharMob::compute()
{
	m_velocity = m_prototype->getVelocity();
	m_fightSpeed = m_prototype->getFightSpeed();

	m_maxHP = m_prototype->getMaxHP();
	m_maxMana = m_prototype->getMaxMana();

	m_str = m_prototype->getStr();
	m_sta = m_prototype->getSta();
	m_int = m_prototype->getInt();
	m_wis = m_prototype->getWis();
	m_con = m_prototype->getCon();
	m_luck = m_prototype->getLuck();

	CWorldCharacter::compute();
};

void CWorldCharMob::update(uint time)
{
	if( CWorld::m_mobMovePulse && (rand() & 1) )
	{
		Real dirAngle = rand() * 2 * Math::PI / RAND_MAX;
		Vector2 dir(Math::Sin(dirAngle,true),Math::Cos(dirAngle,true));
		if	( dir.x > 0 && (getPos().x > CWorld::instance()->getMaxX()-50)
				|| dir.x < 0 && (-getPos().x > CWorld::instance()->getMaxX() - 50)
				)
			dir.x = -dir.x;
		if( dir.y > 0 && (getPos().y > CWorld::instance()->getMaxZ() - 50)
				|| dir.y < 0 && (-getPos().y > CWorld::instance()->getMaxZ() - 50)
			)
			dir.y = -dir.y;
		Vector2 pos = getPos() + dir * getVelocity() * 0.5f;
		if( canWalk(pos) )
			setPos(pos);
		updatePosition();
	};
	CWorldCharacter::update(time);
};

void CWorldCharMob::onEnterWorld()
{
	m_maxHP = m_prototype->getMaxHP();
	m_maxMana = m_prototype->getMaxMana();
	setPos(m_startPos);
	setHP(getMaxHP());
	setMana(getMaxMana());
	CWorldCharacter::onEnterWorld();
};

void CWorldCharMob::onLeaveWorld()
{
	CWorldCharacter::onLeaveWorld();
};

void CWorldCharMob::die(const CharacterPtr& who)
{
	CWorldCharacter::die(who);
	CharMobPtr me = CWorld::instance()->findCharMob(getId());
	if( me )
		CWorld::instance()->addMobRepop(CWorld::m_pulse + 200,me);
	CWorld::instance()->removeCharMob(getId());
};

void CWorldCharMob::repop()
{
};
