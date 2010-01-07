/**
 * @file ireon_client/net/pcws_builder.h
 * player client to world server connection builder
 */

 /* Copyright (C) 2005-2006 ireon.org developers council
 * $Id$

 * See LICENSE for details
 */

#ifndef _PCWS_BUILDER_H
#define _PCWS_BUILDER_H

#include "ireon_client/net/pcws_connection.h"
#include "ireon_client/net/pcws/hello_state.h"
#include "ireon_client/net/pcws/main_state.h"


class CPCWSConnectionBuilder
{
public: 
	static CNetConnection* buildConnection()
	{
		CPCWSConnection *p = new CPCWSConnection();
		p->registerState(CPCWSConnection::hello, new CPCWSHelloState(p));
		p->registerState(CPCWSConnection::main, new CPCWSMainState(p));
		return p;
	}
};

#endif
