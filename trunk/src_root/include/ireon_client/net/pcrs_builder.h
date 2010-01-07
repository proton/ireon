/**
 * @file ireon_rs/net/pcrs_builder.h
 * root server to player client connection builder
 */

 /* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: rspc_builder.h 697 2006-09-03 13:48:56Z zak $

 * See LICENSE for details
 */

#ifndef _PCRS_BUILDER_H
#define _PCRS_BUILDER_H


#include "ireon_client/net/pcrs_connection.h"
#include "ireon_client/net/pcrs/check_version_state.h"
#include "ireon_client/net/pcrs/hello_state.h"
#include "ireon_client/net/pcrs/character_selection_state.h"
class CPCRSConnectionBuilder 
{
public: 
	static CNetConnection* buildConnection()
	{
		CPCRSConnection *p = new CPCRSConnection();
		p->registerState(CPCRSConnection::versionCheck, new CPCRSCheckVersionState(p));
		p->registerState(CPCRSConnection::hello, new CPCRSHelloState(p));
		p->registerState(CPCRSConnection::characterSelection, new CPCRSCharacterSelectionState(p));
		return p;
	}
};

#endif
