/**
 * @file ireon_client/net/pcws/hello_state.h
 * client ws connection welcome (hello) state
 */

/* Copyright (C) 2005-2006 ireon.org developers council
* $Id$

* See LICENSE for details
*/

#ifndef _PCWS_MAIN_STATE_H_
#define _PCWS_MAIN_STATE_H_

#include "common/net/generic_state.h"
#include "ireon_client/net/pcws_connection.h"

class CPCWSMainState : public CGenericState
{
public:
	CPCWSMainState(CNetConnection* ownerConnection);
private:

	
};

#endif
