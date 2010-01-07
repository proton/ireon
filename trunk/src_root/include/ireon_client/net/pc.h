/**
* @file ireon_client/net/pc.h
* player client (pc) connection enumerations
*/

/* Copyright (C) 2005-2006 ireon.org developers council
* $Id: pcrs.h 726 2006-09-13 21:10:24Z zak $

*  See LICENSE for details
*/
#ifndef _PC_H
#define _PC_H
#include "common/net/connections/pcrs.h"
#include "common/net/connections/pcws.h"

namespace ireon
{ 
namespace net
{
namespace pc
{
namespace root
{
	//////////////////////////////////////////////////////////////////////////
	// id: pcReqisterRequest
	// route: pc -> pc
	// response to: internal
	// pc state: hello
	// rs state: 
	// description: 
	// requests reqister sequence with given login and password
	struct initateAccountRegistrationPacket : public PacketBase
	{
		static const ireon::net::commandId commandId = ireon::net::pcrs::initateAccountRegistration;
		enum string_ { login, password};
	};


	//////////////////////////////////////////////////////////////////////////
	// id: pcInitiateLogin
	// route: pc -> pc
	// response to: internal
	// pc state: hello
	// rs state: 
	// description: 
	// requests login sequence with given login and password
	struct initiateLoginPacket : public PacketBase
	{
		static const ireon::net::commandId commandId = ireon::net::pcrs::initiateLogin;
		enum string_ { login, password};
	};
	

	//////////////////////////////////////////////////////////////////////////
	// id: pcCreateCharRequest
	// route: pc -> rs
	// response to: internal
	// pc state:
	// rs state: main
	// description: 
	// request new character creation with given name
	struct initiateCreateCharPacket : public PacketBase
	{
		static const ireon::net::commandId commandId = ireon::net::pcrs::initiateCreateChar;
		enum string_ { name };
	};


	//////////////////////////////////////////////////////////////////////////
	// id: pcSelectCharRequest
	// route: pc -> pc
	// response to: internal
	// pc state: select_charater_state
	// rs state: 
	// description: 
	// request entrance into the game with selected character

	struct initiateSelectCharPacket : public PacketBase
	{
		static const ireon::net::commandId commandId = ireon::net::pcrs::initiateSelectChar;
		enum ushort_ { id };
	};

};

namespace world
{
	//////////////////////////////////////////////////////////////////////////
	// id: pcInitiateLogin
	// route: pc -> pc
	// response to: internal
	// from state: 
	// to state: ws hello_state
	// description:

	struct initiateLoginPacket : public PacketBase
	{
		static const ireon::net::commandId commandId = ireon::net::pcws::initiateLogin;
		enum ushort_ { id };
		enum string_ { token };
	};


};
}
}
}

#endif