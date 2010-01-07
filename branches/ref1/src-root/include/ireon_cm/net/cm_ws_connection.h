/**
 * @file ireon_cm/net/cm_ws_connection.h
 * World connection on cluster manger
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


#ifndef _CM_WS_CONNECTION_H
#define _CM_WS_CONNECTION_H

#include "common/net/net_connection.h"

class CCMWSConnection : public CNetConnection
{
public:
	CCMWSConnection();

	// ----------------------
	enum ConnectionStates
	{
		csHello = 0,
		csMain	= 1
	};
	// ----------------------


public:
	/*void setWorldId(uint newWorldId){ m_worldId = newWorldId; };
	 uint getWorldId() {return m_worldId;}
	*/
	/// remove, because functional replace to CNetConnection
	void setWorldPort(uint newWorldPort){ m_worldPort = newWorldPort; };
	uint getWorldPort() {return m_worldPort;}
private:
	/// remove, because functional replace to CNetConnection
	///Id of world server
	/// uint m_worldId;
	///Port of world server
	uint m_worldPort;
};


#endif

