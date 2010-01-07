/* Copyright (C) 2005 ireon.org developers council
 * $Id: root_iserver.cpp 225 2005-11-21 18:11:56Z llyeli $

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
 * @file root_iserver.cpp
 * Root internal server
 */
#include "stdafx.h"
#include "net/rs_player_context.h"
#include "net/rs_world_context.h"
#include "net/root_eserver.h"
#include "net/root_iserver.h"


bool CRootIServer::start(ushort maximumNumberOfPeers, ushort localPort )
{
	if( !CNetServer::start(maximumNumberOfPeers,localPort) )
		return false;
	CRSWorldContext::initSignals();
	return true;
}

CNetClient* CRootIServer::newContext(TCPsocket sock)
{
	assert(sock);
	assert(m_socketSet);
	CRSWorldContext* p = new CRSWorldContext(sock,m_socketSet);
	return p;
}

void CRootIServer::closeContext(CNetClient* c)
{
	CNetServer::closeContext(c);
}

CRSWorldContext* CRootIServer::findContext(uint id)
{
	std::list<CNetClient*>::iterator i;
	for( i = m_contexts.begin(); i != m_contexts.end(); i++ )
		if( ((CRSWorldContext*)(*i))->getClusterId() == id )
			return (CRSWorldContext*)(*i);
	return NULL;
};