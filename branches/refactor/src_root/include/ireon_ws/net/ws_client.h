/* Copyright (C) 2005 ireon.org developers council
 * $Id: ws_client.h 288 2005-11-26 09:46:52Z zak $

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
 * @file ws_client.h
 * World server network client (connecting to cluster manager server)
 */

#ifndef _WS_CLIENT_H
#define _WS_CLIENT_H

#include "net/net_client.h"

struct ClientOwnCharData;
class CWSClient;

typedef void (CWSClient::*WSCSig)(CData& data);

class CWSClient : public CNetClient
{
public:
	void initSignals();

	bool processCommand(CData& data, byte id);
public:
	/// Commands

	///Login, 0
	void login();

	///Ask for account data, 1
	void getAccount(const char* name);

	///Send char data, 2
	void sendChar(ClientOwnCharData* data);

	///New character, 3
	void sendNewChar(byte res, uint id, ClientOwnCharData* data);

	/// 4 = remove character
public:
	/// Command handlers

	///Login responce, 0
	void onLogin(CData& data);

	///Receiving account data, 1
	void onSendAccount(CData& data);

	///Cluster manager asks for a char data, 2
	void onGetChar(CData& data);

	///Cluster manager sends us new character, 3
	void onNewChar(CData& data);

	///Remove character command, 4
	void onRemoveChar(CData& data);
protected:

	std::vector<WSCSig> m_signals;
};

#endif
