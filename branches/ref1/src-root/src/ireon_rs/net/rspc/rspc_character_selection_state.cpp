/**
 * @file ireon_rs/net/rspc/rspc_character_selection_state.cpp
 * root server player context character manipulations state
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

#include "ireon_rs/net/rspc/rspc_character_selection_state.h"
#include "common/net/signal_codes/rspc_codes.h"
#include "common/net/signal_codes/rscc_codes.h"

#include "ireon_rs/root_app.h"
//#include "ireon_rs/net/rs_world_context.h"
//#include "ireon_rs/net/root_iserver.h"
//#include "net/root_eserver.h"

#include "ireon_rs/db/root_db.h"
#include "common/db/account.h"
#include "common/db/client_char_data.h"
//#include "ireon_rs/accounts/user_account.h"
//#include "ireon_rs/accounts/general_character_info.h"

#include "ireon_rs/stdafx.h"

CRSPCCharacterSelectionState::CRSPCCharacterSelectionState(CRSPlayerConnection *ownerConnection):
m_ownerConnection(ownerConnection)
{
	setStateName("Character Selection State");
	registerSlot(ireon::net::rspcCodes::scCharSelList, (CGenericSlot) &CRSPCCharacterSelectionState::onListCharsRequest);
	registerSlot(ireon::net::rspcCodes::scCharSelSelect, (CGenericSlot) &CRSPCCharacterSelectionState::onSelectCharRequest);
	registerSlot(ireon::net::rspcCodes::scCharSelCreate, (CGenericSlot) &CRSPCCharacterSelectionState::onCreateCharRequest);
	registerSlot(ireon::net::rspcCodes::scCharSelRemove, (CGenericSlot) &CRSPCCharacterSelectionState::onRemoveCharRequest);
}

// ------------------------------------------------------------------------

void CRSPCCharacterSelectionState::onListCharsRequest (CData &data)
{
	data;
	CLog::instance()->log(CLog::msgLvlVerbose, _("Char list request from account %s...\n"), m_ownerConnection->getOwnerAccPtr()->getName());

	CData answer;
	answer.wrt((ireon::net::commandId) ireon::net::rspcCodes::rcCharListOk);
/*
	std::map<uint,CharacterInfoPointer> chars;
	std::map<uint,CharacterInfoPointer>::iterator i;

	assert(m_ownerConnection->getOwnerAccount()->getId());
	if (CGeneralCharacterInfo::getCharactersForAccount(m_ownerConnection->getOwnerAccount()->getId(), chars))
	{
		answer.wrt((ireon::net::commandId) ireon::net::rcCharListOk);
		for( i = chars.begin(); i != chars.end(); ++i )
		{
			answer.wrt((*i).first);
			answer << (*i).second->getName();
		}
	}
*/
	std::map<uint,RootCharPlayerData> chars;
	std::map<uint,RootCharPlayerData>::iterator i;
	CRootDB::instance()->selectCharactersByAccount(m_ownerConnection->getOwnerAccPtr()->getId(),chars);
	for( i = chars.begin(); i != chars.end(); ++i )
	{
		ClientCharRegistrationData chData;
		chData.m_id = (*i).second.m_id;
		chData.m_name = (*i).second.m_name;
		chData.serialize(answer);
	};

	m_ownerConnection->write(answer);

}

// ------------------------------------------------------------------------
	
void CRSPCCharacterSelectionState::onSelectCharRequest (CData &data)
{
	int id;
	data >> id;
	CLog::instance()->log(CLog::msgLvlVerbose, _("Player %s selected char with id %d.\n"), m_ownerConnection->getOwnerAccPtr()->getName(),id);
	if(CRootDB::instance()->findCharacter(id) && CRootDB::instance()->findCharacter(id)->m_account == m_ownerConnection->getOwnerAccPtr()->getId())
	{
		m_ownerConnection->getOwnerAccPtr()->setSelChar(id);
		uint cluster = CRootDB::instance()->getCluster(id);
		CNetConnection* con = CRootApp::instance()->getCMConnectionsManager()->findConnection(cluster);
		if( con )
		{
			/*
			m_ownerConnection->setStateEx(CRSPlayerContext::STATE_WAIT_WORLD);
			con->getWorld(id);
			*/
			m_ownerConnection->setNextState(CRSPlayerConnection::csHello);
			return;
		}
	}
	CLog::instance()->log(CLog::msgLvlError,_("Can't find cluster for char with id %d.\n"),id);
};

// ------------------------------------------------------------------------

void CRSPCCharacterSelectionState::onCreateCharRequest (CData& data)
{
	ClientCharRegistrationData clientCharData;
	clientCharData.serialize(data);
	RootCharPlayerData	ch;

	CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlVerbose,_("Got character creation request: char name %s, acc id %d.\n"),ch.m_name.c_str(),m_ownerConnection->getOwnerAccPtr()->getId());

	ch.m_account = m_ownerConnection->getOwnerAccPtr()->getId();
	// change this to let users select starting cluster
	ch.m_cluster = 1;
	// FIXME check char name for bad words etc
	ch.m_name = clientCharData.m_name;

	CData answer;

	if (CRootDB::instance()->findCharacter(ch.m_name))
	{
		answer.wrt((ireon::net::commandId) ireon::net::rspcCodes::rcCharCreationFailed);
		answer.wrt((byte) ireon::net::rspcCodes::ccAlreadyRegistered);
		m_ownerConnection->write(answer);
		CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlWarn,_("Attempt to create character with existing name (%s), acc id %d.\n"),ch.m_name.c_str(),m_ownerConnection->getOwnerAccPtr()->getId());
		return;
	}

	if (!CRootDB::instance()->addCharacter(&ch))
	{
		answer.wrt((ireon::net::commandId) ireon::net::rspcCodes::rcCharCreationFailed);
		answer.wrt((byte) ireon::net::rspcCodes::ccDBError);
		m_ownerConnection->write(answer);
		CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlError,_("Database manager returned error while creating character %s.\n"),ch.m_name.c_str());
		return;
	}

	CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlVerbose,_("Char %s with id %d created.\n"),ch.m_name.c_str(), ch.m_id);
	clientCharData.m_name = ch.m_name;
	clientCharData.m_id = ch.m_id;
	answer.wrt((ireon::net::commandId) ireon::net::rspcCodes::rcCharCreated);
	clientCharData.serialize(answer);
	m_ownerConnection->write(answer);
};

// ------------------------------------------------------------------------

void CRSPCCharacterSelectionState::onRemoveCharRequest (CData& data)
{
	uint charId;
	data >> charId;

	CLog::instance()->log(CLog::msgLvlVerbose, _("Got character deletion request: account id %d, char is id %d.\n"), m_ownerConnection->getOwnerAccPtr()->getId(), charId);

	CData	answer;

	RootCharPlayerData *ch = CRootDB::instance()->findCharacter(charId);
	if (!ch)
	{
		answer.wrt((ireon::net::commandId) ireon::net::rspcCodes::rcCharRemoveFailed);
		answer.wrt((byte) ireon::net::rspcCodes::crCharNotFound);
		m_ownerConnection->write(answer);
		CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlWarn,_("Attempt to remove nonexisting character %d.\n"), charId);
		return;
	}

	uint cluster = ch->m_cluster;
	CNetConnection* con = CRootApp::instance()->getCMConnectionsManager()->findConnection(cluster);
	if( !con )
	{
		answer.wrt((ireon::net::commandId) ireon::net::rspcCodes::rcCharRemoveFailed);
		answer.wrt((byte) ireon::net::rspcCodes::crClusterNotFound);
		CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlError, _("Could not find cluser to remove char %d.\n"), charId);
		m_ownerConnection->write(answer);
		return;
	}
	
	CData	request;
	request << charId;
	con->processCommandEx(ireon::net::rsccCodes::scCheckRemoveCharRequest, request, netAddress(netAddress::atInternalAddress));

	answer.wrt((ireon::net::commandId) ireon::net::rspcCodes::rcCharRemoveWaitAck);
	m_ownerConnection->write(answer);
	CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlVerbose, _("Sent remove request to cluster %d for char %d.\n"), con->getId(), charId);
	m_ownerConnection->setNextState(CRSPlayerConnection::csWaitRemoveAck);
	return;
};

