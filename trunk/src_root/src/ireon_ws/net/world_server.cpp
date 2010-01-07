/* Copyright (C) 2005 ireon.org developers council
 * $Id: world_server.cpp 672 2006-08-16 18:33:53Z zak $

 *  See LICENSE for details
 */

/**
 * @file world_server.cpp
 * World server
 */
/*
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
*/