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
 * @file net_server.cpp
 * Network server
 */

#include "stdafx.h"
#include "net/net_server.h"
#include "net/net_client.h"


CNetServer::CNetServer():
m_socket(NULL),
m_socketSet(NULL)
{
}

CNetServer::~CNetServer()
{
	kickAll();
	if( m_socket )
	{
		if( m_socketSet )
		{
			SDLNet_TCP_DelSocket(m_socketSet,m_socket);
			SDLNet_FreeSocketSet(m_socketSet);
			m_socketSet = NULL;
		}
		SDLNet_TCP_Close(m_socket);
		m_socket = NULL;
	}
}

bool CNetServer::start(ushort maximumNumberOfPeers, ushort localPort )
{
	assert(m_socket == NULL);
	m_socketSet	= SDLNet_AllocSocketSet(maximumNumberOfPeers + 1); 
	if( !m_socketSet )
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlCritical,_("Can't start server. Error allocating socket set.\n"));
		return false;
	};
	IPaddress addr;
	SDLNet_ResolveHost(&addr, NULL, localPort);
	m_socket = SDLNet_TCP_Open(&addr);
	if ( m_socket == NULL )
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlCritical,_("Can't start server. Error creating socket.\n"));
		return false;
	};
	SDLNet_TCP_AddSocket(m_socketSet, m_socket);
	m_maxContexts = maximumNumberOfPeers;
	return true;
}
/*
void CNetServer::initializeSecurity()
{
	assert(m_server);
	m_server->InitializeSecurity(0,0);
}

void CNetServer::disableSecurity()
{
	assert(m_server);
	m_server->DisableSecurity();
}
*/
void CNetServer::processOutput()
{
	std::list<CNetClient*>::iterator it,nextIt;
	for( it = m_contexts.begin(); it != m_contexts.end(); it = nextIt )
	{
		nextIt = it;
		nextIt++;
		if( !((*it)->processOutput()) )
			closeContext(*it);
	}
}

void CNetServer::acceptContext()
{
	TCPsocket newSock = SDLNet_TCP_Accept(m_socket);
	if ( newSock == NULL )
		return;
	CNetClient* con = newContext(newSock);
	CLog::instance()->log(CLog::msgFlagNetwork,CLog::msgLvlInfo,_("New connection from %s.\n"),con->getHost());
	if( con )
		m_contexts.push_back(con);
};

void CNetServer::processInput()
{
	std::list<CNetClient*>::iterator it,nextIt;
	SDLNet_CheckSockets(m_socketSet, 0); 
	if ( SDLNet_SocketReady(m_socket) )
	{
		acceptContext();
		SDLNet_CheckSockets(m_socketSet, 0);
	}
 	for( it = m_contexts.begin(); it != m_contexts.end(); it = nextIt )
	{
		nextIt = it;
		nextIt++;
		if( !((*it)->processInput()) )
			closeContext(*it);
	}
};

void CNetServer::closeContext(CNetClient* context)
{
	if( !context )
		return;
	context->onClose();
	std::list<CNetClient*>::iterator it;
	for( it = m_contexts.begin(); it != m_contexts.end(); ++it )
		if( *it == context )
		{
			m_contexts.erase(it);
			break;
		}
	delete context;
};

void CNetServer::kickAll()
{
	std::list<CNetClient*>::iterator it,nextIt;
	for( it = m_contexts.begin(); it != m_contexts.end(); it = nextIt )
	{
		nextIt = it;
		nextIt++;
		closeContext(*it);
	}
};
