/**
 * @file ireon_cm/net/cm_rs_connection.cpp
 * Cluster manager connection to root server
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
#include "ireon_cm/net/cm_rs_connection.h"
#include "ireon_cm/net/cmrc/cmrc_hello_state.h"
#include "ireon_cm/net/cmrc/cmrc_main_state.h"

CCMRSConnection::CCMRSConnection()
{
	registerState(CCMRSConnection::csPreLogin, new CStateCreator<CCMRCPreLoginState, CCMRSConnection>(this));
	registerState(CCMRSConnection::csWaitLoginAck, new CStateCreator<CCMRCWaitLoginAckState, CCMRSConnection>(this));
	registerState(CCMRSConnection::csMain, new CStateCreator<CCMRCMainState, CCMRSConnection>(this));
	
	setNextState(CCMRSConnection::csPreLogin);
	switchStates();
};