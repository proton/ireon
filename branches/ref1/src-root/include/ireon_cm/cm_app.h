/**
 * @file cm_app.h
 * Cluster manager application class
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: cm_app.h 579 2006-03-26 12:58:49Z zak $

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

#ifndef _CMAPP_H
#define _CMAPP_H

#include "generic_app.h"

#include "common/net/connections_manager.h"
#include "ireon_cm/net/cm_ws_connection.h"
#include "ireon_cm/net/cm_rs_connection.h"

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
	
	COutgoingConnectionsManager<CCMRSConnection>* getRSConnectionManager() {return m_rsConnectionManager;} 
	CIncomingConnectionsManager<CCMWSConnection>* getWSConnectionsManager() {return m_wsConnectionManager;} 

	void shutdown();

public:

	uint getId() {return m_id;}

	bool findWorldHost(const String& host);

	void setState(State state);

private:
	///Pulse handlers
	void connectingHandler();

	void runHandler();

	static CCMApp* m_singleton;

	/// Cluster id
	uint m_id;

	StringVector m_worlds;

	/// State
	byte m_state;
	std::vector<PulseHandler> m_pulseHandlers;

	COutgoingConnectionsManager<CCMRSConnection>		*m_rsConnectionManager;
	CIncomingConnectionsManager<CCMWSConnection>		*m_wsConnectionManager;

	// some utility functions
	bool checkRequiredConfigOptions();
	bool setPulseHandlers();
	bool initRootServerConnection();
	bool initCMConnectionsManager();
	//bool initInternalServer();
};

#endif
