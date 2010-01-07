/**
 * @file ireon_rs/net/rspc/rspc_hello_state.h
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

#ifndef _RSPC_HELLO_STATE_H_
#define _RSPC_HELLO_STATE_H_

#include "common/net/generic_state.h"
#include "ireon_rs/net/rs_player_connection.h"

class CRSPCHelloState : public CGenericState
{
public:
	CRSPCHelloState(CRSPlayerConnection *ownerConnection);

private:
	CRSPlayerConnection		*m_ownerConnection;

	void onLoginRequest(CData& data);
	void onRegisterRequest(CData& data);
};

#endif
