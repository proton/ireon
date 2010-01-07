/**
 * @file ireon_rs/net/rs_cm_connection.h
 * Cluter manager connection context on root server
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: rs_cm_connection.h 606 2006-04-17 18:49:31Z zak $

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

#ifndef _RS_CM_CONNECTION_H
#define _RS_CM_CONNECTION_H

#include "net/net_connection.h"

class CRSCMConnection : public CNetConnection
{
public:
	CRSCMConnection();

	// ----------------------
	enum ConnectionStates
	{
		csHello = 0,
		csMain = 1
	};
	// ----------------------

};

#endif

