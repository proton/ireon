/**
 * @file ireon_rs/net/rspc/rspc_wait_remove_ack_state.cpp
 * root server player context wiating for ack from cm to remove character state
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

#include "ireon_rs/net/rspc/rspc_wait_remove_ack_state.h"
#include "common/net/signal_codes/rspc_codes.h"
#include "ireon_rs/accounts/user_account.h"

#include "ireon_rs/db/root_db.h"
#include "common/db/account.h"
//#include "ireon_rs/root_app.h"
//#include "ireon_rs/net/root_eserver.h"

#include "ireon_rs/stdafx.h"


CRSPCWaitRemoveAckState::CRSPCWaitRemoveAckState(CRSPlayerConnection *ownerConnection):
m_ownerConnection(ownerConnection)
{
	setStateName("WaitRemoveAcc State");
	// notice addrTypeMask -- signal only available for internal calls
	registerSlot(ireon::net::rspcCodes::scCharRemAck, (CGenericSlot) &CRSPCWaitRemoveAckState::onRemoveAck, netAddress::atInternalAddress);
}

// ---------------------------------------------------------------------

void CRSPCWaitRemoveAckState::onRemoveAck(CData &data)
{
	uint charId;
	data >> charId;
	byte ack;
	data >> ack;

	CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlVerbose, _("Got remove ack from cm for char %d.\n"), charId);

	CData answer;

	if (ack == ireon::net::rspcCodes::craIsInUse)
	{
		answer.wrt((ireon::net::commandId) ireon::net::rspcCodes::rcCharRemoveFailed);
		answer.wrt((byte) ireon::net::rspcCodes::crIsInUse);
		m_ownerConnection->write(answer);
		CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlWarn, _("Char %d is in use on cluster, removal cancelled.\n"), charId);
		m_ownerConnection->setNextState(CRSPlayerConnection::csCharacterSelection);
		return;
	}

	if(ack == ireon::net::rspcCodes::craOk )
	{
		CRootDB::instance()->deleteCharacter(charId);
		answer.wrt((ireon::net::commandId) ireon::net::rspcCodes::rcCharRemoveOk);
		answer << charId;
		m_ownerConnection->write(answer);
		CLog::instance()->log(CLog::msgFlagAccChar,CLog::msgLvlInfo, _("Player %s removed char with id %d.\n"), m_ownerConnection->getOwnerAccPtr()->getName(), charId);
		m_ownerConnection->setNextState(CRSPlayerConnection::csCharacterSelection);
		return;
	}

	answer.wrt((ireon::net::commandId) ireon::net::rspcCodes::rcCharRemoveFailed);
	answer.wrt((byte) ireon::net::rspcCodes::crUnknown);
	m_ownerConnection->write(answer);
	CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlError, _("Got unknown remove ack code %d from cluster (char id %d).\n"), ack, charId);
	m_ownerConnection->setNextState(CRSPlayerConnection::csCharacterSelection);
	return;

};
