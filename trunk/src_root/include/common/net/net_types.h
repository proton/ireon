/* Copyright (C) 2005 ireon.org developers council
 * $Id: net_types.h 806 2006-11-08 14:02:44Z zak $

 *  See LICENSE for details
 */

/**
 * @file common/net/net_types.h
 * Network misc
 */

#ifndef _NET_TYPES_H
#define _NET_TYPES_H

#include "raknet/packetenumerations.h"

namespace ireon
{
namespace net
{
	typedef byte	commandId;

	enum commonPacketsId
	{
		netProcessCycleStart = ID_USER_PACKET_ENUM,
		commonPacketIdLast
	};

	/// used to describe various types of addresses, e.g. net packet from address
	struct netAddress
	{
		enum addrTypeFlags
		{
			atUndefinedAddress = 1,
			atExternalAddress = 2,	//< packet came from network
			atInternalAddress = 4	//< packet was sent from inside of program
		};
		netAddress(addrTypeFlags type): m_type(type) {};
		addrTypeFlags m_type;
	};

	// net exceptions classes
	class EStateMachineError {};
	class EUnknownState : public EStateMachineError {};

	class EStateError {};
	class EUnknownSlot : public EStateError {};
	class ESlotAccessDenied : public EStateError {};
	class EConnectionNotSet : public EStateError {};

	class EClientError {};
	class EUnableToInitializeClient : public EClientError {};

	class EServerError {};
	class EUnableToInitializeServer : public EServerError {};
	class EIncorrectPacketData : public EServerError {};

	class EConnectionExists : public EServerError, public EClientError {};
	class EConnectionNotExist : public EServerError, public EClientError {};
	class EIncorrectSendData : public EServerError, public EClientError {};
	
	inline bool checkBitfield(byte x, byte y)
	{
		return (x & y) == y;
	}
}


}

#include "common/net/net_data.h"
//#include "common/net/connection_builder.h"

#endif

