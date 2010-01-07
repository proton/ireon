/**
 * @file ireon_rs/net/rspc/rspc_hello_state.cpp
 * root server player context welcome (hello) state
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

#include "ireon_rs/net/rspc/rspc_hello_state.h"
#include "common/net/signal_codes/rspc_codes.h"

#include "ireon_rs/db/root_db.h"
#include "common/db/account.h"
//#include "ireon_rs/accounts/user_account.h"
#include "ireon_rs/root_app.h"
//#include "ireon_rs/net/root_eserver.h"

#include "ireon_rs/stdafx.h"


CRSPCHelloState::CRSPCHelloState(CRSPlayerConnection *ownerConnection):
m_ownerConnection(ownerConnection)
{
	setStateName("Hello State");
	registerSlot(ireon::net::rspcCodes::scHelloLogin, (CGenericSlot) &CRSPCHelloState::onLoginRequest);
	registerSlot(ireon::net::rspcCodes::scHelloRegister, (CGenericSlot) &CRSPCHelloState::onRegisterRequest);
}

// ---------------------------------------------------------------------

void CRSPCHelloState::onLoginRequest(CData &data)
{
	String login;
	String password;
	data >> login >> password;
	CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlInfo, _("Login attempt from host %s, login %s...\n"), m_ownerConnection->getConnectionInfo(), login.c_str());

	CData answer;

//	UserAccountPointer account = CUserAccount::fetchAccount(login);
//	if (!account)
	AccPtr account = CRootDB::instance()->findAccount(login);
	if ((account.get() == NULL) || (login != account->getName()))
	{
		answer.wrt((ireon::net::commandId) ireon::net::rspcCodes::rcLoginFailed);
		answer.wrt((byte) ireon::net::rspcCodes::lfAccountNotFound);
	   	CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlWarn, _("Authentification failed. Could not find for user %s.\n"), login.c_str());
		m_ownerConnection->write(answer);
		return;
	}

//	if (!account->passwordMatches(password))
	if( account->getPasswd() != password )
	{
		answer.wrt((ireon::net::commandId) ireon::net::rspcCodes::rcLoginFailed);
		answer.wrt((byte) ireon::net::rspcCodes::lfWrongPassword);
	   	CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlWarn, _("Authentification failed. Wrong password for user %s.\n"), login.c_str());
		m_ownerConnection->write(answer);
		return;
	}

	if (CRootApp::instance()->getPlayerConnectionsManager()->findConnection(account->getId()))
	{
		answer.wrt((ireon::net::commandId) ireon::net::rspcCodes::rcLoginFailed);
		answer.wrt((byte) ireon::net::rspcCodes::lfAlreadyLoggedIn);
	   	CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlWarn, _("Authentification failed. User %s already logged in.\n"), login.c_str());
		m_ownerConnection->write(answer);
		return;
	}

	answer.wrt((ireon::net::commandId) ireon::net::rspcCodes::rcLoginOk);
	CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlInfo, _("User %s logged in.\n"), login.c_str());

	m_ownerConnection->setOwnerAccount(account);
	m_ownerConnection->setId(account->getId());
	m_ownerConnection->setNextState(CRSPlayerConnection::csCharacterSelection);

	m_ownerConnection->write(answer);
};

// ---------------------------------------------------------------------

void CRSPCHelloState::onRegisterRequest(CData &data)
{
	String login;
	String password;

	data >> login >> password;


	CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlWarn, _("Account registration attempt from host %s, login %s...\n"), m_ownerConnection->getConnectionInfo(), login.c_str());

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

	answer.wrt((ireon::net::commandId) ireon::net::rspcCodes::rcRegistrationOk);
	CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlInfo, _("Account %s created.\n"), login.c_str());
	m_ownerConnection->write(answer);
};
