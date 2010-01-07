/**
 * @file ireon_ws/net/wsrs/main_state.h
 * world server root server context main state
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: main_state.h 806 2006-11-08 14:02:44Z zak $

 * See LICENSE for details
 */

#ifndef _WSRS_MAIN_STATE_H_
#define _WSRS_MAIN_STATE_H_

#include "common/net/generic_state.h"

class CWSRSConnection;

class WSRSMainState : public CGenericState
{
public:
	WSRSMainState(CWSRSConnection *ownerConnection);

private:
	void onInitiateLoginRequest(String& packetData);
	void onLoginResponse(String& packetData);
	void onCheckCharRequest(String& packetData);
	void onCharSelected(String& packetData);
};

#endif
