/**
 * @file ireon_client/net/pcrs/character_selection_state.cpp
 * client root server context character manipulations state
 */

/* Copyright (C) 2005-2006 ireon.org developers council
* $Id: pcrs.h 767 2006-09-25 19:35:52Z mip $

*  See LICENSE for details
*/

#include "ireon_client/stdafx.h"
#include "ireon_client/net/pcrs/character_selection_state.h"
#include "ireon_client/net/pc.h"
#include "common/net/connections/pcrs.h"
#include "common/db/client_char_data.h"
#include "ireon_client/interface/client_event_manager.h"

CPCRSCharacterSelectionState::CPCRSCharacterSelectionState(CNetConnection *ownerConnection):
CGenericState(ownerConnection)
{
	setStateName("PC <=> RS Character Selection State");

	registerSlot(ireon::net::pcrs::initiateCreateChar, static_cast<CGenericSlot> (&CPCRSCharacterSelectionState::initiateCreateChar), ireon::net::netAddress::atInternalAddress);
	registerSlot(ireon::net::pcrs::createCharResponse,static_cast<CGenericSlot> (&CPCRSCharacterSelectionState::onCreateCharResponse), ireon::net::netAddress::atExternalAddress);

	registerSlot(ireon::net::pcrs::initiateListChar,   static_cast<CGenericSlot> (&CPCRSCharacterSelectionState::initiateListChar),   ireon::net::netAddress::atInternalAddress);
	registerSlot(ireon::net::pcrs::listCharsResponse,static_cast<CGenericSlot> (&CPCRSCharacterSelectionState::onListCharsResponse),   ireon::net::netAddress::atExternalAddress);

	registerSlot(ireon::net::pcrs::initiateSelectChar, static_cast<CGenericSlot> (&CPCRSCharacterSelectionState::initiateSelectChar), ireon::net::netAddress::atInternalAddress);
	registerSlot(ireon::net::pcrs::selectCharAck,		static_cast<CGenericSlot> (&CPCRSCharacterSelectionState::onSelectCharAck), ireon::net::netAddress::atExternalAddress);
	registerSlot(ireon::net::pcrs::selectCharResponse, static_cast<CGenericSlot> (&CPCRSCharacterSelectionState::onSelectCharResponse), ireon::net::netAddress::atExternalAddress);
	
	setSignalsProcessingMode(CGenericState::denyWithExceptions);
	addProcessingException(ireon::net::pcrs::initiateCreateChar);
	addProcessingException(ireon::net::pcrs::initiateListChar);
	addProcessingException(ireon::net::pcrs::initiateSelectChar);
}


void CPCRSCharacterSelectionState::initiateCreateChar(String &PacketData)
{
	String charName;
	GENERATE_PACKET_TYPE(ireon::net::pc::root::initiateCreateCharPacket) data;
	try
	{
		data.unserialize(PacketData);
		charName = data.get(ireon::net::pc::root::initiateCreateCharPacket::name);
	}
	catch (ENetDataException&)
	{
		_log(_error, _loc, _("Caught ENetDataException in rs connection."));
		return;
	}

	GENERATE_PACKET_TYPE(ireon::net::pcrs::createCharRequestPacket) req;
	req.set(ireon::net::pcrs::createCharRequestPacket::name, charName);
	
	_log(_debug,_loc,  _("Sending new char data: '%s'"), charName.c_str());
	
	send(req.getCommandId(), req.serialize());

	clearProcessingExceptions();
	addProcessingException(ireon::net::pcrs::createCharResponse);

}
//-----------------------------------------------------------------------
void CPCRSCharacterSelectionState::onCreateCharResponse (String &PacketData)
{
	clearProcessingExceptions();
	addProcessingException(ireon::net::pcrs::initiateCreateChar);
	addProcessingException(ireon::net::pcrs::initiateListChar);
	addProcessingException(ireon::net::pcrs::initiateSelectChar);

	byte responseBitfield;
	GENERATE_PACKET_TYPE(ireon::net::pcrs::createCharResponsePacket) data;

	try
	{
		data.unserialize(PacketData);
		responseBitfield = data.get(ireon::net::pcrs::createCharResponsePacket::responseBitfield);
	}
	catch (ENetDataException&)
	{
		_log(_error, _loc, _("Caught ENetDataException in rs connection."));
		return;
	}
	CharCreatedEventArg arg;
	arg.m_res = responseBitfield;
	CEventManager::instance()->execute(ON_CHAR_CREATED,&arg);

	if ( ireon::net::checkBitfield(responseBitfield, ireon::net::pcrs::createCharResponsePacket::ok ) )
	{
		_log(_debug,_loc, _("Char was successfully created."));
		//ClientOwnCharData d;
		//d.serialize(data);
		//CClientApp::instance()->m_characters.push_back(d);

		CEventManager::instance()->execute(ON_CHAR_LIST_CHANGED,&arg);
		return;
	}

	if ( ireon::net::checkBitfield(responseBitfield, ireon::net::pcrs::createCharResponsePacket::ok ) )
		_log(_debug, _loc, _("char creation failed, details follow."));

	if ( ireon::net::checkBitfield(responseBitfield, ireon::net::pcrs::createCharResponsePacket::incorrectCredentials ) )
		_log(_debug, _loc, _("char name is not well formed."));

	if ( ireon::net::checkBitfield(responseBitfield, ireon::net::pcrs::createCharResponsePacket::duplicateName ) )
		_log(_debug, _loc, _("char with this name already exists."));

	if ( ireon::net::checkBitfield(responseBitfield, ireon::net::pcrs::createCharResponsePacket::tooManyCharacters ) )
		_log(_debug, _loc, _("exceeded number of characters allowed for this account"));

	if ( ireon::net::checkBitfield(responseBitfield, ireon::net::pcrs::createCharResponsePacket::unknownError ) )
		_log(_debug, _loc, _("db error or something as severe as this"));
	
};
//---------------------------------------------------------------------------

void CPCRSCharacterSelectionState::initiateListChar(String&)
{
	GENERATE_PACKET_TYPE(ireon::net::pcrs::listCharsRequestPacket) req;
	_log(_debug, _loc, _("Send request for char list."));
	send(req.getCommandId(), req.serialize());

	clearProcessingExceptions();
	addProcessingException(ireon::net::pcrs::listCharsResponse);

}

// ------------------------------------------------------------------------

void CPCRSCharacterSelectionState::onListCharsResponse	(String &PacketData)
{
	GENERATE_PACKET_TYPE(ireon::net::pcrs::listCharsResponsePacket) data;
	try
	{
		uint charID;
		String charName;
		while (true)
		{
			data.unserialize(PacketData);
			charID   = data.get(ireon::net::pcrs::listCharsResponsePacket::id );
			charName = data.get(ireon::net::pcrs::listCharsResponsePacket::name );
			CharAddListArg arg;
			arg.m_id = charID;
			arg.m_name = charName;
			CEventManager::instance()->execute(ON_CHAR_ADD_LIST,&arg);
		}
	}
	catch (ENetDataException&)
	{
		//		CLog::instance()->log(CLog::msgLvlError,__FUNCTION__, _("caught ENetDataException. (Host: %s)"), conn().getConnectionInfo());
		//		return;
	}

	clearProcessingExceptions();
	addProcessingException(ireon::net::pcrs::initiateCreateChar);
	addProcessingException(ireon::net::pcrs::initiateListChar);
	addProcessingException(ireon::net::pcrs::initiateSelectChar);

}

//-------------------------------------------------------------------

void CPCRSCharacterSelectionState::initiateSelectChar(String &PacketData)
{
	ushort charId;
	GENERATE_PACKET_TYPE(ireon::net::pc::root::initiateSelectCharPacket) data;
	
	try
	{
		data.unserialize(PacketData);
		charId = data.get(ireon::net::pc::root::initiateSelectCharPacket::id);
	}
	catch (ENetDataException&)
	{
		_log(_error, _loc, _("Caught ENetDataException."));
		return;
	}
	_log(_debug, _loc, _("Send request for select char with id = '%u'."), charId);

	GENERATE_PACKET_TYPE(ireon::net::pcrs::selectCharRequestPacket) req;
	req.set(ireon::net::pcrs::selectCharRequestPacket::id, charId);
	send(req.getCommandId(), req.serialize());

	clearProcessingExceptions();
	addProcessingException(ireon::net::pcrs::selectCharAck);
}



// ------------------------------------------------------------------------

void CPCRSCharacterSelectionState::onSelectCharAck(String &PacketData)
{
	
	clearProcessingExceptions();
	//addProcessingException(ireon::net::pcrs::initiateCreateChar);
	//addProcessingException(ireon::net::pcrs::initiateListChar);
	addProcessingException(ireon::net::pcrs::selectCharResponse);

	byte responseBitfield;
	GENERATE_PACKET_TYPE(ireon::net::pcrs::selectCharAckPacket) data;
	try
	{
		data.unserialize(PacketData);
		responseBitfield = data.get(ireon::net::pcrs::selectCharAckPacket::responseBitfield);
	}
	catch (ENetDataException&)
	{
		_log(_error, _loc, _("Caught ENetDataException "));
		return;
	}
	if ( ireon::net::checkBitfield(responseBitfield, ireon::net::pcrs::selectCharAckPacket::ok ))
	{
		_log(_debug, _loc, _("select character found."));
		CharSelectAckArg arg;
		arg.m_res = responseBitfield;
		CEventManager::instance()->execute(ON_CHAR_SELECT_ACK,&arg);
		return;
	}

	CharSelectAckFailedArg arg;
	arg.m_res = responseBitfield;
	CEventManager::instance()->execute(ON_CHAR_SELECT_ACK_FAILED,&arg);

	if ( ireon::net::checkBitfield(responseBitfield, ireon::net::pcrs::selectCharAckPacket::failed ))
		_log(_debug, _loc, _("char select ack failed, details follow."));

	if ( ireon::net::checkBitfield(responseBitfield, ireon::net::pcrs::selectCharAckPacket::charNotFound ))
		_log(_debug, _loc, _("selected character not found!"));

	if ( ireon::net::checkBitfield(responseBitfield, ireon::net::pcrs::selectCharAckPacket::unknownError ))
		_log(_debug, _loc, _("db error or something as severe as this"));

	clearProcessingExceptions();
	addProcessingException(ireon::net::pcrs::initiateCreateChar);
	addProcessingException(ireon::net::pcrs::initiateListChar);
	addProcessingException(ireon::net::pcrs::initiateSelectChar);
}

void CPCRSCharacterSelectionState::onSelectCharResponse	(String &PacketData)
{

	byte respCode;
	uint charInGame;
	String WSaddress;
	String token;

	GENERATE_PACKET_TYPE(ireon::net::pcrs::selectCharResponsePacket) data;

	try
	{
		data.unserialize(PacketData);
		respCode = data.get(ireon::net::pcrs::selectCharResponsePacket::responseCode);
		if ( respCode == ireon::net::pcrs::selectCharResponsePacket::ok )
		{
			WSaddress = data.get(ireon::net::pcrs::selectCharResponsePacket::wsAddress);
			token = data.get(ireon::net::pcrs::selectCharResponsePacket::token);
			_log(_debug, _loc, _("Character successfully selected. Got ws login token."));
			CharSelectArg arg;
			arg.m_WSaddress = WSaddress;
			arg.m_token = token;

			CEventManager::instance()->execute(ON_CHAR_SELECT,&arg);
			CClientApp::instance()->setWorldConnectionParams(WSaddress, token);
			return;
	
		};

		CharSelectFailedArg arg;
		arg.m_res = respCode;
		CEventManager::instance()->execute(ON_CHAR_SELECT_FAILED,&arg);

		if ( respCode == ireon::net::pcrs::selectCharResponsePacket::charIsInGame )
		{
			charInGame = data.get(ireon::net::pcrs::selectCharResponsePacket::charInGame);
			_log(_error, _loc, _("Character selection failed, player's char '%u' is already in game ."), charInGame);
			arg.m_charInGame = charInGame;
		}
		
		if ( respCode == ireon::net::pcrs::selectCharResponsePacket::WSUnavailable )
			_log(_error, _loc, _("Character selection failed, current ws is unavailable!"));

	}
	catch (ENetDataException&)
	{
		_log(_error, _loc, _("Caught ENetDataException in rs connection."));
		return;
	}
	clearProcessingExceptions();
	addProcessingException(ireon::net::pcrs::initiateCreateChar);
	addProcessingException(ireon::net::pcrs::initiateListChar);
	addProcessingException(ireon::net::pcrs::initiateSelectChar);
}