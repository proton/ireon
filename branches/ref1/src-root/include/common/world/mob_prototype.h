/**
 * @file mob_prototype.h
 * Mob prototype
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: mob_prototype.h 510 2006-02-26 21:09:40Z zak $
 
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

#ifndef _MOB_PROTOTYPE_H
#define _MOB_PROTOTYPE_H

class TiXmlNode;

class CMobPrototype
{
friend class CWorld;
protected:
	CMobPrototype();
public:
	~CMobPrototype();

	bool load(const String& resourceName);

	uint getId() const {return m_id;}

	const char* getMesh() const {return m_mesh.c_str();}

	const char* getName() const {return m_name.c_str();}

	Vector3 getScale() const {return m_scale;}
protected:

	bool processOption(TiXmlNode* option);
protected:

	// Identifier
	uint m_id;

	/// Mesh name
	String m_mesh;
	/// Change orientation to make it stand right
	Vector3 m_scale;

	/// Mob's name
	String m_name;
};

#endif