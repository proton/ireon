/**
 * @file ireon_ws/net/wsrs_builder.h
 * world server to root server connection builder
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: wsrs_builder.h 806 2006-11-08 14:02:44Z zak $

 * See LICENSE for details
 */

#ifndef _WSRS_BUILDER_H
#define _WSRS_BUILDER_H

#include "ireon_ws/net/wsrs_connection.h"
#include "ireon_ws/net/wsrs/main_state.h"

class CWSRSConnectionBuilder
{
public: 
	static CNetConnection* buildConnection()
	{
		CWSRSConnection *p = new CWSRSConnection();
		p->registerState(CWSRSConnection::main , new WSRSMainState(p));
		return p;
	}
};

#endif
