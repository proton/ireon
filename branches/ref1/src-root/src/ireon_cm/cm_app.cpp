/**
 * @file ireon_cm/cm_app.cpp
 * cluster manager application class
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: cm_app.cpp 579 2006-03-26 12:58:49Z zak $

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

#include "ireon_cm/stdafx.h"
#include "ireon_cm/cm_app.h"
#include "common/xmlconfig.h"
#include "ireon_cm/db/cm_db.h"

#include "common/misc/boost_assign_wrapper.h"

#include "ireon_cm/net/cm_ws_connection.h"
#include "common/net/signal_codes/cmrc_codes.h"

//#ifdef _UNIX_
//#include <unistd.h>
//#include <stdlib.h>
//#endif



CCMApp* CCMApp::m_singleton = 0;


CCMApp::CCMApp():
m_wsConnectionManager(NULL),
m_state(STATE_CONNECTING_ROOT)
{
	assert(m_singleton == 0);
	m_singleton = this;
};

CCMApp::~CCMApp()
{
	assert(m_singleton);
	m_singleton = 0;
};

void CCMApp::init()
{
	CGenericApp::setAppName("cm");
	CGenericApp::initI10N();
	CGenericApp::initLogSystem();

	CLog::instance()->log(CLog::msgLvlVerbose,_("Initializing cluster manager server...\n"));

	if (!CGenericApp::printPIDToFile())
		CLog::instance()->log(CLog::msgLvlWarn,_("PID file was not written!\n"));

	if (!CGenericApp::loadXMLConfigFile("../config/defaults/config.xml", true)) 
	{
		CLog::instance()->log(CLog::msgLvlCritical,_("Config file loading failure!\n"));
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
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlCritical,_("Couldn't initialize net: %s\n"),
						SDLNet_GetError());
		shutdown();
	}

	m_id = atoi(m_config->getFirstValue("/config/id").c_str());;
	if( m_id == 0 )
		m_id = 1;

	m_worlds = m_config->getAllValuesForKey("/config/wsConnections/wsAddress");


	setPulseHandlers();

	CCMDB::init();

	if (!initCMConnectionsManager())
		shutdown();
	/*if (!initInternalServer())
		shutdown();*/

	if (!initRootServerConnection())
		shutdown();
}

//-----------------------------------------------------------------------

bool CCMApp::checkRequiredConfigOptions()
{
	StringVector requiredOptions;
	requiredOptions += "/config/rsConnection/rsAddress", "/config/rsConnection/port", 
						"/config/wsConnections/port", "/config/wsConnections/wsAddress", 
						"/config/wsConnections/maxConnections", "/config/id";
	if (!m_config->checkRequiredOptions(requiredOptions))
		return false;
	return true;
}

//-----------------------------------------------------------------------

bool CCMApp::initCMConnectionsManager()
{
	m_wsConnectionManager = new CIncomingConnectionsManager<CCMWSConnection>;
	int port = atoi(m_config->getFirstValue("/config/wsConnections/port").c_str());
	if ((port<1024) || (port>65535))
		port = atoi(m_config->getFirstDefaultValue("/config/wsConnections/port").c_str());

	int maxConnections = atoi(m_config->getFirstValue("/config/wsConnections/maxConnections").c_str());
	if( !maxConnections )
		maxConnections = atoi(m_config->getFirstDefaultValue("/config/wsConnections/maxConnections").c_str());

	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlInfo,_("Creating server on port %d with maximum %d connections.\n"), port, maxConnections);
	if (! m_wsConnectionManager->start(port, maxConnections) )
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError,_("Error creating server."));
		return false;
	}

	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlInfo,_("Done.\n")); 

	return true;
}

//-----------------------------------------------------------------------

bool CCMApp::initRootServerConnection()
{
	String rootAddress = m_config->getFirstValue("/config/rsConnection/rsAddress");
	int rootPort = atoi(m_config->getFirstValue("/config/rsConnection/port").c_str());
	if ((rootPort<1024) || (rootPort>65535))
		rootPort = atoi(m_config->getFirstDefaultValue("/config/rsConnection/port").c_str());

	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlInfo,_("Connecting to root server(%s:%d)...\n"),rootAddress.c_str(),rootPort);
	m_rsConnectionManager = new COutgoingConnectionsManager<CCMRSConnection>(rootAddress.c_str(), rootPort);
	if (! m_rsConnectionManager->connect() ) 
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlCritical,_("Root server connection failure!\n"));
		return false;
	}
	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlInfo,_("Done.\n")); 

	return true;
}

//-----------------------------------------------------------------------

void CCMApp::go()
{
	while(1)
	{
		(this->*m_pulseHandlers[m_state])();
		CGenericApp::sleep(200);
	}
}

//-----------------------------------------------------------------------

void CCMApp::shutdown()
{
	if (m_config)
		delete m_config;
	if( CCMDB::instance() )
		CCMDB::instance()->save();
	if( CLog::instance() )
		CLog::instance()->close();
	CGenericApp::exitProcess();
}

//-----------------------------------------------------------------------

bool CCMApp::findWorldHost(const String& s)
{
	StringVector::iterator i;
	for( i = m_worlds.begin(); i != m_worlds.end(); i++ )
		if( *i == s )
			return true;
	return false;
};

//-----------------------------------------------------------------------

void CCMApp::setState(State state)
{
	m_state = state;
};

//-----------------------------------------------------------------------

bool CCMApp::setPulseHandlers()
{
	m_pulseHandlers.resize(STATE_COUNT);
	m_pulseHandlers[STATE_CONNECTING_ROOT] = &CCMApp::connectingHandler;
	m_pulseHandlers[STATE_RUN] = &CCMApp::runHandler;
	return true;
}

//-----------------------------------------------------------------------

void CCMApp::connectingHandler()
{
static uint reconnCounter = 0;
	if(m_rsConnectionManager->isConnected())
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlInfo,_("Connected to root server.\n"));
		CData tmp;
		tmp << m_id;
		m_rsConnectionManager->getConnection()->processCommandEx(ireon::net::cmrcCodes::scInitiateLogin, tmp, netAddress(netAddress::atInternalAddress));
		setState(STATE_RUN);
		reconnCounter = 0;
	} else
	{
		++reconnCounter;
		if( reconnCounter > 100 )
		{
			CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlInfo,_("RS connection timeout, trying to reconnect to root server...\n"));
			if (!m_rsConnectionManager->reconnect())
				shutdown();
			reconnCounter = 0;
		};
	}
};

//-----------------------------------------------------------------------

void CCMApp::runHandler()
{
	if( !m_rsConnectionManager->processInput() )
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlWarn,_("Root server input processing failed.\n"));
		setState(STATE_CONNECTING_ROOT);
		m_wsConnectionManager->kickAll();
		return;
	};

	m_wsConnectionManager->processNewConnections();
	m_wsConnectionManager->processInput();

	m_wsConnectionManager->processOutput();
	if( !m_rsConnectionManager->processOutput() )
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlWarn,_("Root server output processing failed.\n"));
		setState(STATE_CONNECTING_ROOT);
		m_wsConnectionManager->kickAll();
		return;
	};
}


