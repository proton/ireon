/**
 * @file common/net/layers/sock_layer.cpp
 * socket wrappers
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
#include "SDL_thread.h"
#include "common/net/layers/sock_layer.h"

// ---------------------------------------------------------
// ---------------------------------------------------------

CDataSocketManager::CDataSocketManager(const char* remoteHost, const ushort remotePort):
m_targetPort(remotePort),
m_targetHost(remoteHost),
m_connThreadData(NULL),
m_connThread(NULL)
{
	if (!remoteHost)
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError, _("Outgoing connection created with NULL remote host!\n"), remoteHost, remotePort);
}

// ---------------------------------------------------------

CNetLayerBasePtr CDataSocketManager::connect()
{
	assert(m_targetHost.size()>0);
	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlVerbose, _("Connecting to host %s, port %d.\n"), m_targetHost.c_str(), m_targetPort);

	m_socketSet = CSockSetPtr(new CSocketSet(1));
	if (m_socketSet->errorHasOccured())
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlCritical, _("Error allocating socket set.\n"));
		return CNetLayerBasePtr((CDataSocket*)NULL);
	}

	m_connThreadData = new connectionThreadData();
	m_connThreadData->dataSockPtr = new CDataSocket(NULL, m_socketSet);
	m_connThreadData->sockSet = m_socketSet.get();
	m_connThreadData->remoteHost = m_targetHost.c_str();
	m_connThreadData->remotePort = m_targetPort;

	CNetLayerBasePtr dataSocket(m_connThreadData->dataSockPtr);

	m_connThread = SDL_CreateThread(connectionThreadFunc, m_connThreadData);
	if( !m_connThread )
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError,_("(%s:%d) : Error creating connection thread.\n"),  m_targetHost.c_str(), m_targetPort);
		return CNetLayerBasePtr((CDataSocket*)NULL);
	}
	return dataSocket;
}

// ---------------------------------------------------------

void CDataSocketManager::cleanup()
{
	if( m_connThread )
	{
		SDL_KillThread(m_connThread);
		m_connThread = NULL;
	}
	if ( m_connThreadData )
	{
		delete m_connThreadData;
		m_connThreadData = NULL;
	}
	
	if (m_socketSet.get())
		m_socketSet.reset();
}

// ---------------------------------------------------------

CDataSocketManager::~CDataSocketManager()
{
	cleanup();
}

// ---------------------------------------------------------

int connectionThreadFunc(void *args)
{
	if( !args )
		return 0;
	connectionThreadData* threadData = (connectionThreadData*)args;
	IPaddress addr;
	SDLNet_ResolveHost(&addr, threadData->remoteHost, threadData->remotePort);
	if( addr.host == INADDR_NONE )
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError, _("Can't resolve host '%s'.\n"), threadData->remoteHost);
		return 0;
	};
	threadData->dataSockPtr->m_socket = SDLNet_TCP_Open(&addr);
	if( threadData->dataSockPtr->m_socket == NULL )
	{
		threadData->dataSockPtr->m_sockErrorOccured = true;
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError, _("Error opening socket -- connection to %s failed.\n"), threadData->remoteHost);
		return 0;
	};
	threadData->dataSockPtr->m_remoteHost = threadData->remoteHost;
//	threadData->dataSockPtr->m_remotePort = threadData->remotePort;
	threadData->dataSockPtr->m_lastPing = time(NULL);
	threadData->dataSockPtr->m_lastPong = time(NULL);
	threadData->sockSet->addSocketToSet(threadData->dataSockPtr->m_socket);
	threadData->dataSockPtr->recalculateRemoteHost();
	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError, _("Connection to %s established.\n"), threadData->remoteHost);
    return 0;
};

// ---------------------------------------------------------
// ---------------------------------------------------------

CIncomingConnectionListener::CIncomingConnectionListener():
m_listeningSocket(NULL)
{
}

// ---------------------------------------------------------
bool CIncomingConnectionListener::start(ushort localPort, ushort maxConnections)
{
	m_listeningSocketSet = CSockSetPtr(new CSocketSet(1)); 
	if( m_listeningSocketSet->errorHasOccured() )
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlCritical,_("Error allocating listening socket set.\n"));
		return false;
	};

	m_dataSocketSet = CSockSetPtr(new CSocketSet(maxConnections)); 
	if( m_dataSocketSet->errorHasOccured() )
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlCritical,_("Error allocating data socket set.\n"));
		return false;
	};

	IPaddress addr;
	SDLNet_ResolveHost(&addr, NULL, localPort);
	m_listeningSocket = SDLNet_TCP_Open(&addr);
	if ( m_listeningSocket == NULL )
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlCritical,_("Error creating listening socket.\n"));
		return false;
	};
	m_listeningSocketSet->addSocketToSet(m_listeningSocket);
	return true;
}

// ---------------------------------------------------------

CIncomingConnectionListener::~CIncomingConnectionListener()
{
	if( m_listeningSocketSet )
	{
		m_listeningSocketSet->delSocketFromSet(m_listeningSocket);
		SDLNet_TCP_Close(m_listeningSocket);
	}
}

// ---------------------------------------------------------

CNetLayerBasePtr CIncomingConnectionListener::checkIncomingConnections()
{
	assert(m_listeningSocketSet);
	assert(m_listeningSocket);

	m_listeningSocketSet->checkSocketsInput(); 
	if ( SDLNet_SocketReady(m_listeningSocket) )
		return acceptConnection();
	return CNetLayerBasePtr((CDataSocket*)NULL);
}

// ---------------------------------------------------------

CNetLayerBasePtr CIncomingConnectionListener::acceptConnection()
{
	assert(m_listeningSocket);
	assert(m_dataSocketSet);

	TCPsocket newSock = SDLNet_TCP_Accept(m_listeningSocket);
	if ( newSock == NULL )
		return CNetLayerBasePtr((CDataSocket*)NULL);

	if (m_dataSocketSet->addSocketToSet(newSock))
	   return CNetLayerBasePtr(new CDataSocket(newSock, m_dataSocketSet));
	else
	{
		SDLNet_TCP_Close(newSock);
		return CNetLayerBasePtr((CDataSocket*)NULL);
	}
};

// ---------------------------------------------------------
// ---------------------------------------------------------

CSocketSet::CSocketSet(ushort size):
m_socketSet(NULL),
m_errorFlag(false)
{
	m_socketSet = SDLNet_AllocSocketSet(size); 
	if( !m_socketSet )
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlCritical,_("Error allocating socket set.\n"));
		m_errorFlag = true;
	};
}

// ---------------------------------------------------------

CSocketSet::~CSocketSet()
{
	if( m_socketSet )
	{
		SDLNet_FreeSocketSet(m_socketSet);
	}
}

// ---------------------------------------------------------

bool CSocketSet::delSocketFromSet(TCPsocket sock)
{
	assert(m_socketSet);
	assert(sock);

	SDLNet_TCP_DelSocket(m_socketSet, sock);
	return true;
}

// ---------------------------------------------------------

bool CSocketSet::addSocketToSet(TCPsocket sock)
{
	assert(m_socketSet);
	assert(sock);

	if (SDLNet_TCP_AddSocket(m_socketSet, sock) == -1)
      return false;

	return true;
}

// ---------------------------------------------------------
// ---------------------------------------------------------

CDataSocket::CDataSocket(TCPsocket sock, CSockSetPtr sockSet):
m_socket(sock),
m_socketSet(sockSet),
m_lastPing(time(NULL)),
m_lastPong(time(NULL)),
m_wrtPtr(m_buf),
m_remotePort(0),
m_remoteHost(""),
m_sockErrorOccured(false)
{
	recalculateRemoteHost();
};

// ---------------------------------------------------------

CDataSocket::~CDataSocket()
{
	onClose();
	if( m_socket )
	{
		m_socketSet->delSocketFromSet(m_socket);
		SDLNet_TCP_Close(m_socket);
	}
}

// ---------------------------------------------------------

void CDataSocket::recalculateRemoteHost()
{
	if (m_socket)
	{
		IPaddress* addr = SDLNet_TCP_GetPeerAddress(m_socket);
		m_remoteHost = getIp(addr->host);
	}
}

// ---------------------------------------------------------

void CDataSocket::write(const CData& data)
{
	m_outputBuffer.push_back(data);
}

// ---------------------------------------------------------
/*
  Network packet format
  +-------------+-----------------------------------------------+
  | packet_size |                 packet_data                   |
  +-------------+-----------------------------------------------+
  0   sizeof(netPacketSize)           sizeof(netPacketSize) + packet_size
*/
typedef ushort	netPacketSize;

bool CDataSocket::flush()
{
	if (!m_socket)
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError, _("Attempt to write to not initialized data socket.\n"));
		return true;
	}
	
	if( time(NULL) - m_lastPong > LOST_CONNECTION_TIME )
	{
		m_lastPong = time(NULL);
		CLog::instance()->log(CLog::msgFlagNetwork,CLog::msgLvlInfo,_("(%s:%d) : Ping timeout.\n"),m_remoteHost.c_str(),m_remotePort);
		m_outputBuffer.clear();
		return false;
	}

	// if it's time to send ping insert dummy packet into output buffer
	if( !m_outputBuffer.size() && (time(NULL) - m_lastPing > PING_TIME) )
		m_outputBuffer.push_back(CData());

	m_lastPing = time(NULL);
	std::vector<CData>::iterator i;
	for(i = m_outputBuffer.begin(); i != m_outputBuffer.end(); i++ )
	{
		if( (*i).length() > SOCK_BUFFER_SIZE )
		{
			CLog::instance()->log(CLog::msgLvlError,_("Data too long (%d).\n"),(*i).length());
			continue;
		};
		netPacketSize dataLength = (netPacketSize)(*i).length();
		if ( SDLNet_TCP_Send(m_socket, &dataLength, sizeof(netPacketSize)) <= 0 )
		{
			CLog::instance()->log(CLog::msgFlagNetwork,CLog::msgLvlInfo,_("(%s:%d) : Error occured while writing socket.\n"), m_remoteHost.c_str(), m_remotePort);
			m_sockErrorOccured = true;
			m_outputBuffer.clear();
			return false;
		}

		if( dataLength )
			if( SDLNet_TCP_Send(m_socket, (char*)(*i).data(), dataLength ) <= 0 )
			{
				CLog::instance()->log(CLog::msgFlagNetwork,CLog::msgLvlInfo,_("(%s:%d) : Error occured while writing socket.\n"), m_remoteHost.c_str(), m_remotePort);
				m_sockErrorOccured = true;
				m_outputBuffer.clear();
				return false;
			}
	}
	m_outputBuffer.clear();
	return true;
}

// ---------------------------------------------------------

byte* CDataSocket::parseBuffer(byte *bufStartPtr, byte *bufEndPtr, std::vector<CData> &dataBuffer)
{
	byte* readPtr = bufStartPtr;
	dataBuffer.clear();
   uint maxBufferSize = uint((bufEndPtr - bufStartPtr)/sizeof(netPacketSize)/20+sizeof(netPacketSize));
   //TODO: write comment
   dataBuffer.reserve(maxBufferSize);
	while( bufEndPtr - readPtr >= sizeof(netPacketSize) )
	{
		if( bufEndPtr - (readPtr + sizeof(netPacketSize)) >= *((netPacketSize*)readPtr) ) // If it's enough data in buffer
		{
			if (*((netPacketSize*)readPtr)) // not ping (packet has data)
			{
				CData data((const char*)(readPtr + sizeof(netPacketSize)), *((netPacketSize*)readPtr), true);
				dataBuffer.push_back(data);
			}
			readPtr += sizeof(netPacketSize) + *((netPacketSize*)readPtr);
		} else 
			break;
	}
	if (dataBuffer.capacity()/dataBuffer.size() >= 2)
		std::vector<CData>(dataBuffer).swap(dataBuffer);
	return readPtr;
}

// ---------------------------------------------------------

bool CDataSocket::read(std::vector<CData> &dataBuffer)
{
	if (!m_socket)
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError, _("Attempt to read from not initialized data socket.\n"));
		return false;
	}

	if( !SDLNet_SocketReady(m_socket) )
		return true;

	m_lastPong = time(NULL);
	if( m_wrtPtr >= m_buf + sizeof(m_buf) )
	{
		CLog::instance()->log(CLog::msgFlagNetwork,CLog::msgLvlInfo,_("(%s:%d) : Data socket input buffer overflow.\n"), m_remoteHost.c_str(), m_remotePort);
		return false;
	}

	// res - number of received bytes
	int res = SDLNet_TCP_Recv(m_socket, m_wrtPtr, (int) (m_buf + sizeof(m_buf) - m_wrtPtr) );
	if( res <= 0 )
	{
		m_sockErrorOccured = true;
		CLog::instance()->log(CLog::msgFlagNetwork,CLog::msgLvlInfo,_("(%s:%d) : Error occured while reading from data soket.\n"), m_remoteHost.c_str(), m_remotePort);
		return false;
	}
	else
		m_wrtPtr += res; // Move write pointer to the end of data

	byte *newBufStartPtr = parseBuffer(m_buf, m_wrtPtr, dataBuffer);

	if( newBufStartPtr != m_buf )
	{
		if( m_wrtPtr > newBufStartPtr )
			memcpy(m_buf, newBufStartPtr, m_wrtPtr - newBufStartPtr);
		m_wrtPtr -= (newBufStartPtr - m_buf);
	};
	return true;
};

// ---------------------------------------------------------
