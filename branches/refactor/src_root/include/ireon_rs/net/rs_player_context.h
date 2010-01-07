/**
 * @file rs_player_context.h
 * Player connection context on root server
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: rs_player_context.h 433 2005-12-20 20:19:15Z zak $

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


#ifndef _RS_PLAYER_CONTEXT_H
#define _RS_PLAYER_CONTEXT_H


#include "net/net_client.h"

#include "common/net/generic_server_state.h"

#include "ireon_rs/accounts/user_account.h"
#include "db/account.h"


struct ClientOwnCharData;

class CRSPlayerContext;

typedef void (CRSPlayerContext::*CRSESig)(CData&);

class CRSPlayerContext : public CNetClient
{
public:

	enum State
	{
		STATE_NONE = 0,
		STATE_WAIT_WORLD
	};


	CRSPlayerContext(TCPsocket socket, SDLNet_SocketSet socketSet);

	bool processCommand(CData& data, ireon::net::commandId id);

	static void initSignals();

	// ----------------------
	enum ContextStates
	{
		csHello = 0,
		csCharacterSelection = 1
	};
	bool setState(byte newState);
	// ----------------------

	UserAccountPointer getOwnerAccount() { return m_ownerAccount; }
	void setOwnerAccount(UserAccountPointer newAccount) { m_ownerAccount = newAccount; }

public:
	///Commands

	/// 0 = login answer

	///Send destination world, 1
	void sendWorld(const char* host, uint port);

	/// create new character, 2
	void sendCreatedNewChar(byte res, ClientOwnCharData& d);

	/// remove char, 3
	void removeChar(uint id, byte res);

public:

	///Command handlers

	///Login attempt, 0
	void onLogin(CData& data);

	///Select char, 1
	void onSelectChar(CData& data);

	///Create new char, 2
	void onCreateChar(CData& data);

	///Remove char, 3
	void onRemoveChar(CData& data);


	AccPtr getAccount() { return m_account; }

private:
	CGenericServerState			*m_contextState;

	UserAccountPointer			m_ownerAccount;


	AccPtr m_account;

	byte m_state;

	///Signals
	static std::vector<CRSESig> m_signals;
};


#endif

