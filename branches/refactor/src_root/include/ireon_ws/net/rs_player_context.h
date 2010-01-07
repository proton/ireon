/* Copyright (C) 2005 ireon.org developers council

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
 * @file ws_player_context.h
 * Player connection context on world server
 */

#ifndef _RS_PLAYER_CONTEXT_H
#define _RS_PLAYER_CONTEXT_H

#include "db/account.h"

class CRSPlayerContext : public CClientContext
{
public:

	enum State
	{
		STATE_NONE = 0,
		STATE_WAIT_WORLD
	};

	CRSPlayerContext(CNetServer* server, PlayerID id);

	bool emit(byte id, CData& data);

public:
	///Commands

	/// 0 = login answer

	///Send destination world, 1
	void sendWorld(const char* host, uint port);

	/// 2 = create new character
public:

	///Command handlers

	///Login attempt, 0
	void onLogin(CData& data);

	///Select char, 1
	void onSelectChar(CData& data);

	///Create new char, 2
	void onCreateChar(CData& data);

public:

	AccPtr getAccount() { return m_account; }

protected:

	AccPtr m_account;

	byte m_state;
};


#endif