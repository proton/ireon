/**
 * @file ireon_rs/net/rspc/hello_state.cpp
 * root server player context welcome (hello) state
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: hello_state.cpp 781 2006-10-07 19:52:08Z zak $

 * See LICENSE for details
 */

#include "ireon_rs/stdafx.h"
#include "ireon_rs/net/rspc/hello_state.h"
#include "common/net/connections/pcrs.h"
#include "ireon_rs/db/user_account.h"
#include "ireon_rs/root_app.h"

CRSPCHelloState::CRSPCHelloState(CRSPCConnection *ownerConnection):
CGenericState(ownerConnection),
m_concreteOwnerConnection(ownerConnection)
{
	setStateName("RS <=> PC Hello State");
	registerSlot(ireon::net::pcrs::loginRequest, static_cast<CGenericSlot> (&CRSPCHelloState::onLoginRequest));
	registerSlot(ireon::net::pcrs::accountRegistrationRequest, static_cast<CGenericSlot> (&CRSPCHelloState::onRegisterRequest));
}

// ---------------------------------------------------------------------

void CRSPCHelloState::onLoginRequest(String &packetData)
{
	_log (_debug, _loc, _("Got login attempt.") );
	String login, password;
	try
	{
		GENERATE_PACKET_TYPE(ireon::net::pcrs::loginRequestPacket) data;
		data.unserialize(packetData);

		login = data.get(ireon::net::pcrs::loginRequestPacket::login);
		password = data.get(ireon::net::pcrs::loginRequestPacket::password);
	}
	catch (ENetDataException&)
	{
		_log (_error, _loc, _("Caught ENetDataException.") );
		// TODO send something to client?
		return;
	}

	byte resCode = 0;

	try
	{
		std::auto_ptr<CUserAccount> acc(new CUserAccount(login, password));
		try
		{
			CRootApp::instance()->getPlayerConnection(acc->getId());
			// same client is already connected - error
			throw 1;
		}
		catch (ireon::net::EConnectionNotExist&)
		{
			// connection not found - everything is ok
		}
		// as soon as conn() takes over pointer control from std::auto_ptr, 'acc' variable becomes indefinite and could not be used later.
		conn().setAccount(acc);
		conn().setNextState(CRSPCConnection::main);
		conn().setId( conn().getAccount()->getId() );
		resCode |= static_cast<byte> (ireon::net::pcrs::loginResponsePacket::ok);
		_log (_info, _loc, _("Login successfull. (Login: %s)"), login.c_str() );
	}
	catch (int)
	{
		resCode |= static_cast<byte> (ireon::net::pcrs::loginResponsePacket::failed | ireon::net::pcrs::loginResponsePacket::doubleConnect);
		_log (_error, _loc, _("Double connect, access denied. (Login: %s)"), login.c_str() );
	}
	catch (EDBError&)
	{
		resCode |= static_cast<byte> (ireon::net::pcrs::loginResponsePacket::failed | ireon::net::pcrs::loginResponsePacket::unknownError);
		_log (_error, _loc, _("Database error! (Login: %s)"), login.c_str() );
	}
	catch (EAccountNotFound&)
	{
		resCode |= static_cast<byte> (ireon::net::pcrs::loginResponsePacket::failed | ireon::net::pcrs::loginResponsePacket::incorrectCredentials);
		_log (_error, _loc, _("Login failed -- account not found or password does not match. (Login: %s)"), login.c_str() );
	}

	GENERATE_PACKET_TYPE(ireon::net::pcrs::loginResponsePacket) resp;
	resp.set(ireon::net::pcrs::loginResponsePacket::responseBitfield, resCode); 
	send(resp.getCommandId(), resp.serialize());
};

// ---------------------------------------------------------------------

void CRSPCHelloState::onRegisterRequest(String &packetData)
{
	_log (_debug, _loc, _("Got account registration request.") );

	String login, password;
	try
	{
		GENERATE_PACKET_TYPE(ireon::net::pcrs::accountRegistrationRequestPacket) data;
		data.unserialize(packetData);

		login = data.get(ireon::net::pcrs::accountRegistrationRequestPacket::login);
		password = data.get(ireon::net::pcrs::accountRegistrationRequestPacket::password);
	}
	catch (ENetDataException&)
	{
		_log (_error, _loc, _("Caught ENetDataException.") );
		// TODO send something to client?
		return;
	}

	byte resCode = 0;

	// TODO check for well-formedness of login and password
	if ((login.length()<4) || (password.length()<4))
	{
		resCode |= static_cast<byte> (ireon::net::pcrs::accountRegistrationResponsePacket::failed | ireon::net::pcrs::accountRegistrationResponsePacket::incorrectCredentials);
		_log (_error, _loc, _("Registration failed. Username or password too short. (Login: %s)"), login.c_str() );
	}

	if (!(resCode & ireon::net::pcrs::accountRegistrationResponsePacket::failed))
	{
		try
		{
			CUserAccount acc(login, password);
			resCode |= static_cast<byte> (ireon::net::pcrs::accountRegistrationResponsePacket::failed | ireon::net::pcrs::accountRegistrationResponsePacket::duplicateLogin);
			_log (_error, _loc, _("Registration failed. Duplicate login. (Login: %s)"), login.c_str() );
		}
		catch (EDBError&)
		{
			resCode |= static_cast<byte> (ireon::net::pcrs::accountRegistrationResponsePacket::failed | ireon::net::pcrs::accountRegistrationResponsePacket::unknownError);
			_log (_error, _loc, _("Database error! (Login: %s)"), login.c_str() );
		}
		catch (EAccountNotFound&)
		{
			// pass - everything is ok, account not found
		}
	}


	if (!(resCode & ireon::net::pcrs::accountRegistrationResponsePacket::failed))
	{
		try
		{
			std::auto_ptr<CUserAccount> acc(new CUserAccount(login, password, false));
			acc->saveToDB();
			// as soon as conn() takes over pointer control from std::auto_ptr, 'acc' variable becomes indefinite and could not be used later.
			conn().setAccount(acc);
			conn().setNextState(CRSPCConnection::main);
			conn().setId( conn().getAccount()->getId() );
			resCode |= static_cast<byte> (ireon::net::pcrs::accountRegistrationResponsePacket::ok);
			_log (_info, _loc, _("Registration successfull. (Login: %s)"), login.c_str() );
		}
		catch (EDBError&)
		{
			resCode |= static_cast<byte> (ireon::net::pcrs::accountRegistrationResponsePacket::failed | ireon::net::pcrs::accountRegistrationResponsePacket::unknownError);
			_log (_error, _loc, _("Database error! (Login: %s)"), login.c_str() );
		}

	}

	GENERATE_PACKET_TYPE(ireon::net::pcrs::accountRegistrationResponsePacket) resp;
	resp.set(ireon::net::pcrs::accountRegistrationResponsePacket::responseBitfield, resCode);
	send(resp.getCommandId(), resp.serialize());
};

