/**
* @file ireon_rs/net/rsws_connection.h
* root server to world server connection class
*/

/* Copyright (C) 2005-2006 ireon.org developers council
* $Id: rsws_connection.h 726 2006-09-13 21:10:24Z zak $

* See LICENSE for details
*/

#ifndef _RSWS_CONNECTION_H
#define _RSWS_CONNECTION_H

#include "common/net/net_connection.h"

class CRSWSConnection : public CNetConnection
{
public:
	enum RSWSStates
	{
		main
	};
private:
};

#endif
