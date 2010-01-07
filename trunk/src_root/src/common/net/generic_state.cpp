/**
 * @file common/net/generic_state.cpp
 * Base class for states
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: generic_state.cpp 781 2006-10-07 19:52:08Z zak $

 *  See LICENSE for details
 */

#include "common/common_stdafx.h"
#include "common/net/generic_state.h"
#include "common/net/net_connection.h"
#include "common/miscutils.h"


// ------------------------------------------------------------------

CGenericState::CGenericState(CNetConnection *ownerConnection):
CLogMixin("common.net"),
m_ownerConnection(ownerConnection), 
m_stateName("Undefined State"), 
m_timeoutEndTime(0),
m_timeoutCallback(NULL),
m_processingMode(CGenericState::allowWithExceptions)
{ 
	assert(ownerConnection && "Could not initialize state with NULL owner connection."); 
	this->registerSlot(ireon::net::netProcessCycleStart, static_cast<CGenericSlot> (&CGenericState::onProcessCycleStart), ireon::net::netAddress::atInternalAddress);
}

// ------------------------------------------------------------------

void CGenericState::processSignal(const ireon::net::commandId requestId, ireon::net::netAddress fromAddress, String &data)
{
	// filter signals
	if ( ( requestId != ireon::net::netProcessCycleStart ) && ( m_processingMode == CGenericState::allowWithExceptions ) ^ ( m_processingExceptions.find(requestId) == m_processingExceptions.end() ) )
	{
		_log (_error, _loc, gtc_("State '%s': access to slot %u denied (signal filter applied)."), m_stateName.c_str(), requestId );
		throw ireon::net::EUnknownSlot();
	}

	if (m_slots.find(requestId) != m_slots.end())
	{
		if (fromAddress.m_type && m_accessMasks[requestId])
		{
			// reset timeout
			if (requestId != ireon::net::netProcessCycleStart)
				m_timeoutEndTime = 0;
			(this->*m_slots[requestId])(data);	
			return;
		}
		else
		{
			_log (_error, _loc, gtc_("State '%s': access to slot %u denied."), m_stateName.c_str(), requestId );
			throw ireon::net::ESlotAccessDenied();
		}
	} 
	else 
	{
		_log (_error, _loc, gtc_("State '%s': could not find signal with id %u."), m_stateName.c_str(), requestId );
		throw ireon::net::EUnknownSlot();
	}
}

// ------------------------------------------------------------------

void CGenericState::send(ireon::net::commandId packetId, const String &packetData, PacketPriority priority, PacketReliability reliability, byte orderingChannel)
{
	conn().send(packetId, packetData, priority, reliability, orderingChannel);
}

// ------------------------------------------------------------------

void CGenericState::registerSlot(const ireon::net::commandId requestId, const CGenericSlot slot, byte addrTypeMask)
{
	assert(slot && "CGenericState::registerSlot: could not register NULL slot");
	if (m_slots.find(requestId) != m_slots.end())
		_log (_warn, _loc, gtc_("Signal handler re-registration -- signal %u."), requestId );
	m_slots[requestId] = slot;
	m_accessMasks[requestId] = addrTypeMask;
}

// ------------------------------------------------------------------

void CGenericState::setTimeout(uint msecsTimeout, TimeoutCallback callback)
{
	assert(callback && "CGenericState::setTimeout: NULL callback provided by caller.");
	if ( msecsTimeout )
	{
		m_timeoutEndTime = ireon::misc::getTime() + msecsTimeout;
		m_timeoutCallback = callback;
	} else {
		m_timeoutEndTime = 0;
		m_timeoutCallback = NULL;
	}
}

// ------------------------------------------------------------------

void CGenericState::onProcessCycleStart(String &packetData)
{
	if ( ( m_timeoutEndTime ) && ( ireon::misc::getTime() > m_timeoutEndTime ))
	{
		_log (_debug, _loc, gtc_("Calling timeout callback. (state: %s)"), m_stateName.c_str() );
		m_timeoutEndTime = 0;
		TimeoutCallback tmp = m_timeoutCallback;
		m_timeoutCallback = NULL;
		(this->*tmp)();
	}
	packetData;
}
