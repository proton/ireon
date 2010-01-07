/* Copyright (C) 2005 ireon.org developers council
 * $Id: cm_world_context.h 287 2005-11-26 09:34:33Z zak $

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

#ifndef _CM_WORLD_CONTEXT_H
#define _CM_WORLD_CONTEXT_H

struct ClientOwnCharData;

#include "net/net_client.h"

class CCMWorldContext;
typedef void (CCMWorldContext::*CMWSig)(CData& data);

class CCMWorldContext : public CNetClient
{
public:
	CCMWorldContext(TCPsocket socket, SDLNet_SocketSet socketSet);

	static void initSignals();

	bool processCommand(CData& data, byte id);
public:
	///Commands

	/// 0 - login

	///Send account data, 1
	void sendAccount(CAccount* acc, const char* name, int64 sync);

	///Ask for player own char data, 2
	void getPlayerChar(uint id, int64 sync);

	///Send new character to world, 3
	void sendNewChar(ClientOwnCharData* p, uint acc);

	///Remove char, 4
	void removeChar(uint id);
public:
	///Command handlers

	///Login world server, 0
	void onLogin(CData& data);

	///World server asks for account, 1
	void onGetAccount(CData& data);

	///World server sends player own char data, 2
	void onSendPlayerChar(CData& data);

	///create new char, 3
	void onNewChar(CData& data);

	///Remove char, 4
	void onRemoveChar(CData& data);
public:
	uint getWorldId() {return m_worldId;}
	uint getWorldPort() {return m_worldPort;}
protected:

	///Id of world server
	uint m_worldId;
	///Port of world server
	uint m_worldPort;
	
	///Signals
	static std::vector<CMWSig> m_signals;
};


#endif
