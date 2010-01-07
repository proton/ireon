/**
 * @file app.h
 * World server application class
 */

/* Copyright (C) 2005 ireon.org developers council
  * $Id: world_app.h 806 2006-11-08 14:02:44Z zak $
 
  *  See LICENSE for details
  */
 

#ifndef _WS_APP_H
#define _WS_APP_H
 
#include "common/generic_app.h"
#include <memory>

#include "ireon_ws/xsd/config_fwd_xsd.h"
#include "common/log_mixin.h"

template <class T>
class CServerConnectionsManager;
template <class T>
class CClientConnectionsManager;
class CWSRSConnectionBuilder;
class CWSPCConnectionBuilder;
class CNetConnection;

class WSApp: private CGenericApp, private CLogMixin
{
public:
	WSApp(String &configPath);
	~WSApp();
	static WSApp* instance() {return singleton_;}

	void go();
	void shutdown() { run_ = false; }

	ireon::ws::CWSConfig * getConfig() { return config_.get(); }

	CNetConnection& getRSConnection();
//	CNetConnection& getPlayerConnection(uint playerid);

	/// restarts connection on next cycle
	void restartRSConnection() { m_restartRSConnection = true; };


private:

	static WSApp* singleton_;

	std::auto_ptr<ireon::ws::CWSConfig> config_;

	bool run_;

	std::auto_ptr<CServerConnectionsManager<CWSPCConnectionBuilder> >	playerConnectionsManager_;
	std::auto_ptr<CClientConnectionsManager<CWSRSConnectionBuilder> >	rsConnectionManager_;
	bool m_restartRSConnection;

	// utility functions
	void loadResourceLocations();
	void startRSConnection();
	void startPlayerConnectionsManager();
	void processInput();
};
 
 #endif
