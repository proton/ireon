/**
 * @file ireon_rs/accounts/character_info.cpp
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

#include "ireon_rs/accounts/general_character_info.h"

#include "common/db/mysql_db.h"
#include <mysql++.h>

CGeneralCharacterInfo::CGeneralCharacterInfo(uint id, String name):
m_id(id),
m_name(name)
{
}

// --------------------------------------------------------------------

bool CGeneralCharacterInfo::getCharactersForAccount(uint accountId, std::map<uint,CharacterInfoPointer> &charMap)
{
	assert(accountId);

	CharacterInfoPointer					*charInfo;

 	mysqlpp::Query		query = CMySQLConnection::instance()->getQuery();

	query << "select * from `" << CMySQLConnection::instance()->getTableNamesPrefix() << 
				"characters` where `owner`=" << accountId;

	mysqlpp::Result result = query.store();

	if (result) {
		mysqlpp::Row row;
		mysqlpp::Row::size_type	i;
		for	(i = 0;	row	= result.at(i); ++i) {
			charInfo = new CharacterInfoPointer(new CGeneralCharacterInfo(row["id"], String(row["name"])));
			charMap.insert(std::pair<uint,CharacterInfoPointer>((uint) row["id"],*charInfo));
		}
		return true;
	}
	return false;
}

// --------------------------------------------------------------------

bool CGeneralCharacterInfo::removeFromDB()
{
	assert(m_id);

 	mysqlpp::Query		query = CMySQLConnection::instance()->getQuery();

	query << "delete from `" << CMySQLConnection::instance()->getTableNamesPrefix() << 
				"characters` where `id`=" << m_id;

	return query.exec(query.str());

}

