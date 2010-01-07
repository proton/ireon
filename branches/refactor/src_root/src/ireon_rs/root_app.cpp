/**
 * @file root_app.cpp
 * Root server application
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: root_app.cpp 433 2005-12-20 20:19:15Z zak $

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "stdafx.h"
#include "root_app.h"
#include "db/root_db.h"
#include "common/db/mysql_db.h"
#include "common/xmlconfig.h"
#include "net/rs_player_context.h"
#include "net/rs_world_context.h"
#include "net/root_eserver.h"
#include "net/root_iserver.h"

#include "ireon_rs/accounts/user_account.h"

CRootApp* CRootApp::m_singleton = 0;


CRootApp::CRootApp():
m_iserver(NULL),
m_eserver(NULL)
{
	assert(m_singleton == 0);
	m_singleton = this;
};

//-----------------------------------------------------------------------

CRootApp::~CRootApp()
{
	assert(m_singleton);
	m_singleton = 0;
};

//-----------------------------------------------------------------------

void CRootApp::init()
{
	CGenericApp::setAppName("rs");
	CGenericApp::initI10N();
	CGenericApp::initLogSystem();

	CLog::instance()->log(CLog::msgLvlVerbose,_("Initializing root server...\n"));


	if (!CGenericApp::printPIDToFile())
		CLog::instance()->log(CLog::msgLvlWarn,_("PID file was not written!\n"));


	if (!loadXMLConfigFile("../config/defaults/config.xml", true)) 
	{
		CLog::instance()->log(CLog::msgLvlCritical,_("Default configuration file loading failure!\n"));
		shutdown();
	}
	// we have defaults, so it's ok if config.xml is missing
	loadXMLConfigFile("../config/config.xml");
	if (!checkRequiredConfigOptions()) 
	{
		CLog::instance()->log(CLog::msgLvlCritical,_("One or more required options not found!\n"));
		shutdown();
	}


	if ( SDLNet_Init() < 0 )
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlCritical, _("Couldn't initialize net: %s\n"),
						SDLNet_GetError());
		shutdown();
	} 


	CRootDB::init();
	CLog::instance()->log(CLog::msgFlagMysql, CLog::msgLvlVerbose,_("Initializing MySQL database connection...\n"));
	if (!CMySQLConnection::instance()->init(m_config->getFirstValue("/config/database/db").c_str(),
											m_config->getFirstValue("/config/database/user").c_str(),
											m_config->getFirstValue("/config/database/password").c_str(),
											m_config->getFirstValue("/config/database/host").c_str(),
											atoi(m_config->getFirstValue("/config/database/port").c_str())))
	{
		CLog::instance()->log(CLog::msgFlagMysql, CLog::msgLvlCritical,_("Could not initialize database connection!\n"));
		shutdown();
	}
	if (m_config->getFirstValue("/config/database/tableNamesPrefix") != "")
		CMySQLConnection::instance()->setTableNamesPrefix( m_config->getFirstValue("/config/database/tableNamesPrefix") );
	CLog::instance()->log(CLog::msgFlagMysql, CLog::msgLvlVerbose,_("Done.\n"));


	m_clusters = m_config->getAllValuesForKey("/config/internalConnections/cmAddress");


	if (!initInternalServer())
		shutdown();

	if (!initExternalServer())
		shutdown();
	
	CLog::instance()->log(CLog::msgLvlVerbose,_("Done root server initialization.\n"));
}

//-----------------------------------------------------------------------

void CRootApp::go()
{
	while(1)
	{
		m_iserver->processInput();
		m_eserver->processInput();
		m_iserver->processOutput();
		m_eserver->processOutput();
		CGenericApp::sleep(200);
	}
	return;
}

//-----------------------------------------------------------------------

void CRootApp::shutdown()
{
	CLog::instance()->log(CLog::msgLvlVerbose,_("Shutting server down...\n"));
	if( m_config )
		delete m_config;
	if( CRootDB::instance() )
		CRootDB::instance()->save();
	if( CLog::instance() )
		CLog::instance()->close();
	CGenericApp::exitProcess();
}

//-----------------------------------------------------------------------

bool CRootApp::checkRequiredConfigOptions()
{
	StringVector requiredOptions;
	requiredOptions += "/config/internalConnections/port", 
						"/config/internalConnections/maxConnections", 
						"/config/playerConnections/port", 
						"/config/playerConnections/maxConnections", 
						"/config/internalConnections/cmAddress",
						"/config/database/host",
						"/config/database/port",
						"/config/database/db",
						"/config/database/user",
						"/config/database/password";
	if (!m_config->checkRequiredOptions(requiredOptions))
		return false;
	return true;
}

//-----------------------------------------------------------------------

bool CRootApp::initInternalServer()
{
	m_iserver = new CRootIServer;
	int port = atoi(m_config->getFirstValue("/config/internalConnections/port").c_str());
	if ((port<1024) || (port>65535))
		port = atoi(m_config->getFirstDefaultValue("/config/internalConnections/port").c_str());

	int maxConnections = atoi(m_config->getFirstValue("/config/internalConnections/maxConnections").c_str());
	if ((maxConnections<1) || (maxConnections>255))
		maxConnections = atoi(m_config->getFirstDefaultValue("/config/internalConnections/maxConnections").c_str());

	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlInfo,_("Creating internal server on port %d with maximum %d connections...\n"), 
																			port, maxConnections);
	
	if (! m_iserver->start(maxConnections, port) ) 
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlCritical,_("Internal server initialization failure!"));
		return false;
	}
	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlInfo,_("Done.\n")); 
	return true;
}

//-----------------------------------------------------------------------

bool CRootApp::initExternalServer()
{
	m_eserver = new CRootEServer;
	int port = atoi(m_config->getFirstValue("/config/playerConnections/port").c_str());
	if ((port<1024) || (port>65535))
		port = atoi(m_config->getFirstDefaultValue("/config/playerConnections/port").c_str());

	int maxConnections = atoi(m_config->getFirstValue("/config/playerConnections/maxConnections").c_str());
	if ((maxConnections<1) || (maxConnections>255))
		maxConnections = atoi(m_config->getFirstDefaultValue("/config/playerConnections/maxConnections").c_str());

	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlInfo,_("Creating external server on port %d with maximum %d connections...\n"), 
																			port, maxConnections);
	if (! m_eserver->start(maxConnections, port) )
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlCritical,_("External server initialization failure!"));
		return false;
	}
	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlInfo,_("Done.\n"));
	return true;
}

//-----------------------------------------------------------------------

bool CRootApp::findCluster(const String& s)
{
	StringVector::iterator i;
	for( i = m_clusters.begin(); i != m_clusters.end(); i++ )
		if( *i == s )
			return true;
	return false;
};
