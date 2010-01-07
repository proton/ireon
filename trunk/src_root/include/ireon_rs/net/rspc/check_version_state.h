/**
 * @file ireon_rs/net/rspc/check_version_state.h
 * root server player context version check state
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: check_version_state.h 765 2006-09-25 13:38:21Z zak $

 * See LICENSE for details
 */

#ifndef _RSPC_CHECK_VERSION_STATE_H_
#define _RSPC_CHECK_VERSION_STATE_H_

#include "common/net/generic_state.h"
#include "ireon_rs/net/rspc_connection.h"

class CRSPCCheckVersionState : public CGenericState
{
public:
	CRSPCCheckVersionState(CRSPCConnection *ownerConnection);

//	CRSPCConnection& conn() {if (m_concreteOwnerConnection) return *m_concreteOwnerConnection; else throw ireon::net::EConnectionNotSet();}

private:
	void onCheckVersionRequest(String &packetData);

//	CRSPCConnection *m_concreteOwnerConnection;
};

#endif
