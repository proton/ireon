/**
 * @file ireon_rs/accounts/character_info.h
 * character info class
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

#ifndef _GENERAL_CHARACTER_INFO_H_
#define _GENERAL_CHARACTER_INFO_H_

#include "ireon_rs/stdafx.h"

class CGeneralCharacterInfo;
#include "boost/shared_ptr.hpp"
typedef boost::shared_ptr<CGeneralCharacterInfo> CharacterInfoPointer;

class CGeneralCharacterInfo
{
public:
	CGeneralCharacterInfo(uint id, String name);

	// methods interacting with mysql
	static bool getCharactersForAccount(uint accountId, std::map<uint,CharacterInfoPointer> &charMap);
	bool removeFromDB();
	// -------------------------------------

	String	getName() { return m_name; }

private:
	uint	m_id;
	String	m_name;
};

#endif
