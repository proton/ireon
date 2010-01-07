/**
 * @file common/net/connections/pc.h
 * world server (ws) <=> player client (pc) connection enumerations
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: pcws.h 828 2006-11-29 18:34:44Z zak $

 *  See LICENSE for details
 */

#pragma once

#include "common/net/net_types.h"

namespace ireon
{
namespace net
{
namespace pcws
{
	enum PCWSSignals
	{
		login  = ireon::net::commonPacketIdLast,
		initiateLogin,		//< internal \see initiateLoginPacket
		loginResponse,
		getPlayerPos,		//< no packet
		logout,				//< no packet
		setCurrentPos,		//< \see newPositionPacket
		stopMovement,		//< \see newPositionPacket
		setHeading,			//< \see setHeadingPacket
	};

	// here follow packet definitions for all signals from PCRSSignals enum

	//////////////////////////////////////////////////////////////////////////
	// route: pc -> ws
	// response to: internal
	// from state:
	// to state: WSPCMainState
	// description:
	struct loginPacket : public PacketBase 
	{
		static const ireon::net::commandId commandId = login;

		enum ushort_ { charId };
		enum string_ { token }; //< client should receive token this from rs
	};

	//////////////////////////////////////////////////////////////////////////
	// route: ws -> pc
	// response to: pcwsLogin
	// from state: WSPCMainState
	// to state: 
	// description:
	struct loginResponsePacket : public PacketBase 
	{
		static const ireon::net::commandId commandId = loginResponse;

		enum byte_ { returnCode }; //< \see returnCodes

		enum returnCodes
		{
			ok,				
			badLogin,			//< token does not match, login timed out or could not character with requested id
			badCharacterName,	//< character name (supplied by rs) is not well-formed
			internalError,		//< something nasty has happened, call some coder asap
		};
	};

	//////////////////////////////////////////////////////////////////////////
	// route: pc <-> ws 
	// response to: internal
	// from state: 
	// to state: WSPCMainState
	// description: holds data for up-to-date character position
	// this packet is used for various movement-related tasks: 
	//   periodic character position synchronization (should be sent regularly while moving)
	//   movement endpoint coordinates (when stopping)
	//   forced character move by server (when server finds error in client's data)
	struct newPositionPacket : public PacketBase 
	{
		static const ireon::net::commandId commandId = 0; // multiple commands use this packet

		enum uint_ { triangleId };	//< current character position in navigation mesh
		enum vector2_ { pos };		//< current character position in xOy plane
	};

	//////////////////////////////////////////////////////////////////////////
	// route: pc -> ws
	// response to: internal
	// from state: 
	// to state: WSPCMainState
	// description: sets new character heading
	struct setHeadingPacket : public PacketBase 
	{
		static const ireon::net::commandId commandId = setHeading;

		enum vector2_ { heading }; //< new character heading
	};
};
};
};

