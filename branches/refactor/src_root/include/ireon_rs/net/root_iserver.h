/* Copyright (C) 2005 ireon.org developers council
 * $Id: root_iserver.h 286 2005-11-26 09:28:45Z zak $

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
 * @file root_iserver.h
 * Internal root server
 */
#ifndef _ROOT_ISERVER_H
#define _ROOT_ISERVER_H

#include "net/net_server.h"

class CRootIServer : public CNetServer
{
public:

	bool start(ushort maximumNumberOfPeers, ushort localPort);

	CNetClient* newContext(TCPsocket sock);

	void closeContext(CNetClient* context);

	///Find context by cluster id
	CRSWorldContext* findContext(uint id);
protected:

};
#endif

