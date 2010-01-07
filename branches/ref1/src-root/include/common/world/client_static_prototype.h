/**
 * @file client_static_prototype.h
 * Prototype of static object
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
#ifndef _CLIENT_STATIC_PROTOTYPE_H
#define _CLIENT_STATIC_PROTOTYPE_H

#include "world/geometry.h"

class TiXmlNode;

class CClientStaticPrototype
{
public:
	CClientStaticPrototype();
	~CClientStaticPrototype();

	bool load(const String& resourceName);

	uint getId() const {return m_id;}

	const char* getMesh() const {return m_mesh.c_str();}
	const char* getName() const {return m_name.c_str();}

	const Vector3& getScale() const {return m_scale;}
	const Vector3& getSize() const {return m_size;}

	std::vector<CSegment>& getSegments() {return m_segments;}
protected:

	bool processOption(TiXmlNode* node);
	bool processSegment(TiXmlNode* node);
protected:

	// Identifier
	uint m_id;

	String m_name;

	/// Name of object's mesh
	String m_mesh;

	/* One of scale or size parameters can be used
	 * if scale parameter is specified, it will be used
	 * even if size paramter is specified.
	*/
	/// Scale by axes
	Vector3 m_scale;
	/// Size of object
	Vector3 m_size;

	/// Impassable segments
	std::vector<CSegment> m_segments;
};

#endif