/**
 * @file ireon_ws/net/wspc_connection.h
 * world server to player client connection class
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: wspc_connection.h 806 2006-11-08 14:02:44Z zak $

 * See LICENSE for details
 */

#ifndef _WSPC_CONNECTION_H
#define _WSPC_CONNECTION_H

#include "common/net/net_connection.h"
#include "common/net/connections/pcws.h"

#include "boost/shared_ptr.hpp"
class CWSPlayerCharacter;
typedef boost::shared_ptr<CWSPlayerCharacter> WSPlayerCharPtr;

class CWSPCConnection : public CNetConnection
{
public:
	virtual ~CWSPCConnection() { processPacket(ireon::net::pcws::logout, ireon::net::netAddress(ireon::net::netAddress::atInternalAddress)); }
	enum WSPCStates
	{
		main
	};

	void setCharacter(WSPlayerCharPtr ch) { character_ = ch; }
	WSPlayerCharPtr getCharacter() { return character_; }

private:
	WSPlayerCharPtr		character_;
};

#endif
