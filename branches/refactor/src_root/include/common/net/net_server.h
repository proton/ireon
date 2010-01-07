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
 * @file net_server.h
 * Network server
 */
#ifndef _NET_SERVER_H
#define _NET_SERVER_H

#include "SDL_net.h"

class CNetClient;

class CNetServer
{
public:

	CNetServer();
	virtual ~CNetServer();

	virtual bool start( ushort maximumNumberOfPeers, ushort localPort);

	///Receive and process all incoming packets
	virtual void processInput();

	void acceptContext();
	virtual CNetClient* newContext(TCPsocket socket) = 0;

	virtual void closeContext(CNetClient* context);

//	virtual CClientContext* findContext(PlayerID playerId) = 0;

	///Process and send all outcoming packets
	virtual void processOutput();

	/// Kick all clients
	void kickAll();
protected:
	
	///Connection, using RakNet
	TCPsocket m_socket;
	SDLNet_SocketSet m_socketSet;

	std::list<CNetClient*> m_contexts;

	uint m_maxContexts;
};

#endif


