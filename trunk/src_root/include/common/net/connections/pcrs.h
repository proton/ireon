/**
 * @file common/net/connections/pcrs.h
 * player client (pc) <=> root server (rs) connection enumerations
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: pcrs.h 778 2006-10-05 20:15:59Z zak $

 *  See LICENSE for details
 */

#ifndef _PCRS_H
#define _PCRS_H

#include "common/net/net_types.h"

namespace ireon
{ 
namespace net
{
namespace pcrs
{

	/// list of all network signals
	enum PCRSSignals
	{
		versionCheckRequest = ireon::net::commonPacketIdLast,
		initiateVersionCheck,				//< internal client signal
		versionCheckResponse,
		accountRegistrationRequest,
		initateAccountRegistration,			//< internal client signal
		accountRegistrationResponse,
		initiateLogin,						//< internal client signal
		loginRequest,
		loginResponse,
		initiateCreateChar,					//< internal client signal
		createCharRequest,
		createCharResponse,
		initiateListChar,					//< internal client signal
		listCharsRequest,
		listCharsResponse,
		initiateSelectChar,					//< internal client signal
		selectCharRequest,
		selectCharAck,
		rsCharCheckResponse,				//< internal rs	signal
		selectCharResponse,
	};

	// here follow packet definitions for all signals from PCRSSignals enum

	//////////////////////////////////////////////////////////////////////////
	// route: pc -> rs
	// response to: internal
	// pc state:
	// rs state: verion check
	// description: 
	// first signal in connection sequence, client reports its version numbers 
	// for rs to check
	struct versionCheckRequestPacket : public PacketBase
	{
		static const ireon::net::commandId commandId = versionCheckRequest;

		enum byte_ { majorVersion, minorVersion };
		enum ushort_ { releaseNumber, buildNumber };
	};

	//////////////////////////////////////////////////////////////////////////
	// route: rs -> pc
	// response to: pcrsVersionCheckRequest
	// pc state:
	// rs state: verion check
	// description:
	// sent in response to version check request from client, 
	// if vcrespCheckPassed == true then client is allowed to proceed to 
	// login/register sequence, also sends (in any case) minimal version numbers
	// required by root server
	struct versionCheckResponsePacket : public PacketBase
	{
		static const ireon::net::commandId commandId = versionCheckResponse;

		enum bool_ { checkPassed };
		enum byte_ { majorVersion, minorVersion };
		enum ushort_ { releaseNumber, buildNumber };
	};

	//////////////////////////////////////////////////////////////////////////
	// route: pc -> rs
	// response to: internal
	// pc state:
	// rs state: hello
	// description: 
	// requests creation of new account with given login and password
	struct accountRegistrationRequestPacket : public PacketBase
	{
		static const ireon::net::commandId commandId = accountRegistrationRequest;

		enum string_ { login, password };
	};

	//////////////////////////////////////////////////////////////////////////
	// route: rs -> pc
	// response to: pcrsAccountRegistrationRequest
	// pc state:
	// rs state: hello
	// description: 
	// contains return code for account registration, if registration is ok, 
	// user automatically logged in
	struct accountRegistrationResponsePacket : public PacketBase
	{
		static const ireon::net::commandId commandId = accountRegistrationResponse;

		enum byte_ { responseBitfield };

		enum codes
		{
			ok = 1,
			failed = 2,
			incorrectCredentials = 4,	//< login or password is not well formed
			duplicateLogin = 8,			//< user with same login already exists
			unknownError = 16			//< db error or something as severe as this
		};
	};

	//////////////////////////////////////////////////////////////////////////
	// route: pc -> rs
	// response to: internal
	// pc state:
	// rs state: hello
	// description: 
	// requests login with given login and password
	struct loginRequestPacket : public PacketBase
	{
		static const ireon::net::commandId commandId = loginRequest;

		enum string_ { login, password };
	};

	//////////////////////////////////////////////////////////////////////////
	// route: rs -> pc
	// response to: pcrsLoginRequest
	// pc state:
	// rs state: hello
	// description: 
	// contains return code for login request
	struct loginResponsePacket : public PacketBase
	{
		static const ireon::net::commandId commandId = loginResponse;

		enum byte_ { responseBitfield };

		enum codes
		{
			ok = 1,
			failed = 2,
			incorrectCredentials = 4,	//< no such account or password is incorrect
			doubleConnect = 8,			//< this client is already connected to root server
			unknownError = 16			//< db error or something as severe as this
		};
	};

	//////////////////////////////////////////////////////////////////////////
	// route: pc -> rs
	// response to: internal
	// pc state:
	// rs state: main
	// description: 
	// request new character creation with given name
	struct createCharRequestPacket : public PacketBase
	{
		static const ireon::net::commandId commandId = createCharRequest;

		enum string_ { name };
	};

	//////////////////////////////////////////////////////////////////////////
	// route: rs -> pc
	// response to: pcrsCreateCharRequest
	// pc state:
	// rs state: main
	// description: 
	// contains return code for character creation request
	struct createCharResponsePacket : public PacketBase
	{
		static const ireon::net::commandId commandId = createCharResponse;

		enum byte_ { responseBitfield };

		enum codes
		{
			ok = 1,
			failed = 2,
			incorrectCredentials = 4,	//< char name is not well formed
			duplicateName = 8,			//< char with this name already exists
			tooManyCharacters = 16,		//< exceeded number of characters allowed for this account
			unknownError = 32			//< db error or something as severe as this
		};
	};

	//////////////////////////////////////////////////////////////////////////
	// id: listCharsRequest
	// route: pc -> rs
	// response to: internal
	// pc state:
	// rs state: main
	// description: 
	// request character list for current account from root server. No data packet is needed.

	struct listCharsRequestPacket : public PacketBase
	{
		static const ireon::net::commandId commandId = listCharsRequest;
	};

	//////////////////////////////////////////////////////////////////////////
	// route: rs -> pc
	// response to: pcrsListCharsRequest
	// pc state:
	// rs state: hello
	// description: 
	// series of packets, 1 packet per character or 1 packet if no characters found
	struct listCharsResponsePacket : public PacketBase
	{
		static const ireon::net::commandId commandId = listCharsResponse;

		enum ushort_ { id };
		enum string_ { name };
		enum bool_ { new_ };	//< true if char was never played
	};

	//////////////////////////////////////////////////////////////////////////
	// route: pc -> rs
	// response to: internal
	// pc state:
	// rs state: main
	// description: 
	// request entrance into the game with selected character

	struct selectCharRequestPacket : public PacketBase
	{
		static const ireon::net::commandId commandId = selectCharRequest;

		enum ushort_ { id };
	};

	//////////////////////////////////////////////////////////////////////////
	// route: rs -> pc
	// response to: pcrsSelectCharRequest
	// pc state:
	// rs state: main
	// description: 
	// contains ack code for character select request
	
	struct selectCharAckPacket : public PacketBase
	{
		static const ireon::net::commandId commandId = selectCharAck;

		enum byte_ { responseBitfield };

		enum codes
		{
			ok = 1,
			failed = 2,
			charNotFound = 4,
			unknownError = 8			//< db error or something as severe as this
		};
	};

	//////////////////////////////////////////////////////////////////////////
	// route: rs ws context -> rs client context
	// response to: internal
	// from state: CRSWSMainState
	// to state: CRSPCMainState
	// description: 
	// contains response for character check 
	struct charCheckResponsePacket : public PacketBase
	{
		static const ireon::net::commandId commandId = rsCharCheckResponse;

		enum ushort_ { charId };
		enum bool_ { isInGame };
	};

	//////////////////////////////////////////////////////////////////////////
	// route: rs -> pc
	// response to: pcrsCharCheckResponsePacket
	// pc state:
	// rs state: main
	// description: 
	// ack or deny packet for game entrance
	struct selectCharResponsePacket : public PacketBase
	{
		static const ireon::net::commandId commandId = selectCharResponse;

		enum byte_ { responseCode };
		enum ushort_ { charInGame }; //< (optional) char with this id is already in game (if charInGame in responseCode)
		enum string_ { wsAddress, token }; //< (optional) ws address and auth string for ws connection (if ok in responseCode)

		enum codes
		{
			ok = 1,
			charIsInGame = 2,		//< one of player's characters is already in game
			WSUnavailable = 3,	//< world server is not available
		};

	};
};
};
};

#endif
