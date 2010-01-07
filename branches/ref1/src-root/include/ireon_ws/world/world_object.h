/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: world_object.h 522 2006-03-06 16:49:30Z zak $

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
 * @file world_object.h
 * Object
 */
#ifndef _WORLD_OBJECT_H
#define _WORLD_OBJECT_H

class CWorldObject
{
public:
	CWorldObject();
	virtual ~CWorldObject();

	virtual void setPos(const Vector2& pos) {m_pos = pos;}
	Vector2 getPos() const {return m_pos;}

	virtual void setRotation(byte rot) {m_rotation = rot;}
	byte getRotation() const {return m_rotation;}

	/// time im miliseconds
	virtual void update(uint time);

	bool inWorld() {return m_inWorld;}
protected:

	/** This function is called when character enters world
	*/
	virtual void onEnterWorld() {}

	/** This function is called when character leaves world
	*/
	virtual void onLeaveWorld() {}
protected:

	Vector2 m_pos;
	///Rotation
	byte m_rotation;

	/// Object is in world
	bool m_inWorld;
};

#endif
