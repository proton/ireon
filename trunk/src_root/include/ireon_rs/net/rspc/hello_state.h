/**
 * @file ireon_rs/net/rspc/hello_state.h
 * root server player context welcome (hello) state
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: hello_state.h 765 2006-09-25 13:38:21Z zak $

 * See LICENSE for details
 */

#ifndef _RSPC_HELLO_STATE_H_
#define _RSPC_HELLO_STATE_H_

#include "common/net/generic_state.h"
#include "ireon_rs/net/rspc_connection.h"

class CRSPCHelloState : public CGenericState
{
public:
	CRSPCHelloState(CRSPCConnection *ownerConnection);

protected:
	CRSPCConnection& conn() {if (m_concreteOwnerConnection) return *m_concreteOwnerConnection; else throw ireon::net::EConnectionNotSet();}

private:
	void onLoginRequest(String &packetData);
	void onRegisterRequest(String &packetData);

	CRSPCConnection *m_concreteOwnerConnection;
};

#endif
