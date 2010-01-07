/**
 * @file common/net/connections/wsrs.h
 * world server (ws) <=> root server (rs) connection enumerations
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: wsrs.h 806 2006-11-08 14:02:44Z zak $

 *  See LICENSE for details
 */

#pragma once

#include "common/net/net_types.h"

namespace ireon
{
namespace net_wsrs
{
	enum RSWSSignals
	{
		wsInitiateLogin = ireon::net::commonPacketIdLast, //< internal
		login,
		loginResponse,
		rsCheckCharInGame,				//< internal
		checkCharInGameRequest,
		checkCharInGameResponse,
		rsCharSelected,					//< internal
		charSelectedInfo,
	};

	// here follow packet definitions for all signals from PCRSSignals enum

	//////////////////////////////////////////////////////////////////////////
	// route: ws -> rs
	// response to: internal
	// from state: WSRSMainState
	// to state: CRSWSMainState
	// description:
	struct loginPacket : public PacketBase 
	{
		static const ireon::net::commandId commandId = login;

		enum byte_ { WorldServerID };
	};

	//////////////////////////////////////////////////////////////////////////
	// route: rs -> ws
	// response to: internal
	// from state: CRSWSMainState
	// to state: WSRSMainState
	// description:
	struct loginResponsePacket : public PacketBase 
	{
		static const ireon::net::commandId commandId = loginResponse;

		enum bool_ { loginSuccessfull };
	};

	//////////////////////////////////////////////////////////////////////////
	// route: rs (player connection) -> rs (world server connection)
	// response to: internal
	// from state: CRSPCMainState
	// to state: CRSWSMainState
	// description:
	// check if character with certain id is in the game
	struct checkCharInGamePacket : public PacketBase 
	{
		static const ireon::net::commandId commandId = rsCheckCharInGame;

		enum ushort_ { charIdToCheck };
		enum uint_ 
		{
			callerId		//< caller connection id for call back when response arrives
		};
	};

	//////////////////////////////////////////////////////////////////////////
	// route: rs -> ws
	// response to: rswsCheckCharInGame
	// from state: CRSPCMainState
	// to state:
	// description:
	// check if character with certain id is in the game
	struct checkCharInGameRequestPacket : public PacketBase 
	{
		static const ireon::net::commandId commandId = checkCharInGameRequest;

		enum ushort_ { charIdToCheck };
	};

	//////////////////////////////////////////////////////////////////////////
	// route: ws -> pc
	// response to: 
	// from state:
	// to state: main
	// description: 
	// contains char in game check result
	struct checkCharInGameResponsePacket : public PacketBase 
	{
		static const ireon::net::commandId commandId = checkCharInGameResponse;

		enum ushort_ { charId };
		enum bool_ { isInGame };
	};

	//////////////////////////////////////////////////////////////////////////
	// id: rswsCharSelected
	// route: rs (player connection) -> rs (world server connection)
	// response to: internal
	// from state: CRSPCMainState
	// to state: CRSWSMainState
	// description:
	// contains login information for character
	struct charSelectedPacket : public PacketBase 
	{
		static const ireon::net::commandId commandId = rsCharSelected;

		enum ushort_ { charId };
		enum bool_ { createNewCharacter };
		enum string_
		{
			characterName,		//< optional, set if createNewCharacter == true
			token				//< disposable password
		};
	};

	//////////////////////////////////////////////////////////////////////////
	// route: rs -> ws
	// response to: rswsCharSelected
	// from state: CRSWSMainState
	// to state:
	// description:
	// contains login information for character
	struct charSelectedInfoPacket : public PacketBase 
	{
		static const ireon::net::commandId commandId = charSelectedInfo;

		enum ushort_ { charId };
		enum bool_ { createNewCharacter };
		enum string_
		{
			characterName,		//< optional, set if createNewCharacter == true
			token				//< disposable password
		};
	};

};
};

