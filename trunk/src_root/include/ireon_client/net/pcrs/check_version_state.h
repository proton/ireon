/**
 * @file ireon_rs/net/rspc/check_version_state.h
 * root server player context version check state
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: check_version_state.h 698 2006-09-03 16:46:33Z zak $

 * See LICENSE for details
 */

#ifndef _PCRS_CHECK_VERSION_STATE_H_
#define _PCRS_CHECK_VERSION_STATE_H_

#include "common/net/generic_state.h"
#include "ireon_client/net/pcrs_connection.h"

class CPCRSCheckVersionState : public CGenericState
{
public:
	CPCRSCheckVersionState(CNetConnection *ownerConnection);

	//CPCRSConnection& conn() {if (m_concreteOwnerConnection) return *m_concreteOwnerConnection; else throw ireon::net::EConnectionNotSet();}

private:
	void checkVersionRequest(String &packetData);
	void onCheckVersionResponse(String &packetData);
	void onTimeoutCallback();
//	CPCRSConnection *m_concreteOwnerConnection;
};

#endif
