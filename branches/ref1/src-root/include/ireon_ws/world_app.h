/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: world_app.h 522 2006-03-06 16:49:30Z zak $

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

/**
 * @file app.h
 * Application class
 */
#ifndef _APP_H
#define _APP_H

#include "xmlconfig.h"

class CWorldServer;
class CWSClient;
class CConfig;
class CWorldApp;

typedef void (CWorldApp::*PulseHandler)();

class CWorldApp
{
enum State
{
	STATE_CONNECTING_CM = 0,
	STATE_RUN,

	///Must be last
	STATE_COUNT
};
public:
	CWorldApp(String config);
	~CWorldApp();

	///Initialize application
	void init();
	///Run application
	void go();
	///Get instance
	static CWorldApp* instance() {return m_singleton;}

	///Load resources
	void loadResources();

	void shutdown();

	String getSetting(const String& key);

	void setState(State state);
public:
	
	uint getId() {return m_id;}
	uint getPort() {return m_port;}

	CWorldServer* server() {return m_server;}
	CWSClient* client() {return m_client;}
protected:
	///Pulse handlers
	void connectingHandler();

	void runHandler();

protected:

	static CWorldApp* m_singleton;

	String m_configPath;
	CXMLConfig* m_config;

	///World server's id
	uint m_id;
	///World server's port
	uint m_port;

	/// State
	byte m_state;
	std::vector<PulseHandler> m_pulseHandlers;

	CWorldServer* m_server;
	CWSClient* m_client;

private:
	// some utility functions
	void initI10N();
	void initLogSystem();
	bool loadConfigFile(const char *filename);
};

#endif
