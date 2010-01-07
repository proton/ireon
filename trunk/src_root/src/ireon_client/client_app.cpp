/**
* @file ireon_client/client_app.cpp
* Contains CClientApp class.
* CClientApp class source file.
*/


/* Copyright (C) 2005 ireon.org developers council
* $Id: client_app.cpp 855 2007-04-27 09:33:36Z mip $

*  See LICENSE for details
*/


#include "ireon_client/stdafx.h"
#include "ireon_client/client_app.h"
#include "ireon_client/version.h"
#include "common/log/log_listener.h"

#include <Ogre/main/OgreNoMemoryMacros.h>
#include "ireon_client/xsd/config_xsd.h"
#include <Ogre/main/OgreMemoryMacros.h>

#include "common/resource/resource_manager.h"
#include "ireon_client/client_frame_listener.h"
#include "ireon_client/interface/client_interface.h"
#include "ireon_client/interface/client_event_manager.h "
#include "ireon_client/world/client_world.h"

#include "common/net/client_connections_manager.h"
#include "ireon_client/net/pcrs_builder.h"
#include "ireon_client/net/pcws_builder.h"
#include "ireon_client/net/pc.h"
#include "common/net/connections/pcrs.h"
#include "common/net/connections/pcws.h"

#include "common/db/client_char_data.h"
#include "ireon_client/python_script_module/python_script_module.h"
#include "Python.h"

//------------------------------ Init Python Module ------------------------------------
extern "C" void init_Event(void);
extern "C" void init_App(void);
extern "C" void init_World(void);
extern "C" void init_Interface(void);
extern "C" void init_Client(void);
extern "C" void init_OgreTypes(void);
extern "C" void init_IreonClient(void);

//------------------------------ Command-handlers for net event ------------------------------------
class CConnectToRSCommand : public CNetMessageProcessCommand
{
public:
	void execute() { CClientApp::instance()->setState(CClientApp::AS_CONNECTING_ROOT); }
};
class CDisconnectCommand : public CNetMessageProcessCommand
{
public:
	void execute() 
	{ 
		CClientApp::instance()->disconnect();
		CClientApp::instance()->setState(CClientApp::AS_MENU);; 
	}
};
class CConnectToWSCommand : public CNetMessageProcessCommand
{
public:
	void execute() { CClientApp::instance()->setState(CClientApp::AS_CONNECTING_WORLD);; }
};

//------------------------------------------------------------------
CClientApp* CClientApp::m_singleton = 0;
//------------------------------------------------------------------
CClientApp::CClientApp(String& configPath):
m_rsConnectionManager(NULL),
m_ogreRoot(NULL),
m_frameListener(NULL),
m_window(NULL),
m_charId(0),
m_restartRSConnection(false),
m_restartWSConnection(false),
m_config(ireon::client::config(configPath))
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
};

// ---------------------------------------------------------------

bool CClientApp::init()
{
	//	FIXME -- find out wtf
	//	do not uncomment this, or gui will not work!!!
	//CGenericApp::initI10N("ireon_client");
	
	CGenericApp::printPIDToFile("client.pid");
	CClientApp::initLogSystem();

#ifdef _DEBUG
	const char *plugins = "../config/plugins_debug.cfg";
#elif defined NDEBUG
	const char *plugins = "../config/plugins_release.cfg";
#else 
#error _DEBUG or NDEBUG must be defined.
#endif

	m_ogreRoot = new Ogre::Root(plugins,"../config/ogre.cfg","../logs/ogre.log");	

	if( !configure() )
	{
		_log(_error, _loc, "Don't found Ogre config. Please run Ireon-Configurator. See for more info ogre.log");
		return false;
	}
	else
		// Here we choose to let the system create a default rendering window by passing 'true'
		m_window = m_ogreRoot->initialise(true,"Ireon: The real One");

	if(!setupResources() || !loadResources() )
		return false;

	//=== Init world ===
	if( !CClientWorld::instance()->init() )
		return false;

	m_frameListener = new CClientFrameListener();
	m_ogreRoot->addFrameListener(m_frameListener);

	m_scriptModule = new CEGUI::PythonScriptModule();
	//--- Init interface ---
	if( !CClientInterface::instance()->init(m_window) )
		return false;
	// --- init python ---
	exportAllToPython();
	loadScripts();
		
	setPulseHandlers();

// temp for test... go to game NOW !
#define TEST_CAMERA
#ifdef TEST_CAMERA
	enterWorld();
	/*ClientOwnCharData* d = new ClientOwnCharData();
	d->m_name = "player";
	d->m_con = 200;
	d->m_fightSpeed = 4;
	d->m_HP = 20;
	d->m_id = 5;
	d->m_int = 1;
	d->m_maxHP = 20;
	d->m_percentHP = 100;
	d->m_velocity = 20;
	d->m_wis = 1;
	d->m_str = 1;
	d->m_sta = 1;
	d->m_luck = 1;
	CClientWorld::instance()->createOwnCharacter(d);
	*/
	setState(AS_PLAY);
#else
	// normal cycle.. going to menu
	setState(AS_MENU);
	// remove it if used python for start connect to rs
	//startRSConnection();
#endif
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

void CClientApp::pulse(Real time)
{
	(this->*m_pulseHandlers[m_state])(time);
	pulseNet();
};

void CClientApp::pulseNet()
{
	try
	{
		if (m_rsConnectionManager.get() && m_rsConnectionManager->isConnected())
			m_rsConnectionManager->processInput();

		if (m_wsConnectionManager.get() && m_wsConnectionManager->isConnected())
			m_wsConnectionManager->processInput();
	}
	catch (ireon::net::EConnectionNotExist&)
	{
		_log (_error, _loc, _("caught exception EConnectionNotExist") );
		setState(AS_MENU);
	}

};

//----------------------- State Handlers ------------------------
// ---------------------------------------------------------------

//----------------------------------------------------------------

void CClientApp::connectingRootHandler(Real)
{
	//TODO: make packet for initiateCheckVersion
	m_rsConnectionManager->getConnection().processPacket(ireon::net::pcrs::initiateVersionCheck, ireon::net::netAddress::atInternalAddress );
	setState(AS_CHECKING_VERSION);
};

//----------------------------------------------------------------

void CClientApp::checkingVersionHandler(Real)
{
};

//---------------------------------------------------------------
void CClientApp::logingRootHandler(Real)
{
	String login;
	String password;
	bool registerAccount;
	
	login = m_config->profile().get().login();
	password = m_config->profile().get().password();
	
	GENERATE_PACKET_TYPE(ireon::net::pc::root::initateAccountRegistrationPacket) registerData;
	registerData.set(ireon::net::pc::root::initateAccountRegistrationPacket::login, login);
	registerData.set(ireon::net::pc::root::initateAccountRegistrationPacket::password, password);
	
	GENERATE_PACKET_TYPE(ireon::net::pc::root::initiateLoginPacket) loginData;
	loginData.set(ireon::net::pc::root::initiateLoginPacket::login, login);
	loginData.set(ireon::net::pc::root::initiateLoginPacket::password, password);

	if ( (m_config->profile().get().registration().present()) )
		registerAccount = m_config->profile().get().registration().get();
	else
		registerAccount = ireon::client::CProfile::registration::default_value();
	
	if (registerAccount)
		m_rsConnectionManager->getConnection().processPacket(ireon::net::pcrs::initateAccountRegistration, registerData.serialize(), ireon::net::netAddress::atInternalAddress );
	else
		m_rsConnectionManager->getConnection().processPacket(ireon::net::pcrs::initiateLogin, loginData.serialize(), ireon::net::netAddress::atInternalAddress );

	setState(AS_EMPTY);	
}

//---------------------------------------------------------------

void CClientApp::rootHandler(Real)
{
		//remove this with work python
	/*static bool temp = false;
	if (!temp)		
	{
		selectChar( 3 );
		temp = true;
	}*/
	if (m_restartWSConnection)
		startWSConnection();	
}

void CClientApp::connectingWorldHandler(Real)
{
	setState(AS_LOGING_WORLD);
};

void CClientApp::logingWorldHandler(Real)
{
	GENERATE_PACKET_TYPE(ireon::net::pc::world::initiateLoginPacket) req;
	req.set(ireon::net::pc::world::initiateLoginPacket::id, m_charId);
	req.set(ireon::net::pc::world::initiateLoginPacket::token, m_authWorldToken);
	m_wsConnectionManager->getConnection().processPacket(ireon::net::pcws::initiateLogin, req.serialize() , ireon::net::netAddress::atInternalAddress );
	setState(AS_EMPTY);
}

void CClientApp::playHandler(Real time)
{

	CClientWorld::instance()->update(time); 
	CClientInterface::instance()->pulseUpdate(time);
}
 
// ---------------------------------------------------------------

void CClientApp::shutdown()
{
	disconnect();
	CGenericApp::exitProcess(0);
};

void CClientApp::setState(State s)
{

	AppChangeStateEventArg arg;
	arg.m_newState = s;
	arg.m_oldState = m_state;
	CEventManager::instance()->execute(ON_APP_CHANGES_STATE,&arg);
	m_state = s;
	_log (_info, _loc, _("World state changed to") );
	switch( m_state )
	{
		case CClientApp::AS_MENU:
			_log (_info, _loc, _("AS_MENU") );
			break;
		case CClientApp::AS_CONNECTING_ROOT:
			_log (_info, _loc, _("AS_CONNECTING_ROOT") );
			break;
		case CClientApp::AS_CHECKING_VERSION:
			_log (_info, _loc, _("AS_CHECKING_VERSION") );
			break;
		case CClientApp::AS_LOGING_ROOT:
			_log (_info, _loc, _("AS_LOGING_ROOT") );
			break;
		case CClientApp::AS_ROOT:
			_log (_info, _loc, _("AS_ROOT") );
			break;
		case CClientApp::AS_CONNECTING_WORLD:
			_log (_info, _loc, _("AS_CONNECTING_WORLD") );
			break;
		case CClientApp::AS_LOGING_WORLD:
			_log (_info, _loc, _("AS_LOGING_WORLD") );
			break;
		case CClientApp::AS_PLAY:
			_log (_info, _loc, _("AS_PLAY") );
			break;
		case CClientApp::AS_EMPTY:
			_log (_debug, _loc, _("AS_EMPTY") );
			break;
	};
};

// ---------------------------------------------------------------

void CClientApp::createChar(String name)
{
	GENERATE_PACKET_TYPE(ireon::net::pc::root::initiateCreateCharPacket) data;

	data.set(ireon::net::pc::root::initiateCreateCharPacket::name, name);
	
	m_rsConnectionManager->getConnection().processPacket(ireon::net::pcrs::initiateCreateChar, data.serialize(), ireon::net::netAddress::atInternalAddress );
}

// ---------------------------------------------------------------

void CClientApp::refreshListChar()
{
	m_rsConnectionManager->getConnection().processPacket(ireon::net::pcrs::initiateListChar, ireon::net::netAddress::atInternalAddress );
};

// ---------------------------------------------------------------

void CClientApp::selectChar(ushort id)
{
	
	m_charId = id;

	GENERATE_PACKET_TYPE(ireon::net::pc::root::initiateSelectCharPacket) data;
	data.set(ireon::net::pc::root::initiateSelectCharPacket::id, id);
	m_rsConnectionManager->getConnection().processPacket(ireon::net::pcrs::initiateSelectChar, data.serialize(), ireon::net::netAddress::atInternalAddress );
};

// ---------------------------------------------------------------

void CClientApp::removeChar(uint id)
{
	id;
	//m_rootConn->removeChar(id);
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

// ---------------------------------------------------------------

void CClientApp::startRSConnection()
{
	
	String rootHost;
	int rootPort;
	if ((m_config->net().present()) &&
		(m_config->net().get().rootServer().present()) &&
		(m_config->net().get().rootServer().get().ip().present()) 	) 
		rootHost = m_config->net().get().rootServer().get().ip().get();		 
	else
		rootHost = ireon::client::CRootServer::ip::default_value();
	
	if ((m_config->net().present()) &&
		(m_config->net().get().rootServer().present()) &&
		(m_config->net().get().rootServer().get().port().present()) 	) 
		rootPort = m_config->net().get().rootServer().get().port().get();		 
	else
		rootPort = ireon::client::CRootServer::port::default_value();

	_log (_info,
		_loc,
		_("startRSConnection: Creating connection to root server (host: %s, port %d)."), 
		rootHost.c_str(), rootPort);
	
	m_rsConnectionManager.reset(new CClientConnectionsManager<CPCRSConnectionBuilder>);
	
	m_rsConnectionManager->registerClientMessageHandler(ID_CONNECTION_REQUEST_ACCEPTED, new CConnectToRSCommand);

	m_rsConnectionManager->registerClientMessageHandler(ID_CONNECTION_ATTEMPT_FAILED, new CDisconnectCommand);
	m_rsConnectionManager->registerClientMessageHandler(ID_CONNECTION_LOST, new CDisconnectCommand);
	m_rsConnectionManager->registerClientMessageHandler(ID_DISCONNECTION_NOTIFICATION, new CDisconnectCommand);
	m_rsConnectionManager->registerClientMessageHandler(ID_NO_FREE_INCOMING_CONNECTIONS, new CDisconnectCommand);
	m_restartRSConnection = false;
	try
	{
		m_rsConnectionManager->connect(rootHost.c_str(), static_cast<ushort>(rootPort));
	}
	catch (ireon::net::EUnableToInitializeClient)
	{
		_log(_error, _loc,_("startRSConnection: Can't create connection to root server."));

		ConnectFailedEventArg arg;
		arg.m_error = _("startRSConnection: Can't initialise client connection to root server.");
		arg.m_host = rootHost;
		arg.m_port = static_cast<ushort>(rootPort);

		CEventManager::instance()->execute(ON_CONN_FAILED,&arg);

		return;
	}
	_log(_info, _loc, _("Done.")); 
	_log(_info, _loc, _("Connected to root server.\n"));
	return;
}

// ---------------------------------------------------------------

void CClientApp::startWSConnection()
{

	int worldPort;
	if (m_rsConnectionManager->isConnected())
		m_rsConnectionManager->disconnect();
	if( !m_worldHost.length() )
	{
		_log (_error, _loc, _("Can't connect to world server. Host isn't specified.\n"));
		return;
	}
	
	if ((m_config->net().present()) &&
		(m_config->net().get().worldServer().present()) &&
		(m_config->net().get().worldServer().get().port().present())) 
		worldPort = m_config->net().get().worldServer().get().port().get();		 
	else
		worldPort = ireon::client::CWorldServer::port::default_value();

	_log(_info,
		_loc,
		_("Creating connection to world server (host: %s, port %d)."), 
		m_worldHost.c_str(), worldPort);

	m_restartWSConnection = false;
	m_wsConnectionManager.reset(new CClientConnectionsManager<CPCWSConnectionBuilder>);

	m_wsConnectionManager->registerClientMessageHandler(ID_CONNECTION_REQUEST_ACCEPTED, new CConnectToWSCommand);

	m_wsConnectionManager->registerClientMessageHandler(ID_CONNECTION_ATTEMPT_FAILED, new CDisconnectCommand);
	m_wsConnectionManager->registerClientMessageHandler(ID_CONNECTION_LOST, new CDisconnectCommand);
	m_wsConnectionManager->registerClientMessageHandler(ID_DISCONNECTION_NOTIFICATION, new CDisconnectCommand);
	m_wsConnectionManager->registerClientMessageHandler(ID_NO_FREE_INCOMING_CONNECTIONS, new CDisconnectCommand);

	try
	{
		m_wsConnectionManager->connect(m_worldHost.c_str(), static_cast<ushort>(worldPort));
	}
	catch (ireon::net::EUnableToInitializeClient)
	{
		_log (_info, _loc, _("Can't create connection to world server."));

		ConnectFailedEventArg arg;
		arg.m_error = _("startWSConnection: Can't initialise client connection to world server.");
		arg.m_host = m_worldHost;
		arg.m_port = static_cast<ushort>(worldPort);

		CEventManager::instance()->execute(ON_CONN_FAILED,&arg);

		return;
	}
	_log (_info, _loc, _("Done.")); 
	_log (_info, _loc, _("Connected to world server.\n"));
	return;
}
void CClientApp::disconnect()
{
	if (m_rsConnectionManager.get() && m_rsConnectionManager->isConnected())
		m_rsConnectionManager->disconnect();

	if (m_wsConnectionManager.get() && m_wsConnectionManager->isConnected())
		m_wsConnectionManager->disconnect();

}

///-------------------initialise client functions--------------
// ---------------------------------------------------------------

bool CClientApp::checkRequiredConfigOptions()
{
	//StringVector requiredOptions;
	//requiredOptions.push_back("/config/interface/dir");
	//requiredOptions.push_back("/config/world/config");
	//requiredOptions.push_back("/config/interface/symbols");
	//if (!m_config->checkRequiredOptions(requiredOptions))
	//	return false;
	return true;
}

// ---------------------------------------------------------------

bool CClientApp::configure()
{
	return  m_ogreRoot && m_ogreRoot->restoreConfig();
};

// ---------------------------------------------------------------

bool CClientApp::initLogSystem()
{
	return true;
}

// ---------------------------------------------------------------

bool CClientApp::loadResources()
{
	// Initialise, parse scripts etc
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	return true;
};

///------------------------------------------------------------

void CClientApp::setPulseHandlers()
{
	//=== Setting pulse handlers ===
	m_pulseHandlers.resize(AS_COUNT);

	m_pulseHandlers[AS_MENU] = &CClientApp::emptyHandler;
	m_pulseHandlers[AS_CONNECTING_ROOT] = &CClientApp::connectingRootHandler;
	m_pulseHandlers[AS_CHECKING_VERSION] = &CClientApp::checkingVersionHandler;
	m_pulseHandlers[AS_LOGING_ROOT] = &CClientApp::logingRootHandler;
	m_pulseHandlers[AS_CONNECTING_WORLD] = &CClientApp::connectingWorldHandler;
	m_pulseHandlers[AS_LOGING_WORLD] = &CClientApp::logingWorldHandler;
	m_pulseHandlers[AS_ROOT] = &CClientApp::rootHandler;
	m_pulseHandlers[AS_PLAY] = &CClientApp::playHandler;
	m_pulseHandlers[AS_DEAD] = &CClientApp::playHandler;
	m_pulseHandlers[AS_EMPTY] = &CClientApp::emptyHandler;

}

// ---------------------------------------------------------------

bool CClientApp::setupResources()
{
	if ( m_config->resources().present() )
		for ( ireon::client::CResources::resource::iterator 
				it = m_config->resources().get().resource().begin();
				it != m_config->resources().get().resource().end();
				++it )
			if (it->category() == "Ireon")
				CResourceManager::instance()->addLocation( it->type() , (*it) );
			else
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation( (*it), it->type() , it->category() );
	
	return true;
};

//------------------------------------------------------------------

void CClientApp::setWorldConnectionParams(String& worldHost, String& token)
{
	m_worldHost = worldHost;
	m_authWorldToken = token;
	m_restartWSConnection = true;
}
//------------------------------------------------------------------
void CClientApp::exportAllToPython()
{
	_log(_info,_loc,"Export modules to python... ");

	init_IreonClient();

	//_log(_error,_loc,_("Can't initialize python."));
	//shutdown();
	_log(_info,_loc,"Export modules completed.");
}
//------------------------------------------------------------------
void CClientApp::loadScripts()
{
	_log(_info,_loc,"Init loading scripts.");
	
	if ( m_config->_interface().present() )
		for ( ireon::client::CInterfaceSettings::script::iterator 
				it = m_config->_interface().get().script().begin();
				it != m_config->_interface().get().script().end();
				++it )
			if ( it->type() == "py")
			{	
				_log(_info,_loc,("Execute script file : %s"),(*it).c_str());
				m_scriptModule->executeScriptFile( (*it), it->category());
			}
	_log(_info,_loc,"Loading scripts completed.");

}

