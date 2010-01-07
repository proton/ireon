/**
 * @file ireon_ws/net/wspc/main_state.cpp
 * world server player context main state
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: main_state.cpp 828 2006-11-29 18:34:44Z zak $

 * See LICENSE for details
 */

#include "ireon_ws/stdafx.h"
#include "ireon_ws/net/wspc/main_state.h"
#include "ireon_ws/net/wspc_connection.h"
#include "common/net/connections/pcws.h"
//#include "ireon_ws/world_app.h"
//#include "ireon_ws/world/ws_world.h"
#include "ireon_ws/db/player_characters_manager.h"
#include "ireon_ws/world/world_character.h"

WSPCMainState::WSPCMainState(CWSPCConnection *ownerConnection):
CGenericState(ownerConnection),
m_concreteOwnerConnection(ownerConnection)
{
	setStateName("WS <=> PC Connection Main State");
	registerSlot(ireon::net::pcws::login, static_cast<CGenericSlot> (&WSPCMainState::onLoginRequest));
	registerSlot(ireon::net::pcws::logout, static_cast<CGenericSlot> (&WSPCMainState::onLogoutRequest));
	registerSlot(ireon::net::pcws::getPlayerPos, static_cast<CGenericSlot> (&WSPCMainState::sendPlayerCoords));
	registerSlot(ireon::net::pcws::setHeading, static_cast<CGenericSlot> (&WSPCMainState::onSetCharacterHeading));
	registerSlot(ireon::net::pcws::setCurrentPos, static_cast<CGenericSlot> (&WSPCMainState::onCharacterPositionUpdate));
	registerSlot(ireon::net::pcws::stopMovement, static_cast<CGenericSlot> (&WSPCMainState::onStopCharacterMovement));

	setSignalsProcessingMode(CGenericState::denyWithExceptions);
	addProcessingException(ireon::net::pcws::login);
}

// ---------------------------------------------------------------------

void WSPCMainState::onLoginRequest(String& packetData)
{
	_log (_debug, _loc, _("Got login request from %s."), conn().getConnectionInfo() );
	
	GENERATE_PACKET_TYPE(ireon::net::pcws::loginResponsePacket) resp;
	try
	{
		GENERATE_PACKET_TYPE(ireon::net::pcws::loginPacket) data;
		data.unserialize(packetData);

		ushort charId = data.get(ireon::net::pcws::loginPacket::charId);
		String token( data.get(ireon::net::pcws::loginPacket::token) );

		conn().setCharacter( PlayerCharactersManager::instance()->characterLogin(charId, token) );
		conn().setId(charId);
		resp.set(ireon::net::pcws::loginResponsePacket::returnCode, ireon::net::pcws::loginResponsePacket::ok); 
		send(resp.getCommandId(), resp.serialize());
		setSignalsProcessingMode(CGenericState::allowWithExceptions);

		String tmp;
		sendPlayerCoords(tmp);

		_log (_info, _loc, _("Character %d successfully logged in."), charId );
	}
	catch (PlayerCharactersManager::EIncorrectLoginInfo&)
	{
		resp.set(ireon::net::pcws::loginResponsePacket::returnCode, ireon::net::pcws::loginResponsePacket::badLogin); 
		_log (_error, _loc, _("Bad login from client.") );
	}
	catch (PlayerCharactersManager::EBadCharacterName&)
	{
		resp.set(ireon::net::pcws::loginResponsePacket::returnCode, ireon::net::pcws::loginResponsePacket::badCharacterName); 
		_log (_error, _loc, _("Bad character name supplied for client by root server.") );
	}
	catch (EWSError&)
	{
		resp.set(ireon::net::pcws::loginResponsePacket::returnCode, ireon::net::pcws::loginResponsePacket::internalError); 
		_log (_error, _loc, _("Internal error happened while inserting character into the world.") );
	}
}

// ---------------------------------------------------------------------

void WSPCMainState::sendPlayerCoords( String& )
{

}

// ---------------------------------------------------------------------

// actually there is no need in calling this signal
// it will be called automatically on client disconnection
void WSPCMainState::onLogoutRequest(String&)
{
	_log (_debug, _loc, _("Got logout request for char %d from %s."), conn().getId(), conn().getConnectionInfo() );

	if (conn().getCharacter().get())
	{
		PlayerCharactersManager::instance()->characterLogout(conn().getCharacter());
		conn().getCharacter().reset();
	}
}

// ---------------------------------------------------------------------

void WSPCMainState::onSetCharacterHeading(String& packetData)
{
	_log (_debug, _loc, _("New heading request from %s."), conn().getConnectionInfo() );

	GENERATE_PACKET_TYPE(ireon::net::pcws::setHeadingPacket) req;
	req.unserialize(packetData);
	conn().getCharacter()->setHeading(req.get(ireon::net::pcws::setHeadingPacket::heading));
}

// ---------------------------------------------------------------------

void WSPCMainState::onCharacterPositionUpdate(String& packetData)
{
	_log (_debug, _loc, _("Got update position request from %s."), conn().getConnectionInfo() );

	GENERATE_PACKET_TYPE(ireon::net::pcws::newPositionPacket) req;
	req.unserialize(packetData);

	conn().getCharacter()->updatePosition(req.get(ireon::net::pcws::newPositionPacket::pos),req.get(ireon::net::pcws::newPositionPacket::triangleId));
}

// ---------------------------------------------------------------------

void WSPCMainState::onStopCharacterMovement(String& packetData)
{
	_log (_debug, _loc, _("Got stop movement request from %s."), conn().getConnectionInfo() );

	GENERATE_PACKET_TYPE(ireon::net::pcws::newPositionPacket) req;
	req.unserialize(packetData);

	conn().getCharacter()->stopMovement(req.get(ireon::net::pcws::newPositionPacket::pos),req.get(ireon::net::pcws::newPositionPacket::triangleId));
}

// ---------------------------------------------------------------------
