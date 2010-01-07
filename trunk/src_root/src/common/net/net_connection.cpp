/**
 * @file common/net/net_connection.cpp
 * Generic network connection class
 */


/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: net_connection.cpp 828 2006-11-29 18:34:44Z zak $

 * See LICENSE for details
 */


#include "common/common_stdafx.h"
#include "common/net/net_connection.h"
#include "common/net/generic_connections_manager.h"

String CNetConnection::m_emptyPacketData = String();

void CNetConnection::send(ireon::net::commandId packetId, const String &packetData, PacketPriority priority, PacketReliability reliability, byte orderingChannel)
{
	_log (_debug, _loc, gtc_("Sending network packet (packetId = %d)."),packetId );
	m_manager->send(getId(), packetId, packetData.c_str(), static_cast<uint> (packetData.length()), priority, reliability, orderingChannel);
}

// --------------------------------------------------------------------

void CNetConnection::processPacket(ireon::net::commandId packetId, String &packetData, ireon::net::netAddress fromAddress) 
{
	_pushNDC(getConnectionInfo());
	_log (_debug, _loc, gtc_("Processing network packet #%d."), packetId );
	try
	{
		// switch context state if needed
		switchStates();
		if ( !m_statePointer )
		{
			_log (_error, _loc, gtc_("Current state not found, state machine not initialized?") );
			throw ireon::net::EUnknownState();
		}
		m_statePointer->processSignal(packetId, fromAddress, packetData);
	}
	catch (ireon::net::EUnknownSlot &)
	{	
		_log (_error, _loc, gtc_("Unknown packet id %d from #%s."),packetId );
		// TODO send <<unknown packet>> message?
	}
	catch (ireon::net::EStateMachineError &)
	{	
		_log (_error, _loc, gtc_("Got EStateMachineError exception.") );
	}
	catch (ireon::net::EStateError &)
	{
		_log (_error, _loc, gtc_("Got EStateError exception.") );
	}
	catch (EIDDoesNotMatch&)
	{
		_log (_error, _loc, gtc_("Caught EIDDoesNotMatch. Hey man! You messed up packets!") );
	}
	catch (ENetDataException&)
	{
		_log (_error, _loc, gtc_("Caught ENetDataException.") );
		// TODO send something to client?
	}
	_popNDC();
}

// --------------------------------------------------------------------

/// Actually changes state (opposing to setNextState())
void CNetConnection::switchStates()
{
	if ( m_currentStateId == m_nextStateId )
		return;

	std::map<stateId, boost::shared_ptr<CGenericState>>::iterator it = m_states.find(m_nextStateId);
	if ( it == m_states.end())
		throw ireon::net::EUnknownState();

	m_currentStateId = m_nextStateId;
	m_statePointer = it->second.get();
}

// --------------------------------------------------------------------

void CNetConnection::registerState(stateId si, CGenericState *state) 
{ 
	assert(state && "CNetStateMachine::registerState: could not register NULL state.");
	if (m_states.find(si) != m_states.end())
		_log (_warn, _loc, gtc_("State creator redefinition.") );
	m_states[si].reset(state);
	// if this is first call to function set state as first
	if ( ( m_currentStateId == 0xFF ) && ( m_nextStateId == 0xFF ) )
		m_nextStateId = si;
}
