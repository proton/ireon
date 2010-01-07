/* Copyright (C) 2005 ireon.org developers council
 * $Id: root_eserver.cpp 225 2005-11-21 18:11:56Z llyeli $

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
 * @file root_eserver.cpp
 * Root external server
 */
#include "stdafx.h"
#include "net/rs_player_context.h"
#include "net/rs_world_context.h"
#include "net/root_eserver.h"
#include "net/root_iserver.h"


bool CRootEServer::start(ushort maximumNumberOfPeers, ushort localPort )
{
	if( !CNetServer::start(maximumNumberOfPeers,localPort) )
		return false;
	CRSPlayerContext::initSignals();
	return true;
}

CNetClient* CRootEServer::newContext(TCPsocket sock)
{
	assert(sock);
	assert(m_socketSet);
	CRSPlayerContext* p = new CRSPlayerContext(sock,m_socketSet);
	return p;
}

void CRootEServer::closeContext(CNetClient* context)
{
	CNetServer::closeContext(context);
}


CRSPlayerContext* CRootEServer::findContext(uint id)
{
	std::list<CNetClient*>::iterator i;
	for( i = m_contexts.begin(); i != m_contexts.end(); ++i )
		if( ((CRSPlayerContext*)(*i))->getAccount().get() && ((CRSPlayerContext*)(*i))->getAccount()->getId() == id )
			return (CRSPlayerContext*)(*i);
	return NULL;
};