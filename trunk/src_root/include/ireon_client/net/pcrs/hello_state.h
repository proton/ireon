/**
 * @file ireon_client/net/pcrs/pcrc_hello_state.h
 * client rs connection welcome (hello) state
 */

/* Copyright (C) 2005-2006 ireon.org developers council
* $Id: check_version_state.h 698 2006-09-03 16:46:33Z zak $

* See LICENSE for details
*/

#ifndef _PCRS_HELLO_STATE_H_
#define _PCRS_HELLO_STATE_H_

#include "common/net/generic_state.h"
#include "ireon_client/net/pcrs_connection.h"

class CPCRSHelloState : public CGenericState
{
public:
	CPCRSHelloState(CNetConnection* ownerConnection);
private:

	void registerRequest(String &packetData);
	void onRegisterResponse(String &packetData);
	void onInitiateLoginSignal(String &packetData);
	void onLoginResponse(String &packetData);

};

#endif
