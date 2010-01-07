/**
 * @file ireon_cm/net/cmwc/cmwc_hello_state.h
 * cm server world context welcome (hello) state
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

#ifndef _CMWC_HELLO_STATE_H_
#define _CMWC_HELLO_STATE_H_

#include "common/net/generic_state.h"
#include "ireon_cm/net/cm_ws_connection.h"


class CCMWCHelloState : public CGenericState
{
public:
	CCMWCHelloState(CCMWSConnection *ownerConnection);

private:
	CCMWSConnection		*m_ownerConnection;


private:
	///Command handlers

	///Login world server, 0
	void onLoginRequest(CData& data);
	
};

#endif
