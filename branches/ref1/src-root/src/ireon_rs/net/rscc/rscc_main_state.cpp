/**
 * @file ireon_rs/net/rscc/rscc_main_state.cpp
 * root server cm context main state
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

#include "ireon_rs/net/rscc/rscc_main_state.h"
#include "common/net/signal_codes/rscc_codes.h"
#include "common/net/signal_codes/rspc_codes.h"
#include "ireon_rs/root_app.h"
#include "ireon_rs/db/root_db.h"

#include "ireon_rs/stdafx.h"


CRSCCMainState::CRSCCMainState(CRSCMConnection *ownerConnection):
m_ownerConnection(ownerConnection)
{
	setStateName("CM Connection Main State");
	registerSlot(ireon::net::rsccCodes::scCharRemoveAck, (CGenericSlot) &CRSCCMainState::onRemoveCharAck);
	// notice addrTypeMask -- signal only available for internal calls
	registerSlot(ireon::net::rsccCodes::scCheckRemoveCharRequest, (CGenericSlot) &CRSCCMainState::onCheckRemoveCharRequest, netAddress::atInternalAddress);
}

// ---------------------------------------------------------------------

void CRSCCMainState::onCheckRemoveCharRequest(CData& data)
{
	uint charId;
	data >> charId;

	CData request;

	request.wrt((ireon::net::commandId) ireon::net::rsccCodes::rcCheckCharRemove);
	request << charId;
	m_ownerConnection->write(request);
}

// ---------------------------------------------------------------------

void CRSCCMainState::onRemoveCharAck(CData& data)
{
	uint charId;
	byte res;
	data >> charId >> res;

	RootCharPlayerData* charData = CRootDB::instance()->findCharacter(charId);
	if (!charData)
	{
		CLog::instance()->log(CLog::msgLvlError, _("Char deletion: could not find character info for char id %d.\n"), charId);
		return;
	}
	CNetConnection* con = CRootApp::instance()->getPlayerConnectionsManager()->findConnection(charData->m_account);
	if (!con)
	{
		CLog::instance()->log(CLog::msgLvlError, _("Char deletion: could not find player connection for account id %d.\n"), charData->m_account);
		return;
	}

	CData answer;

	if (res == ireon::net::rsccCodes::craCharUnknown)
	{
		answer.wrt((byte) ireon::net::rspcCodes::craCharUnknown);
		con->processCommandEx(ireon::net::rspcCodes::scCharRemAck, answer, netAddress(netAddress::atInternalAddress));
		CLog::instance()->log(CLog::msgLvlError, _("Got deletion response from cluster %d: character %d unknown.\n"), m_ownerConnection->getId(), charId);
		return;
	}

	if (res == ireon::net::rsccCodes::craInUse)
	{
		answer.wrt((byte) ireon::net::rspcCodes::craIsInUse);
		con->processCommandEx(ireon::net::rspcCodes::scCharRemAck, answer, netAddress(netAddress::atInternalAddress));
		CLog::instance()->log(CLog::msgLvlError, _("Got deletion response from cluster %d: character %d is in use.\n"), m_ownerConnection->getId(), charId);
		return;
	}

	if (res == ireon::net::rsccCodes::craRemoveOk)
	{
		answer.wrt((byte) ireon::net::rspcCodes::craOk);
		con->processCommandEx(ireon::net::rspcCodes::scCharRemAck, answer, netAddress(netAddress::atInternalAddress));
		CLog::instance()->log(CLog::msgLvlInfo, _("Got deletion response from cluster %d: character %d is ok to delete.\n"), m_ownerConnection->getId(), charId);
		return;
	}
	CLog::instance()->log(CLog::msgLvlError, _("Char deletion: got unknown ack code %d for char id %d.\n"), res, charId);
};

/*
void CRSCCMainState::onLoginRequest(CData &data)
{
	uint clusterId;
	data >> clusterId;

	CData answer;

	if (!CRootApp::instance()->findCluster(m_ownerConnection->getHost()))
	{
		CLog::instance()->log(CLog::msgLvlWarn, _("Attempt to log in cm from unknown host %s.\n"), m_ownerConnection->getHost());
		answer.wrt((ireon::net::commandId) ireon::net::rsccCodes::rcLoginFailed);
		answer.wrt((byte) ireon::net::rsccCodes::lfHostNotAllowed);
		m_ownerConnection->write(answer);
		return;
	}

	if (CRootApp::instance()->getCMConnectionsManager()->findConnection(clusterId))
	{
		CLog::instance()->log(CLog::msgLvlError, _("Attempt to log in second cluster manager with id %d (host: %s).\n"), clusterId, m_ownerConnection->getHost());
		answer.wrt((ireon::net::commandId) ireon::net::rsccCodes::rcLoginFailed);
		answer.wrt((byte) ireon::net::rsccCodes::lfAlreadyLoggedIn);
		m_ownerConnection->write(answer);
		return;
	}

	m_ownerConnection->setId(clusterId);
	m_ownerConnection->setNextState(CRSCMConnection::csMain);

	answer.wrt((ireon::net::commandId) ireon::net::rsccCodes::rcLoginOk);
	m_ownerConnection->write(answer);
	CLog::instance()->log(CLog::msgLvlInfo,_("Cluster manager with id %d logged in.\n"), clusterId);
};

/*
// Network commands 

void CRSCMConnection::getWorld(uint id)
{
	CData data;
	data.wrt((byte)(SIGNALS_START + 3));
	data << id;
	write(data);
	CLog::instance()->log(CLog::msgLvlInfo,_("Sending request for world for char with id %d.\n"),id);
};

void CRSCMConnection::sendChar(ClientOwnCharData *c, uint acc)
{
	CData data;
	data.wrt((byte)(SIGNALS_START+4));
	c->serialize(data);
	data << acc;
	write(data);
};

void CRSCMConnection::removeChar(uint id)
{
	CData data;
	data.wrt((byte)(SIGNALS_START+5));
	data << id;
	write(data);
};


void CRSCMConnection::onGetAccount(CData& data)
{
	String name;
	int64 sync;
	data >> name;
	data >> sync;
	CData answer;
	answer.wrt((byte)(SIGNALS_START + 1));
	AccPtr a = CRootDB::instance()->findAccount(name);
	if( !a.get() )
	{
		answer.wrt((uint)0);
		answer << name;
	} else
	{
		answer.wrt(a->getId());
		if(a->checkSync(sync))
		{
			answer.wrt((byte)0); ///Account isn't up to date
			a->serialize(answer);
		} else
			answer.wrt((byte)1); ///Account up to date
		a->needSync();
	}
	CLog::instance()->log(CLog::msgLvlInfo,_("Sending data for account '%s'.\n"),name.c_str());
	write(answer);
};

void CRSCMConnection::onSendChar(CData& data)
{
};

void CRSCMConnection::onSendWorld(CData& data)
{
	String ip;
	uint port, id;
	data >> id;
	data >> ip;
	data >> port;
	CRSPlayerContext* con = CRootApp::instance()->eserver()->findContext(CRootDB::instance()->findCharacter(id) ? CRootDB::instance()->findCharacter(id)->m_account : 0);
	if( con )
		con->sendWorld(ip.c_str(),port);
};

void CRSCMConnection::onNewChar(CData& data)
{
	byte res;
	ClientOwnCharData d;
	data >> res;
	uint id;
	CRSPlayerContext* con;
	if( !res )
	{
		d.serialize(data);
		CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlInfo,_("New character with id %d created sucsessfully.\n"),d.m_id);
		id = d.m_id;
		RootCharPlayerData* rootData = CRootDB::instance()->findCreating(id);
		if( !rootData )
			res = CCE_OTHER;
		else if( !CRootDB::instance()->addCharacter(rootData,id) )
			res = CCE_OTHER;
	}
	if( res )
	{
		data >> id;
		CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlError,_("New character with id %d wasn't created!\n"),id);
		con = CRootDB::instance()->findCreating(id) ? 
			CRootApp::instance()->eserver()->findContext(CRootDB::instance()->findCreating(id)->m_account) : 0;
		CRootDB::instance()->deleteCreating(id);
	} else
	{
		con = CRootDB::instance()->findCharacter(id) ?
			CRootApp::instance()->eserver()->findContext(CRootDB::instance()->findCharacter(id)->m_account) : 0;
	}
	if( con )
		con->sendCreatedNewChar(res,d);
};

void CRSCMConnection::onRemoveChar(CData& data)
{
	uint id;
	byte res;
	data >> id >> res;
	RootCharPlayerData* d = CRootDB::instance()->findCharacter(id);
	if( d )
	{
		CRSPlayerContext* con = CRootApp::instance()->eserver()->findContext(d->m_account);
		if( con )
			con->removeChar(id,res);
	}
	if( res == 1 )
		CRootDB::instance()->deleteCharacter(id);
};

// End signal handlers 

*/

