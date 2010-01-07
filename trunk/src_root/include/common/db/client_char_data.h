/**
 * @file client_char_data.h
 * Character data for client side
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: client_char_data.h 527 2006-03-09 17:44:32Z zak $
 
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

#ifndef _CLIENT_CHAR_DATA
#define _CLIENT_CHAR_DATA

struct ClientCharData
{
	//virtual void serialize(CData& data);

	float m_velocity;
	byte m_fightSpeed;

	uint m_id;

	byte m_percentHP;
};

// --------------------------------------------------------

struct ClientCharPlayerData : public ClientCharData
{
	//void serialize(CData& data);

	String m_name;
};

// --------------------------------------------------------

struct ClientOwnCharData : public ClientCharPlayerData
{
	bool operator==(const ClientOwnCharData& d) const {return m_id == d.m_id;}

	//void serialize(CData& data);

	uint m_HP;
	uint m_maxHP;
	uint m_mana;
	uint m_maxMana;
	uint m_str;
	uint m_sta;
	uint m_int;
	uint m_wis;
	uint m_con;
	uint m_luck;
};

// --------------------------------------------------------

// used to exchange char information with rs
struct ClientCharRegistrationData
{
	ClientCharRegistrationData(): m_id(0){};
	bool operator==(const ClientCharRegistrationData& d) const {return m_id == d.m_id;}

	//void serialize(CData& data);

	uint m_id;
	String m_name;
};

// --------------------------------------------------------

struct ClientCharMobData : public ClientCharData
{
	//void serialize(CData& data);

	/// Mob prototype
	uint m_prototype;
};
#endif
