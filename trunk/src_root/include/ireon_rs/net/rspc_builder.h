/**
 * @file ireon_rs/net/rspc_builder.h
 * root server to player client connection builder
 */

 /* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: rspc_builder.h 714 2006-09-07 16:32:40Z zak $

 * See LICENSE for details
 */

#ifndef _RSPC_BUILDER_H
#define _RSPC_BUILDER_H

#include "ireon_rs/net/rspc_connection.h"
#include "ireon_rs/net/rspc/check_version_state.h"
#include "ireon_rs/net/rspc/hello_state.h"
#include "ireon_rs/net/rspc/main_state.h"

class CRSPCConnectionBuilder
{
public: 
	static CNetConnection* buildConnection()
	{
		CRSPCConnection *p = new CRSPCConnection();
		p->registerState(CRSPCConnection::versionCheck, new CRSPCCheckVersionState(p));
		p->registerState(CRSPCConnection::hello, new CRSPCHelloState(p));
		p->registerState(CRSPCConnection::main, new CRSPCMainState(p));
		return p;
	}
};

#endif
