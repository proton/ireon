/**
 * @file ireon_client/client_app.h
 * Application class header file.
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: client_app.h 635 2006-06-02 15:55:11Z zak $

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

#ifndef _CLIENT_APP_H
#define _CLIENT_APP_H

#include "common/generic_app.h"
#include "common/xmlconfig.h"
#include "common/db/client_char_data.h"

#include "common/net/connections_manager.h"
#include "ireon_client/net/pc_rs_connection.h"

//class CPlayerClient;
class CConfig;
namespace Ogre{
class Root;
class RenderWindow;
};
class CClientFrameListener;
struct ClientOwnCharData;

class CClientApp;
typedef bool (CClientApp::*PulseHandler)(Real);

class CClientApp : private CGenericApp
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
		AS_ROOT_CHECK_VERSION,
		AS_LOGGING_ROOT,
		AS_ROOT,
		AS_CONNECTING_WORLD,
		AS_LOGGING_WORLD,
		AS_PLAY,
		AS_DEAD,

		///Must be last
		AS_COUNT
	};

  /**
   * Initializes config path and positions application as "singleton".
   * @param config config file path.
   */
	CClientApp();
	~CClientApp();

private:
	virtual bool initLogSystem();
	bool checkRequiredConfigOptions();
	///Setup resources
	bool setupResources();
	///Configure ogre
	bool configure();
	///Load resources
	bool loadResources();

public:
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

	COutgoingConnectionsManager<CPCRSConnection>* getRSConnectionManager() {return m_rsConnectionManager;} 
//	CPlayerClient* getWorldConn() {return m_worldConn;}
//	CPlayerClient* getRootConn() {return m_rootConn;}

	String getSetting(const String& key);
	void setSetting(const String& key, const String& value);

	void createChar(ClientCharRegistrationData& d);

	/// We are in game
	bool inGame() {return m_state == AS_PLAY;}

	///Perform pulse
	bool pulse(Real time);

//	SDLNet_SocketSet getSocketSet() {return m_socketSet;}


	///Connect to root server
	void connect();


	///Select character
	void selectChar(uint id);

	///Remove character
	void removeChar(uint id);

	///Cancel connecting
	void cancelConnect() {m_cancelConnect = true;}

	Real getFPS();
	uint getTriCount();
	/// Get program version 
	std::vector<byte> getVersion();
	RenderWindow* getRenderWindow() const {return m_window;}

private:

	/// Enter world when logged in
	void enterWorld();

protected:
	/**
   * Doing client connection to server.
   * Checks if player connected to server and if it is so,
   * then it sends message about login to connection object.
   * Otherwise it checks for connection timeout and reconnects player.
   */
	bool connectingHandler(Real time);

  /**
   * Periodically calling during running application.
   * Contains calls about connection input and output.
   */
	bool playHandler(Real time);

	bool rootHandler(Real time);

	bool emptyHandler(Real time) {return true;}

	bool initRootServerConnectionManager();

private:
	static CClientApp* m_singleton; ///< Application single instance ("singleton").

	Ogre::Root* m_ogreRoot;
	CClientFrameListener* m_frameListener;
	Ogre::RenderWindow* m_window;

	State m_state;
	std::vector<PulseHandler> m_pulseHandlers;

	COutgoingConnectionsManager<CPCRSConnection>		*m_rsConnectionManager;

	bool m_cancelConnect;

public:

	std::vector<ClientOwnCharData> m_characters;
};

#endif
