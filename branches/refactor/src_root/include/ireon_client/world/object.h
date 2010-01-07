/* Copyright (C) 2005 ireon.org developers council
 * $Id: object.h 397 2005-12-15 22:19:52Z llyeli $

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
 * @file object.h
 * Object class, base for all world object
 */
#ifndef _OBJECT_H
#define _OBJECT_H

#include "OgreOde_Core.h"

class CObject
{
public:
	CObject(const String& name);
	virtual ~CObject();

	enum Type
	{
		NONE,
		PLAYER,
		OWN_PLAYER,
		MOB
	};

public:

	virtual void setPosition(const Vector3& pos);
	/// Set 2d position (at terrain)
	virtual void setPosition(const Vector2& pos);

	Vector3 getPosition(); 
	/// Get 2d position
	Vector2 getPosition2();

	void setOrientation(const Quaternion& q);
	Quaternion getOrientation();

	/// Rotate around on local X axis
	void pitch(const Radian& angle);
	/// Rotate around local Y axis
	void yaw(const Radian& angle);
	/// Rotate around local Z axis
	void roll(const Radian& angle);

	void rotate(const Vector3& axis, const Radian& angle);
	void rotate(const Quaternion& q);

	OgreOde::Body* getBody(){return m_body;}

	const char* getName() {return m_name.c_str();}

	Type getType() {return m_type;}

	SceneNode* getSceneNode() {return m_node;}

	Entity* getEntity() {return m_entity;}

	virtual bool processCollision( OgreOde::Contact* contact ) {return false;}

	/// Update, time = time from last call
	virtual void update(Real time) {}

	virtual bool visible();
protected:

	OgreOde::Body* m_body;

	OgreOde::Geometry* m_geometry;

	String m_name;

	SceneNode* m_node;
	SceneNode* m_entityNode;

	Entity* m_entity;

	Type m_type;
};

#endif