/**
* @file ireon_client/net/pcws_connection.h
* client connection to world server
*/

/* Copyright (C) 2005-2006 ireon.org developers council
* $Id: pcrs.h 698 2006-09-03 16:46:33Z zak $

*  See LICENSE for details
*/


#ifndef _PC_WS_CONNECTION_H
#define _PC_WS_CONNECTION_H

#include "common/net/net_connection.h"

class CPCWSConnection : public CNetConnection
{
public:
	// ----------------------
	enum PCWSStates
	{
		hello = 0,
		main = 1
	};
	// ----------------------
	
	CPCWSConnection();

	void onClose(){};
};

#endif
