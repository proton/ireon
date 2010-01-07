/* Copyright (C) 2005 ireon.org developers council
 * $Id: net_client.cpp 421 2005-12-18 15:15:17Z zak $

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

/**
 * @file net_client.cpp
 * Network client
 */

#include "stdafx.h"
#include "SDL_thread.h"
#include "net/net_client.h"


CNetClient::CNetClient():
m_socket(NULL),
m_connThread(NULL),
m_socketSet(NULL),
m_lastPing(time(NULL)),
m_lastPong(time(NULL)),
m_ownSocketSet(false),
m_readPtr(m_buf),
m_wrtPtr(m_buf),
m_waitFor(0)
{

}

CNetClient::CNetClient(TCPsocket sock, SDLNet_SocketSet socketSet):
m_socket(sock),
m_connThread(NULL),
m_socketSet(socketSet),
m_lastPing(time(NULL)),
m_lastPong(time(NULL)),
m_ownSocketSet(false),
m_readPtr(m_buf),
m_wrtPtr(m_buf),
m_waitFor(0)
{
	assert(m_socket);
	assert(m_socketSet);
	IPaddress* addr = SDLNet_TCP_GetPeerAddress(m_socket);
	//m_host = SDLNet_ResolveIP(addr);
	m_host = getIp(addr->host);
	m_port = 0;
	SDLNet_TCP_AddSocket(m_socketSet,m_socket);
};

CNetClient::~CNetClient()
{
	if( m_socket )
	{
		if( m_socketSet )
		{
			SDLNet_TCP_DelSocket(m_socketSet,m_socket);
			if( m_ownSocketSet )
				SDLNet_FreeSocketSet(m_socketSet);
		}
		SDLNet_TCP_Close(m_socket);
		m_socket = NULL;
	}
	if( m_connThread )
	{
		SDL_KillThread(m_connThread);
		m_connThread = NULL;
	}
}

int connectionThreadFunc(void *args)
{
	CNetClient* client = (CNetClient*)args;
	if( !client )
		return 0;
	IPaddress addr;
	SDLNet_ResolveHost(&addr, client->m_host.c_str(), client->m_port);
	if( addr.host == INADDR_NONE )
	{
		client->connectionFailed(1);
		CLog::instance()->log(CLog::msgLvlCritical,_("Can't resolve host '%s'.\n"),client->m_host.c_str());
		return 0;
	};
	client->m_socket = SDLNet_TCP_Open(&addr);
	if ( client->m_socket == NULL )
	{
		CLog::instance()->log(CLog::msgLvlCritical,_("Error opening socket.\n"));
		client->connectionFailed(2);
		return 0;
	};
	client->m_lastPing = time(NULL);
	client->m_lastPong = time(NULL),
	SDLNet_TCP_AddSocket(client->m_socketSet,client->m_socket);
    return 0;
};

bool CNetClient::connect(const char* host, ushort serverPort, SDLNet_SocketSet socketSet)
{
	assert(host);
	if( !socketSet )
	{
		socketSet = SDLNet_AllocSocketSet(1);
		m_ownSocketSet = true;
	} else
	{
		if( m_ownSocketSet && m_socketSet && m_socketSet != socketSet )
			SDLNet_FreeSocketSet(m_socketSet);
		m_ownSocketSet = false;
	}
	m_socketSet = socketSet;
	if( m_socket )
	{
		CLog::instance()->log(CLog::msgLvlError,_("Trying to connect when connection is already established.\n"));
		return false;
	};
	m_host = host;
	m_port = serverPort;
	IPaddress addr;
	SDLNet_ResolveHost(&addr, m_host.c_str(), m_port);
	m_connThread = SDL_CreateThread(connectionThreadFunc,this);
	if( !m_connThread )
	{
		CLog::instance()->log(CLog::msgLvlError,_("(%s:%d) : Error creating connection thread.\n"),m_host.c_str(),m_port);
		return false;
	};
	CLog::instance()->log(CLog::msgLvlInfo,_("New connection to (%s:%d)...\n"),host,serverPort);
	return true;
}

void CNetClient::disconnect()
{
	m_output.clear();
	if( m_socket )
	{
		SDLNet_TCP_DelSocket(m_socketSet,m_socket);
		SDLNet_TCP_Close(m_socket);
		m_socket = NULL;
	}
	if( m_connThread )
	{
		SDL_KillThread(m_connThread);
		m_connThread = NULL;
	}
}

void CNetClient::reconnect()
{
	assert(m_host != BLANK_STRING);
	assert(m_socketSet);
	if (m_socket)
		disconnect();
	CLog::instance()->log(CLog::msgLvlInfo,_("Reconnecting to %s:%d.\n"),m_host.c_str(),m_port);
	connect(m_host.c_str(),m_port,m_socketSet);
};

/*
void CNetClient::initializeSecurity()
{
	assert(m_socket);
	m_socket->InitializeSecurity(0,0);
}

void CNetClient::disableSecurity()
{
	assert(m_socket);
	m_socket->DisableSecurity();
}
*/
bool CNetClient::isConnected() const
{
	return m_socket != NULL;
}

/*
void CNetClient::write(const char* data, uint length)
{
	assert(data);
	CData d(data,length);
	write(d);
}
*/

void CNetClient::write(CData& data)
{
	m_output.push_back(data);
}

bool CNetClient::processOutput()
{
	assert(m_socket);
	if( time(NULL) - m_lastPong > LOST_CONNECTION_TIME )
	{
		m_lastPong = time(NULL);
		CLog::instance()->log(CLog::msgFlagNetwork,CLog::msgLvlInfo,_("(%s:%d) : Ping timeout.\n"),m_host.c_str(),m_port);
		return false;
	}
	if( m_output.size() )
		m_lastPing = time(NULL);
	if( time(NULL) - m_lastPing > PING_TIME )
	{
		const ushort zero = 0;
		m_lastPing = time(NULL);
		if(SDLNet_TCP_Send(m_socket, (char*)&zero, sizeof(ushort)) <= 0 ) /// Ping
		{
			CLog::instance()->log(CLog::msgFlagNetwork,CLog::msgLvlInfo,_("(%s:%d) : Error write socket.\n"),m_host.c_str(),m_port);
			return false;
		}
	}
	std::vector<CData>::iterator i;
	for(i = m_output.begin(); i != m_output.end(); i++ )
	{
		if( (*i).length() > 0xFFFF )
		{
			CLog::instance()->log(CLog::msgLvlError,_("Data length too big (%d).\n"),(*i).length());
			continue;
		};
		ushort len = (ushort)(*i).length();
		if ( SDLNet_TCP_Send(m_socket, &len, sizeof(ushort)) <= 0 )
		{
			CLog::instance()->log(CLog::msgFlagNetwork,CLog::msgLvlInfo,_("(%s:%d) : Error write socket.\n"),m_host.c_str(),m_port);
			return false;
		}

		if(	SDLNet_TCP_Send(m_socket, (char*)(*i).data(), (*i).length() ) <= 0 )
		{
			CLog::instance()->log(CLog::msgFlagNetwork,CLog::msgLvlInfo,_("(%s:%d) : Error write socket.\n"),m_host.c_str(),m_port);
			return false;
		}
	}
	m_output.clear();
	return true;
}

bool CNetClient::processInput()
{
	assert(m_socket);
	/// If we created socket set manually, then we
	/// must check it
	if( m_ownSocketSet )
		SDLNet_CheckSockets(m_socketSet, 0);
	if( SDLNet_SocketReady(m_socket) )
	{
		/// We obtained data, so we set PONG time to now
		m_lastPong = time(NULL);
		/// If write pointer is at the end of buffer. Overflow
		if( m_wrtPtr >= m_buf + sizeof(m_buf) )
		{
			CLog::instance()->log(CLog::msgFlagNetwork,CLog::msgLvlInfo,_("(%s:%d) : Input m_buffer overflow.\n"),m_host.c_str(),m_port);
			return false;
		}
		/// Receive data and write it to write pointer.
		/// res - number of received bytes
		int res = SDLNet_TCP_Recv(m_socket,m_wrtPtr,m_buf - m_wrtPtr + sizeof(m_buf));
		if( res <= 0 )
		{
			/// If result less then zero it's error
			CLog::instance()->log(CLog::msgFlagNetwork,CLog::msgLvlInfo,_("(%s:%d) : Error read from socket.\n"),m_host.c_str(),m_port);
			return false;
		}
		else
			m_wrtPtr += res; /// Move write pointer to the end of data
	} else
		return true;
	while( 1 ) /// Process obtained data until it finishes
	{
		if( m_waitFor ) /// If we reading command (m_waitFor - size of current command)
		{
			if( m_wrtPtr - m_readPtr >= m_waitFor ) /// If it's enough data in buffer
			{
				/// Wrap data for sending it to handler
				CData data((const char*)m_readPtr,m_waitFor,false);
				/// Process one block of data (command)
				processBlock(data);
				/// Move read pointer to data after current command
				m_readPtr += m_waitFor;
				m_waitFor = 0;
			} else
				break;
		} else if( m_wrtPtr - m_readPtr >= 2 )
		{
			/// 2 bytes - size of word (in which we store size of next command)

			/// take size of next command
			m_waitFor = *((ushort*)m_readPtr);
			m_readPtr += sizeof(ushort);
			if( !m_waitFor )
			{
				/// Connection must send some data at least
				/// every 5 seconds, so if it is nothing to
				/// send, it sends empty command (with zero
				/// length)
				continue; /// Just ping
			}
		} else
			break;
	}

	if( m_readPtr != m_buf )
	{
		/// If we readed something, shift buffer to
		/// first unprocessed byte
		if( m_wrtPtr > m_readPtr )
			memcpy(m_buf,m_readPtr,m_wrtPtr - m_readPtr);
		/// Decrease write pointer too
		m_wrtPtr -= (m_readPtr - m_buf);
		m_readPtr = m_buf;
	};
	return true;
};

void CNetClient::processBlock(CData& data)
{
	ireon::net::commandId id;
	/// Read command id
	data >> id;
	/// Obtain number of command
	id -= SIGNALS_START;
	CLog::instance()->log(CLog::msgFlagNetwork,CLog::msgLvlInfo,_("Received packet. Id = %d.\n"),id);
	if( !processCommand(data,id) )
	{
		/// If some error happened
		CLog::instance()->log(CLog::msgLvlError,_("(%s:%d) : Can't process packet. Contents:\n"),m_host.c_str(),m_port);
		CLog::instance()->dump(CLog::msgLvlError,data.data(),data.length());
		return;
	}
};

void CNetClient::connectionLost()
{
	CLog::instance()->log(CLog::msgLvlInfo,_("Connection to %s:%d lost.\n"),m_host.c_str(),m_port);
	reconnect();
};
