/**
 * @file ireon_ws/net/wspc/main_state.h
 * world server player context main state
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: main_state.h 821 2006-11-21 21:00:36Z zak $

 * See LICENSE for details
 */

#pragma once

#include "common/net/generic_state.h"
#include "ireon_ws/net/wspc_connection.h"

class WSPCMainState : public CGenericState
{
public:
	WSPCMainState(CWSPCConnection *ownerConnection);

protected:
	CWSPCConnection& conn() {if (m_concreteOwnerConnection) return *m_concreteOwnerConnection; else throw ireon::net::EConnectionNotSet();}

private:
	CWSPCConnection *m_concreteOwnerConnection;
	void onLoginRequest(String& packetData);
	void sendPlayerCoords(String&);
	void onLogoutRequest(String&);
	void onSetCharacterHeading(String& packetData);
	void onCharacterPositionUpdate(String& packetData);
	void onStopCharacterMovement(String& packetData);
};

