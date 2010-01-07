/**
 * @file client_char_data.cpp
 * Character data for client side
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: client_char_data.cpp 527 2006-03-09 17:44:32Z zak $
 
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

#include "common/common_stdafx.h"
#include "common/db/client_char_data.h"
//
//void ClientCharData::serialize(CData& data)
//{
//	if( data.store() )
//	{
//		data << m_id;
//		data << m_velocity;
//		data << m_fightSpeed;
//		data << m_percentHP;
//	} else
//	{
//		data >> m_id;
//		data >> m_velocity;
//		data >> m_fightSpeed;
//		data >> m_percentHP;
//	}
//};
//
//void ClientCharPlayerData::serialize(CData& data)
//{
//	ClientCharData::serialize(data);
//	if( data.store() )
//	{
//		data << m_name;
//	} else
//	{
//		data >> m_name;
//	}
//};
//
//void ClientOwnCharData::serialize(CData &data)
//{
//	ClientCharPlayerData::serialize(data);
//	if( data.store() )
//	{
//		data << m_HP;
//		data << m_maxHP;
//		data << m_mana;
//		data << m_maxMana;
//		data << m_str;
//		data << m_sta;
//		data << m_int;
//		data << m_wis;
//		data << m_con;
//		data << m_luck;
//	} else
//	{
//		data >> m_HP;
//		data >> m_maxHP;
//		data >> m_mana;
//		data >> m_maxMana;
//		data >> m_str;
//		data >> m_sta;
//		data >> m_int;
//		data >> m_wis;
//		data >> m_con;
//		data >> m_luck;
//	}
//};
//
//void ClientCharMobData::serialize(CData& data)
//{
//	ClientCharData::serialize(data);
//	if( data.store() )
//	{
//		data << m_prototype;
//	} else
//	{
//		data >> m_prototype;
//	}
//};
//
//// -------------------------------------------------------
//
//void ClientCharRegistrationData::serialize(CData& data)
//{
//	if( data.store() )
//	{
//		data << m_id;
//		data << m_name;
//	} else
//	{
//		data >> m_id;
//		data >> m_name;
//	}
//};