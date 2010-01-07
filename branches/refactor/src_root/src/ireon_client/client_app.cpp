/**
 * @file ireon_client/client_app.cpp
 * Contains CClientApp class.
 * CClientApp class source file.
 */


/* Copyright (C) 2005 ireon.org developers council
 * $Id: client_app.cpp 433 2005-12-20 20:19:15Z zak $

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
#include "config.h"
#include "client_app.h"
#include "client_frame_listener.h"
#include "world/world.h"
#include "interface/interface.h"
#include "resource/resource_manager.h"

#include "ireon_client/app_states/menu_state.h"
#include "ireon_client/app_states/root_connection_state.h"
#include "ireon_client/app_states/world_connection_state.h"


CClientApp* CClientApp::m_singleton = 0;

// ---------------------------------------------------------------

CClientApp::CClientApp():
  m_ogreRoot(NULL),
  m_frameListener(NULL),
  m_window(NULL),
  m_cancelConnect(false),
  m_appState(NULL),
  m_socketSet(NULL)
{
	assert(m_singleton == 0);

	m_singleton = this;
};

// ---------------------------------------------------------------

CClientApp::~CClientApp()
{
	assert(m_singleton);

	m_singleton = 0;
	if( m_frameListener )
		delete m_frameListener;
	if( m_socketSet )
		SDLNet_FreeSocketSet(m_socketSet);
};

// ---------------------------------------------------------------

extern void exportAllToPython();
extern void loadScripts();

// ---------------------------------------------------------------

bool CClientApp::init()
{
	CGenericApp::setAppName("ireon_client");

//	FIXME -- find out wtf
//	do not uncomment this, or gui wont work!!!
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

	if ( SDLNet_Init() < 0 )
	{
		CLog::instance()->log(CLog::msgLvlCritical,_("Couldn't initialize net: %s\n"),
						SDLNet_GetError());
		shutdown();
	}

	//=== Init connection ===
	m_socketSet = SDLNet_AllocSocketSet(2);
	if( !m_socketSet )
	{
		CLog::instance()->log(CLog::msgLvlError,_("Error creating socket set.\n"));
		return false;
	}

	m_ogreRoot = new Ogre::Root("../config/plugins.cfg","../config/ogre.cfg","../logs/ogre.log");
	if( !setupResources() )
		return false;
	if( !configureOgre() )
		return false;
	if( !loadResources() )
		return false;

	//=== Init world ===
	if( !CWorld::instance()->init() )
		return false;

	m_frameListener = new CClientFrameListener();
	m_ogreRoot->addFrameListener(m_frameListener);

	//=== Init interface ===
	if( !CInterface::instance()->init(m_window) )
		return false;

	exportAllToPython();
	loadScripts();

	setState(AS_MENU);
	// don't forget this, as setState do not actually set state
	_changeState();

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
	return true;
}

// ---------------------------------------------------------------

void CClientApp::go()
{
	/// Create math object to create angle tables
	Math math;
	m_ogreRoot->startRendering();
}

// ---------------------------------------------------------------

bool CClientApp::pulse(Real time)
{
	bool t = m_appState->pulse(time);
	if (m_thisState != m_nextState)
		_changeState();
	return t;
};

// ---------------------------------------------------------------

void CClientApp::shutdown()
{
	if (m_appState)
		delete m_appState;
	if( m_config )
		delete m_config;
	this->exitProcess();
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

bool CClientApp::configureOgre()
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
		return false;
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

void CClientApp::setState(State s)
{
	m_nextState = s;
}

// ---------------------------------------------------------------

void CClientApp::_changeState()
{

	AppChangeStateEventArg arg;
	arg.m_newState = m_nextState;
	arg.m_oldState = m_thisState;
	CEventManager::instance()->execute(CEventManager::ON_APP_CHANGES_STATE,&arg);
	CLog::instance()->log(CLog::msgLvlInfo,"World state changed to ");
	switch( m_nextState )
	{
		case CClientApp::AS_MENU:
			if (m_appState)
				delete m_appState;
			m_appState = new CMenuState;
			m_appState->init();
			CLog::instance()->log(CLog::msgLvlInfo,"AS_MENU\n");
			break;
		case CClientApp::AS_ROOT:
			if (m_appState)
				delete m_appState;
			m_appState = new CRootConectionState;
			CLog::instance()->log(CLog::msgLvlInfo,"AS_ROOT\n");
			break;
		case CClientApp::AS_PLAY:
			if (m_appState)
				delete m_appState;
			m_appState = new CWorldConectionState;
			CLog::instance()->log(CLog::msgLvlInfo,"AS_PLAY\n");
			break;
	};
	m_thisState = m_nextState;
};

/*
// ---------------------------------------------------------------

void CClientApp::createChar(ClientOwnCharData& d)
{
	m_rootConn->createChar(d);
}

// ---------------------------------------------------------------

void CClientApp::selectChar(uint id)
{
	m_rootConn->selectChar(id);
};

// ---------------------------------------------------------------

void CClientApp::removeChar(uint id)
{
	m_rootConn->removeChar(id);
};

// ---------------------------------------------------------------

void CClientApp::enterWorld()
{
	CWorld::instance()->load();
};
*/
