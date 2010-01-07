/* Copyright (C) 2005 ireon.org developers council
 * $Id: char_player.h 510 2006-02-26 21:09:40Z zak $
 
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
 * @file char_player.h
 * Player's character
 */

#ifndef _CHAR_PLAYER_H
#define _CHAR_PLAYER_H

struct ClientOwnCharData;
struct ClientCharData;
struct ClientCharPlayerData;

class CCharPlayerData :public CSynchronizer
{
	friend class CRootDB;
public:
	CCharPlayerData() {};
	~CCharPlayerData() {};

	///Initialize new character
	void init();

	void serialize(CData& d);

	void toClientOwnCharData(ClientOwnCharData& data);
	void toClientCharPlayerData(ClientCharPlayerData& data);
	void fromClientOwnCharData(ClientOwnCharData& data);

	uint getAccountId() {return m_accountId;}
	void setAccountId(uint id) {m_accountId = id; updSync();}

	uint getId() {return m_id;}

	///Name must be associated with id to avoid errors
	///void setName(const char* str) {m_name = str; updSync();}
	const char* getName() {return m_name.c_str();}

	Vector2 getStartPos() {return m_startPos;}
	void setStartPos(const Vector2& pos) {m_startPos = pos; updSync();}
protected:

	uint m_id;

	uint m_accountId;

	AccPtr m_account;

	String m_name;

	Vector2 m_startPos;
};

#endif