/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: world_server.cpp 522 2006-03-06 16:49:30Z zak $

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
 * @file world_server.cpp
 * World server
 */
#include "stdafx.h"
#include "net/ws_player_context.h"
#include "net/world_server.h"
#include "db/world_db.h"
#include "db/account.h"
#include "world/world_char_player.h"

bool CWorldServer::start(ushort maximumNumberOfPeers, ushort localPort )
{
	if( !CNetServer::start(maximumNumberOfPeers,localPort) )
		return false;
	CWSPlayerContext::initSignals();
	return true;
}

CNetDataProcessor* CWorldServer::newContext(TCPsocket sock)
{
	assert(sock);
	assert(m_socketSet);
	CWSPlayerContext* p = new CWSPlayerContext(sock,m_socketSet);
	return p;
}

void CWorldServer::closeContext(CNetDataProcessor* context)
{
	CNetServer::closeContext(context);
}

CWSPlayerContext* CWorldServer::findContext(uint id)
{
	std::list<CNetDataProcessor*>::iterator i;
	for( i = m_contexts.begin(); i != m_contexts.end(); ++i )
		if( ((CWSPlayerContext*)(*i))->getAccount() && ((CWSPlayerContext*)(*i))->getAccount()->getId() == id )
			return (CWSPlayerContext*)(*i);
	return NULL;
};
