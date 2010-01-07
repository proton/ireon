/**
 * @file ireon_rs/net/rspc/main_state.h
 * root server player context main state
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: main_state.h 775 2006-09-29 20:11:59Z zak $

 * See LICENSE for details
 */

#ifndef _RSPC_MAIN_H_
#define _RSPC_MAIN_H_

#include "common/net/generic_state.h"
#include "ireon_rs/net/rspc_connection.h"
#include <map>

class CRSPCMainState : public CGenericState
{
public:
	CRSPCMainState(CRSPCConnection *ownerConnection);

protected:
	CRSPCConnection& conn() {if (m_concreteOwnerConnection) return *m_concreteOwnerConnection; else throw ireon::net::EConnectionNotSet();}

private:
	CRSPCConnection *m_concreteOwnerConnection;

	std::map<ushort, int64>	m_charsToCheck;
	ushort			m_selectedCharId;
	byte			m_selectedCharWS;

	void onCreateCharRequest (String &packetData);
	void onListCharsRequest (String &packetData);
	void onSelectCharRequest (String &packetData);
	void onCharCheckResponse (String &packetData);
	void onCheckCharTimeout ();
	void deleteTimedOutChars ();
	void sendSelectResponse ();
//	void onRemoveCharRequest (CData& data);
};

#endif
