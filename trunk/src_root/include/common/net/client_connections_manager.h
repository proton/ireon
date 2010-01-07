/**
 * @file common/net/client_connections_manager.h
 * Client's manager of connections
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: client_connections_manager.h 755 2006-09-24 14:10:30Z zak $

 *  See LICENSE for details
 */

#ifndef _CLIENT_CONNECTIONS_MANAGER_H_
#define _CLIENT_CONNECTIONS_MANAGER_H_

#include "common/common_stdafx.h"
#include "common/net/generic_connections_manager.h"

class RakClientInterface;

template <class T>
class CClientConnectionsManager: public CGenericConnectionsManager
{
public: 
	CClientConnectionsManager();
	~CClientConnectionsManager();

	void connect(const char *serverHost, ushort serverPort);
	void disconnect();

	bool isConnected();
	CNetConnection &getConnection() { if (isConnected()) return *(m_connection.get()); else throw ireon::net::EConnectionNotExist();}

	void processInput();
	/// should be used only by CNetConnection classes
	void send(uint playerId, ireon::net::commandId packetId, const char *data, uint dataLength, PacketPriority priority, PacketReliability reliability, byte orderingChannel);

private:
	void createNewConnection(const Packet *p);
	void deleteConnection(const Packet *p);
	void connectionFailed(const Packet *p);

	RakClientInterface* m_rakClient;
	std::auto_ptr<CNetConnection> m_connection;
};

#include "../src/common/net/client_connections_manager.cpp"

#endif