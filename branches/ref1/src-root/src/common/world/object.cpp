/* Copyright (C) 2005 ireon.org developers council
 * $Id: object.cpp 510 2006-02-26 21:09:40Z zak $

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
		I_WORLD->sceneManager()->destroySceneNode(m_node->getName());
	}
	if( m_entity )
		I_WORLD->sceneManager()->removeEntity(m_entity);
};

String CObject::getUniqueName(const char* base)
{
	String entityName;
	SceneManager::EntityIterator iter = I_WORLD->sceneManager()->getEntityIterator();
	bool found = true;
	while( found )
	{
		entityName = base + StringConverter::toString(rand());
		found = false;
		while( iter.hasMoreElements() )
		{
			if( iter.peekNextKey() == entityName )
			{
				found = true;
				break;
			}
			iter.moveNext();
		}
	}
	return entityName;
}

void CObject::setPosition(const Vector3 &pos)
{
	if( m_node )
		m_node->setPosition(pos);
};

void CObject::setPosition(const Vector2& pos)
{
	Vector3 pos3(pos.x,0,pos.y);
	if( I_WORLD->loaded() )
	{
		pos3.y = I_WORLD->heightAt(pos3);
		Real y = I_WORLD->heightAt(pos3);
		y = 5;
	}
	setPosition(pos3);
};

void CObject::setOrientation(const Quaternion &q)
{
	if( m_node )
		m_node->setOrientation(q);
};

Vector3 CObject::getPosition()
{
	if( m_node )
		return m_node->getPosition();
	return Vector3::ZERO;
};

Vector2 CObject::getPosition2()
{
	if( m_node )
		return Vector2(m_node->getPosition().x, m_node->getPosition().z);
	return Vector2::ZERO;
};

Quaternion CObject::getOrientation()
{
	if( m_node )
		return m_node->getOrientation();
	return Quaternion::IDENTITY;
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
	return I_WORLD->pointIsVisible(getPosition());
};
