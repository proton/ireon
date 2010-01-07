/**
 * @file ireon_cm/net/cmwc/cmwc_main_state.h
 * cm server world connection main state
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

#ifndef _CMWC_MAIN_STATE_H_
#define _CMWC_MAIN_STATE_H_

#include "common/net/generic_state.h"
#include "ireon_cm/net/cm_ws_connection.h"


class CCMWCMainState : public CGenericState
{
public:
	CCMWCMainState (CCMWSConnection *ownerConnection);

private:
	CCMWSConnection		*m_ownerConnection;


private:
	///Command handlers

	///World server asks for account, 1
	void onGetAccount(CData& data);

	///World server sends player own char data, 2
	void onSendPlayerChar(CData& data);

	///create new char, 3
	void onNewChar(CData& data);

	///Remove char, 4
	void onRemoveChar(CData& data);
	///----------------------
		///Commands

	/// 0 - login

	///Send account data, 1
	void sendAccount(CData& data);

	///Ask for player own char data, 2
	//void getPlayerChar(uint id, int64 sync);

	///Send new character to world, 3
	void sendNewChar(CData& data);

	///Remove char, 4
	void removeChar(CData& data);
	
};

#endif
