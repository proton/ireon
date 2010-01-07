/**
 * @file common/net/server_connections_manager.h
 * Server's manager of connections
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: server_connections_manager.h 718 2006-09-11 20:43:29Z zak $

 *  See LICENSE for details
 */

#ifndef _SERVER_CONNECTIONS_MANAGER_H_
#define _SERVER_CONNECTIONS_MANAGER_H_

#include "common/common_stdafx.h"
#include "common/net/generic_connections_manager.h"

class RakServerInterface;

template <class T>
class CServerConnectionsManager: public CGenericConnectionsManager
{
public: 
	CServerConnectionsManager();
	~CServerConnectionsManager();
	void startListening(ushort serverPort, ushort maxConnections);
	void stopListening();

	void processInput();
	void send(uint playerId, ireon::net::commandId packetId, const char *data, uint dataLength, PacketPriority priority, PacketReliability reliability, byte orderingChannel);

	bool isActive();

	CNetConnection &getConnectionById(uint id);

private:
	// no copy
	CServerConnectionsManager(CServerConnectionsManager &nocopy): CGenericConnectionsManager(NULL) {nocopy;}
	CServerConnectionsManager& operator=(CServerConnectionsManager &right) {right;}
	void kickAll();

	void createNewConnection(const Packet *p);
	void deleteConnection(const Packet *p);

	ushort getIndexByConnectionId(uint id);

	RakServerInterface* m_rakServer;
	std::map<ushort, CNetConnection *>		m_connections;
};

#include "../src/common/net/server_connections_manager.cpp"

#endif
