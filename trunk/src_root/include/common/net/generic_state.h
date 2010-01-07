/**
 * @file common/net/generic_state.h
 * Declares base class for states
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: generic_state.h 785 2006-10-25 15:48:43Z dox $

 *  See LICENSE for details
 */

#ifndef _GENERIC_STATE_H_
#define _GENERIC_STATE_H_

#include "common/common_stdafx.h"
#include "common/net/net_types.h"
#include "raknet/PacketPriority.h"
#include "common/log_mixin.h"
#include <set>
#include <map>


class CNetConnection;
class CGenericState;
typedef void (CGenericState::*CGenericSlot)(String &packetData);
typedef void (CGenericState::*TimeoutCallback)();

class CGenericState : protected CLogMixin
{
public:
	void processSignal(const ireon::net::commandId requestId, ireon::net::netAddress fromAddress, String &data);

	void send(ireon::net::commandId packetId, const String &packetData, PacketPriority priority = MEDIUM_PRIORITY, PacketReliability reliability = RELIABLE_ORDERED, byte orderingChannel = 0);

protected:
	explicit CGenericState(CNetConnection *ownerConnection);
	void setStateName(const String newName) { m_stateName = newName ; }
	void registerSlot(const ireon::net::commandId requestId, const CGenericSlot slot, byte addrTypeMask = 255);

	using CLogMixin::_log;

	enum processingMode
	{
		allowWithExceptions = 0, //< allow all signals except specified with addProcessingException() call
		denyWithExceptions = 1 //< deny all signals, allow only signals specified with addProcessingException() call
	};
	/// Sets signals filtering mode \see enum processingMode
	void setSignalsProcessingMode(processingMode mode) { m_processingMode = mode; }
	/// Add signal to list of exceptions
	void addProcessingException(ireon::net::commandId signal) { m_processingExceptions.insert(signal); }
	void clearProcessingExceptions() { m_processingExceptions.clear(); };

	/// calls callback function if no packets were processed until timeout
	/// use setTimeout(0,NULL) to reset timeout
	void setTimeout(uint msecsTimeout, TimeoutCallback callback);

	virtual CNetConnection& conn() {if (m_ownerConnection) return *m_ownerConnection; else throw ireon::net::EConnectionNotSet();}

private:
	void onProcessCycleStart(String &packetData);

	CNetConnection		*m_ownerConnection;

	processingMode					m_processingMode;
	std::set<ireon::net::commandId>	m_processingExceptions;

	// timeout
	int64							m_timeoutEndTime;
	TimeoutCallback					m_timeoutCallback;

	std::map<ireon::net::commandId, CGenericSlot>	m_slots;
	std::map<ireon::net::commandId, byte>	m_accessMasks;
	String	m_stateName;
};

#endif
