/**
 * @file ireon_rs/root_app.cpp
 * Root server application
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: root_app.cpp 782 2006-10-09 18:51:33Z zak $

 *  See LICENSE for details
 */

#include "ireon_rs/stdafx.h"
#include "ireon_rs/root_app.h"
#include "ireon_rs/xsd/config_xsd.h"
#include "common/db/mysql.h"
#include "common/log/log_listener.h"
#include "common/net/server_connections_manager.h"
#include "ireon_rs/net/rspc_builder.h"
#include "ireon_rs/net/rsws_builder.h"

CRootApp* CRootApp::m_singleton = 0;


CRootApp::CRootApp(String& configPath):
m_run(true),
m_config(ireon::rs::config(configPath))
{
	assert(m_singleton == 0);
	m_singleton = this;

	CGenericApp::initI10N("ireon_rs");

	if (!CGenericApp::printPIDToFile("rs.pid"))
		_log (_warn, _loc, _("PID file was not written!") );
};

//-----------------------------------------------------------------------

CRootApp::~CRootApp()
{
//	_log (_info, _loc, _("Shutting server down...") );
};

//-----------------------------------------------------------------------

void CRootApp::connectToDB()
{
	_log (_info, _loc, _("Starting MySQL database connection...") );

	String db, host;
	ushort port;
	if ( m_config->db().ip().present() )
		host = m_config->db().ip().get();
	else
		host = ireon::rs::CDBconnection::ip::default_value();

	if ( m_config->db().port().present() )
		port = m_config->db().port().get();
	else
		port = ireon::rs::CDBconnection::port::default_value();

	if ( m_config->db().schema().present() )
		db = m_config->db().schema().get();
	else
		db = ireon::rs::CDBconnection::schema::default_value();

	if (!CMySQLConnection::instance()->init(db.c_str(), m_config->db().login().c_str(), m_config->db().password().c_str(),
			host.c_str(), port ))
	{
		_log (_fatal, _loc, _("Could not initialize database connection!") );
		throw EInitError();
	}

	_log (_info, _loc, _("Done.") );
}

//-----------------------------------------------------------------------

void CRootApp::startWSConnectionsManager()
{
	ushort port;
	if ( (m_config->wsConnectionsListener().present()) && (m_config->wsConnectionsListener().get().port().present()) )
		port = m_config->wsConnectionsListener().get().port().get();
	else
		port = ireon::rs::CWSListener::port::default_value();

	ushort maxConnections;
	if ( (m_config->wsConnectionsListener().present()) && (m_config->wsConnectionsListener().get().maxConnections().present()) )
		maxConnections = m_config->wsConnectionsListener().get().maxConnections().get();
	else
		maxConnections = ireon::rs::CWSListener::maxConnections::default_value();

	_log (_info, _loc, _("Creating server for ws connections on port %d with maximum %d connections..."), port, maxConnections );
	
	if (!m_wsConnectionsManager.get())
		m_wsConnectionsManager.reset(new CServerConnectionsManager<CRSWSConnectionBuilder>);
	m_wsConnectionsManager->startListening(port, maxConnections);

	_log (_info, _loc, _("Done.") );
}

//-----------------------------------------------------------------------

void CRootApp::startPlayerConnectionsManager()
{
	ushort port;
	if ( (m_config->playerConnectionsListener().present()) && (m_config->playerConnectionsListener().get().port().present()) )
		port = m_config->playerConnectionsListener().get().port().get();
	else
		port = ireon::rs::CPlayerListener::port::default_value();

	ushort maxConnections;
	if ( (m_config->playerConnectionsListener().present()) && (m_config->playerConnectionsListener().get().maxConnections().present()) )
		maxConnections = m_config->playerConnectionsListener().get().maxConnections().get();
	else
		maxConnections = ireon::rs::CPlayerListener::maxConnections::default_value();

	_log (_info, _loc, _("Creating server for player connections on port %d with maximum %d connections..."), port, maxConnections );

	if (!m_playerConnectionsManager.get())
		m_playerConnectionsManager.reset(new CServerConnectionsManager<CRSPCConnectionBuilder>);
	m_playerConnectionsManager->startListening(port, maxConnections);

	_log (_info, _loc, _("Done.") );
}

//-----------------------------------------------------------------------

void CRootApp::go()
{
	connectToDB();

	// start net servers
	startWSConnectionsManager();
	startPlayerConnectionsManager();

	_log (_info, _loc, _("Starting main loop.") );

	while(m_run)
	{
		m_wsConnectionsManager->processInput();
		m_playerConnectionsManager->processInput();
		CGenericApp::sleep(200);
	}
}

//-----------------------------------------------------------------------

CNetConnection& CRootApp::getWSConnection(ushort wsid)
{
	return m_wsConnectionsManager->getConnectionById(wsid);
}

//-----------------------------------------------------------------------

CNetConnection& CRootApp::getPlayerConnection(uint playerid)
{
	return m_playerConnectionsManager->getConnectionById(playerid);
}

//-----------------------------------------------------------------------


