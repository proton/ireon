/**
 * @file ireon_rs/net/rspc/main_state.cpp
 * root server player context character manipulations state
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: main_state.cpp 790 2006-10-29 20:43:24Z zak $

 * See LICENSE for details
 */

#include "ireon_rs/stdafx.h"
#include "ireon_rs/net/rspc/main_state.h"
#include "common/net/connections/pcrs.h"
#include "common/net/connections/wsrs.h"
#include "ireon_rs/db/character.h"
#include "ireon_rs/root_app.h"
#include "common/miscutils.h"

#include <boost/static_assert.hpp>
BOOST_STATIC_ASSERT(nullWSId != defaultWSId);


CRSPCMainState::CRSPCMainState(CRSPCConnection *ownerConnection):
CGenericState(ownerConnection),
m_concreteOwnerConnection(ownerConnection)
{
	setStateName("RS <=> PC Main State");
	registerSlot(ireon::net::pcrs::createCharRequest, static_cast<CGenericSlot> (&CRSPCMainState::onCreateCharRequest));
	registerSlot(ireon::net::pcrs::listCharsRequest, static_cast<CGenericSlot> (&CRSPCMainState::onListCharsRequest));
	registerSlot(ireon::net::pcrs::selectCharRequest, static_cast<CGenericSlot> (&CRSPCMainState::onSelectCharRequest));
	registerSlot(ireon::net::pcrs::rsCharCheckResponse, static_cast<CGenericSlot> (&CRSPCMainState::onCharCheckResponse), ireon::net::netAddress::atInternalAddress);

	setSignalsProcessingMode(CGenericState::allowWithExceptions);
	addProcessingException(ireon::net::pcrs::rsCharCheckResponse);
}

void CRSPCMainState::onCreateCharRequest (String &packetData)
{
	_log (_debug, _loc, _("Got create char request.") );

	String name;
	try
	{
		GENERATE_PACKET_TYPE(ireon::net::pcrs::createCharRequestPacket) data;
		data.unserialize(packetData);

		name = data.get(ireon::net::pcrs::createCharRequestPacket::name);
	}
	catch (ENetDataException&)
	{
		_log (_error, _loc, _("Caught ENetDataException.") );
		// TODO send something to client?
		return;
	}

	byte resCode = 0;

	// TODO check for well-formedness of name

	try
	{
		CRSCharacterInfo ch(name);
		// if no exception is thrown character is found
		resCode |= static_cast<byte> (ireon::net::pcrs::createCharResponsePacket::failed | ireon::net::pcrs::createCharResponsePacket::duplicateName);
		_log (_error, _loc, _("Char creation failed. Duplicate name %s."), name.c_str() );
	}
	catch (EDBError&)
	{
		resCode |= static_cast<byte> (ireon::net::pcrs::createCharResponsePacket::failed | ireon::net::pcrs::createCharResponsePacket::unknownError);
		_log (_error, _loc, _("Database error! (Char name: %s)"), name.c_str() );
	}
	catch (ECharacterNotFound&)
	{
		// pass - everything is ok, character not found
	}

	if (!(resCode & ireon::net::pcrs::createCharResponsePacket::failed))
	{
		try
		{
			if ( CRSCharacterInfo::getCharactersCount(conn().getAccount()->getId()) >= 8 )
			{
				resCode |= static_cast<byte> (resCode | ireon::net::pcrs::createCharResponsePacket::failed | ireon::net::pcrs::createCharResponsePacket::tooManyCharacters);
				_log (_error, _loc, _("Exceeded allowed number of characters per account. (Account id: %d)"), conn().getAccount()->getId() );
			}
		}
		catch (EDBError&)
		{
			resCode |= static_cast<byte> (ireon::net::pcrs::createCharResponsePacket::failed | ireon::net::pcrs::createCharResponsePacket::unknownError);
			_log (_error, _loc, _("Database error! (Char name: %s)"), name.c_str() );
		}
	}


	if (!(resCode & ireon::net::pcrs::createCharResponsePacket::failed))
	{
		try
		{
			CRSCharacterInfo ch(name, false);
			ch.setOwner(conn().getAccount()->getId());
			ch.saveToDB();
			resCode |= static_cast<byte> (ireon::net::pcrs::createCharResponsePacket::ok);
			_log (_error, _loc, _("Created new char. (Account id: %d, char name: %s)"), conn().getAccount()->getId(), name.c_str() );
		}
		catch (EDBError&)
		{
			resCode |= static_cast<byte> (ireon::net::pcrs::createCharResponsePacket::failed | ireon::net::pcrs::createCharResponsePacket::unknownError);
			_log (_error, _loc, _("Database error! (Char name: %s)"), name.c_str() );
		}
	}

	GENERATE_PACKET_TYPE(ireon::net::pcrs::createCharResponsePacket) resp;
	resp.set(ireon::net::pcrs::createCharResponsePacket::responseBitfield, resCode);
	send(resp.getCommandId(), resp.serialize());
};

// ------------------------------------------------------------------------

void CRSPCMainState::onListCharsRequest (String &)
{
	_log (_debug, _loc, _("Char list request from account %s."), conn().getAccount()->getLogin().c_str() );

	std::map<ushort, boost::shared_ptr<CRSCharacterInfo>> charList;

	String s;
	try
	{
		CRSCharacterInfo::getCharacters(conn().getAccount()->getId(), charList);
	}
	catch (EDBError&)
	{
		_log (_error, _loc, _("Database error! (acc: %s)"), conn().getAccount()->getLogin().c_str() );
		send(ireon::net::pcrs::listCharsResponse, s);
		return; 
	}

	if ( charList.empty() )
	{
		send(ireon::net::pcrs::listCharsResponse, s);
		return; 
	}

	for ( std::map<ushort, boost::shared_ptr<CRSCharacterInfo>>::iterator i = charList.begin(); i != charList.end(); ++i )
	{
		GENERATE_PACKET_TYPE(ireon::net::pcrs::listCharsResponsePacket) resp;
		resp.set(ireon::net::pcrs::listCharsResponsePacket::id, i->second->getId());
		resp.set(ireon::net::pcrs::listCharsResponsePacket::name, i->second->getName());
		resp.set(ireon::net::pcrs::listCharsResponsePacket::new_, i->second->getWS() == nullWSId );
		s.append(resp.serialize());
	}
	send(ireon::net::pcrs::listCharsResponse, s);
}

// ------------------------------------------------------------------------

void CRSPCMainState::onSelectCharRequest (String &packetData)
{
	_log (_debug, _loc, _("Got char selection request.") );

	ushort charId;
	try
	{
		GENERATE_PACKET_TYPE(ireon::net::pcrs::selectCharRequestPacket) data;
		data.unserialize(packetData);

		charId = data.get(ireon::net::pcrs::selectCharRequestPacket::id);
	}
	catch (ENetDataException&)
	{
		_log (_error, _loc, _("Caught ENetDataException.") );
		// TODO send something to client?
		return;
	}


	std::map<ushort, boost::shared_ptr<CRSCharacterInfo>> charList;
	try
	{
		CRSCharacterInfo::getCharacters(conn().getAccount()->getId(), charList);
	}
	catch (EDBError&)
	{
		_log (_error, _loc, _("Database error! (acc: %s)"), conn().getAccount()->getLogin().c_str() );
		GENERATE_PACKET_TYPE(ireon::net::pcrs::selectCharAckPacket) resp;
		resp.set(ireon::net::pcrs::selectCharAckPacket::responseBitfield, static_cast<byte> (ireon::net::pcrs::selectCharAckPacket::failed | ireon::net::pcrs::selectCharAckPacket::unknownError));
		send(resp.getCommandId(), resp.serialize());
		return; 
	}

	if ( charList.find(charId) == charList.end() )
	{
		_log (_error, _loc, _("Selected character not found! (acc: %s)"), conn().getAccount()->getLogin().c_str() );
		GENERATE_PACKET_TYPE(ireon::net::pcrs::selectCharAckPacket) resp;
		resp.set(ireon::net::pcrs::selectCharAckPacket::responseBitfield, static_cast<byte> (ireon::net::pcrs::selectCharAckPacket::failed | ireon::net::pcrs::selectCharAckPacket::charNotFound));
		send(resp.getCommandId(), resp.serialize());
		return; 
	}

	GENERATE_PACKET_TYPE(ireon::net::pcrs::selectCharAckPacket) resp;
	resp.set(ireon::net::pcrs::selectCharAckPacket::responseBitfield, static_cast<byte> (ireon::net::pcrs::selectCharAckPacket::ok));
	send(resp.getCommandId(), resp.serialize());

	if (m_charsToCheck.size())
	{
		_log (_warn, _loc, _("m_charsToCheck list is not emrty! Cleaning up. (acc: %s)"), conn().getAccount()->getLogin().c_str() );
		m_charsToCheck.clear();
	}

	GENERATE_PACKET_TYPE(ireon::net_wsrs::checkCharInGamePacket) req;
	for ( std::map<ushort, boost::shared_ptr<CRSCharacterInfo>>::iterator it = charList.begin(); it != charList.end(); ++it )
		if ( it->second->getWS() != nullWSId )
			try
			{
				req.set(ireon::net_wsrs::checkCharInGamePacket::charIdToCheck, it->second->getId());
				req.set(ireon::net_wsrs::checkCharInGamePacket::callerId, conn().getId());
				CRootApp::instance()->getWSConnection(it->second->getWS()).processPacket(req.getCommandId(), 
					req.serialize(), ireon::net::netAddress(ireon::net::netAddress::atInternalAddress));
				m_charsToCheck[it->second->getId()] = ireon::misc::getTime() + 2000;
			}
			catch (ireon::net::EConnectionNotExist&)
			{
				// if ws is not logged in do nothing	
			}

	m_selectedCharId = charId;
	m_selectedCharWS = charList.find(charId)->second->getWS();

	if (m_charsToCheck.size())
		setTimeout(2500, static_cast<TimeoutCallback> (&CRSPCMainState::onCheckCharTimeout));
	else
		sendSelectResponse(); // if no characters to check do not wait for timeout
	setSignalsProcessingMode(CGenericState::denyWithExceptions);
};

// ------------------------------------------------------------------------

void CRSPCMainState::onCharCheckResponse (String &packetData)
{
	_log (_debug, _loc, _("Got char check response.") );

	ushort charId;
	bool isInGame;
	try
	{
		GENERATE_PACKET_TYPE(ireon::net::pcrs::charCheckResponsePacket) data;
		data.unserialize(packetData);
		charId = data.get(ireon::net::pcrs::charCheckResponsePacket::charId);
		isInGame = data.get(ireon::net::pcrs::charCheckResponsePacket::isInGame);
	}
	catch (ENetDataException&)
	{
		_log (_error, _loc, _("Caught ENetDataException.") );
		setSignalsProcessingMode(CGenericState::allowWithExceptions);
		return;
	}

	std::map<ushort, int64>::iterator it = m_charsToCheck.find(charId);
	if ( it != m_charsToCheck.end() )
		if ( isInGame == true )
		{
			GENERATE_PACKET_TYPE(ireon::net::pcrs::selectCharResponsePacket) resp;
			resp.set(ireon::net::pcrs::selectCharResponsePacket::responseCode, static_cast<byte> (ireon::net::pcrs::selectCharResponsePacket::charIsInGame));
			resp.set(ireon::net::pcrs::selectCharResponsePacket::charInGame, charId);
			send(resp.getCommandId(), resp.serialize());				

			m_charsToCheck.clear();
			setSignalsProcessingMode(CGenericState::allowWithExceptions);
		} else {
			m_charsToCheck.erase(it);
			sendSelectResponse();
		}
	else
		_log (_error, _loc, _("Unknown char id %d."), charId );
	deleteTimedOutChars ();
};

// ------------------------------------------------------------------------

void CRSPCMainState::deleteTimedOutChars ()
{
 	for (std::map<ushort, int64>::iterator it = m_charsToCheck.begin(); it != m_charsToCheck.end(); /* nothing */)
 		if ( it->second < ireon::misc::getTime() )
			m_charsToCheck.erase(it++);
		else
			++it;
}

// ------------------------------------------------------------------------

void CRSPCMainState::onCheckCharTimeout ()
{
	deleteTimedOutChars();
	sendSelectResponse();
}


// ------------------------------------------------------------------------

void CRSPCMainState::sendSelectResponse ()
{
	if ( m_charsToCheck.size() )
	{
		setTimeout(2500, static_cast<TimeoutCallback> (&CRSPCMainState::onCheckCharTimeout));
		return;
	} 

	char buffer[65];
#pragma warning(push)
#pragma warning(disable: 4996) // disables "'_ui64toa' was declared deprecated" warning
	_ui64toa( ireon::misc::getTime() + conn().getId(), buffer, 10 ); 
#pragma warning(pop)
	String token = ireon::misc::getSHA256Digest(buffer);

	try
	{
		{
			GENERATE_PACKET_TYPE(ireon::net_wsrs::charSelectedPacket) resp;
			resp.set(ireon::net_wsrs::charSelectedPacket::charId, m_selectedCharId);
			resp.set(ireon::net_wsrs::charSelectedPacket::token, token);
			if ( m_selectedCharWS == nullWSId )
			{
				resp.set(ireon::net_wsrs::charSelectedPacket::createNewCharacter, true);
				resp.set(ireon::net_wsrs::charSelectedPacket::characterName, CRSCharacterInfo(m_selectedCharId).getName());
				CRootApp::instance()->getWSConnection(defaultWSId).processPacket(
					resp.getCommandId(), resp.serialize(), 
					ireon::net::netAddress(ireon::net::netAddress::atInternalAddress));
			} else {
				resp.set(ireon::net_wsrs::charSelectedPacket::createNewCharacter, false);
				CRootApp::instance()->getWSConnection(m_selectedCharWS).processPacket(
					resp.getCommandId(), resp.serialize(), 
					ireon::net::netAddress(ireon::net::netAddress::atInternalAddress));
			}
		}

		{
			GENERATE_PACKET_TYPE(ireon::net::pcrs::selectCharResponsePacket) resp;
			resp.set(ireon::net::pcrs::selectCharResponsePacket::responseCode, static_cast<byte> (ireon::net::pcrs::selectCharResponsePacket::ok));
			if ( m_selectedCharWS == nullWSId )
			{
				resp.set(ireon::net::pcrs::selectCharResponsePacket::wsAddress, CRootApp::instance()->getWSConnection(defaultWSId).getRemoteHost());
				CRSCharacterInfo ch(m_selectedCharId);
				ch.setWS(defaultWSId);
				ch.saveToDB();
			}
			else 
				resp.set(ireon::net::pcrs::selectCharResponsePacket::wsAddress, CRootApp::instance()->getWSConnection(m_selectedCharWS).getRemoteHost());
			resp.set(ireon::net::pcrs::selectCharResponsePacket::token, token);
			send(resp.getCommandId(), resp.serialize());
		}
	}
	catch (ireon::net::EConnectionNotExist&)
	{
		GENERATE_PACKET_TYPE(ireon::net::pcrs::selectCharResponsePacket) resp;
		resp.set(ireon::net::pcrs::selectCharResponsePacket::responseCode, static_cast<byte> (ireon::net::pcrs::selectCharResponsePacket::WSUnavailable));
		send(resp.getCommandId(), resp.serialize());
	}

	setSignalsProcessingMode(CGenericState::allowWithExceptions);
}


#if 0
void CRSPCMainState::onRemoveCharRequest (CData& data)
{
	uint charId;
	data >> charId;

	CLog::instance()->log(CLog::msgLvlVerbose,__FUNCTION__, _("Got character deletion request: account id %d, char is id %d.\n"), m_ownerConnection->getOwnerAccPtr()->getId(), charId);

	CData	answer;

	RootCharPlayerData *ch = CRootDB::instance()->findCharacter(charId);
	if (!ch)
	{
		answer.wrt((ireon::net::commandId) ireon::net::rspcCodes::rcCharRemoveFailed);
		answer.wrt((byte) ireon::net::rspcCodes::crCharNotFound);
		m_ownerConnection->write(answer);
		CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlWarn,__FUNCTION__,_("Attempt to remove nonexisting character %d.\n"), charId);
		return;
	}

	uint cluster = ch->m_cluster;
	CNetConnection* con = CRootApp::instance()->getCMConnectionsManager()->findConnection(cluster);
	if( !con )
	{
		answer.wrt((ireon::net::commandId) ireon::net::rspcCodes::rcCharRemoveFailed);
		answer.wrt((byte) ireon::net::rspcCodes::crClusterNotFound);
		CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlError,__FUNCTION__, _("Could not find cluser to remove char %d.\n"), charId);
		m_ownerConnection->write(answer);
		return;
	}
	
	CData	request;
	request << charId;
	con->processCommandEx(ireon::net::rsccCodes::scCheckRemoveCharRequest, request, netAddress(netAddress::atInternalAddress));

	answer.wrt((ireon::net::commandId) ireon::net::rspcCodes::rcCharRemoveWaitAck);
	m_ownerConnection->write(answer);
	CLog::instance()->log(CLog::msgFlagAccChar, CLog::msgLvlVerbose,__FUNCTION__, _("Sent remove request to cluster %d for char %d.\n"), con->getId(), charId);
	m_ownerConnection->setNextState(CRSPlayerConnection::csWaitRemoveAck);
	return;
};

#endif
