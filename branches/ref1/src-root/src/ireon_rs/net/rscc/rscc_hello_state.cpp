/**
 * @file ireon_rs/net/rscc/rscc_hello_state.cpp
 * root server cm context welcome (hello) state
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

#include "ireon_rs/net/rscc/rscc_hello_state.h"
#include "common/net/signal_codes/rscc_codes.h"
#include "common/net/signal_codes/cmrc_codes.h"
#include "ireon_rs/root_app.h"

#include "ireon_rs/stdafx.h"


CRSCCHelloState::CRSCCHelloState(CRSCMConnection *ownerConnection):
m_ownerConnection(ownerConnection)
{
	setStateName("CM Connection Hello State");
	registerSlot(ireon::net::rsccCodes::scHelloLogin, (CGenericSlot) &CRSCCHelloState::onLoginRequest);
}

// ---------------------------------------------------------------------

void CRSCCHelloState::onLoginRequest(CData &data)
{
	uint clusterId;
	data >> clusterId;

	CData answer;

	if (!CRootApp::instance()->findCluster(m_ownerConnection->getHost()))
	{
		CLog::instance()->log(CLog::msgLvlWarn, _("Attempt to log in cm from unknown host %s.\n"), m_ownerConnection->getConnectionInfo());
		answer.wrt((ireon::net::commandId) ireon::net::cmrcCodes::scLoginFailed);
		answer.wrt((byte) ireon::net::cmrcCodes::lfHostNotAllowed);
		m_ownerConnection->write(answer);
		return;
	}

	if (CRootApp::instance()->getCMConnectionsManager()->findConnection(clusterId))
	{
		CLog::instance()->log(CLog::msgLvlError, _("Attempt to log in second cluster manager with id %d (host: %s).\n"), clusterId, m_ownerConnection->getConnectionInfo());
		answer.wrt((ireon::net::commandId) ireon::net::cmrcCodes::scLoginFailed);
		answer.wrt((byte) ireon::net::cmrcCodes::lfAlreadyLoggedIn);
		m_ownerConnection->write(answer);
		return;
	}

	m_ownerConnection->setId(clusterId);
	m_ownerConnection->setNextState(CRSCMConnection::csMain);

	answer.wrt((ireon::net::commandId) ireon::net::cmrcCodes::scLoginOk);
	m_ownerConnection->write(answer);
	CLog::instance()->log(CLog::msgLvlInfo,_("Cluster manager with id %d logged in.\n"), clusterId);
};
