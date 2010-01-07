/**
 * @file ireon_ws/ws_app.cpp
 * world server application
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: ws_app.cpp 854 2007-04-09 12:45:28Z mip $

 *  See LICENSE for details
 */

#include "ireon_ws/stdafx.h"
//#include "common/xmlconfig.h"
#include "common/miscutils.h"
#include "ireon_ws/xsd/config_xsd.h"
#include "ireon_ws/world_app.h"
#include "ireon_ws/world/ws_world.h"
#include "ireon_ws/db/player_characters_manager.h"
#include "common/resource/resource_manager.h"

#include "common/net/connections/wsrs.h"
#include "common/net/client_connections_manager.h"
#include "ireon_ws/net/wsrs_builder.h"
#include "common/net/server_connections_manager.h"
#include "ireon_ws/net/wspc_builder.h"

WSApp* WSApp::singleton_ = 0;

WSApp::WSApp(String &configPath):
run_(true),
m_restartRSConnection(false),
config_( ireon::ws::config(configPath) )
{
	// move this somewhere
	srand( (unsigned)time( NULL ) );

	assert(singleton_ == 0);
	singleton_ = this;

	CGenericApp::initI10N("ireon_ws");	   

	_log( _debug, _loc, _("Initializing ireon.org world server...") );

	if (!CGenericApp::printPIDToFile("ws.pid"))
		_log( _warn, _loc, _("PID file was not written!") );

	loadResourceLocations();

	// To init static angle tables
	Math math;

	// call to create singletons
	WSWorld::instance()->init();
	PlayerCharactersManager::instance();

/*
	if( !CWorldDB::init() )
	{
		CLog::instance()->log(CLog::msgLvlError,__FUNCTION__,_("Error initialization world DB."));
		shutdown();
	};
*/

	_log( _debug, _loc, _("Done world server initialization.") );
};

//-----------------------------------------------------------------------

WSApp::~WSApp()
{
	_log( _debug, _loc, _("Shutting world server down...") );

	//	if( CWorldDB::instance() )
	//		CWorldDB::instance()->save();
// 	if( CLog::instance() )
// 		CLog::instance()->close();
};

//-----------------------------------------------------------------------

void WSApp::loadResourceLocations()
{
	if ( config_->resources().present() )
		for ( ireon::ws::CResources::dir::iterator it = config_->resources().get().dir().begin(); it != config_->resources().get().dir().end(); ++it )
			CResourceManager::instance()->addLocation(*it, "Dir");
};

//-----------------------------------------------------------------------

/// minimal pulse (one game loop cycle) time in milliseconds
const int64 PULSE_TIME = 100;

void WSApp::go()
{
	startRSConnection();
	startPlayerConnectionsManager();

	int64 msecsPassed = 0, msecsThisPulseStart = 0;

	_log( _debug, _loc, _("Starting world server main loop...") );
	//THE MAIN LOOP!
	while(run_)
	{
		msecsThisPulseStart = ireon::misc::getTime();

		if (m_restartRSConnection)
			startRSConnection();

		try
		{
			rsConnectionManager_->processInput();
			playerConnectionsManager_->processInput();
		}
		catch (ireon::net::EConnectionNotExist &)
		{
			_log( _warn, _loc, _("Caught EConnectionNotExist exception.") );
		}
		
		WSWorld::instance()->update(static_cast<uint> (PULSE_TIME));

		msecsPassed = ireon::misc::getTime() - msecsThisPulseStart;
		if (msecsPassed >= PULSE_TIME)
			_log( _debug, _loc, _("World server lagged for %d msec"), (int)msecsPassed - PULSE_TIME );
		else
			CGenericApp::sleep (static_cast<uint> (PULSE_TIME - msecsPassed));
	}
}

//-----------------------------------------------------------------------

void WSApp::startRSConnection()
{
	// commands definitions
	struct CReconnectToRSCommand : public CNetMessageProcessCommand
	{ void execute() { WSApp::instance()->restartRSConnection(); }};

	struct CLoginToRSCommand : public CNetMessageProcessCommand
	{ void execute() { WSApp::instance()->getRSConnection().processPacket(ireon::net_wsrs::wsInitiateLogin, ireon::net::netAddress::atInternalAddress); }	};
	//////////////////////////////////////////////////////////////////////////

	ushort port;
	if ( (config_->rsConnection().present()) && (config_->rsConnection().get().port().present()) )
		port = config_->rsConnection().get().port().present();
	else
		port = ireon::ws::CRSConnection::port::default_value();

	String host;
	if ( (config_->rsConnection().present()) && (config_->rsConnection().get().ip().present()) )
		host = config_->rsConnection().get().ip().present();
	else
		host = ireon::ws::CRSConnection::ip::default_value();

	_log( _info, _loc, _("Creating connection to root server (host: %s, port %d)."), host.c_str(), port );

	rsConnectionManager_.reset(new CClientConnectionsManager<CWSRSConnectionBuilder>);

	rsConnectionManager_->registerClientMessageHandler(ID_CONNECTION_REQUEST_ACCEPTED, new CLoginToRSCommand);

	rsConnectionManager_->registerClientMessageHandler(ID_CONNECTION_ATTEMPT_FAILED, new CReconnectToRSCommand);
	rsConnectionManager_->registerClientMessageHandler(ID_CONNECTION_LOST, new CReconnectToRSCommand);
	rsConnectionManager_->registerClientMessageHandler(ID_DISCONNECTION_NOTIFICATION, new CReconnectToRSCommand);
	rsConnectionManager_->registerClientMessageHandler(ID_NO_FREE_INCOMING_CONNECTIONS, new CReconnectToRSCommand);

	rsConnectionManager_->connect(host.c_str(), static_cast<ushort> (port));

	m_restartRSConnection = false;

	_log( _info, _loc, _("Done.") );
}

//-----------------------------------------------------------------------

void WSApp::startPlayerConnectionsManager()
{
	ushort port;
	if ( (config_->playerConnectionsListener().present()) && (config_->playerConnectionsListener().get().port().present()) )
		port = config_->playerConnectionsListener().get().port().present();
	else
		port = ireon::ws::CPlayerListener::port::default_value();

	ushort maxConnections;
	if ( (config_->playerConnectionsListener().present()) && (config_->playerConnectionsListener().get().maxConnections().present()) )
		maxConnections = config_->playerConnectionsListener().get().maxConnections().present();
	else
		maxConnections = ireon::ws::CPlayerListener::maxConnections::default_value();

	_log( _info, _loc, _("Creating player listener on port %d with maximum %d connections..."), port, maxConnections );

	playerConnectionsManager_.reset(new CServerConnectionsManager<CWSPCConnectionBuilder>);
	playerConnectionsManager_->startListening(port, maxConnections);

	_log( _info, _loc, _("Done.") );
}

//-----------------------------------------------------------------------

CNetConnection& WSApp::getRSConnection() { return rsConnectionManager_->getConnection(); }

