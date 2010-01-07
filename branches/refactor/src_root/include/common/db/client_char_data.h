/**
 * @file client_char_data.h
 * Character data for client side
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: client_char_data.h 433 2005-12-20 20:19:15Z zak $
 
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
	virtual void serialize(CData& data);

	float m_velocity;
	byte m_fightSpeed;

	uint m_id;
};

struct ClientCharPlayerData : public ClientCharData
{
	void serialize(CData& data);

	String m_name;
};

struct ClientOwnCharData : public ClientCharPlayerData
{
	bool operator==(const ClientOwnCharData& d) const {return m_id == d.m_id;}

	void serialize(CData& data);
};

struct ClientCharMobData : public ClientCharData
{
	void serialize(CData& data);

	/// Mob prototype
	uint m_prototype;
};
#endif
