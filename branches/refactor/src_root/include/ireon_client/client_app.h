/**
 * @file ireon_client/client_app.h
 * Application class header file.
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: client_app.h 433 2005-12-20 20:19:15Z zak $

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

#include "db/client_char_data.h"
#include "SDL_net.h"

#include "common/xmlconfig.h"
#include "common/generic_app.h"

// ===========  states  ============
#include "ireon_client/app_states/client_app_state.h"
// =========== /states  ===========

class CPlayerClient;
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
//		AS_CONNECTING_ROOT,
//		AS_LOGGING_ROOT,
		AS_ROOT,
//		AS_CONNECTING_WORLD,
//		AS_LOGGING_WORLD,
		AS_PLAY,

		///Must be last
		AS_COUNT
	};

  /**
   * Initializes config path and positions application as "singleton".
   * @param config config file path.
   */
	CClientApp();
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

	State getState() {return m_thisState;}
	/// state change delayed until pulse end
	void setState(State s);

//	CPlayerClient* getWorldConn() {return m_worldConn;}
//	CPlayerClient* getRootConn() {return m_rootConn;}

	String getSetting(const String& key);
	void setSetting(const String& key, const String& value);

	void createChar(ClientOwnCharData& d);

	/// We are in game
	bool isInGame() {return m_thisState == AS_PLAY;}

	///Perform pulse
	bool pulse(Real time);

	SDLNet_SocketSet getSocketSet() {return m_socketSet;}
	void	checkSockets()	{ SDLNet_CheckSockets(CClientApp::instance()->m_socketSet, 0); }


	///Select character
//	void selectChar(uint id);

	///Remove character
//	void removeChar(uint id);

	///Cancel connecting
//	void cancelConnect() {m_cancelConnect = true;}


private:
	virtual bool initLogSystem();
	bool checkRequiredConfigOptions();
	///Setup resources
	bool setupResources();
	///Configure ogre
	bool configureOgre();
	///Load resources
	bool loadResources();

	/// Enter world when logged in
//	void enterWorld();

	/// actual change of state
	void _changeState();

	static CClientApp*		m_singleton; ///< Application single instance ("singleton").

	CClientAppState*		m_appState;
	State m_thisState;
	State m_nextState;

	Ogre::Root*				m_ogreRoot;
	CClientFrameListener*	m_frameListener;
	Ogre::RenderWindow*		m_window;


	bool m_cancelConnect;

	SDLNet_SocketSet		m_socketSet;


public:

	std::vector<ClientOwnCharData> m_characters;
};

#endif
