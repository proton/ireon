/**
 * @file common/net/client_connections_manager.cpp
 * Client's manager of connections
 */

 /* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: client_connections_manager.cpp 781 2006-10-07 19:52:08Z zak $

 *  See LICENSE for details
 */


#include "raknet/RakNetworkFactory.h"
#include "raknet/RakClientInterface.h"
#include "raknet/PacketEnumerations.h"
#include "raknet/NetworkTypes.h"

//#include "common/net/client_connections_manager.h"
#include "common/net/net_connection.h"

template <class T>
CClientConnectionsManager<T>::CClientConnectionsManager()
{
	m_rakClient = RakNetworkFactory::GetRakClientInterface();
	if (!m_rakClient)
		throw ireon::net::EUnableToInitializeClient();

	registerMessageHandler(ID_CONNECTION_REQUEST_ACCEPTED, static_cast<ServiceMessageHandler>(&CClientConnectionsManager::createNewConnection));
	registerMessageHandler(ID_DISCONNECTION_NOTIFICATION, static_cast<ServiceMessageHandler>(&CClientConnectionsManager::deleteConnection));
	registerMessageHandler(ID_CONNECTION_ATTEMPT_FAILED, static_cast<ServiceMessageHandler>(&CClientConnectionsManager::connectionFailed));
	registerMessageHandler(ID_CONNECTION_LOST, static_cast<ServiceMessageHandler>(&CClientConnectionsManager::deleteConnection));
	registerMessageHandler(ID_RECEIVED_STATIC_DATA, static_cast<ServiceMessageHandler>(&CGenericConnectionsManager::nullHandler));

	/*
	case ID_CONNECTION_BANNED:
	case ID_CONNECTION_ATTEMPT_FAILED:
	case ID_NO_FREE_INCOMING_CONNECTIONS:
	case ID_RECEIVED_STATIC_DATA:
	case ID_INVALID_PASSWORD:
	case ID_MODIFIED_PACKET:
	*/
}

// ----------------------------------------------------------------

template <class T>
CClientConnectionsManager<T>::~CClientConnectionsManager()
{
	m_rakClient->Disconnect(500);
	RakNetworkFactory::DestroyRakClientInterface(m_rakClient);
}

// ----------------------------------------------------------------

template <class T>
void CClientConnectionsManager<T>::connect(const char *serverHost, ushort serverPort)
{
	if (!m_rakClient->Connect(serverHost, serverPort, 0, 0, 30))
		throw ireon::net::EUnableToInitializeClient();
	m_rakClient->StartOccasionalPing();
}

// ----------------------------------------------------------------

template <class T>
void CClientConnectionsManager<T>::disconnect()
{
	m_rakClient->Disconnect(500);
	if (m_connection.get())
		m_connection.reset();
}

// ----------------------------------------------------------------

template <class T>
bool CClientConnectionsManager<T>::isConnected() 
{ 
	if (!m_connection.get())
		processInput();

	return m_connection.get() != NULL;

/*
	if (m_rakClient->IsConnected())
	{
		if (!m_connection.get())
		{
			processInput();
			return m_connection.get() != NULL;
		}
		else return true;
	} else
		return false;
*/
}

// ----------------------------------------------------------------

template <class T>
void CClientConnectionsManager<T>::processInput() 
{
	if (m_connection.get())
		m_connection->processPacket(ireon::net::netProcessCycleStart, ireon::net::netAddress::atInternalAddress);

	Packet * p;
	ireon::net::commandId packetIdentifier;
	char *dataStart;
	uint dataLength;

	p = m_rakClient->Receive();

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
			if ( !m_connection.get() )
			{
				_log (_warn, _loc, gtc_("Got user message but CNetConnection does not exist.") );
				throw ireon::net::EConnectionNotExist();
			}
			String tmp(dataStart, dataLength);
			m_connection->processPacket(packetIdentifier, tmp, ireon::net::netAddress(ireon::net::netAddress::atExternalAddress));

		}

		m_rakClient->DeallocatePacket(p);	
		p = m_rakClient->Receive();
	}
}


// ----------------------------------------------------------------

template <class T>
void CClientConnectionsManager<T>::send(uint playerId, ireon::net::commandId packetId, const char *data, uint dataLength, PacketPriority priority, PacketReliability reliability, byte orderingChannel)
{
	assert((data || (dataLength == 0)) && "CClientConnectionsManager::send: data pointer is NULL while data length is not 0.");
	if (static_cast<byte> (packetId) < ID_USER_PACKET_ENUM)
	{
		_log (_error, _loc, gtc_("Attempt to send system message (id = %d)."), static_cast<int> (packetId) );
		throw ireon::net::EIncorrectSendData();
	}
	RakNet::BitStream s;
	s.Write(static_cast<unsigned char> (packetId));
	s.Write(data, dataLength);
	m_rakClient->Send(&s, priority, reliability, orderingChannel);
	playerId;
}

// ----------------------------------------------------------------

template <class T>
void CClientConnectionsManager<T>::createNewConnection(const Packet *p)
{
	assert(p && "CClientConnectionsManager::createNewConnection: packet pointer is NULL");
	if ( m_connection.get() )
	{
		_log (_error, _loc, gtc_("Connection with server %s already exists."), p->playerId.ToString() );
		throw ireon::net::EConnectionExists();
	}

	CNetConnection *con = T::buildConnection();
	con->setConnectionManager(this);
	con->setConnectionInfo(p->playerId.ToString());
	con->setRemoteHost(p->playerId.ToString(false));
	m_connection.reset(con);

	m_rakClient->SetTimeoutTime(5000);

	_log (_info, _loc, gtc_("created new connection with server %s."), p->playerId.ToString() );
}

// ----------------------------------------------------------------

template <class T>
void CClientConnectionsManager<T>::deleteConnection(const Packet *p)
{
	assert(p && "CClientConnectionsManager::deleteConnection: packet pointer is NULL");
	if ( !m_connection.get() )
	{
		_log (_error, _loc, gtc_("Connection for deletion (server %s) does not exist."), p->playerId.ToString() );
		throw ireon::net::EConnectionNotExist();
	}

	m_connection.reset();

	_log (_info, _loc, gtc_("Removed connection with server %s."), p->playerId.ToString() );
}

// ----------------------------------------------------------------

template <class T>
void CClientConnectionsManager<T>::connectionFailed(const Packet *p)
{
	assert(p && "CClientConnectionsManager::connectionFailed: packet pointer is NULL");

	_log (_warn, _loc, gtc_("Connection attempt to server %s failed!"), p->playerId.ToString() );
}

