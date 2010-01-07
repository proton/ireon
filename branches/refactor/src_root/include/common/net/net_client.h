/* Copyright (C) 2005 ireon.org developers council
 * $Id: net_client.h 416 2005-12-18 15:10:11Z zak $

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
 * @file net_client.h
 * Network client
 */

#ifndef _NET_CLIENT_H
#define _NET_CLIENT_H

#include "SDL_net.h"

struct SDL_Thread;

class CNetClient
{
	friend int connectionThreadFunc(void*);
public:

	CNetClient();
	CNetClient(TCPsocket sock,SDLNet_SocketSet socketSet);
	virtual ~CNetClient();

	bool connect(const char* host, ushort serverPort, SDLNet_SocketSet socketSet);
	virtual void connectionFailed(uint error) {}

	void disconnect();

	bool isConnected() const;

	///Reconnect
	virtual void reconnect();

	///When lost connection
	virtual void connectionLost();

/*	void initializeSecurity();

	void disableSecurity();*/

	///Receive and process all incoming packets
	virtual bool processInput();

	///Write data to output
//	void write(SendData& data);
//	void write(const char *data, uint length);
	void write(CData& data);

	///Process and send all outcoming packets
	virtual bool processOutput();

	///Process block of data
	virtual void processBlock(CData& data);
	///Process command
	virtual bool processCommand(CData& data, byte id) = 0;

	ushort getPort() {return m_port;}
	const char* getHost() {return m_host.c_str();}

	/// Function that is called when connection closes
	virtual void onClose() {} 


private:
	///Host and port where connected
	String m_host;
	ushort m_port;

	///Socket
	TCPsocket m_socket;
	///Socket set (for nonblocking reading)
	SDLNet_SocketSet m_socketSet;
	/** Socket set was created in this client, so it'll be
	 *  removed with it
	*/
	bool m_ownSocketSet;

	/// Thread for connection
	SDL_Thread *m_connThread;


	time_t m_lastPing; ///Last time when data was sended
	time_t m_lastPong; ///Last time when data was obtained

	byte m_buf[0xFFFF]; /// Read buffer
	byte* m_wrtPtr; /// Write pointer
	byte* m_readPtr; /// Read pointer
	uint m_waitFor; /// Number of bytes to read

	///Output buffer
	std::vector<CData> m_output;
};

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

