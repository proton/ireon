/**
 * @file common/net/net_connection.cpp
 * Generic network connection class
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


#include "common/common_stdafx.h"
#include "common/net/net_connection.h"


void CNetConnection::write(CData& data)
{
	m_nextLayer->write(data);
}

// --------------------------------------------------------------------

bool CNetConnection::processOutput()
{
	return m_nextLayer->flush();
}

// --------------------------------------------------------------------

bool CNetConnection::processInput()
{
	std::vector<CData> input;
	if (!m_nextLayer->read(input))
		return false;
	std::vector<CData>::iterator i;
	for (i = input.begin(); i != input.end(); i++)
	{
		CData data((*i).data(),(*i).length(),false);	
		ireon::net::commandId id;
		data >> id;
		processCommand(id, data, netAddress(netAddress::atExternalAddress));
	}
	return true;
};

// --------------------------------------------------------------------

void CNetConnection::processCommandEx(ireon::net::commandId id, CData& data, netAddress fromAddress)
{
	// TODO maybe we should buffer commands to process them 
	// on next processInput() call instead of processing immediately
	data.reset(false);
	processCommand(id, data, fromAddress);
}

// --------------------------------------------------------------------

void CNetConnection::processCommand(ireon::net::commandId id, CData& data, netAddress fromAddress)
{
	if (fromAddress.m_type == netAddress::atInternalAddress)
		CLog::instance()->log(CLog::msgFlagNetwork,CLog::msgLvlDebug,_("Received internal packet. Id = %d.\n"),id);
	else
		CLog::instance()->log(CLog::msgFlagNetwork,CLog::msgLvlDebug,_("Received packet. Id = %d.\n"),id);

	if( !processSignal(id, fromAddress, data) )
	{
		if (fromAddress.m_type == netAddress::atExternalAddress)
		{
			CData response;
			response.wrt((ireon::net::commandId) ireon::net::rcCommandNotFound);
			response << id;
			write(response);
		}
		CLog::instance()->log(CLog::msgLvlError,_("Can't process packet. Contents:\n"));
		CLog::instance()->dump(CLog::msgLvlError,data.data(),data.length());
	}
	return;
};

// --------------------------------------------------------------------
