/* Copyright (C) 2005 ireon.org developers council
 * $Id: char_player.cpp 510 2006-02-26 21:09:40Z zak $

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
 * @file char_player.cpp
 * Player's character
 */

#include "stdafx.h"
#include "db/char_player_data.h"
#include "db/client_char_data.h"

void CCharPlayerData::serialize(CData& data)
{
	CSynchronizer::serialize(data);
	if( data.store() )
	{
		data << m_id;
		data << m_accountId;
		data << m_name;
	} else
	{
		data >> m_id;
		data >> m_accountId;
		data >> m_name;
	}
};

void CCharPlayerData::init()
{
	setStartPos(Vector2(0,0));
};

void CCharPlayerData::toClientOwnCharData(ClientOwnCharData& d)
{
	toClientCharPlayerData(d.m_charPlayerData);
};

void CCharPlayerData::toClientCharPlayerData(ClientCharPlayerData &data)
{
	data.m_id = m_id;
	data.m_name = m_name;
};

void CCharPlayerData::fromClientOwnCharData(ClientOwnCharData &data)
{
	m_id = data.m_charPlayerData.m_id;
	m_name = data.m_charPlayerData.m_name;
};