/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: object.cpp 522 2006-03-06 16:49:30Z zak $

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
 * @file object.cpp
 * Object class
 */
#include "stdafx.h"
#include "world/object.h"
#include "world/world.h"

CObject::CObject(const String& name):
m_type(NONE),
m_name(name),
m_entity(NULL),
m_node(NULL),
m_entityNode(NULL)
{

};

CObject::~CObject()
{
	if( m_entityNode )
		m_entityNode->detachAllObjects();
	if( m_node )
	{
		m_node->detachAllObjects();
		m_node->removeAndDestroyAllChildren();
		CWorld::instance()->sceneManager()->destroySceneNode(m_node->getName());
	}
	if( m_entity )
		CWorld::instance()->sceneManager()->removeEntity(m_entity);
};

void CObject::setPosition(const Vector3 &pos)
{
	m_node->setPosition(pos);
};

void CObject::setPosition(const Vector2& pos)
{
	Vector3 pos3(pos.x,0,pos.y);
	if( CWorld::instance()->loaded() )
	{
		pos3.y = CWorld::instance()->heightAt(pos3);
		Real y = CWorld::instance()->heightAt(pos3);
		y = 5;
	}
	setPosition(pos3);
};

void CObject::setOrientation(const Quaternion &q)
{
	m_node->setOrientation(q);
};

Vector3 CObject::getPosition()
{
	return m_node->getPosition();
};

Vector2 CObject::getPosition2()
{
	return Vector2(m_node->getPosition().x, m_node->getPosition().z);
};

Quaternion CObject::getOrientation()
{
	return m_node->getOrientation();
};

void CObject::pitch(const Radian& angle)
{
	Vector3 xAxis = getOrientation() * Vector3::UNIT_X;
	rotate(xAxis, angle);
};

void CObject::yaw(const Radian& angle)
{
	Vector3 yAxis = getOrientation() * Vector3::UNIT_Y;
	rotate(yAxis, angle);
};

void CObject::roll(const Radian& angle)
{
	Vector3 zAxis = getOrientation() * Vector3::UNIT_Z;
	rotate(zAxis, angle);
};

void CObject::rotate(const Vector3 &axis, const Radian &angle)
{
	Quaternion q;
	q.FromAngleAxis(angle,axis);
	rotate(q);
};

void CObject::rotate(const Quaternion& q)
{
	setOrientation(getOrientation() * q);
};

bool CObject::visible()
{
	return CWorld::instance()->pointIsVisible(getPosition());
};
