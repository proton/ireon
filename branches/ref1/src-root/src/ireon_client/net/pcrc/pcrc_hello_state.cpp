/**
 * @file ireon_client/net/pcrc/pcrc_hello_state.cpp
 * client root server context welcome (hello) state
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

#include "ireon_client/stdafx.h"
#include "ireon_client/version.h"
#include "ireon_client/net/pcrc/pcrc_hello_state.h"
#include "common/net/signal_codes/rspc_codes.h"
#include "common/net/signal_codes/pcrs_codes.h"
#include "ireon_client/interface/client_event_manager.h"

//#include "ireon_rs/accounts/user_account.h"


CPCRCHelloState::CPCRCHelloState(CPCRSConnection *ownerConnection):
m_ownerConnection(ownerConnection)
{
	setStateName("Hello State");
	registerSlot(ireon::net::pcrsCodes::scSendVersionInfo, (CGenericSlot) &CPCRCHelloState::onSendVersionSignal, ireon::net::netAddress::atInternalAddress);
	registerSlot(ireon::net::pcrsCodes::scCheckVersionOk, (CGenericSlot) &CPCRCHelloState::onCheckVersionOk);
	registerSlot(ireon::net::pcrsCodes::scCheckVersionOld, (CGenericSlot) &CPCRCHelloState::onCheckVersionOld);

	registerSlot(ireon::net::pcrsCodes::scSendRegistrationRequest, (CGenericSlot) &CPCRCHelloState::onRegisterSignal, ireon::net::netAddress::atInternalAddress);
	registerSlot(ireon::net::pcrsCodes::scSendLoginRequest, (CGenericSlot) &CPCRCHelloState::onLoginSignal, ireon::net::netAddress::atInternalAddress);
}

// ---------------------------------------------------------------------

void CPCRCHelloState::onSendVersionSignal(CData &data)
{
	CData versionData;
	char clientVersion[] = {PRODUCT_VERSION};
	for (int i = 0; i < 4; i++)
		versionData << clientVersion[i];
	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlInfo,_("Initiating version negotiation.\n"));
	CData request;
	request.wrt((ireon::net::commandId)ireon::net::rspcCodes::scCheckVersion);
	request.write(versionData.data(), versionData.length());
	m_ownerConnection->write(request);
};

// ---------------------------------------------------------------------

void CPCRCHelloState::onCheckVersionOk(CData &data)
{
	CClientApp::instance()->setState(CClientApp::AS_LOGGING_ROOT);
	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlInfo,_("Version negoriation ok.\n"));
}

// ---------------------------------------------------------------------

void CPCRCHelloState::onCheckVersionOld(CData &data)
{
	CEventManager::instance()->execute(ON_VERSION_CHECK_FAILED,NULL);
	//CClientApp::instance()->setState(CClientApp::AS_MENU);
	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlInfo,_("Client version is too old for root server.\n"));
}

// ---------------------------------------------------------------------

void CPCRCHelloState::onLoginSignal(CData &data)
{
	CLog::instance()->log(CLog::msgLvlInfo,_("Sending login request to root server.\n"));
	CData request;
	request.wrt((ireon::net::commandId)ireon::net::rspcCodes::scHelloLogin);
	request.append(data.data(), data.length());
	m_ownerConnection->write(request);
}

	
// ---------------------------------------------------------------------

void CPCRCHelloState::onRegisterSignal(CData &data)
{
/*
	String login;
	String password;

	data >> login >> password;


	CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlWarn, _("Account registration attempt from host %s, login %s...\n"), m_ownerConnection->getHost(), login.c_str());

	CData answer;

	// TODO check for well-formedness of login and password
	if ((login.length()<4) || (password.length()<4))
	{
		answer.wrt((ireon::net::commandId) ireon::net::rspcCodes::rcRegistrationFailed);
		answer.wrt((byte) ireon::net::rspcCodes::rfMalformedData);
	   	CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlInfo, _("Registration failed. Username or password too short.\n"));
		m_ownerConnection->write(answer);
		return;
	}

	AccPtr account = CRootDB::instance()->findAccount(login);
//	if (CUserAccount::fetchAccount(login))
	if (account.get())
	{
		answer.wrt((ireon::net::commandId) ireon::net::rspcCodes::rcRegistrationFailed);
		answer.wrt((byte) ireon::net::rspcCodes::rfAlreadyRegistered);
	   	CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlInfo, _("Registration failed. User %s already registered.\n"), login.c_str());
		m_ownerConnection->write(answer);
		return;
	}

	CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlInfo,_("Creating new account: login = '%s'...  "), login.c_str());

	account = CRootDB::instance()->createAccount(login);
	if( !account.get() )
	{
		answer.wrt((ireon::net::commandId) ireon::net::rspcCodes::rcRegistrationFailed);
		answer.wrt((byte) ireon::net::rspcCodes::rfUnknown);
		CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlError, _("Registration of account %s failed for unknown reason.\n"), login);
		m_ownerConnection->write(answer);
		return;
	}
	account->setPasswd(password);
/*
/*
	CUserAccount account;
	account.setLogin(login);
	account.setPassword(password);

	if (!account.saveToDB())
	{
		answer.wrt((ireon::net::commandId) ireon::net::rcRegistrationFailed);
		answer.wrt((byte) ireon::net::rfUnknown);
		CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlInfo, _("Registration of account %s failed for unknown reason.\n"), login);
		m_ownerConnection->write(answer);
		return;
	}
*/
/*
	answer.wrt((ireon::net::commandId) ireon::net::rspcCodes::rcRegistrationOk);
	CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlInfo, _("Account %s created.\n"), login.c_str());
	m_ownerConnection->write(answer);
*/
};
