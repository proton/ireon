/**
 * @file ireon_client/net/pcws/hello_state.h
 * client ws connection welcome (hello) state
 */

/* Copyright (C) 2005-2006 ireon.org developers council
* $Id$

* See LICENSE for details
*/

#ifndef _PCWS_HELLO_STATE_H_
#define _PCWS_HELLO_STATE_H_

#include "common/net/generic_state.h"
#include "ireon_client/net/pcws_connection.h"

class CPCWSHelloState : public CGenericState
{
public:
	CPCWSHelloState(CNetConnection* ownerConnection);
private:

	void onInitiateLoginSignal(String &packetData);
	void onLoginResponse(String &packetData);

};

#endif
