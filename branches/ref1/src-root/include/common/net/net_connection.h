/**
 * @file common/net/net_connection.h
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

#ifndef _NET_CONNECTION_H
#define _NET_CONNECTION_H

#include "common/net/state_machine.h"
#include "net/layers/net_layer_base.h"

class CNetConnection : public CStateMachine
{
public:
	~CNetConnection() {};
	virtual void onClose() {} 

	void setUnderlyingLayer(CNetLayerBasePtr layer) { m_nextLayer = layer; m_connectionInfo = m_nextLayer->getInfo(); }
//	CNetLayerBasePtr getUnderlyingLayer() { return m_nextLayer; }

	///Receive and process all incoming packets
	bool processInput();
	///Process and send all outcoming packets
	bool processOutput();
	///Process command -- for executing actions in this connection from another connections
	void processCommandEx(ireon::net::commandId id, CData& data, netAddress fromAddress = netAddress(netAddress::atUndefinedAddress));

	///Write data to output
	void write(CData& data);
	
	///Attention !Hack! m_connectionInfo supposed to contain only remote host ip -- FIXME
	const char* getHost() {return m_connectionInfo.c_str();}
	const char* getConnectionInfo() {return m_connectionInfo.c_str();}

	uint getId() {return m_connectionId;};
	void setId(uint id) {m_connectionId = id;};

protected:
	CNetConnection():m_connectionId(0) {};

private:
	///Does actual processing for processCommandEx and processInput
	void processCommand(ireon::net::commandId id, CData& data, netAddress fromAddress);
	///First layer(next for CNetConnection ) in layers chain 
	CNetLayerBasePtr m_nextLayer;

	String	m_connectionInfo;

	uint		m_connectionId;
};


#endif

