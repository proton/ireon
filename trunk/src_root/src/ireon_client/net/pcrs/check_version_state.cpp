/**
 * @file ireon_rs/net/rspc/check_version_state.cpp
 * root server player context checking version state
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: check_version_state.cpp 698 2006-09-03 16:46:33Z zak $

 * See LICENSE for details
 */

#include "ireon_client/stdafx.h"
#include "ireon_client/net/pcrs/check_version_state.h"
#include "common/net/connections/pcrs.h"
#include "ireon_client/client_app.h"
#include "ireon_client/interface/client_event_manager.h"

CPCRSCheckVersionState::CPCRSCheckVersionState(CNetConnection *ownerConnection):
CGenericState(ownerConnection)
{
	setStateName("PC <=> RS Check Version State");
	registerSlot(ireon::net::pcrs::initiateVersionCheck, static_cast<CGenericSlot> (&CPCRSCheckVersionState::checkVersionRequest), ireon::net::netAddress::atInternalAddress);
	registerSlot(ireon::net::pcrs::versionCheckResponse, static_cast<CGenericSlot> (&CPCRSCheckVersionState::onCheckVersionResponse), ireon::net::netAddress::atExternalAddress);

}

// ---------------------------------------------------------------------

void CPCRSCheckVersionState::checkVersionRequest( String& )
{
	std::vector<byte> clientVersion = CClientApp::instance()->getVersion();
	GENERATE_PACKET_TYPE(ireon::net::pcrs::versionCheckRequestPacket) data;
	
	data.set(ireon::net::pcrs::versionCheckRequestPacket::majorVersion, clientVersion[0] );
	data.set(ireon::net::pcrs::versionCheckRequestPacket::minorVersion, clientVersion[1] );
	data.set(ireon::net::pcrs::versionCheckRequestPacket::releaseNumber, static_cast<ushort> (clientVersion[2]) );
	data.set(ireon::net::pcrs::versionCheckRequestPacket::buildNumber, static_cast<ushort> (clientVersion[3]) );
	
	_log(_debug,_loc, _("send client version '%u.%u.%u.%u'"),clientVersion[0],clientVersion[1],clientVersion[2],clientVersion[3]);
	//setTimeout(10000,static_cast<TimeoutCallback>(&CPCRSCheckVersionState::onTimeoutCallback));

	send(data.getCommandId(), data.serialize());
	
};

// ---------------------------------------------------------------------

void CPCRSCheckVersionState::onCheckVersionResponse(String &packetData)
{
	ushort serverVersion[4];
	bool passedCheck;
	try
	{
		GENERATE_PACKET_TYPE(ireon::net::pcrs::versionCheckResponsePacket) resp;
		
		resp.unserialize(packetData);
		passedCheck = resp.get(ireon::net::pcrs::versionCheckResponsePacket::checkPassed);
		serverVersion[0] = resp.get(ireon::net::pcrs::versionCheckResponsePacket::majorVersion);
		serverVersion[1] = resp.get(ireon::net::pcrs::versionCheckResponsePacket::minorVersion);
		serverVersion[2] = resp.get(ireon::net::pcrs::versionCheckResponsePacket::releaseNumber);
		serverVersion[3] = resp.get(ireon::net::pcrs::versionCheckResponsePacket::buildNumber);
	}
	catch (ENetDataException&)
	{
		_log(_error,_loc, _("caught ENetDataException. (Host: %s)"), conn().getConnectionInfo());
		return;
	}

	if (passedCheck)
	{
		_log(_debug,_loc, _("version check for host %s: version ok."), conn().getConnectionInfo());
		conn().setNextState(CPCRSConnection::hello);
		CClientApp::instance()->setState(CClientApp::AS_LOGING_ROOT);
	}
	else
	{
		_log(_debug,_loc, _("version check for host %s: failed."), conn().getConnectionInfo());
		CheckVersionFailedEventArg arg;
		arg.m_serverVersion.push_back(serverVersion[0]);
		arg.m_serverVersion.push_back(serverVersion[1]);
		arg.m_serverVersion.push_back(serverVersion[2]);
		arg.m_serverVersion.push_back(serverVersion[3]);
		arg.m_host = conn().getConnectionInfo();
		arg.m_port = 0; // TODO: getPort() or from config? later, later....
		CEventManager::instance()->execute(ON_CHECK_VERSION_FAILED, &arg);
		// TODO: disconnect. как?
		CClientApp::instance()->setState(CClientApp::AS_MENU);
	}

}

void CPCRSCheckVersionState::onTimeoutCallback()
{
	ConnectFailedEventArg arg;
	arg.m_error = _("Failed wait for answer from Root Server.");
	arg.m_host = "127.0.0.1";
	arg.m_port = 0;
	CEventManager::instance()->execute(ON_CONN_FAILED, &arg);
	// try re-send version or disconnect ?
	CClientApp::instance()->setState(CClientApp::AS_MENU);
}