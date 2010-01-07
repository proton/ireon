/**
 * @file cm_app.h
 * Cluster manager application class
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: cm_app.h 433 2005-12-20 20:19:15Z zak $

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

#ifndef _CM_APP_H
#define _CM_APP_H

#include "common/generic_app.h"

class CCMServer;
class CCMClient;
class CConfig;
class CCMApp;

typedef void (CCMApp::*PulseHandler)();

class CCMApp : private CGenericApp
{
enum State
{
	STATE_CONNECTING_ROOT = 0,
	STATE_RUN,

	///Must be last
	STATE_COUNT
};

public:
	CCMApp();
	~CCMApp();

	///Initialize application
	void init();
	///Run application
	void go();
	///Get instance
	static CCMApp* instance() {return m_singleton;}

	void shutdown();

public:

	uint getId() {return m_id;}

	bool findWorldHost(const String& host);

	void setState(State state);

	CCMServer* server() {return m_server;}
	CCMClient* client() {return m_client;}

protected:
	///Pulse handlers
	void connectingHandler();

	void runHandler();

protected:

	static CCMApp* m_singleton;

	/// Cluster id
	uint m_id;

	StringVector m_worlds;

	/// State
	byte m_state;
	std::vector<PulseHandler> m_pulseHandlers;

	CCMServer* m_server;
	CCMClient* m_client;

private:
	// some utility functions
	bool checkRequiredConfigOptions();
	bool setPulseHandlers();
	bool connectToRootServer();
	bool initInternalServer();
};

#endif
