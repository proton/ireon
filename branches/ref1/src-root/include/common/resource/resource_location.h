/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: resource_location.h 522 2006-03-06 16:49:30Z zak $

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
 * @file resource_location.h
 * Resource location
 */
#ifndef _RESOURCE_LOCATION_H
#define _RESOURCE_LOCATION_H

class CResourceLocation
{
public:
	CResourceLocation(const String& name): m_name(name) {}
	virtual ~CResourceLocation() {}

	/// Open the location
	virtual void open() = 0;

	/// Close the location
	virtual void close() = 0;

	/// Get list of entities
	virtual void list(StringVector* res) = 0;

	/// Load resource
	virtual DataPtr load(const String& name) = 0;
protected:

	String m_name;
};

#endif
