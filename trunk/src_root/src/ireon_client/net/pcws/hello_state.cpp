/**
* @file ireon_client/net/pcws/hello_state.cpp
* client ws connection welcome (hello) state
*/

/* Copyright (C) 2005-2006 ireon.org developers council
* $Id$

* See LICENSE for details
*/
#include "ireon_client/stdafx.h"
#include "ireon_client/net/pcws/hello_state.h"
#include "ireon_client/net/pc.h"
#include "common/net/connections/pcws.h"
#include "ireon_client/interface/client_event_manager.h"




CPCWSHelloState::CPCWSHelloState(CNetConnection *ownerConnection):
CGenericState(ownerConnection)
{
	setStateName("PC <=> WS Hello State");

	registerSlot(ireon::net::pcws::initiateLogin, static_cast<CGenericSlot> (&CPCWSHelloState::onInitiateLoginSignal), ireon::net::netAddress::atInternalAddress);
	registerSlot(ireon::net::pcws::loginResponse, static_cast<CGenericSlot> (&CPCWSHelloState::onLoginResponse), ireon::net::netAddress::atExternalAddress);

	setSignalsProcessingMode(CGenericState::denyWithExceptions);
	addProcessingException(ireon::net::pcws::initiateLogin);
}


// ---------------------------------------------------------------------

void CPCWSHelloState::onInitiateLoginSignal (String &packetData)
{
	ushort charId;
	String authToken;
	GENERATE_PACKET_TYPE(ireon::net::pc::world::initiateLoginPacket) data;

	try
	{
		data.unserialize(packetData);
		charId = data.get(ireon::net::pc::world::initiateLoginPacket::id);
		authToken = data.get(ireon::net::pc::world::initiateLoginPacket::token);
	}
	catch (ENetDataException&)
	{
		_log(_error, _loc, _("Caught ENetDataException"));
		return;
	}
	_log(_debug, _loc, _("Send login request for char with id = '%u'."), charId);


	GENERATE_PACKET_TYPE(ireon::net::pcws::loginPacket) req;

	req.set(ireon::net::pcws::loginPacket::charId, charId);
	req.set(ireon::net::pcws::loginPacket::token, authToken);
	
	send(req.getCommandId(), req.serialize());
	
	clearProcessingExceptions();
	addProcessingException(ireon::net::pcws::loginResponse);
}

	
// ---------------------------------------------------------------------

void CPCWSHelloState::onLoginResponse (String &packetData)
{
	_log(_debug, _loc, _("got login response."));
	byte resCode = 0;
	try
	{
		GENERATE_PACKET_TYPE(ireon::net::pcws::loginResponsePacket) data;

		data.unserialize(packetData);

		resCode = data.get(ireon::net::pcws::loginResponsePacket::returnCode);
	}
	catch (ENetDataException&)
	{
		_log(_error,_loc, _("caught ENetDataException."));
		// TODO send something to event manager?
		return;
	}

	if ( resCode  == ireon::net::pcws::loginResponsePacket::ok )
	{
		_log(_debug, _loc,_("successfully logged id."));
		// TODO send something to event manager?
		conn().setNextState(CPCWSConnection::main);

		CClientApp::instance()->setState(CClientApp::AS_PLAY);
		return;
	}

	if ( resCode  == ireon::net::pcws::loginResponsePacket::badLogin )
		_log(_debug, _loc,_("token does not match, login timed out or could not character with requested id"));

	if ( resCode  == ireon::net::pcws::loginResponsePacket::badCharacterName )
		_log(_debug, _loc,_("character name (supplied by rs) is not well-formed"));

	// TODO send something to event manager?
	CClientApp::instance()->setState(CClientApp::AS_MENU);
};
