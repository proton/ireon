/**
 * @file ws_app.cpp
 * world server application
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: ws_app.cpp 522 2006-03-06 16:49:30Z zak $

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
#include "world_app.h"
#include "config.h"
#include "xmlconfig.h"
#include "net/ws_player_context.h"
#include "net/world_server.h"
#include "net/ws_client.h"
#include "db/world_db.h"
#include "resource/resource_manager.h"
#include "world/world.h"

#include "file/file.h"
#include "miscutils.h"

#ifdef __UNIX__
#include <unistd.h>
#include <stdlib.h>
#endif


CWorldApp* CWorldApp::m_singleton = 0;


CWorldApp::CWorldApp(String config):
m_config(NULL),
m_state(STATE_CONNECTING_CM),
m_configPath(config)
{
	assert(m_singleton == 0);
	m_singleton = this;
};

CWorldApp::~CWorldApp()
{
	assert(m_singleton);
	m_singleton = 0;
};

void CWorldApp::init()
{
	initI10N();
	initLogSystem();

	CLog::instance()->log(CLog::msgLvlVerbose,_("Initializing world server...\n"));

	if (!ireon::misc::printPIDToFile("ws.pid"))
		CLog::instance()->log(CLog::msgLvlWarn,_("PID file was not written!\n"));

	m_config = new CXMLConfig;
	if (!loadConfigFile("../config/config.xml")) 
	{
		CLog::instance()->log(CLog::msgLvlCritical,_("Config file loading failure!\n"));
		shutdown();
	}

	if ( SDLNet_Init() < 0 )
	{
		CLog::instance()->log(CLog::msgLvlCritical,_("Couldn't initialize net: %s\n"),
						SDLNet_GetError());
		shutdown();
	}

	m_port = atoi(m_config->getFirstValue("/config/Port").c_str());
	int maxCon = atoi(m_config->getFirstValue("/config/MaxConnections").c_str());
	m_id = atoi(m_config->getFirstValue("/config/ID").c_str());
	if( m_id == 0 )
		m_id = 1;
	String cm = m_config->getFirstValue("/config/CM");
	int cmPort = atoi(m_config->getFirstValue("/config/CMPort").c_str());


	if( !maxCon )
		maxCon = 1;

	/*=== Setting pulse handlers ===*/
	m_pulseHandlers.resize(STATE_COUNT);
	m_pulseHandlers[STATE_CONNECTING_CM] = &CWorldApp::connectingHandler;
	m_pulseHandlers[STATE_RUN] = &CWorldApp::runHandler;

	loadResources();

	if( !CWorldDB::init() )
	{
		CLog::instance()->log(CLog::msgLvlError,_("Error initialization world DB.\n"));
		shutdown();
	};

	if( !CWorld::instance()->init() )
	{
		CLog::instance()->log(CLog::msgLvlError,_("Error initialization world.\n"));
		shutdown();
	};

	Vector3 norm = CWorld::instance()->normalAt(0,0);
	CLog::instance()->log(CLog::msgLvlInfo,"(%f,%f,%f)\n",norm.x,norm.y,norm.z);
	norm = CWorld::instance()->normalAt(10,10);
	CLog::instance()->log(CLog::msgLvlInfo,"(%f,%f,%f)\n",norm.x,norm.y,norm.z);
	norm = CWorld::instance()->normalAt(100,100);
	CLog::instance()->log(CLog::msgLvlInfo,"(%f,%f,%f)\n",norm.x,norm.y,norm.z);
	norm = CWorld::instance()->normalAt(-50,-50);
	CLog::instance()->log(CLog::msgLvlInfo,"(%f,%f,%f)\n",norm.x,norm.y,norm.z);
	norm = CWorld::instance()->normalAt(10,1000);
	CLog::instance()->log(CLog::msgLvlInfo,"(%f,%f,%f)\n",norm.x,norm.y,norm.z);
	norm = CWorld::instance()->normalAt(-139,50);
	CLog::instance()->log(CLog::msgLvlInfo,"(%f,%f,%f)\n",norm.x,norm.y,norm.z);
	m_server = new CWorldServer;
	CLog::instance()->log(CLog::msgLvlInfo,_("Creating server on port %d with maximum %d connections.\n"),m_port,maxCon);
	if (! m_server->start(maxCon,m_port) )
	{
		CLog::instance()->log(CLog::msgLvlError,_("Error creating server.\n"));
		shutdown();
	}
	m_client = new CWSClient;
	m_client->initSignals();
	CLog::instance()->log(CLog::msgLvlInfo,_("Connecting to cluster manager(%s:%d).\n"),cm.c_str(),cmPort);
	if (!m_client->connect(cm.c_str(),cmPort,0))
	{
		CLog::instance()->log(CLog::msgLvlError,_("Error connect to cluster manager.\n"));
		shutdown();
	};
}

//-----------------------------------------------------------------------

void CWorldApp::initI10N()
{
	//Gettext initialization
	setlocale( LC_ALL, "" );
	bindtextdomain( "ireon", "locale" );
	textdomain( "ireon" );
	return;
}

//-----------------------------------------------------------------------

void CWorldApp::initLogSystem()
{
	CLog::instance()->regListener(new CFileLogListener("../logs/log_common.txt",CLog::msgFlagNone));
	CLog::instance()->regListener(new CFileLogListener("../logs/log_net.txt",CLog::msgFlagNetwork));
	CLog::instance()->regListener(new CFileLogListener("../logs/log_walk.txt",CLog::msgFlagResources));
	CLog::instance()->regListener(new CFileLogListener("../logs/log_visible.txt",CLog::msgFlagMysql));
	CLog::instance()->regListener(new CScreenLogListener("ws"));
	return;
}

//-----------------------------------------------------------------------

bool CWorldApp::loadConfigFile(const char *filename)
{
	// to be moved
	FilePtr f(new CFile);
	if( !f )
		return false;
	f->open(filename,"r");
	if( !f->isOpen() )
		return false;

	char* configFileContents;
	configFileContents = new char[f->getSize()];
	f->read(configFileContents,f->getSize());

	// ----
	if (!m_config->load(configFileContents,false,"/"))
		return false;

	StringVector requiredOptions;
	requiredOptions += "/config/CM", "/config/Port", "/config/CMPort", "/config/ID";
	if (!m_config->checkRequiredOptions(requiredOptions))
		return false;
	return true;
}

//-----------------------------------------------------------------------

void CWorldApp::go()
{
	#ifdef __WIN32__
	DWORD now, last_pass;
	#else
	struct timeval now, last_pass;
	#endif
	int64 msecsSleep = 0;
	int missed_pulses = 0;
	int64 msecsPassed = 0; // In curren cycle
	int64 msecsMissed = 0; // Need to come up
	int64 msecsLag = 0; // Lag in curren cycle

	#ifdef __WIN32__
	now = timeGetTime();
	last_pass = timeGetTime();
	#else
	gettimeofday( &now, (struct timezone *) 0 );
	gettimeofday( &last_pass, (struct timezone *) 0 );
	#endif

	///To init angle tables
	Math math;

	CLog::instance()->log(CLog::msgLvlInfo,_("Begin game loop.\n"));
	//THE MAIN GAME LOOP!
	while(1)
	{
		/// Perform pulse
		(this->*m_pulseHandlers[m_state])();

		#ifdef __WIN32__
		now = timeGetTime();
		msecsPassed = now - last_pass; /// If now or last_pass will overflow, result still will be corect
		#else
		gettimeofday( &now, (struct timezone *) 0 );
		msecsPassed = (now.tv_sec - last_pass.tv_sec)*1000 + (now.tv_usec-last_pass.tv_usec)/1000;
		#endif
		// Increase counter of time passed
		msecsMissed += msecsPassed;

		// Increase lag counter if we're in lag
		if (msecsPassed > PULSE_TIME)
			msecsLag += msecsPassed;

		// Protection from some issues with time
		if (msecsMissed < 0) msecsMissed = 0;

		// Protection from some issues with time or too long lags in some reason
		if (msecsMissed > 5000)
		{
			CLog::instance()->log(CLog::msgLvlInfo,_("Server lag is too big (%u sec), let it be equal to 5s\n"),(int)(msecsMissed/1000));
			msecsMissed = 5000;
		}

		// If time difference less then one pulse, sleep
		if (msecsMissed < PULSE_TIME )
		{
		#ifdef __WIN32__
			Sleep (PULSE_TIME - msecsMissed);
			last_pass = now + PULSE_TIME - msecsMissed;
		#else
			usleep (1000 * (PULSE_TIME - msecsMissed));
			last_pass = now;
			last_pass.tv_usec += (PULSE_TIME - msecsMissed)*1000;
			while( last_pass.tv_usec >= 1000000 )
			{
				last_pass.tv_usec -= 1000000;
				++last_pass.tv_sec;
			};
		#endif
			msecsMissed = 0;
			// If it was lag, log it
			if (msecsLag)
			{
				CLog::instance()->log(CLog::msgLvlInfo,_("Server lagged for %u msec (%u pulse)\n"),(int)msecsLag,(int)(msecsLag/PULSE_TIME));
				msecsLag = 0;
			}
		//Current time
		} else
		{
			// Worked one pulse
			msecsMissed-=PULSE_TIME;
			// Current time
			last_pass = now;
		}
		if( m_state == CWorldApp::STATE_RUN )
			CWorld::m_pulse++;
	}
	return;
}

void CWorldApp::connectingHandler()
{
static uint reconnCounter = 0;
	if(m_client->isConnected())
	{
		CLog::instance()->log(CLog::msgLvlInfo,_("Connected to cluster manager.\n"));
		m_client->login();
		setState(STATE_RUN);
		reconnCounter = 0;
	} else
	{
		++reconnCounter;
		if( reconnCounter > 100 )
		{
			CLog::instance()->log(CLog::msgLvlInfo,_("Trying to connect...\n"));
			m_client->reconnect();
			reconnCounter = 0;
		};
	}
};

void CWorldApp::runHandler()
{
	if( !m_client->processInput() )
	{
		m_client->reconnect();
		setState(STATE_CONNECTING_CM);
		m_server->kickAll();
		return;
	};
	m_server->processInput();


	CWorld::instance()->update(CWorld::m_pulse);

	if( CWorld::m_processOutputPulse )
		m_server->processOutput();

	if( !m_client->processOutput() )
	{
		m_client->reconnect();
		setState(STATE_CONNECTING_CM);
		m_server->kickAll();
		return;
	};
}

void CWorldApp::shutdown()
{
	if( m_config )
		delete m_config;
	if( CWorldDB::instance() )
		CWorldDB::instance()->save();
	if( CLog::instance() )
		CLog::instance()->close();
	#ifdef __WIN32__
	ExitProcess(0);
	#elif defined __UNIX__
	exit(0);
	#endif
}

void CWorldApp::loadResources()
{
	CConfig cf;
	cf.load("resources.cfg");

		// Go through all sections & settings in the file
	CConfig::SectionIterator* seci = cf.getFirstSection();

	String secName, typeName, archName;
	while (seci)
	{
		secName = (*seci)->first;
		CConfig::SettingsMultiMap *settings = (*seci)->second;
		CConfig::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			CResourceManager::instance()->addLocation(
				archName, typeName );
		}
		seci = cf.getNextSection();
	}
};

String CWorldApp::getSetting(const String& key)
{
	return m_config->getFirstValue(key);
};


void CWorldApp::setState(State state)
{
	m_state = state;
};
