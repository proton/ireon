/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: world_server.h 522 2006-03-06 16:49:30Z zak $

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
 * @file world_server.h
 * World server
 */
#ifndef _WORLD_SERVER_H
#define _WORLD_SERVER_H

#include "net/net_server.h"

class CWorldServer : public CNetServer
{
public:

	bool start(ushort maximumNumberOfPeers, ushort localPort);

	CNetDataProcessor* newContext(TCPsocket sock);

	void closeContext(CNetDataProcessor* context);

	///Find context by account id
	CWSPlayerContext* findContext(uint id);

};
#endif
