/**
 * @file common/net/generic_connections_manager.cpp
 * Generic network connections manager class
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: generic_connections_manager.cpp 781 2006-10-07 19:52:08Z zak $

 * See LICENSE for details
 */

#include "common/common_stdafx.h"
#include "common/net/generic_connections_manager.h"
#include "raknet/NetworkTypes.h"
#include "raknet/PacketEnumerations.h"

/*
CGenericConnectionsManager::CGenericConnectionsManager(CConnectionBuilderBase *connectionsBuilder):
m_connectionsBuilder(connectionsBuilder)
{
	assert(m_connectionsBuilder.get() && "CGenericConnectionsManager: Connection builder pointer is NULL.");
}
*/
CGenericConnectionsManager::~CGenericConnectionsManager()
{
	for (std::map<byte, CNetMessageProcessCommand *>::iterator it = m_clientServiceMessagesHandlers.begin(); it != m_clientServiceMessagesHandlers.end(); ++it)
		delete it->second;
}

// ----------------------------------------------------------------

void CGenericConnectionsManager::registerMessageHandler(byte msgId, ServiceMessageHandler handler)
{
	assert(handler && "CGenericConnectionsManager::registerMessageHandler: could not register NULL handler.");
	if ( m_serviceMessagesHandlers.find(msgId) != m_serviceMessagesHandlers.end() )
		_log(_warn, _loc, gtc_("service message handler already exists (msg id = %d)"), msgId );
	if ( msgId >= ID_USER_PACKET_ENUM )
		_log(_warn, _loc, gtc_("registering handler for user packets (msg id = %d)"), msgId );
	m_serviceMessagesHandlers[msgId] = handler;
}

// ----------------------------------------------------------------

void CGenericConnectionsManager::registerClientMessageHandler(byte msgId, CNetMessageProcessCommand *handler)
{
	assert(handler && "CGenericConnectionsManager::registerClientMessageHandler: could not register NULL handler.");
	if ( m_clientServiceMessagesHandlers.find(msgId) != m_clientServiceMessagesHandlers.end() )
		_log(_warn, _loc, gtc_("client service message handler already exists (msg id = %d)"), msgId );
	if ( msgId >= ID_USER_PACKET_ENUM )
		_log(_warn, _loc, gtc_("registering client handler for user packets (msg id = %d)"), msgId );
	m_clientServiceMessagesHandlers[msgId] = handler;
}

// ----------------------------------------------------------------

void CGenericConnectionsManager::getPacketInfo(const Packet *p, ireon::net::commandId &packetId, char *&packetDataStart, uint &packetDataLength)
{
	assert(p && "CServerConnectionsManager::getPacketInfo: packet pointer is NULL.");
	assert(packetDataStart && "CServerConnectionsManager::getPacketInfo: packetDataStart pointer is NULL.");
	if ( (byte) p->data[0] == ID_TIMESTAMP )
	{
		if ( p->length > sizeof(byte) + sizeof(unsigned int) )
		{
			packetId = static_cast<ireon::net::commandId> (p->data[sizeof(byte) + sizeof(unsigned int) ]);
			packetDataStart = reinterpret_cast<char *> (p->data + 1 + sizeof(unsigned int));
			if (p->bitSize % 8) // should be bytes-aligned
				throw ireon::net::EIncorrectPacketData();
			packetDataLength = static_cast<uint> ((p->bitSize) / 8 - 1 - sizeof(unsigned int));
		}
		else
			throw ireon::net::EIncorrectPacketData();
	}
	else
	{
		packetId = static_cast<ireon::net::commandId> (p->data[0]);
		packetDataStart = reinterpret_cast<char *> (p->data + 1);
		if (p->bitSize % 8) // should be bytes-aligned
			throw ireon::net::EIncorrectPacketData();
		packetDataLength = static_cast<uint> ((p->bitSize) / 8 - 1);
	}
}

// ----------------------------------------------------------------

void CGenericConnectionsManager::nullHandler(const Packet *p)
{
	// do nothing
	p;
}

// ----------------------------------------------------------------

