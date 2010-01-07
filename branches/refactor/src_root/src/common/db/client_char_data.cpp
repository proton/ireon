/**
 * @file client_char_data.cpp
 * Character data for client side
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: client_char_data.cpp 433 2005-12-20 20:19:15Z zak $
 
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

#include "stdafx.h"
#include "db/client_char_data.h"

void ClientCharData::serialize(CData& data)
{
	if( data.store() )
	{
		data << m_id;
		data << m_velocity;
		data << m_fightSpeed;
	} else
	{
		data >> m_id;
		data >> m_velocity;
		data >> m_fightSpeed;
	}
};

void ClientCharPlayerData::serialize(CData& data)
{
	ClientCharData::serialize(data);
	if( data.store() )
	{
		data << m_name;
	} else
	{
		data >> m_name;
	}
};

void ClientOwnCharData::serialize(CData &data)
{
	ClientCharPlayerData::serialize(data);
	if( data.store() )
	{
	} else
	{
	}
};

void ClientCharMobData::serialize(CData& data)
{
	ClientCharData::serialize(data);
	if( data.store() )
	{
		data << m_prototype;
	} else
	{
		data >> m_prototype;
	}
};
