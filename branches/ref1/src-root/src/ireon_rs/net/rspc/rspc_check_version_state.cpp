/**
 * @file ireon_rs/net/rspc/rspc_check_version_state.cpp
 * root server player context checking version state
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id$

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "ireon_rs/net/rspc/rspc_check_version_state.h"
#include "common/net/signal_codes/rspc_codes.h"
#include "common/net/signal_codes/pcrs_codes.h"

//#include "ireon_rs/accounts/user_account.h"
#include "ireon_rs/root_app.h"
//#include "ireon_rs/net/root_eserver.h"

#include "ireon_client/version.h"

#include "ireon_rs/stdafx.h"


CRSPCCheckVersionState::CRSPCCheckVersionState(CRSPlayerConnection *ownerConnection):
m_ownerConnection(ownerConnection)
{
	setStateName("Check Version State");
	registerSlot(ireon::net::rspcCodes::scCheckVersion, (CGenericSlot) &CRSPCCheckVersionState::onCheckVersionRequest);
}

// ---------------------------------------------------------------------

void CRSPCCheckVersionState::onCheckVersionRequest(CData &data)
{
	byte clientVersion[4];
	//memset(&clientVersion[0],0,4*sizeof(byte));
	for (int i = 0; i < 4; i++)
		data >> clientVersion[i];
	CLog::instance()->log(CLog::msgLvlVerbose, _("Processing received version = '%u.%u.%u.%u' from host %s ... "),clientVersion[0],clientVersion[1],clientVersion[2],clientVersion[3], m_ownerConnection->getConnectionInfo());
	
	bool versionCompareFail = false;
	// note: this is CLIENT version (from ireon_client/vesrion.h)
	const byte rsVersion[4]={PRODUCT_VERSION};
	for(int i = 0; i < 4; i++)
		if (clientVersion[i] != rsVersion[i])
			versionCompareFail = true;
	
	CData answer;
	// TODO compare client version and server version
	if (!versionCompareFail)
	{
		answer.wrt((ireon::net::commandId) ireon::net::pcrsCodes::scCheckVersionOk);
		CLog::instance()->log(CLog::msgLvlVerbose, _("OK\n"));
		m_ownerConnection->setNextState(CRSPlayerConnection::csHello);
		m_ownerConnection->write(answer);
		return;
	}
	else
	{
		answer.wrt((ireon::net::commandId) ireon::net::pcrsCodes::scCheckVersionOld);
		for(int i = 0; i < 4; i++)
			answer.wrt((byte) rsVersion[i]);
		CLog::instance()->log(CLog::msgLvlVerbose, _("Failed\n"));
		m_ownerConnection->write(answer);
		return;
	}	
	return;
};

// ---------------------------------------------------------------------
