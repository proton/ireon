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

#ifndef _CMRC_HELLO_STATE_H_
#define _CMRC_HELLO_STATE_H_

#include "common/net/generic_state.h"
#include "ireon_cm/net/cm_rs_connection.h"


class CCMRCPreLoginState : public CGenericState
{
public:
	CCMRCPreLoginState(CCMRSConnection *ownerConnection);

private:
	CCMRSConnection		*m_ownerConnection;


private:
	void initiateLogin(CData& data);
};

// -------------------------------------------------------

class CCMRCWaitLoginAckState : public CGenericState
{
public:
	CCMRCWaitLoginAckState(CCMRSConnection *ownerConnection);

private:
	CCMRSConnection		*m_ownerConnection;


private:
	void onLoginOk(CData& data);
	void onLoginFailed(CData& data);
	
};

#endif
