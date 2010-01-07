/**
 * @file ireon_rs/net/rsws/rsws_main_state.h
 * root server world server context main state
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: main_state.h 775 2006-09-29 20:11:59Z zak $

 * See LICENSE for details
 */

#ifndef _RSWS_MAIN_STATE_H_
#define _RSWS_MAIN_STATE_H_

#include "common/net/generic_state.h"

class CRSWSConnection;

class CRSWSMainState : public CGenericState
{
public:
	CRSWSMainState(CRSWSConnection *ownerConnection);

private:

	void onLoginRequest(String& packetData);
	void onCheckCharRequest(String& packetData);
	void onCheckCharResponse(String& packetData);
	void onCharSelected(String& packetData);

	std::map<ushort,uint> m_checkCharRequests;
};

#endif
