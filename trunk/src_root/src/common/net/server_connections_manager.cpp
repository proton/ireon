/**
 * @file common/net/server_connections_manager.cpp
 * Server's manager of connections
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: server_connections_manager.cpp 781 2006-10-07 19:52:08Z zak $

 *  See LICENSE for details
 */


#include "raknet/RakNetworkFactory.h"
#include "raknet/RakServerInterface.h"
#include "raknet/PacketEnumerations.h"
#include "raknet/NetworkTypes.h"

//#include "common/net/server_connections_manager.h"
#include "common/net/net_connection.h"

template <class T>
CServerConnectionsManager<T>::CServerConnectionsManager()
{
	m_rakServer = RakNetworkFactory::GetRakServerInterface();
	if (!m_rakServer)
		throw ireon::net::EUnableToInitializeServer();

	registerMessageHandler(ID_NEW_INCOMING_CONNECTION, static_cast<ServiceMessageHandler>(&CServerConnectionsManager::createNewConnection));
	registerMessageHandler(ID_CONNECTION_LOST, static_cast<ServiceMessageHandler>(&CServerConnectionsManager::deleteConnection));
	registerMessageHandler(ID_DISCONNECTION_NOTIFICATION, static_cast<ServiceMessageHandler>(&CServerConnectionsManager::deleteConnection));
	registerMessageHandler(ID_RECEIVED_STATIC_DATA, static_cast<ServiceMessageHandler>(&CGenericConnectionsManager::nullHandler));
	/*
	ID_CONNECTION_BANNED
	ID_CONNECTION_REQUEST_ACCEPTED
	ID_CONNECTION_ATTEMPT_FAILED
	ID_NO_FREE_INCOMING_CONNECTIONS
	ID_RECEIVED_STATIC_DATA
	ID_INVALID_PASSWORD
	ID_MODIFIED_PACKET
	*/
}

// ----------------------------------------------------------------

template <class T>
CServerConnectionsManager<T>::~CServerConnectionsManager()
{
	m_rakServer->Disconnect(500);
	RakNetworkFactory::DestroyRakServerInterface(m_rakServer);

	for (std::map<ushort, CNetConnection *>::iterator it = m_connections.begin(); it != m_connections.end(); ++ it)
	{
		assert (it->second && "~CServerConnectionsManager: connection pointer is NULL");
		delete it->second;
	}
}

// ----------------------------------------------------------------

template <class T>
void CServerConnectionsManager<T>::startListening(ushort serverPort, ushort maxConnections)
{
	if (!m_rakServer->Start(maxConnections, 0, 100, serverPort))
		throw ireon::net::EUnableToInitializeServer();
	m_rakServer->StopSynchronizedRandomInteger();

}

// ----------------------------------------------------------------

template <class T>
void CServerConnectionsManager<T>::stopListening()
{
	kickAll();
	m_rakServer->Disconnect(500);
}

// ----------------------------------------------------------------

template <class T>
bool CServerConnectionsManager<T>::isActive() { return m_rakServer->IsActive(); }

// ----------------------------------------------------------------

template <class T>
void CServerConnectionsManager<T>::kickAll()
{
	std::map<ushort, CNetConnection *>::iterator it;
	for ( ushort i = 0; i < m_rakServer->GetAllowedPlayers(); ++i )
	{
		m_rakServer->Kick(m_rakServer->GetPlayerIDFromIndex(i));
		it = m_connections.find(i);
		if (  it != m_connections.end() )
		{
			if (it->second)
				delete it->second;
			m_connections.erase(it);
		}
	}
}

// ----------------------------------------------------------------

template <class T>
ushort CServerConnectionsManager<T>::getIndexByConnectionId(uint id)
{
	for ( std::map<ushort, CNetConnection *>::iterator it = m_connections.begin(); it != m_connections.end(); ++it )
		if ( it->second->getId() == id )
			return it->first;
	throw ireon::net::EConnectionNotExist();
}

// ----------------------------------------------------------------

template <class T>
CNetConnection & CServerConnectionsManager<T>::getConnectionById(uint id)
{
	return *m_connections[getIndexByConnectionId(id)];
}

// ----------------------------------------------------------------

template <class T>
void CServerConnectionsManager<T>::processInput()
{
	for ( std::map<ushort, CNetConnection *>::iterator it = m_connections.begin(); it != m_connections.end(); ++it )
			it->second->processPacket(ireon::net::netProcessCycleStart, ireon::net::netAddress::atInternalAddress);

	Packet * p;
	ireon::net::commandId packetIdentifier;
	char *dataStart;
	uint dataLength;

	p = m_rakServer->Receive();

	while (p)
	{
		getPacketInfo(p, packetIdentifier, dataStart, dataLength);
	
		if (m_serviceMessagesHandlers.find(packetIdentifier) != m_serviceMessagesHandlers.end())
			(this->*m_serviceMessagesHandlers[packetIdentifier])(p);	

		if (m_clientServiceMessagesHandlers.find(packetIdentifier) != m_clientServiceMessagesHandlers.end())
			m_clientServiceMessagesHandlers[packetIdentifier]->execute();

		// if this is not service packet, process it with CNetConnection::processPacket
		if ( packetIdentifier >= ID_USER_PACKET_ENUM )
		{
			std::map<ushort, CNetConnection *>::iterator it = m_connections.find(p->playerIndex);
			if (it == m_connections.end())
			{
				_log (_warn, _loc, gtc_("Got user message for unknown player index %d."), p->playerIndex );
				throw ireon::net::EConnectionNotExist();
			}
			String tmp(dataStart, dataLength);
			it->second->processPacket(packetIdentifier, tmp, ireon::net::netAddress(ireon::net::netAddress::atExternalAddress));

		}
		m_rakServer->DeallocatePacket(p);	
		p = m_rakServer->Receive();
	}
}

// ----------------------------------------------------------------

template <class T>
void CServerConnectionsManager<T>::send(uint playerId, ireon::net::commandId packetId, const char *data, uint dataLength, PacketPriority priority, PacketReliability reliability, byte orderingChannel)
{
	assert((data || (dataLength == 0)) && "CServerConnectionsManager::send: data pointer is NULL while data length is not 0.");
	if (static_cast<byte> (packetId) < ID_USER_PACKET_ENUM)
	{
		_log (_warn, _loc, gtc_("Attempt to send system message (id = %d)."), static_cast<int> (packetId) );
		throw ireon::net::EIncorrectSendData();
	}
	RakNet::BitStream s;
	s.Write(static_cast<unsigned char> (packetId));
	s.Write(data, dataLength);
	m_rakServer->Send(&s, priority, reliability, orderingChannel, 
						m_rakServer->GetPlayerIDFromIndex(getIndexByConnectionId(playerId)), false);
}

// ----------------------------------------------------------------

template <class T>
void CServerConnectionsManager<T>::createNewConnection(const Packet *p)
{
	assert(p && "CServerConnectionsManager::createNewConnection: packet pointer is NULL");
	if ( m_connections.find(p->playerIndex) != m_connections.end() )
	{
		_log (_error, _loc, gtc_("Connection with id %d already exists."), p->playerIndex );
		throw ireon::net::EConnectionExists();
	}

	CNetConnection *con = T::buildConnection();
	con->setConnectionManager(this);
	con->setConnectionInfo(p->playerId.ToString());
	con->setRemoteHost(p->playerId.ToString(false));
	m_connections[p->playerIndex] = con;
	
	m_rakServer->SetTimeoutTime(5000, p->playerId);

	_log (_info, _loc, gtc_("Created new connection with index %d, for client %s."), p->playerIndex, p->playerId.ToString() );
}

// ----------------------------------------------------------------

template <class T>
void CServerConnectionsManager<T>::deleteConnection(const Packet *p)
{
	assert(p && "CServerConnectionsManager::deleteConnection: packet pointer is NULL");
	std::map<ushort, CNetConnection *>::iterator it = m_connections.find(p->playerIndex);
	if ( it == m_connections.end() )
	{
		_log (_error, _loc, gtc_("Could not find connection for deletion (index = %d)."), p->playerIndex );
		throw ireon::net::EConnectionNotExist();
	}

	if (it->second)
		delete it->second;
	m_connections.erase(it);

	_log (_info, _loc, gtc_("Removed connection with index %d, for client %s."), p->playerIndex, p->playerId.ToString() );
}
