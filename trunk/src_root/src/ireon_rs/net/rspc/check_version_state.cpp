/**
 * @file ireon_rs/net/rspc/check_version_state.cpp
 * root server player context checking version state
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: check_version_state.cpp 781 2006-10-07 19:52:08Z zak $

 * See LICENSE for details
 */

#include "ireon_rs/stdafx.h"
#include "ireon_rs/net/rspc/check_version_state.h"
#include "common/net/connections/pcrs.h"
#include "ireon_rs/xsd/config_xsd.h"

#include "ireon_rs/root_app.h"

CRSPCCheckVersionState::CRSPCCheckVersionState(CRSPCConnection *ownerConnection):
CGenericState(ownerConnection)
//m_concreteOwnerConnection(ownerConnection)
{
	setStateName("RS <=> PC Check Version State");
	registerSlot(ireon::net::pcrs::versionCheckRequest, static_cast<CGenericSlot> (&CRSPCCheckVersionState::onCheckVersionRequest));
}

// ---------------------------------------------------------------------

void CRSPCCheckVersionState::onCheckVersionRequest(String &packetData)
{
	ushort clientVersion[4];
	try
	{
		GENERATE_PACKET_TYPE(ireon::net::pcrs::versionCheckRequestPacket) data;
		data.unserialize(packetData);

		clientVersion[0] = data.get(ireon::net::pcrs::versionCheckRequestPacket::majorVersion);
		clientVersion[1] = data.get(ireon::net::pcrs::versionCheckRequestPacket::minorVersion);
		clientVersion[2] = data.get(ireon::net::pcrs::versionCheckRequestPacket::releaseNumber);
		clientVersion[3] = data.get(ireon::net::pcrs::versionCheckRequestPacket::buildNumber);
	}
	catch (ENetDataException&)
	{
		_log (_error, _loc, _("Caught ENetDataException.") );
		// TODO send something to client?
		return;
	}

	_log (_debug, _loc, _("Received version '%u.%u.%u.%u'."),clientVersion[0],clientVersion[1],clientVersion[2],clientVersion[3] );

	ushort requiredClientVersion[4];
	if ( CRootApp::instance()->getConfig()->requiredClientVersion().present() )
	{
		requiredClientVersion[0] = CRootApp::instance()->getConfig()->requiredClientVersion().get().major();
		requiredClientVersion[1] = CRootApp::instance()->getConfig()->requiredClientVersion().get().minor();
		requiredClientVersion[2] = CRootApp::instance()->getConfig()->requiredClientVersion().get().release();
		requiredClientVersion[3] = CRootApp::instance()->getConfig()->requiredClientVersion().get().build();
	} else {
		requiredClientVersion[0] = ireon::rs::CClientVersion::major::default_value();
		requiredClientVersion[1] = ireon::rs::CClientVersion::minor::default_value();
		requiredClientVersion[2] = ireon::rs::CClientVersion::release::default_value();
		requiredClientVersion[3] = ireon::rs::CClientVersion::build::default_value();
	}

	bool passedCheck = true;
	for (int i=0; i < 4; ++i)
		if ( clientVersion[i] < requiredClientVersion[i] )
			passedCheck = false;

	GENERATE_PACKET_TYPE(ireon::net::pcrs::versionCheckResponsePacket) resp;
	resp.set(ireon::net::pcrs::versionCheckResponsePacket::checkPassed, passedCheck);
	resp.set(ireon::net::pcrs::versionCheckResponsePacket::majorVersion, static_cast<byte> (requiredClientVersion[0]));
	resp.set(ireon::net::pcrs::versionCheckResponsePacket::minorVersion, static_cast<byte> (requiredClientVersion[1]));
	resp.set(ireon::net::pcrs::versionCheckResponsePacket::releaseNumber, requiredClientVersion[2]);
	resp.set(ireon::net::pcrs::versionCheckResponsePacket::buildNumber, requiredClientVersion[3]);

	send(resp.getCommandId(), resp.serialize());

	if (passedCheck)
	{
		_log (_debug, _loc, _("Version check: version ok.") );
		conn().setNextState(CRSPCConnection::hello);
	}
	else
		_log (_warn, _loc, _("Version check: failed.") );

};

// ---------------------------------------------------------------------

