/**
 * @file ireon_rs/net/rsws_builder.h
 * root server to world server connection builder
 */

 /* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: rsws_builder.h 726 2006-09-13 21:10:24Z zak $

 * See LICENSE for details
 */

#ifndef _RSWS_BUILDER_H
#define _RSWS_BUILDER_H

#include "ireon_rs/net/rsws_connection.h"
#include "ireon_rs/net/rsws/main_state.h"

class CRSWSConnectionBuilder
{
public: 
	static CNetConnection* buildConnection()
	{
		CRSWSConnection *p = new CRSWSConnection();
		p->registerState(CRSWSConnection::main , new CRSWSMainState(p));
		return p;
	}
};

#endif
