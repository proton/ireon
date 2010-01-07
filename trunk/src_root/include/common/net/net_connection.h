/**
 * @file common/net/net_connection.h
 * Generic network connection class
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: net_connection.h 806 2006-11-08 14:02:44Z zak $

 * See LICENSE for details
 */

#ifndef _NET_CONNECTION_H
#define _NET_CONNECTION_H

#include "common/net/generic_state.h"
#include "common/net/net_types.h"
#include "common/log_mixin.h"

#include "raknet/PacketPriority.h"
#include "boost/shared_ptr.hpp"
#include <map>

class CGenericConnectionsManager;
typedef byte stateId;

class CNetConnection : private CLogMixin
{
	friend void CGenericState::send(ireon::net::commandId, const String &, PacketPriority, PacketReliability, byte);
public:
	CNetConnection():CLogMixin("common.net"), m_manager(NULL), m_id(0), m_currentStateId(0xFF), m_nextStateId(0xFF) {};
	virtual ~CNetConnection() {};

	void setNextState(stateId newState) {m_nextStateId = newState;}
	void registerState(stateId si, CGenericState *statePrototype);
	void switchStates();

	void setConnectionManager(CGenericConnectionsManager *manager) {m_manager = manager;}

	/// Process packet -- for processing data from network or another connections (uses states)
	void processPacket(ireon::net::commandId packetId, const String& packetData, ireon::net::netAddress fromAddress)
			{ String tmp(packetData); processPacket(packetId, tmp, fromAddress); }
	void processPacket(ireon::net::commandId packetId, String& packetData, ireon::net::netAddress fromAddress);
	void processPacket(ireon::net::commandId packetId, ireon::net::netAddress fromAddress)
		{ processPacket(packetId, m_emptyPacketData, fromAddress); }
//		{ processPacket(packetId, packetData.c_str(), static_cast<uint> (packetData.size()), fromAddress); }
//	void processPacket(ireon::net::commandId packetId, const char *packetData, uint packetDataSize, ireon::net::netAddress fromAddress);

	void setId(uint id) { m_id = id; }
	uint getId() { return m_id; }
	void setConnectionInfo(const char *connInfo) { m_connInfo = connInfo; }
	const char *getConnectionInfo() { return m_connInfo.c_str(); }
	void setRemoteHost(const char *host) { m_remoteHost = host; }
	const char *getRemoteHost() { return m_remoteHost.c_str(); }

private:
	/// sends packet directly to other end of connection
	/// only for use in states classes, should not be called from elsewhere
	void send(ireon::net::commandId packetId, const String &packetData, PacketPriority priority, PacketReliability reliability, byte orderingChannel);

	CGenericState											*m_statePointer;
	stateId													m_nextStateId, m_currentStateId;
	std::map< stateId, boost::shared_ptr<CGenericState> >	m_states;

	static String		m_emptyPacketData;

	CGenericConnectionsManager *m_manager;
	String		m_connInfo, m_remoteHost;
	uint		m_id;
};


#endif

