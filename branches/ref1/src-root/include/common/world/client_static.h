/**
 * @file client_static.h
 * Static object
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id$

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
#ifndef _CLIENT_STATIC_H
#define _CLIENT_STATIC_H

#include "world/object.h"
#include "world/geometry.h"

class CClientStaticObject : public CObject
{
public:
	CClientStaticObject(const StaticPrototypePtr& prot);
	~CClientStaticObject();

	bool load();
	bool unload();

	const StaticPrototypePtr& getPrototype() {return m_prot;}

	void setRotation(const Radian &q);
	Radian getRotation() {return m_rotation;}
	void setPosition(const Vector3& pos);
	Vector3 getPosition() const {return m_position;}

	bool isLoaded() const {return m_loaded;}

	std::pair<uint,uint>& getPage() {return m_page;}

	void setPosition(const Vector2& pos) {CObject::setPosition(pos);}

	std::vector<CSegment>& getSegments() {return m_segments;}

	bool canWalk(const Vector2& from, const Vector2& to, Vector2& norm);
protected:

	/// Update object's geometry and bounding square
	void updateGeometry();

	void setPage(uint x,uint z) {m_page.first = x; m_page.second = z;}

protected:
	bool m_loaded;

	StaticPrototypePtr m_prot;

	/// Rotation of the object. It can be rotated only round the Y axis
	Radian m_rotation;

	/// Position of the object.
	Vector3 m_position;

	/// Page in what we're situated
	std::pair<uint,uint> m_page;

	/// Impassable segments
	std::vector<CSegment> m_segments;

	/// Axis-aligned bounding square of this object
	Vector2 m_min;
	Vector2 m_max;
};

#endif