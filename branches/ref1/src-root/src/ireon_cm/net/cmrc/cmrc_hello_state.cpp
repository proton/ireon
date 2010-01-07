/**
 * @file ireon_cm/net/cmrc/cmrc_hello_state.cpp
 * cm root server connection welcome (hello) state
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

#include "ireon_cm/net/cmrc/cmrc_hello_state.h"
#include "common/net/signal_codes/cmrc_codes.h"
#include "common/net/signal_codes/rscc_codes.h"

#include "ireon_cm/cm_app.h"
#include "ireon_cm/stdafx.h"

CCMRCPreLoginState::CCMRCPreLoginState(CCMRSConnection *ownerConnection):
m_ownerConnection(ownerConnection)
{
	setStateName("Pre Login State");
	registerSlot(ireon::net::cmrcCodes::scInitiateLogin, (CGenericSlot) &CCMRCPreLoginState::initiateLogin, ireon::net::netAddress::atInternalAddress);
}

// ---------------------------------------------------------------------

void CCMRCPreLoginState::initiateLogin(CData& data)
{
	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlInfo,_("Initiating root server login sequence.\n"));

	uint	cmId;
	data >> cmId;
	m_ownerConnection->setId(cmId);
	CData request;
	request.wrt((ireon::net::commandId) ireon::net::rsccCodes::scHelloLogin);
	request << cmId;
	m_ownerConnection->write(request);
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

CCMRCWaitLoginAckState::CCMRCWaitLoginAckState(CCMRSConnection *ownerConnection):
m_ownerConnection(ownerConnection)
{
	setStateName("Wait login ack State");
	registerSlot(ireon::net::cmrcCodes::scLoginOk, (CGenericSlot) &CCMRCWaitLoginAckState::onLoginOk, ireon::net::netAddress::atExternalAddress);
	registerSlot(ireon::net::cmrcCodes::scLoginFailed, (CGenericSlot) &CCMRCWaitLoginAckState::onLoginFailed, ireon::net::netAddress::atExternalAddress);
}

// ---------------------------------------------------------------------

void CCMRCWaitLoginAckState::onLoginOk(CData& data)
{
	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlVerbose,_("Got ok login response from root server.\n"));
	m_ownerConnection->setNextState(CCMRSConnection::csMain);
	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlInfo,_("Successfully logged in to root server.\n"));
	data;
};

// ---------------------------------------------------------------------

void CCMRCWaitLoginAckState::onLoginFailed(CData& data)
{
	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlVerbose,_("Got login response from root server.\n"));
	byte	resp;
	data >> resp;
	if (resp == ireon::net::cmrcCodes::lfHostNotAllowed)
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlCritical,_("Authentification on root server failed, shutting cm down...\n"));
		CCMApp::instance()->shutdown();
		return;
	}
	if (resp == ireon::net::cmrcCodes::lfAlreadyLoggedIn)
	{
		m_ownerConnection->setNextState(CCMRSConnection::csPreLogin);
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError,_("Root server login error: such cm already logged in.\n"));
		return;
	}
	m_ownerConnection->setNextState(CCMRSConnection::csPreLogin);
	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError,_("Root server returned unknown login response.\n"));
};

// ---------------------------------------------------------------------

