/**
 * @file ireon_client/net/pcws_connection.h
 * client connection to root server
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: pcrs.h 698 2006-09-03 16:46:33Z zak $

 *  See LICENSE for details
 */


#ifndef _PC_RS_CONNECTION_H
#define _PC_RS_CONNECTION_H

#include "common/net/net_connection.h"

class CPCRSConnection : public CNetConnection
{
public:
	enum PCRSStates
	{
		versionCheck,
		hello,
		characterSelection
	};

	CPCRSConnection();

	void onClose(){}
public: 
	///Network commands

/*	///Login, 0
	void login(const String& login, const String& password);
	void getCharsList();

	///Select char, 1
	void selectChar(uint id);

	///Create new character, 2
	void createChar(ClientCharRegistrationData& ch);

	///Remove character, 3
	void removeChar(uint id);

public:
	/// World server commands
	
	/// 0 = login

	/// 1, Send own coordinates
	void sendCoords(Vector2 coords, Degree rotation, uint time);

	/// 2, Send chat message
	void sendChatCmd(ChatCommand cmd, const String& arg1, const String& arg2, const String& arg3);

	/// 3, Send fight command
	void sendFightCmd(FightCommand cmd, uint enemy );

	/// 4, Resurrect
	void sendResurrect();
public:
	/// World signal handlers

	/// 0, Login responce from world server
	void onWorldLogin(CData& data);
	void getInitialInfo(CData& data);

	/// 1, World server sends coordinates
	void onSendCoords(CData& data);

	/// 2, World server sends chat message
	void onChatCmd(CData& data);

	/// 3, Fight command
	void onFightCmd(CData& data);

	/// 4, Resurrect
	void onResurrect(CData& data);

private:
	void onRootRegistrationOk(CData& data);
	void onRootRegistrationFailed(CData& data);

	void onRootLoginOk(CData& data);
	void onRootLoginFailed(CData& data);

	void onRootCharList(CData& data);

	void onCreateChar(CData& data);
	void onCharCreationFailed(CData& data);

	void onRemoveChar(CData& data);
	void onSendWorld(CData& data);


	void loginToRoot(const String& login, const String& password);
	void registerToRoot(const String& login, const String& password);
	void loginSomewhere(const String& login, const String& password);

	void getCharsList();
	void selectChar(uint id);
	void createChar(ClientCharRegistrationData& ch);
	void removeChar(uint id);
*/

};

#endif
