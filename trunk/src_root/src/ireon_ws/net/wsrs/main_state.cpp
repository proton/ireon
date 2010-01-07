/**
 * @file ireon_ws/net/wsrs/main_state.cpp
 * world server root server context main state
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: main_state.cpp 806 2006-11-08 14:02:44Z zak $

 * See LICENSE for details
 */

#include "ireon_ws/stdafx.h"
#include "ireon_ws/net/wsrs/main_state.h"
#include "ireon_ws/net/wsrs_connection.h"
#include "common/net/connections/wsrs.h"
#include "ireon_ws/world_app.h"
#include "ireon_ws/xsd/config_xsd.h"
//#include "ireon_ws/world/ws_world.h"
#include "ireon_ws/db/player_characters_manager.h"


WSRSMainState::WSRSMainState(CWSRSConnection *ownerConnection):
CGenericState(ownerConnection)
{
	setStateName("WS <=> RS Connection Main State");
	registerSlot(ireon::net_wsrs::wsInitiateLogin, static_cast<CGenericSlot> (&WSRSMainState::onInitiateLoginRequest), ireon::net::netAddress::atInternalAddress);
	registerSlot(ireon::net_wsrs::loginResponse, static_cast<CGenericSlot> (&WSRSMainState::onLoginResponse));
	registerSlot(ireon::net_wsrs::checkCharInGameRequest, static_cast<CGenericSlot> (&WSRSMainState::onCheckCharRequest));
	registerSlot(ireon::net_wsrs::charSelectedInfo, static_cast<CGenericSlot> (&WSRSMainState::onCharSelected));

	setSignalsProcessingMode(CGenericState::denyWithExceptions);
	addProcessingException(ireon::net_wsrs::wsInitiateLogin);
}

// ---------------------------------------------------------------------

void WSRSMainState::onInitiateLoginRequest(String&)
{
	_log(_debug, _loc, _("Sending login request to root server.") );
	GENERATE_PACKET_TYPE(ireon::net_wsrs::loginPacket) req;
	req.set(ireon::net_wsrs::loginPacket::WorldServerID, WSApp::instance()->getConfig()->wsID());

	send(req.getCommandId(), req.serialize());

	clearProcessingExceptions();
	setSignalsProcessingMode(CGenericState::denyWithExceptions);
	addProcessingException(ireon::net_wsrs::loginResponse);
}

// ---------------------------------------------------------------------

void WSRSMainState::onLoginResponse(String& packetData)
{
	_log(_debug, _loc, _("Got login response from root server.") );
	GENERATE_PACKET_TYPE(ireon::net_wsrs::loginResponsePacket) data;
	data.unserialize(packetData);

	if ( data.get(ireon::net_wsrs::loginResponsePacket::loginSuccessfull) )
	{
		clearProcessingExceptions();
		setSignalsProcessingMode(CGenericState::allowWithExceptions);
		addProcessingException(ireon::net_wsrs::loginResponse);
		addProcessingException(ireon::net_wsrs::wsInitiateLogin);
		_log(_info, _loc, _("Logged in to root server.") );
	} else {
		clearProcessingExceptions();
		setSignalsProcessingMode(CGenericState::denyWithExceptions);
		addProcessingException(ireon::net_wsrs::wsInitiateLogin);
		_log(_error, _loc, _("Got login response from rs: login failed.") );
		// TODO try to reconnect?
	}
}

// ---------------------------------------------------------------------

void WSRSMainState::onCheckCharRequest(String& packetData)
{
	_log(_debug, _loc, _("Got check char request from root server.") );

	ushort charId;
	GENERATE_PACKET_TYPE(ireon::net_wsrs::checkCharInGameRequestPacket) data;
	data.unserialize(packetData);
	charId = data.get(ireon::net_wsrs::checkCharInGameRequestPacket::charIdToCheck);

	GENERATE_PACKET_TYPE(ireon::net_wsrs::checkCharInGameResponsePacket) resp;
	resp.set(ireon::net_wsrs::checkCharInGameResponsePacket::charId, charId); 
	resp.set(ireon::net_wsrs::checkCharInGameResponsePacket::isInGame, PlayerCharactersManager::instance()->isCharInGame(charId)); 
	send(resp.getCommandId(), resp.serialize());
}

// ---------------------------------------------------------------------

void WSRSMainState::onCharSelected(String& packetData)
{
	_log(_debug, _loc, _("Got char login information from rs.") );

	ushort charId = 0;
	try
	{
		GENERATE_PACKET_TYPE(ireon::net_wsrs::charSelectedInfoPacket) data;
		data.unserialize(packetData);

		charId = data.get(ireon::net_wsrs::charSelectedInfoPacket::charId); 
		String token = data.get(ireon::net_wsrs::charSelectedInfoPacket::token); 
		bool createNew = data.get(ireon::net_wsrs::charSelectedInfoPacket::createNewCharacter); 
		String charName;
		if (createNew)
			charName = data.get(ireon::net_wsrs::charSelectedInfoPacket::characterName); 

		_log(_debug, _loc, _("Registering char %d login information."), charId, createNew );

		PlayerCharactersManager::instance()->registerNewLogin(charId, token, createNew, charName);
	}
	catch (PlayerCharactersManager::ECharacterIsInGame&)
	{
		_log(_error, _loc, _("Could not register temporary login: char %d is already in game."), charId );
		// TODO send something to rs?
	}
}
