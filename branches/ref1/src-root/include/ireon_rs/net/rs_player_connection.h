/**
 * @file ireon_rs/net/rs_player_context.h
 * Player connection context on root server
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: rs_player_connection.h 606 2006-04-17 18:49:31Z zak $

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


#ifndef _RS_PLAYER_CONNECTION_H
#define _RS_PLAYER_CONNECTION_H

#include "common/net/net_connection.h"

class CRSPlayerConnection : public CNetConnection
{
public:
	CRSPlayerConnection();

	// ----------------------
	enum ConnectionStates
	{
		csCheckVersion = 0,
		csHello = 1,
		csCharacterSelection = 2,
		csWaitRemoveAck = 3
	};
	// ----------------------

//	UserAccountPointer getOwnerAccount() { return m_ownerAccount; }
//	void setOwnerAccount(UserAccountPointer newAccount) { m_ownerAccount = newAccount; }

	AccPtr getOwnerAccPtr() { return m_account; }
	void setOwnerAccount(AccPtr newAccount) { m_account = newAccount; }

private:
//	UserAccountPointer			m_ownerAccount;
	/// old var, to be removed
	AccPtr m_account;
};


#endif

