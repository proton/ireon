/**
 * @file ireon_client/net/pcrs/hello_state.cpp
 * client root server context welcome (hello) state
 */

/* Copyright (C) 2005-2006 ireon.org developers council
* $$

* See LICENSE for details
*/

#include "ireon_client/stdafx.h"
#include "ireon_client/version.h"
#include "ireon_client/net/pcrs/hello_state.h"
#include "common/net/connections/pcrs.h"
#include "ireon_client/interface/client_event_manager.h"
#include "ireon_client/net/pc.h"
//#include "ireon_rs/accounts/user_account.h"

CPCRSHelloState::CPCRSHelloState(CNetConnection *ownerConnection):
CGenericState(ownerConnection)
{
	setStateName("PC <=> RS Hello State");
	registerSlot(ireon::net::pcrs::initateAccountRegistration, static_cast<CGenericSlot> (&CPCRSHelloState::registerRequest), ireon::net::netAddress::atInternalAddress);
	registerSlot(ireon::net::pcrs::accountRegistrationResponse, static_cast<CGenericSlot> (&CPCRSHelloState::onRegisterResponse), ireon::net::netAddress::atExternalAddress);
	registerSlot(ireon::net::pcrs::initiateLogin, static_cast<CGenericSlot> (&CPCRSHelloState::onInitiateLoginSignal), ireon::net::netAddress::atInternalAddress);
	registerSlot(ireon::net::pcrs::loginResponse, static_cast<CGenericSlot> (&CPCRSHelloState::onLoginResponse), ireon::net::netAddress::atExternalAddress);

	setSignalsProcessingMode(CGenericState::denyWithExceptions);
	addProcessingException(ireon::net::pcrs::initateAccountRegistration);
	addProcessingException(ireon::net::pcrs::initiateLogin);
}

// ---------------------------------------------------------------------

void CPCRSHelloState::registerRequest(String &packetData)
{
	String login, password;
	try
	{

		GENERATE_PACKET_TYPE(ireon::net::pc::root::initateAccountRegistrationPacket) data;
		data.unserialize(packetData);

		login = data.get(ireon::net::pc::root::initateAccountRegistrationPacket::login);
		password = data.get(ireon::net::pc::root::initateAccountRegistrationPacket::password);
	}
	catch (ENetDataException&)
	{
		_log(_error,_loc, _("caught ENetDataException."));
		// TODO send something to event manager?
		return;
	}

	GENERATE_PACKET_TYPE(ireon::net::pcrs::accountRegistrationRequestPacket) req;
	
	req.set(ireon::net::pcrs::accountRegistrationRequestPacket::login, login);
	req.set(ireon::net::pcrs::accountRegistrationRequestPacket::password, password);
	
	_log(_debug,_loc,_("sending account '%s' registration request."), login.c_str());

	send(req.getCommandId(), req.serialize());
	clearProcessingExceptions();
	addProcessingException(ireon::net::pcrs::accountRegistrationResponse);
}

// ---------------------------------------------------------------------

void CPCRSHelloState::onRegisterResponse(String &packetData)
{

	byte responseBitfield;
	try
	{
		GENERATE_PACKET_TYPE(ireon::net::pcrs::accountRegistrationResponsePacket) resp;

		resp.unserialize(packetData);
		responseBitfield = resp.get(ireon::net::pcrs::accountRegistrationResponsePacket::responseBitfield);
	}
	catch (ENetDataException&)
	{
		_log(_error,_loc, _("caught ENetDataException."));
		return;
	}
	//CLog::instance()->log(CLog::msgFlagAuth, CLog::msgLvlVerbose,__FUNCTION__,
		//_("got account registration response... "));
	


	if (ireon::net::checkBitfield(responseBitfield,ireon::net::pcrs::accountRegistrationResponsePacket::ok))
	{
		_log(_debug,_loc,_("account registered successfully."));
		// TODO send something to event manager?
		conn().setNextState(CPCRSConnection::characterSelection);
		CClientApp::instance()->setState(CClientApp::AS_ROOT);
		return;
	}
	if (ireon::net::checkBitfield(responseBitfield,ireon::net::pcrs::accountRegistrationResponsePacket::failed))
		_log(_debug,_loc,_(" account registration failed, details follow."));

	if (ireon::net::checkBitfield(responseBitfield,ireon::net::pcrs::accountRegistrationResponsePacket::incorrectCredentials))
		_log(_debug,_loc,_(" login or password is not well formed."));

	if (ireon::net::checkBitfield(responseBitfield,ireon::net::pcrs::accountRegistrationResponsePacket::duplicateLogin))
		_log(_debug,_loc,_(" user with same login already exists."));

	if (ireon::net::checkBitfield(responseBitfield,ireon::net::pcrs::accountRegistrationResponsePacket::unknownError))
		_log(_debug,_loc,_(" db error or something as severe as this :)"));

	// TODO send something to event manager?
	CClientApp::instance()->setState(CClientApp::AS_MENU);
}

// ---------------------------------------------------------------------

void CPCRSHelloState::onInitiateLoginSignal (String &packetData)
{
	_log(_info,_loc, _("Sending login request to root server."));

	String login, password;
	try
	{
		GENERATE_PACKET_TYPE(ireon::net::pc::root::initiateLoginPacket) data;
		data.unserialize(packetData);

		login = data.get(ireon::net::pc::root::initiateLoginPacket::login);
		password = data.get(ireon::net::pc::root::initiateLoginPacket::password);
	}
	catch (ENetDataException&)
	{
		_log(_error,_loc, _("caught ENetDataException."));
		// TODO send something to event manager?
		return;
	}

	GENERATE_PACKET_TYPE(ireon::net::pcrs::loginRequestPacket) req;

	req.set(ireon::net::pcrs::loginRequestPacket::login, login); 
	req.set(ireon::net::pcrs::loginRequestPacket::password, password); 
	
	send(req.getCommandId(), req.serialize());
	// TODO send something to event manager?

	clearProcessingExceptions();
	addProcessingException(ireon::net::pcrs::loginResponse);
}

	
// ---------------------------------------------------------------------

void CPCRSHelloState::onLoginResponse (String &packetData)
{
	_log(_debug,_loc, _("got login response."));

	byte responseBitfield = 0;
	try
	{
		GENERATE_PACKET_TYPE(ireon::net::pcrs::loginResponsePacket) data;
		data.unserialize(packetData);
		responseBitfield = data.get(ireon::net::pcrs::loginResponsePacket::responseBitfield);
	}
	catch (ENetDataException&)
	{
		_log(_error,_loc, _("CRSPCHelloState::onLoginResponse: caught ENetDataException."));
		// TODO send something to event manager?
		return;
	}

	if (ireon::net::checkBitfield(responseBitfield,ireon::net::pcrs::loginResponsePacket::ok))
	{
		_log(_debug,_loc,_("successfully logged id."));
		// TODO send something to event manager?
		conn().setNextState(CPCRSConnection::characterSelection);
		CClientApp::instance()->setState(CClientApp::AS_ROOT);
		return;
	}
	
	if (ireon::net::checkBitfield(responseBitfield,ireon::net::pcrs::loginResponsePacket::failed))
		_log(_debug,_loc,_("login failed, details follow."));

	if (ireon::net::checkBitfield(responseBitfield,ireon::net::pcrs::loginResponsePacket::incorrectCredentials))
		_log(_debug,_loc,_("no such account or password is incorrect."));

	if (ireon::net::checkBitfield(responseBitfield,ireon::net::pcrs::loginResponsePacket::doubleConnect))
		_log(_debug,_loc,_("this user is already connected to root server."));

	if (ireon::net::checkBitfield(responseBitfield,ireon::net::pcrs::loginResponsePacket::unknownError))
		_log(_debug,_loc,_("db error or something as severe as this :)"));

	// TODO send something to event manager?
	CClientApp::instance()->setState(CClientApp::AS_MENU);
};
