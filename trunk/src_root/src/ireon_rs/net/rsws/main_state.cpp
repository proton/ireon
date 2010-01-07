/**
 * @file ireon_rs/net/rsws/rsws_main_state.cpp
 * root server world server context main state
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: main_state.cpp 781 2006-10-07 19:52:08Z zak $

 * See LICENSE for details
 */

#include "ireon_rs/stdafx.h"
#include "ireon_rs/net/rsws/main_state.h"
#include "ireon_rs/net/rsws_connection.h"
#include "common/net/connections/wsrs.h"
#include "common/net/connections/pcrs.h"
#include "ireon_rs/root_app.h"

CRSWSMainState::CRSWSMainState(CRSWSConnection *ownerConnection):
CGenericState(ownerConnection)
{
	setStateName("RS <=> WS Connection Main State");
	registerSlot(ireon::net_wsrs::login, static_cast<CGenericSlot> (&CRSWSMainState::onLoginRequest));
	// notice addrTypeMask -- signal only available for internal calls
	registerSlot(ireon::net_wsrs::rsCheckCharInGame, static_cast<CGenericSlot> (&CRSWSMainState::onCheckCharRequest), ireon::net::netAddress::atInternalAddress);
	registerSlot(ireon::net_wsrs::checkCharInGameResponse, static_cast<CGenericSlot> (&CRSWSMainState::onCheckCharResponse));
	registerSlot(ireon::net_wsrs::rsCharSelected, static_cast<CGenericSlot> (&CRSWSMainState::onCharSelected), ireon::net::netAddress::atInternalAddress);

	setSignalsProcessingMode(CGenericState::denyWithExceptions);
	addProcessingException(ireon::net_wsrs::login);
}

// ---------------------------------------------------------------------

void CRSWSMainState::onLoginRequest(String& packetData)
{
	_log (_debug, _loc, _("Got ws login request.") );

	try
	{
		GENERATE_PACKET_TYPE(ireon::net_wsrs::loginPacket) data;
		data.unserialize(packetData);

		byte id = data.get(ireon::net_wsrs::loginPacket::WorldServerID);

		GENERATE_PACKET_TYPE(ireon::net_wsrs::loginResponsePacket) resp;
		if ( id != nullWSId )
		{
			conn().setId(id);
			resp.set(ireon::net_wsrs::loginResponsePacket::loginSuccessfull, true);
			setSignalsProcessingMode(CGenericState::allowWithExceptions);
		} else {
			_log (_error, _loc, _("Got ws login request with id == nullWSId.") );
			resp.set(ireon::net_wsrs::loginResponsePacket::loginSuccessfull, false);
		}
		send(resp.getCommandId(), resp.serialize());
	}
	catch (ENetDataException&)
	{
		_log (_error, _loc, _("Caught ENetDataException.") );
		// TODO send something to client?
		return;
	}
}

// ---------------------------------------------------------------------

void CRSWSMainState::onCheckCharRequest(String& packetData)
{
	_log (_debug, _loc, _("Got check char request.") );

	ushort charId;
	uint connId;
	try
	{
		GENERATE_PACKET_TYPE(ireon::net_wsrs::checkCharInGamePacket) data;
		data.unserialize(packetData);

		charId = data.get(ireon::net_wsrs::checkCharInGamePacket::charIdToCheck);
		connId = data.get(ireon::net_wsrs::checkCharInGamePacket::callerId);
	}
	catch (ENetDataException&)
	{
		_log (_error, _loc, _("Caught ENetDataException.") );
		// TODO send something to client?
		return;
	}

	if ( m_checkCharRequests.find(charId) == m_checkCharRequests.end() )
	{
		m_checkCharRequests[charId]	= connId;
		GENERATE_PACKET_TYPE(ireon::net_wsrs::checkCharInGameRequestPacket) req;
		req.set(ireon::net_wsrs::checkCharInGameRequestPacket::charIdToCheck, charId);
		send(req.getCommandId(), req.serialize());
		_log (_info, _loc, _("Sent check char %d in game request to ws."), charId );
	} else 
		_log (_warn, _loc, _("Duplicate check char request for char %d."), charId );
}

// ---------------------------------------------------------------------

void CRSWSMainState::onCheckCharResponse(String& packetData)
{
	_log (_debug, _loc, _("Got char check result.") );

	ushort charId;
	bool inGame;
	try
	{
		GENERATE_PACKET_TYPE(ireon::net_wsrs::checkCharInGameResponsePacket) data;
		data.unserialize(packetData);

		charId = data.get(ireon::net_wsrs::checkCharInGameResponsePacket::charId);
		inGame = data.get(ireon::net_wsrs::checkCharInGameResponsePacket::isInGame);
	}
	catch (ENetDataException&)
	{
		_log (_error, _loc, _("Caught ENetDataException.") );
		// TODO send something to client?
		return;
	}

	std::map<ushort,uint>::iterator it = m_checkCharRequests.find(charId);
	if ( it != m_checkCharRequests.end() )
	{
		GENERATE_PACKET_TYPE(ireon::net::pcrs::charCheckResponsePacket) req;
		req.set(ireon::net::pcrs::charCheckResponsePacket::charId, charId);
		req.set(ireon::net::pcrs::charCheckResponsePacket::isInGame, inGame);
		CRootApp::instance()->getPlayerConnection(it->second).processPacket(req.getCommandId(), req.serialize(), ireon::net::netAddress::atInternalAddress);		
		m_checkCharRequests.erase(it);
	} else 
		_log (_error, _loc, _("Player context id for char %d not found."), charId );
}

// ---------------------------------------------------------------------

void CRSWSMainState::onCharSelected(String& packetData)
{
	_log (_debug, _loc, _("Got char selected packet from rspc context.") );

	ushort charId;
	bool createNew;
	String token, charName;
	try
	{
		GENERATE_PACKET_TYPE(ireon::net_wsrs::charSelectedPacket) data;
		data.unserialize(packetData);

		charId = data.get(ireon::net_wsrs::charSelectedPacket::charId); 
		token = data.get(ireon::net_wsrs::charSelectedPacket::token); 
		createNew = data.get(ireon::net_wsrs::charSelectedPacket::createNewCharacter); 
		if (createNew)
			charName = data.get(ireon::net_wsrs::charSelectedPacket::characterName); 
	}
	catch (ENetDataException&)
	{
		_log (_error, _loc, _("Caught ENetDataException.") );
		// TODO send something to client?
		return;
	}

	GENERATE_PACKET_TYPE(ireon::net_wsrs::charSelectedInfoPacket) req;
	req.set(ireon::net_wsrs::charSelectedInfoPacket::charId, charId); 
	req.set(ireon::net_wsrs::charSelectedInfoPacket::token, token); 
	req.set(ireon::net_wsrs::charSelectedInfoPacket::createNewCharacter, createNew); 
	if (createNew)
		req.set(ireon::net_wsrs::charSelectedInfoPacket::characterName, charName); 
	send(req.getCommandId(), req.serialize());
	_log (_info, _loc, _("Sent char %d connection token to ws."), charId );
}


