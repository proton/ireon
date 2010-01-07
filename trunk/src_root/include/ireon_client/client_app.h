/**
  * @file ireon_client/client_app.h
  * Application class header file.
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: client_app.h 845 2007-01-04 21:48:12Z mip $
 
  *  See LICENSE for details
  */
 
#ifndef _CLIENT_APP_H
#define _CLIENT_APP_H

#include "common/generic_app.h"
#include <Ogre/main/OgreNoMemoryMacros.h>
#include "ireon_client/xsd/config_fwd_xsd.h"
#pragma warning(push, 1)
#include "common/log_mixin.h"
#pragma warning(pop)
#include <Ogre/main/OgreMemoryMacros.h>
#include <memory>

//------------------------------- File Object for python output -----------------------------------
class PythonOutput: private CLogMixin
{
public:
	void write(Ogre::String arg) {_log(_info,_loc,_("%s"),arg.c_str());}
};

template <class T>
class CClientConnectionsManager;

class CPCRSConnectionBuilder;
class CPCWSConnectionBuilder;

class CPlayerClient;
class CClientApp;
class CConfig;
namespace Ogre
{
	class Root;
	class RenderWindow;
};
class CClientFrameListener;
struct ClientOwnCharData;
namespace CEGUI
{
	class PythonScriptModule;
}

typedef void (CClientApp::*PulseHandler)(Real);

#define I_APP (CClientApp::instance())

class CClientApp : private CGenericApp, private CLogMixin
{
public:
	/**
	* Enum that used for indicate state of player.
	* Values of enum used as keys for application handlers such as CClientApp::connectingHandler, CClientApp::playHandler etc.
	*/
	enum State
	{
		AS_MENU   = 0,
		AS_CONNECTING_ROOT,
		AS_CHECKING_VERSION,
		AS_LOGING_ROOT,
		AS_ROOT,
		AS_CONNECTING_WORLD,
		AS_LOGING_WORLD,
		AS_PLAY,
		AS_DEAD,
		AS_EMPTY,
		///Must be last
		AS_COUNT
	};

	/**
	* Initializes config path and positions application as "singleton".
	* @param config config file path.
	*/
	CClientApp(String& configPath);
	~CClientApp();

	/**
	* Main application initialization procedure.
	* It registers log listeners, that are used for saving application operations results,
	* loads config file, connects states with their handlers (CClientApp::connectingHandler, CClientApp::playHandler etc)
	* and creates connection object.
	* Also, it initializes gettext textdomain for translating string constants.
	*/
	bool init();

	/**
	* Application running procedure.
	* It runs endless loop in that depending on state it calls someone of application handler.
	* When player successfully connected it periodically calls CClientApp::playHandler.
	*/
	void go();


	/**
	* Application shutdown procedure. Deletes memory resources obtained by config file
	* and sends disconnect() message to connection object.
	*/
	void shutdown();

	/**
	* Returning application instance.
	* As application positions itself as "singleton" there can be only one instance of it.
	* So that proc creates instance at first time calling and always returns current instance later.
	*/
	static CClientApp* instance() {return m_singleton;}

	///States
	State getState() {return m_state;}
	void setState(State s);

//	CPlayerClient* getWorldConn() {return m_worldConn;}
//	CPlayerClient* getRootConn() {return m_rootConn;}
	
	// ---------------- Config Operation -------------
	ireon::client::CPCConfig * getConfig() { return m_config.get(); }

	// ---------------- Net Command -------------------
	bool inGame() {return m_state == AS_PLAY;}
	void createChar(String name);
	void selectChar(ushort id);
	void removeChar(uint id);
	void refreshListChar();

	//-------------- Utility Function ------------------
	Real getFPS();
	uint getTriCount();
	/// Get program version 
	std::vector<byte> getVersion();

	Ogre::RenderWindow* getRenderWindow() const {return m_window;}
	CEGUI::PythonScriptModule* getScriptModule() const {return m_scriptModule;}
	//std::vector<ClientOwnCharData> m_characters;

	//--------- Net Function --------
	void startRSConnection();
	void startWSConnection();
	void setWorldConnectionParams(String& worldHost, String& token);
	void disconnect();
	void pulse(Real time);

	/// Enter world when logged in
	void enterWorld();

private:
	
	///Perform io net operation
	void pulseNet();
	
	//------------ State Handlers -------
	void checkingVersionHandler(Real time);
	void logingRootHandler(Real time);
	void logingWorldHandler(Real time);
	void connectingRootHandler(Real time);
	void connectingWorldHandler(Real time);
#ifdef _IREON_NO_GRAPH
	void menuHandler(Real time);
#endif // _IREON_NO_GRAPH
	void playHandler(Real time);
	void rootHandler(Real time);
	void emptyHandler(Real time) {time;}
	
	/// initialise client functions
	bool checkRequiredConfigOptions();
	bool configure();
	bool initLogSystem();
	bool loadResources();
	void setPulseHandlers();
	bool setupResources();
	void exportAllToPython();
	void loadScripts();

private:
	std::auto_ptr<CClientConnectionsManager<CPCRSConnectionBuilder>> m_rsConnectionManager;
	std::auto_ptr<CClientConnectionsManager<CPCWSConnectionBuilder>> m_wsConnectionManager;

	ushort m_charId;
	String m_authWorldToken;
	String m_worldHost;
	bool m_restartRSConnection;
	bool m_restartWSConnection;

	std::vector<PulseHandler> m_pulseHandlers;
	State m_state;
	
	Ogre::Root* m_ogreRoot;
	CClientFrameListener* m_frameListener;
	Ogre::RenderWindow* m_window;
	CEGUI::PythonScriptModule* m_scriptModule;

	std::auto_ptr<ireon::client::CPCConfig> m_config;
	static CClientApp* m_singleton; ///< Application single instance ("singleton").
	
}; 
 
 #endif
