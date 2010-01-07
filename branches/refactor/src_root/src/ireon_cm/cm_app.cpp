/**
 * @file cm_app.cpp
 * cluster manager application
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: cm_app.cpp 433 2005-12-20 20:19:15Z zak $

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
#include "cm_app.h"
#include "xmlconfig.h"
#include "db/cm_db.h"
#include "net/cm_client.h"
#include "net/cm_world_context.h"
#include "net/cm_server.h"


CCMApp* CCMApp::m_singleton = 0;


CCMApp::CCMApp():
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


	if (!initInternalServer())
		shutdown();

	if (!connectToRootServer())
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

bool CCMApp::initInternalServer()
{
	int port = atoi(m_config->getFirstValue("/config/wsConnections/port").c_str());
	if ((port<1024) || (port>65535))
		port = atoi(m_config->getFirstDefaultValue("/config/wsConnections/port").c_str());

	int maxConnections = atoi(m_config->getFirstValue("/config/wsConnections/maxConnections").c_str());
	if( !maxConnections )
		maxConnections = atoi(m_config->getFirstDefaultValue("/config/wsConnections/maxConnections").c_str());

	m_server = new CCMServer;
	
	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlInfo,_("Creating server on port %d with maximum %d connections.\n"), port, maxConnections);
	if (! m_server->start(maxConnections,port) )
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError,_("Error creating server."));
		return false;
	}

	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlInfo,_("Done.\n")); 

	return true;
}

//-----------------------------------------------------------------------

bool CCMApp::connectToRootServer()
{
	String rootAddress = m_config->getFirstValue("/config/rsConnection/rsAddress");
	int port = atoi(m_config->getFirstValue("/config/rsConnection/port").c_str());
	if ((port<1024) || (port>65535))
		port = atoi(m_config->getFirstDefaultValue("/config/rsConnection/port").c_str());

	m_client = new CCMClient;
	m_client->initSignals();

	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlInfo,_("Connecting to root server(%s:%d)...\n"),rootAddress.c_str(),port);
	if (! m_client->connect(rootAddress.c_str(), port, 0) ) 
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlCritical,_("Connection failure!"));
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
	if(m_client->isConnected())
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlInfo,_("Connected to root server.\n"));
		m_client->login();
		setState(STATE_RUN);
		reconnCounter = 0;
	} else
	{
		++reconnCounter;
		if( reconnCounter > 100 )
		{
			CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlInfo,_("Trying to connect...\n"));
			m_client->reconnect();
			reconnCounter = 0;
		};
	}
};

//-----------------------------------------------------------------------

void CCMApp::runHandler()
{
	if( !m_client->processInput() )
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlWarn,_("Root server input processing failed, reconnecting...\n"));
		setState(STATE_CONNECTING_ROOT);
		m_server->kickAll();
		m_client->reconnect();
		return;
	};
	m_server->processInput();

	m_server->processOutput();
	if( !m_client->processOutput() )
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlWarn,_("Root server output processing failed, reconnecting...\n"));
		setState(STATE_CONNECTING_ROOT);
		m_server->kickAll();
		m_client->reconnect();
		return;
	};
}


