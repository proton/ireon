/**
 * @file common/net/layers/sock_layer.h
 * Socket wrappers
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



#ifndef _SOCK_LAYER_H
#define _SOCK_LAYER_H

#define SOCK_BUFFER_SIZE 0xFFFF

#include "net/layers/net_layer_base.h"
#include "SDL_net.h"

struct connectionThreadData;
class CDataSocket;
class CSocketSet;
//typedef boost::shared_ptr<CDataSocket>            CNetLayerBasePtr;
typedef boost::shared_ptr<CSocketSet>             CSockSetPtr;
typedef boost::shared_ptr<connectionThreadData>   conThreadDataPtr;

class CSocketSet
{
public:
	CSocketSet(ushort size);
	~CSocketSet();
	void checkSocketsInput() {
//		if(
			SDLNet_CheckSockets(m_socketSet, 0)
//		== -1)
//			CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError, _("SDLNet_CheckSockets: %s\n"), SDLNet_GetError());
	;}			///< call this in every loop cycle
	bool addSocketToSet(TCPsocket sock);
	bool delSocketFromSet(TCPsocket sock);
	bool errorHasOccured() {return m_errorFlag;}

private:
	SDLNet_SocketSet	m_socketSet;
	bool				   m_errorFlag;
};

// -----------------------------------------------------

class CDataSocket: public CNetLayerBase
{
	friend int connectionThreadFunc(void*);
public:
	CDataSocket(TCPsocket sock, CSockSetPtr sockSet);
	~CDataSocket();

	///Receive and process all incoming packets
	bool read(std::vector<CData> &dataBuffer);
	///Write data to output buffer
	void write(const CData& data);
	///Process and send all outcoming packets
	bool flush();

	virtual String getInfo() {return m_remoteHost;}

	ushort getPort() {return m_remotePort;}
	String getHost() {return m_remoteHost;}
	bool isConnected() {return m_socket != NULL;}

private:
	/// called automatically from destructor
	void			onClose() {}

	/// fill m_remoteHost from m_socket info
	void			recalculateRemoteHost();

	/// get packets from raw buffer and put em into dataBuffer
	byte*         parseBuffer(byte *bufStartPtr, byte *bufEndPtr, std::vector<CData> &dataBuffer);

	TCPsocket	  m_socket;
	CSockSetPtr	  m_socketSet;

	bool			m_sockErrorOccured;		///< something has happened with socket (read/write/connection error)

	String        m_remoteHost;
	ushort        m_remotePort;

	time_t        m_lastPing; ///< Last time when data was sent
	time_t        m_lastPong; ///< Last time when data was obtained

	byte	        m_buf[SOCK_BUFFER_SIZE];	///< Read buffer
	byte*	        m_wrtPtr;					///< Buffer write pointer

	std::vector<CData> m_outputBuffer;	///< Output buffer
};

// -----------------------------------------------------

class CIncomingConnectionListener
{
public:
	CIncomingConnectionListener();
	~CIncomingConnectionListener();
	bool start(ushort localPort, ushort maxConnections);

	/// User should delete CDataSocket himself!
	CNetLayerBasePtr  checkIncomingConnections();

	void checkSocketsInput() {if (m_dataSocketSet) m_dataSocketSet->checkSocketsInput();}

private:
	CNetLayerBasePtr  acceptConnection();

	TCPsocket	  m_listeningSocket;
	CSockSetPtr   m_dataSocketSet;
	CSockSetPtr   m_listeningSocketSet;
};

// -----------------------------------------------------

struct SDL_Thread;
struct connectionThreadData
{
	CDataSocket* 		dataSockPtr;
	CSocketSet*			sockSet;
	const char*			remoteHost; ///< for CDataSocket m_remoteHost filed only
	ushort				remotePort; ///< for CDataSocket m_remotePort filed only
};

class CDataSocketManager
{
public:
	CDataSocketManager(const char* remoteHost, const ushort remotePort);
	~CDataSocketManager();
	CNetLayerBasePtr connect();
	/// Data socket should be deleted by user if already initialized!
	// bool reconnect() {cleanup(); return connect();}
	void checkSocketsInput() {if (m_socketSet) m_socketSet->checkSocketsInput();}

private:
	void           cleanup();

	String		      m_targetHost;
	ushort		      m_targetPort;


	CSockSetPtr				m_socketSet;

	SDL_Thread*				m_connThread;
//	conThreadDataPtr		m_threadData;
//	int						m_waitForThread;
	connectionThreadData*	m_connThreadData;
};

// -----------------------------------------------------

inline const char* getIp(uint binAddr)
{
	in_addr a;
	#ifdef __WIN32__
	a.S_un.S_addr = binAddr;
	#elif defined(__UNIX__)
	a.s_addr = binAddr;
	#endif
	return inet_ntoa(a);
};

inline byte degreeToByte(const Real& degree)
{
	return static_cast<byte>( ((degree > 360.0f/255.0f || degree < -360.0f/255.0f) ? degree : 360.0f ) * 255.0f / 360.0f);
};

inline Real byteToDegree(byte value)
{
	return ( (value > 127) ? (static_cast<Real>(value) - 256.0f) : (static_cast<Real>(value)) ) * 360.0f  / 255.0f;
};


#endif

