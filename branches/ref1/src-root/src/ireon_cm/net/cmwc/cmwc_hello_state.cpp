/**
 * @file ireon_cm/net/cmwc/cmwc_hello_state.cpp
 * cm world server connection welcome (hello) state
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

#include "ireon_cm/net/cmwc/cmwc_hello_state.h"
#include "common/net/signal_codes/cmwc_codes.h"

#include "db/client_char_data.h"
#include "db/account.h"
#include "db/cm_db.h"
//#include "ireon_rs/accounts/user_account.h"
#include "ireon_cm/cm_app.h"
//#include "ireon_rs/net/root_eserver.h"

#include "ireon_cm/stdafx.h"


CCMWCHelloState::CCMWCHelloState(CCMWSConnection *ownerConnection):
m_ownerConnection(ownerConnection)
{
	setStateName("Hello State");
	registerSlot(ireon::net::cmwcCodes::scHelloLogin, (CGenericSlot) &CCMWCHelloState::onLoginRequest);
}


/* Signal handlers */

void CCMWCHelloState::onLoginRequest(CData& data)
{
	uint worldId;
	uint worldPort;
	data >> worldId;
	CData answer;
	answer.wrt((ireon::net::commandId) ireon::net::cmwcCodes::cmLogin);
	if( CCMApp::instance()->findWorldHost(m_ownerConnection->getHost()) )
	{
		if( CCMApp::instance()->getWSConnectionsManager()->findConnection(worldId))
		{
			CLog::instance()->log(CLog::msgLvlInfo,_("Try to log in second world with id %d.\n"),worldId);
			answer.wrt((ireon::net::commandId) ireon::net::cmwcCodes::ldLoggedFailed);
		} else
		{
			CLog::instance()->log(CLog::msgLvlInfo,_("World with id %d logged in.\n"),worldId);
			answer.wrt((ireon::net::commandId) ireon::net::cmwcCodes::ldLoggedOk);
			m_ownerConnection->setId(worldId);
			data >> worldPort;
			m_ownerConnection->setWorldPort(worldPort);
			m_ownerConnection->setNextState(CCMWSConnection::csMain);
		}
	} else
	{
		CLog::instance()->log(CLog::msgLvlInfo,_("World with id %d failed login.\n"),worldId);
		answer.wrt((ireon::net::commandId) ireon::net::cmwcCodes::ldLoggedFailed);
	}
	m_ownerConnection->write(answer);
};


/* End signal handlers */

// ---------------------------------------------------------------------

