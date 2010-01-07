/**
 * @file common/net/generic_connections_manager.h
 * Generic network connections manager class
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: generic_connections_manager.h 781 2006-10-07 19:52:08Z zak $

 * See LICENSE for details
 */


#ifndef _GENERIC_CONNECTIONS_MANAGER_H_
#define _GENERIC_CONNECTIONS_MANAGER_H_

#include "common/net/net_types.h"
#include "common/log_mixin.h"
#include <map>

class CNetConnection;

struct Packet;
enum PacketPriority;
enum PacketReliability;

class CGenericConnectionsManager;
typedef void (CGenericConnectionsManager::*ServiceMessageHandler)(const Packet *p);

struct CNetMessageProcessCommand { virtual void execute() = 0; };

class CGenericConnectionsManager : private CLogMixin
{
public: 
	virtual void processInput() = 0;
	virtual void send(uint playerId, ireon::net::commandId packetId, const char *data, uint dataLength, PacketPriority priority, PacketReliability reliability, byte orderingChannel) = 0;

	void nullHandler(const Packet *p);

	/// register additional callback for network service message
	/// handler will be called after internal message handler (if any)
	/// handler object will be deleted by connection manager
	void registerClientMessageHandler(byte msgId, CNetMessageProcessCommand *handler);

protected:
	CGenericConnectionsManager(): CLogMixin("common.net") {}
	~CGenericConnectionsManager();

	using CLogMixin::_log;

	void getPacketInfo(const Packet *p, ireon::net::commandId &packetId, char *&packetDataStart, uint &packetDataLength);

	void registerMessageHandler(byte msgId, ServiceMessageHandler handler);
	std::map<byte, ServiceMessageHandler>	m_serviceMessagesHandlers;
	std::map<byte, CNetMessageProcessCommand *>	m_clientServiceMessagesHandlers;
};

#endif
