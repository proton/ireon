/**
 * @file common/net/connections_manager.h
 * Expanded templated version of old CNetServer class
 * Should replace all CNetServer, CRootEServer etc hierarchy
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id$

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



#ifndef _CONNECTIONS_MANAGER_H_
#define _CONNECTIONS_MANAGER_H_

#include "common/net/layers/net_layer_base.h"
#include "common/net/layers/sock_layer.h"
#include "common/net/layers/compressing_layer.h"
#include "common/net/net_connection.h"

template <class Connection>
class CIncomingConnectionsManager
{
public:

	CIncomingConnectionsManager();
	~CIncomingConnectionsManager();

	bool start(ushort localPort, ushort maximumNumberOfPeers);

	void processNewConnections();
	///Receive and process all incoming packets
	void processInput();
	///Process and send all outcoming packets
	void processOutput();

	///Find connection by id
	CNetConnection* findConnection(uint id);

	/// Kick all clients
	void kickAll();

private:
//	void acceptConnection();
	void closeConnection(CNetConnection* connection);

	CIncomingConnectionListener* m_connectionsListener;

	std::list<CNetConnection*> m_connections;

//	uint m_maxConnections;
};

// -------------------------------------------------------------------

template <class Connection>
CIncomingConnectionsManager<Connection>::CIncomingConnectionsManager():
m_connectionsListener(NULL)
{
}

// -------------------------------------------------------------------

template <class Connection>
CIncomingConnectionsManager<Connection>::~CIncomingConnectionsManager()
{
	kickAll();
	if (m_connectionsListener)
		delete m_connectionsListener;
}

// -------------------------------------------------------------------

template <class Connection>
bool CIncomingConnectionsManager<Connection>::start(ushort localPort, ushort maximumNumberOfPeers)
{
	if (m_connectionsListener)
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError,_("ConnectionManager::start -- listener already started.\n"));
		return true;
	}
	m_connectionsListener = new CIncomingConnectionListener(); 
	return m_connectionsListener->start(localPort, maximumNumberOfPeers);
}

// -------------------------------------------------------------------

template <class Connection>
void CIncomingConnectionsManager<Connection>::processOutput()
{
	std::list<CNetConnection*>::iterator it,nextIt;
	for( it = m_connections.begin(); it != m_connections.end(); it = nextIt )
	{
		nextIt = it;
		nextIt++;
		if( !((*it)->processOutput()) )
			closeConnection(*it);
	}
}

// -------------------------------------------------------------------

template <class Connection>
void CIncomingConnectionsManager<Connection>::processNewConnections()
{
	CNetLayerBasePtr dataSock = m_connectionsListener->checkIncomingConnections();
	if (dataSock.get())
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlInfo,_("New connection from host %s.\n"), dataSock->getInfo().c_str());
		CCompressingLayer* compressingLayer = new CCompressingLayer;
		compressingLayer->setUnderlyingLayer(CNetLayerBasePtr(dataSock));
		Connection* con = new Connection();
		con->setUnderlyingLayer(CNetLayerBasePtr(compressingLayer));
		if( con )
			m_connections.push_back(con);
	}
}

// -------------------------------------------------------------------

template <class Connection>
void CIncomingConnectionsManager<Connection>::processInput()
{
	m_connectionsListener->checkSocketsInput();
	std::list<CNetConnection*>::iterator it,nextIt;
 	for( it = m_connections.begin(); it != m_connections.end(); it = nextIt )
	{
		nextIt = it;
		nextIt++;
		if( !((*it)->processInput()) )
			closeConnection(*it);
	}
};

// -------------------------------------------------------------------

template <class Connection>
void CIncomingConnectionsManager<Connection>::closeConnection(CNetConnection* connection)
{
	if( !connection )
		return;
	connection->onClose();

/*
	CNetLayerBase *nextLayer, *currentLayer;
	currentLayer = connection->getUnderlyingLayer();
	while (currentLayer)
	{
		nextLayer = currentLayer->getUnderlyingLayer();
		delete currentLayer;
		currentLayer = nextLayer;
	}
*/

	std::list<CNetConnection*>::iterator it;
	for( it = m_connections.begin(); it != m_connections.end(); ++it )
		if( *it == connection )
		{
			m_connections.erase(it);
			break;
		}

	delete connection;
};

// -------------------------------------------------------------------

template <class Connection>
void CIncomingConnectionsManager<Connection>::kickAll()
{
	std::list<CNetConnection*>::iterator it,nextIt;
	for( it = m_connections.begin(); it != m_connections.end(); it = nextIt )
	{
		nextIt = it;
		nextIt++;
		closeConnection(*it);
	}
};

// -------------------------------------------------------------------

template <class Connection>
CNetConnection* CIncomingConnectionsManager<Connection>::findConnection(uint id)
{
	std::list<CNetConnection*>::iterator i;
	for( i = m_connections.begin(); i != m_connections.end(); i++ )
		if( (*i)->getId() == id )
			return *i;
	return NULL;
};

// -------------------------------------------------------------------

template <class Connection>
class COutgoingConnectionsManager
{
public:

	COutgoingConnectionsManager(String targetHost, ushort targetPort);
	~COutgoingConnectionsManager();

	bool connect();
	bool reconnect();

	///Receive and process all incoming packets
	bool processInput();
	///Process and send all outcoming packets
	bool processOutput();

//	bool isConnected() {return m_socketManager->getDataSocket()->isConnected();}

	// FIXME!!
	bool isConnected() {return true;}
	CNetConnection* getConnection() {return m_connection;}

private:
	void deleteConnection();
	Connection* createConnection(CNetLayerBasePtr dataSock);
	CDataSocketManager* m_socketManager;
	Connection*			m_connection;
};

// -------------------------------------------------------------------

template <class Connection>
COutgoingConnectionsManager<Connection>::COutgoingConnectionsManager(String targetHost, ushort targetPort):
m_socketManager(NULL),
m_connection(NULL)
{
	m_socketManager = new CDataSocketManager(targetHost.c_str(), targetPort);
}

// -------------------------------------------------------------------

template <class Connection>
COutgoingConnectionsManager<Connection>::~COutgoingConnectionsManager()
{
	deleteConnection();
	if (m_socketManager)
		delete m_socketManager;
}

// -------------------------------------------------------------------

template <class Connection>
void COutgoingConnectionsManager<Connection>::deleteConnection()
{
	if (!m_connection)
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlWarn,_("Attempt to delete not initialized connection.\n"));
		return;
	}

	m_connection->onClose();

/*
	CNetLayerBase *nextLayer, *currentLayer;
	currentLayer = m_connection->getUnderlyingLayer();
	while (currentLayer)
	{
		nextLayer = currentLayer->getUnderlyingLayer();
		delete currentLayer;
		currentLayer = nextLayer;
	}
*/
	delete m_connection;

	m_connection = NULL;
}

// -------------------------------------------------------------------

template <class Connection>
bool COutgoingConnectionsManager<Connection>::connect()
{
	if (m_connection)
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError,_("Outgoing connection manager already connected.\n"));
		return false;
	}

	CNetLayerBasePtr socket =	m_socketManager->connect();
	if (!socket.get())
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError,_("Outgoing connection failed.\n"));
		return false;
	}
	m_connection = createConnection(socket);
	return true;
}

// -------------------------------------------------------------------

template <class Connection>
bool COutgoingConnectionsManager<Connection>::reconnect()
{
	deleteConnection();
/*
	if (!m_socketManager->reconnect())
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError,_("Error occured while trying to reconnect after input processing failure.\n"));
		return false;
	}
*/
	m_connection = createConnection(m_socketManager->connect());
	return true;
}

// -------------------------------------------------------------------

template <class Connection>
Connection* COutgoingConnectionsManager<Connection>::createConnection(CNetLayerBasePtr dataSock)
{
	CCompressingLayer* compressingLayer = new CCompressingLayer;
	compressingLayer->setUnderlyingLayer(dataSock);
	Connection* connection = new Connection();
	connection->setUnderlyingLayer(CNetLayerBasePtr(compressingLayer));
	return connection;
}

// -------------------------------------------------------------------

template <class Connection>
bool COutgoingConnectionsManager<Connection>::processInput()
{
	if (!m_connection)
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError,_("Outgoing connection manager not connected.\n"));
		return false;
	}
	m_socketManager->checkSocketsInput();
	if (!m_connection->processInput())
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError,_("Outgoing connection manager input processing failed, reconnecting...\n"));
		deleteConnection();
/*
		if (!m_socketManager->reconnect())
			CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError,_("Error occured while trying to reconnect after input processing failure.\n"));
		else
			m_connection = createConnection(m_socketManager->getDataSocket());
*/
			m_connection = createConnection(m_socketManager->connect());
		return false;
	}
	return true;
};

// -------------------------------------------------------------------

template <class Connection>
bool COutgoingConnectionsManager<Connection>::processOutput()
{
	if (!m_connection)
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError,_("Outgoing connection manager not connected.\n"));
		return false;
	}
	if (!m_connection->processOutput())
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError,_("Outgoing connection manager output processing failed, reconnecting...\n"));
		deleteConnection();
/*
		if (!m_socketManager->reconnect())
			CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError,_("Error occured while trying to reconnect after output processing failure.\n"));
		else
			m_connection = createConnection(m_socketManager->getDataSocket());
*/
			m_connection = createConnection(m_socketManager->connect());
		return false;
	}
	return true;
};

#endif


