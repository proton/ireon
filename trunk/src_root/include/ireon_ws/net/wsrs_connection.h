/**
 * @file ireon_ws/net/wsrs_connection.h
 * world server to root server connection class
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: wsrs_connection.h 755 2006-09-24 14:10:30Z zak $

 * See LICENSE for details
 */

#ifndef _WSRS_CONNECTION_H
#define _WSRS_CONNECTION_H

#include "common/net/net_connection.h"

class CWSRSConnection : public CNetConnection
{
public:
	enum WSRSStates
	{
		main
	};
private:
};

#endif
