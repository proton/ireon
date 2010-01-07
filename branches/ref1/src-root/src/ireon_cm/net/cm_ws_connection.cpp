/**
 * @file ireon_cm/net/cm_ws_connection.cpp
 * World connection on cluster manager
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

#include "ireon_cm/stdafx.h"
#include "ireon_cm/net/cm_ws_connection.h"
//#include "db/client_char_data.h"
//#include "db/account.h"
//#include "db/cm_db.h"
//#include "common/net/cmwc_codes.h"

#include "ireon_cm/net/cmwc/cmwc_hello_state.h"
#include "ireon_cm/net/cmwc/cmwc_main_state.h"

CCMWSConnection::CCMWSConnection()
{
	registerState(CCMWSConnection::csHello, new CStateCreator<CCMWCHelloState, CCMWSConnection>(this));
	registerState(CCMWSConnection::csMain, new CStateCreator<CCMWCMainState, CCMWSConnection>(this));
	
	setNextState(CCMWSConnection::csHello);
	switchStates();
};
