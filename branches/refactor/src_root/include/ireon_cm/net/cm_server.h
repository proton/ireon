/* Copyright (C) 2005 ireon.org developers council
 * $Id: cm_server.h 287 2005-11-26 09:34:33Z zak $

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
#ifndef _CM_SERVER_H
#define _CM_SERVER_H

#include "net/net_server.h"

class CCMWorldContext;

class CCMServer : public CNetServer
{
public:

	bool start(ushort maximumNumberOfPeers, ushort localPort);

	CNetClient* newContext(TCPsocket sock);

	void closeContext(CNetClient* context);

	///Find context by world id
	CCMWorldContext* findContext(uint id);

/*	CMWSig* signal(byte id) {return &m_signals[id];}
	int sigcount() {return (int)m_signals.size();};
*/
protected:

	/// List of worlds that waiting account data
	std::list< std::pair<String,uint> > m_waitAccounts;
};
#endif
