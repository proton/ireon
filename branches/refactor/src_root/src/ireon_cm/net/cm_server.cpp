/* Copyright (C) 2005 ireon.org developers council
 * $Id: cm_server.cpp 287 2005-11-26 09:34:33Z zak $

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
#include "net/cm_world_context.h"
#include "net/cm_server.h"
#include "db/cm_db.h"

bool CCMServer::start(ushort maximumNumberOfPeers, ushort localPort )
{
	if( !CNetServer::start(maximumNumberOfPeers,localPort) )
		return false;
	CCMWorldContext::initSignals();
	return true;
}

CNetClient* CCMServer::newContext(TCPsocket sock)
{
	assert(sock);
	CCMWorldContext* p = new CCMWorldContext(sock,m_socketSet);
	return p;
}

CCMWorldContext* CCMServer::findContext(uint id)
{
	std::list<CNetClient*>::iterator i;
	for( i = m_contexts.begin(); i != m_contexts.end(); i++ )
		if( ((CCMWorldContext*)(*i))->getWorldId() == id )
			return (CCMWorldContext*)(*i);
	return NULL;
};

void CCMServer::closeContext(CNetClient* context)
{
	CCMDB::instance()->removeWaitAccount((CCMWorldContext*)context);
	CNetServer::closeContext(context);
};
