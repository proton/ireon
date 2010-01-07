/**
 * @file ireon_client\net\player_client.cpp
 * Player's network client
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: player_client.cpp 433 2005-12-20 20:19:15Z zak $

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


#include "stdafx.h"
#include "interface/interface.h"

bool CPlayerClient::processCommand(CData &data,byte id)
{
	if( id >= m_signals.size() )
		return false;
	(this->*m_signals[id])(data);
	return true;
};

void CPlayerClient::connectionFailed(uint error)
{
	ConnectFailedEventArg arg;
	arg.m_host =  getHost();
	arg.m_port = getPort();
	switch(error)
	{
	case 1:
		{
			arg.m_error = _("Can't resolve host.");
			break;
		}
	case 2:
		{
			arg.m_error = _("Can't establish connection.");
			break;
		}
	default:
		{
			break;
		}
	};
	CEventManager::instance()->execute(CEventManager::ON_CONN_FAILED,&arg);
};


