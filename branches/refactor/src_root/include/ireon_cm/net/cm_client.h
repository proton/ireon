/* Copyright (C) 2005 ireon.org developers council
 * $Id: cm_client.h 385 2005-12-11 22:59:30Z zak $

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
 * @file cm_client.h
 * Cluster manager network client (connecting to root server)
 */

#ifndef _CM_CLIENT_H
#define _CM_CLIENT_H

#include "net/net_client.h"

struct ClientOwnCharData;
class CCMClient;

typedef void (CCMClient::*CMCSig)(CData& data);

class CCMClient : public CNetClient
{
public:
	void initSignals();

	bool processCommand(CData& data, byte id);
public:
	/// Commands

	///Login, 0
	void login();

	///Ask for account data, 1
	void getAccount(const char* name, int64 sync);

	///Send char data, 2
	void sendNewChar(ClientOwnCharData* data);

	/// 3 = send world server

	/// New character, 4
	void sendNewChar(byte res, uint id, ClientOwnCharData* data);

	/// Remove char, 5
	void removeChar(byte res, uint id);
public:
	/// Command handlers

	///Login response, 0
	void onLogin(CData& data);

	///Receiving account data, 1
	void onSendAccount(CData& data);

	///Root server asks for a char data, 2
	void onGetChar(CData& data);

	///Root server asks for a world server, 3
	void onGetWorld(CData& data);

	///Root server sends new character, 4
	void onNewChar(CData& data);

	///Remove character command, 5
	void onRemoveChar(CData& data);
protected:

	std::vector<CMCSig> m_signals;
};

#endif
