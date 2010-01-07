/**
* @file ireon_client/net/pcws/hello_state.cpp
* client ws connection welcome (hello) state
*/

/* Copyright (C) 2005-2006 ireon.org developers council
* $Id$

* See LICENSE for details
*/
#include "ireon_client/stdafx.h"
#include "ireon_client/net/pcws/main_state.h"
#include "common/net/connections/pcws.h"
//#include "ireon_client/interface/client_event_manager.h"



CPCWSMainState::CPCWSMainState(CNetConnection *ownerConnection):
CGenericState(ownerConnection)
{
	setStateName("PC <=> WS Main State");

	//registerSlot(ireon::net_pcInitiateLogin, static_cast<CGenericSlot> (&CPCWSHelloState::onInitiateLoginSignal), ireon::net::netAddress::atInternalAddress);
	//registerSlot(ireon::net::pcrsLoginResponse, static_cast<CGenericSlot> (&CPCWSHelloState::onLoginResponse), ireon::net::netAddress::atExternalAddress);

	//setSignalsProcessingMode(CGenericState::denyWithExceptions);
	//addProcessingException(ireon::net_pcInitiateLogin);
}
