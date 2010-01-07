/**
 * @file char_mob.cpp
 * Mob
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: char_mob.cpp 433 2005-12-20 20:19:15Z zak $

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

#include "stdafx.h"
#include "world/char_mob.h"
#include "world/mob_prototype.h"

CCharMob::CCharMob(ClientCharMobData* data, const MobPrototypePtr& prot):
CCharacter(prot->getName(),prot->getMesh()),
m_prot(prot),
m_data(data)
{
	m_type = CObject::MOB;
	Quaternion q;
	q.FromAxes(Vector3::UNIT_X * m_prot->getScale().x,Vector3::UNIT_Y * m_prot->getScale().y,Vector3::UNIT_Z * m_prot->getScale().z);
	m_entityNode->setOrientation(q);
	assert(m_data);
	setCharData(data);
};

CCharMob::~CCharMob()
{
	if( m_data )
		delete m_data;
};

uint CCharMob::getId()
{
	return m_data->m_id;
};