/**
 * @file ireon_client/client_app.cpp
 * Contains CClientApp class.
 * CClientApp class source file.
 */


/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: client_app.cpp 635 2006-06-02 15:55:11Z zak $

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


#include "ireon_client/stdafx.h"
#include "ireon_client/version.h"
#include "common/config.h"
#include "ireon_client/client_app.h"
#include "ireon_client/client_frame_listener.h"
#include "ireon_client/world/client_world.h"
#include "common/interface/interface.h"
#include "common/resource/resource_manager.h"
#include "common/win32/mbox_log_listener.h"
#include "common/misc/boost_assign_wrapper.h"

#include "common/net/signal_codes/pcrs_codes.h"

CClientApp* CClientApp::m_singleton = 0;

// ---------------------------------------------------------------

CClientApp::CClientApp():
  m_rsConnectionManager(NULL),
  m_ogreRoot(NULL),
  m_frameListener(NULL),
  m_window(NULL),
  m_cancelConnect(false)
{
	assert(m_singleton == 0);

	m_singleton = this;
};

// ---------------------------------------------------------------

CClientApp::~CClientApp()
{
	assert(m_singleton);

	m_singleton = 0;
	if( m_rsConnectionManager )
		delete m_rsConnectionManager;
	if( m_frameListener )
		delete m_frameListener;
//	if( m_config )
//		delete m_config;
};

// ---------------------------------------------------------------

extern void exportAllToPython();
extern void loadScripts();

// ---------------------------------------------------------------

bool CClientApp::init()
{
	CGenericApp::setAppName("ireon_client");
//	FIXME -- find out wtf
//	do not uncomment this, or gui will not work!!!
//	CGenericApp::initI10N();
	CGenericApp::printPIDToFile();
	CClientApp::initLogSystem();

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

	//=== Setting pulse handlers ===
 	m_pulseHandlers.resize(AS_COUNT);
	m_pulseHandlers[AS_MENU] = &CClientApp::emptyHandler;
	m_pulseHandlers[AS_CONNECTING_ROOT] = &CClientApp::connectingHandler;
	m_pulseHandlers[AS_ROOT_CHECK_VERSION] = &CClientApp::rootHandler;
	m_pulseHandlers[AS_LOGGING_ROOT] = &CClientApp::rootHandler;
	m_pulseHandlers[AS_CONNECTING_WORLD] = &CClientApp::connectingHandler;
	m_pulseHandlers[AS_LOGGING_WORLD] = &CClientApp::playHandler;
	m_pulseHandlers[AS_ROOT] = &CClientApp::rootHandler;
	m_pulseHandlers[AS_PLAY] = &CClientApp::playHandler;
	m_pulseHandlers[AS_DEAD] = &CClientApp::playHandler;

	if ( SDLNet_Init() < 0 )
	{
		CLog::instance()->log(CLog::msgLvlCritical,_("Couldn't initialize net: %s\n"),
						SDLNet_GetError());
		shutdown();
	}

/*
	if( !initRootServerConnectionManager())
	{
		CLog::instance()->log(CLog::msgLvlError,_("Root server connection manager initialization failed.\n"));
		return false;
	} else
		CLog::instance()->log(CLog::msgLvlInfo,_("Root server connection manager initialized.\n"));
*/
/*
	m_worldConn = new CPlayerClient;
	if( !m_worldConn || !(m_worldConn->init(true)) )
	{
		CLog::instance()->log(CLog::msgLvlError,_("Error creating world connection.\n"));
		return false;
	} else
		CLog::instance()->log(CLog::msgLvlInfo,_("World connection created.\n"));
*/

	m_ogreRoot = new Ogre::Root("../config/plugins.cfg","../config/ogre.cfg","../logs/ogre.log");
	if( !setupResources() )
		return false;
	if( !configure() )
		return false;
	if( !loadResources() )
		return false;

	//=== Init world ===
	if( !CClientWorld::instance()->init() )
		return false;

	m_frameListener = new CClientFrameListener();
	m_ogreRoot->addFrameListener(m_frameListener);

	//=== Init interface ===
	if( !CClientInterface::instance()->init(m_window) )
		return false;

	exportAllToPython();
	loadScripts();

	setState(AS_MENU);

	return true;
}

//-----------------------------------------------------------------------

bool CClientApp::initRootServerConnectionManager()
{
	String rootAddress = m_config->getFirstValue("/config/rootServer/host");
	int rootPort = atoi(m_config->getFirstValue("/config/rootServer/port").c_str());
	if ((rootPort<1024) || (rootPort>65535))
		rootPort = atoi(m_config->getFirstDefaultValue("/config/rsConnection/port").c_str());

	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlInfo,_("Initializing root server connection (%s:%d)...\n"),rootAddress.c_str(),rootPort);
	m_rsConnectionManager = new COutgoingConnectionsManager<CPCRSConnection>(rootAddress.c_str(), rootPort);
/*
	if (! m_rsConnectionManager->connect() ) 
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlCritical,_("Root server connection failure!"));
		return false;
	}
*/
	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlInfo,_("Done.\n")); 

	return true;
}

// ---------------------------------------------------------------

bool CClientApp::checkRequiredConfigOptions()
{
	StringVector requiredOptions;
	requiredOptions += "/config/interface/dir", "/config/world/config", "/config/interface/symbols";
	if (!m_config->checkRequiredOptions(requiredOptions))
		return false;
	return true;
}
	
// ---------------------------------------------------------------

bool CClientApp::initLogSystem()
{
	CGenericApp::initLogSystem();
	CLog::instance()->regListener(new CFileLogListener("../logs/log_error.txt",CLog::msgFlagALL,CLog::msgLvlError));
	CLog::instance()->regListener(new CFileLogListener("../logs/log_python.txt",CLog::msgFlagPython));
	CLog::instance()->regListener(new CFileLogListener("../logs/log_resources.txt",CLog::msgFlagResources));
	CLog::instance()->regListener(new CMBoxLogListener());
	CLog::instance()->timeStamp();
	return true;
}

// ---------------------------------------------------------------

void CClientApp::go()
{
	// Create math object to create angle tables
	Math math;
	m_ogreRoot->startRendering();
}

// ---------------------------------------------------------------

bool CClientApp::pulse(Real time)
{
	return (this->*m_pulseHandlers[m_state])(time);
};

// ---------------------------------------------------------------

bool CClientApp::connectingHandler(Real t)
{
	static time_t counter = time(NULL);
	if( m_state == AS_CONNECTING_ROOT )
			{
		if(m_rsConnectionManager->isConnected())
		{
			counter = 0;
			CLog::instance()->log(CLog::msgLvlInfo,_("Connected to root server.\n"));
			CData tmp;
			m_rsConnectionManager->getConnection()->processCommandEx(ireon::net::pcrsCodes::scSendVersionInfo, tmp, netAddress(netAddress::atInternalAddress));
			setState(AS_ROOT_CHECK_VERSION);
		} else
		{
/*			if( counter == 0 )
				counter = time(NULL);
			else if( time(NULL) - counter > 15 )
			{
				counter = 0;
				CLog::instance()->log(CLog::msgLvlInfo,_("Connection timed out.\n"));
				m_rootConn->reconnect();
			};	*/
		}
	} else
	{
		//if(m_worldConn->isConnected())
		//{
		//	counter = 0;
		//	CLog::instance()->log(CLog::msgLvlInfo,_("Connected to world server.\n"));
		//	m_worldConn->login(m_config->getFirstValue("/config/profile/login"),m_config->getFirstValue("/config/profile/password"));
		//	setState(AS_LOGGING_WORLD);
		//} else
		//{
/*			if( counter == 0 )
				counter = time(NULL);
			else if( time(NULL) - counter > 15 )
			{
				counter = 0;
				CLog::instance()->log(CLog::msgLvlInfo,_("Connection timed out.\n"));
				m_worldConn->reconnect();
			};	*/
		//}
		}
	if( m_cancelConnect )
	{
		counter = 0;
		m_cancelConnect = false;
		if( m_state == AS_CONNECTING_WORLD )
			setState(AS_ROOT);
		else if( m_state == AS_CONNECTING_ROOT )
			setState(AS_MENU);
	};
	return true;
};

// ---------------------------------------------------------------

bool CClientApp::playHandler(Real time)
{
	/*
	SDLNet_CheckSockets(m_socketSet, 0);
	if( !m_worldConn->processInput() )
	{
		m_worldConn->disconnect();
		setState(AS_ROOT);
		return true;
	}

	CClientWorld::instance()->update(time);
	CClientInterface::instance()->pulseUpdate(time);

	if( !m_worldConn->processOutput() )
	{
		m_worldConn->disconnect();
		setState(AS_ROOT);
	}
	*/
	return true;
}

// ---------------------------------------------------------------

bool CClientApp::rootHandler(Real time)
{
	static int counter = 0;
	if( !m_rsConnectionManager->processInput() )
	{
//		m_rsConnectionManager->disconnect();
		setState(AS_MENU);
	}
	if( getState() != AS_ROOT && getState() != AS_LOGGING_ROOT && getState() != AS_ROOT_CHECK_VERSION )
		return true;
	if ((getState() == AS_LOGGING_ROOT) && (counter == 0))
	{
		CData tmp;
		tmp << m_config->getFirstValue("/config/profile/login") << m_config->getFirstValue("/config/profile/password");
		m_rsConnectionManager->getConnection()->processCommandEx(ireon::net::pcrsCodes::scSendLoginRequest, tmp, netAddress(netAddress::atInternalAddress));
		++counter;
	}
	if (counter == 1000)
		counter = 0;
	if( !m_rsConnectionManager->processOutput() )
	{
//		m_rsConnectionManager->disconnect();
		setState(AS_MENU);
	}
	return true;
}

// ---------------------------------------------------------------

void CClientApp::shutdown()
{
/*
	if( m_worldConn )
		m_worldConn->disconnect();
	if( m_rootConn )
		m_rootConn->disconnect();
*/
	if( m_config )
		delete m_config;
	exitProcess(0);
};

// ---------------------------------------------------------------

bool CClientApp::setupResources()
{
	// Load resource paths from config file
	CConfig cf;
	cf.load("../config/resources.cfg");

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
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
		seci = cf.getNextSection();
	}

	// Load resource paths from config file
	cf.clear();
	cf.load("../config/ireon_resources.ini");

	// Go through all sections & settings in the file
	seci = cf.getFirstSection();

	while (seci)
	{
		secName = (*seci)->first;
		CConfig::SettingsMultiMap *settings = (*seci)->second;
		CConfig::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			CResourceManager::instance()->addLocation(archName,typeName);
		}
		seci = cf.getNextSection();
	}
return true;
};

// ---------------------------------------------------------------

bool CClientApp::loadResources()
{
	// Initialise, parse scripts etc
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	return true;
};

// ---------------------------------------------------------------

bool CClientApp::configure()
{
	if( !m_ogreRoot )
		return false;
	if(m_ogreRoot->showConfigDialog())
	{
		// If returned true, user clicked OK so initialise
		// Here we choose to let the system create a default rendering window by passing 'true'
		m_window = m_ogreRoot->initialise(true,"Ireon: The real One");
		return true;
	}
	else
	{
		return false;
	}
	return true;
};

// ---------------------------------------------------------------

String CClientApp::getSetting(const String& key)
{
	return m_config->getFirstValue(key);
};

// ---------------------------------------------------------------

void CClientApp::setSetting(const String &key, const String &value)
{
	m_config->setFirstValue(key,value);
};

// ---------------------------------------------------------------

void CClientApp::connect()
{
	if (!initRootServerConnectionManager())
	{
		CLog::instance()->log(CLog::msgLvlError,_("Root server connection manager initialization failed.\n"));
		return;
	} else 
		CLog::instance()->log(CLog::msgLvlInfo,_("Root server connection manager initialized.\n"));

	if (m_rsConnectionManager->connect())
	{
		setState(AS_CONNECTING_ROOT);
	} else
	{
		CLog::instance()->log(CLog::msgLvlError,_("Can't connect to root server.\n"));
		ConnectFailedEventArg arg;
		arg.m_error = _("Can't create connectioin to root server.");
		/*
		arg.m_host = host;
		arg.m_port = port;
		*/
		CEventManager::instance()->execute(ON_CONN_FAILED,&arg);
	}
};

// ---------------------------------------------------------------

void CClientApp::setState(State s)
{

	AppChangeStateEventArg arg;
	arg.m_newState = s;
	arg.m_oldState = m_state;
	CEventManager::instance()->execute(ON_APP_CHANGES_STATE,&arg);
	m_state = s;
	CLog::instance()->log(CLog::msgLvlInfo,"World state changed to ");
	switch( m_state )
	{
		case CClientApp::AS_MENU:
			CLog::instance()->log(CLog::msgLvlInfo,"AS_MENU\n");
			break;
		case CClientApp::AS_PLAY:
			CLog::instance()->log(CLog::msgLvlInfo,"AS_PLAY\n");
			break;
		case CClientApp::AS_ROOT:
			CLog::instance()->log(CLog::msgLvlInfo,"AS_ROOT\n");
			break;
		case CClientApp::AS_CONNECTING_ROOT:
			CLog::instance()->log(CLog::msgLvlInfo,"AS_CONNECTING_ROOT\n");
			break;
		case CClientApp::AS_CONNECTING_WORLD:
			CLog::instance()->log(CLog::msgLvlInfo,"AS_CONNECTING_WORLD\n");
			break;
		case CClientApp::AS_LOGGING_ROOT:
			CLog::instance()->log(CLog::msgLvlInfo,"AS_LOGGING_ROOT\n");
			break;
		case CClientApp::AS_ROOT_CHECK_VERSION:
			CLog::instance()->log(CLog::msgLvlInfo,"AS_ROOT_CHECK_VERSION\n");
			break;
		case CClientApp::AS_LOGGING_WORLD:
			CLog::instance()->log(CLog::msgLvlInfo,"AS_LOGGING_WORLD\n");
			break;
	};
};

// ---------------------------------------------------------------

void CClientApp::createChar(ClientCharRegistrationData& d)
{
//	m_rootConn->createChar(d);
}

// ---------------------------------------------------------------

void CClientApp::selectChar(uint id)
{
//	m_rootConn->selectChar(id);
};

// ---------------------------------------------------------------

void CClientApp::removeChar(uint id)
{
//	m_rootConn->removeChar(id);
};

// ---------------------------------------------------------------

void CClientApp::enterWorld()
{
	CClientWorld::instance()->load();
};

Real CClientApp::getFPS()
{
	return m_window->getLastFPS();
};

uint CClientApp::getTriCount()
{
	return (uint)m_window->getTriangleCount();
};

std::vector<byte> CClientApp::getVersion()
{
	byte bVersion[] = {PRODUCT_VERSION};
	std::vector<byte> vVersion;
	vVersion.assign(bVersion,&bVersion[sizeof(bVersion)]);
	return vVersion;
}