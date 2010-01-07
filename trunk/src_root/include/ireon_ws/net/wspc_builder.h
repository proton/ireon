/**
 * @file ireon_ws/net/wspc_builder.h
 * world server to player client connection builder
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: wspc_builder.h 806 2006-11-08 14:02:44Z zak $

 * See LICENSE for details
 */

#ifndef _WSPC_BUILDER_H
#define _WSPC_BUILDER_H

#include "ireon_ws/net/wspc_connection.h"
#include "ireon_ws/net/wspc/main_state.h"

class CWSPCConnectionBuilder
{
public: 
	static CNetConnection* buildConnection()
	{
		CWSPCConnection *p = new CWSPCConnection();
		p->registerState(CWSPCConnection::main, new WSPCMainState(p));
		return p;
	}
};

#endif
