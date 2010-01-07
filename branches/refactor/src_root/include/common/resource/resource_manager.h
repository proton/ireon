/* Copyright (C) 2005 ireon.org developers council
 * $Id: resource_manager.h 288 2005-11-26 09:46:52Z zak $

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
 * @file resource_manager.h
 * Resource manager
 */
#ifndef _RESOURCE_MANAGER_H
#define _RESOURCE_MANAGER_H

class CResourceLocation;

class CResourceManager
{
public:

	static CResourceManager* instance() { if( !m_singleton ) m_singleton = new CResourceManager(); return m_singleton; }

	void addLocation(const String& loc, const String& type);

	DataPtr load(const String& name);

protected:

	/// List of resource locations
	std::vector<CResourceLocation*> m_locations;
	/// Vector of available resource names
	std::map<String,CResourceLocation*> m_resources;

	static CResourceManager* m_singleton;
};

#endif
