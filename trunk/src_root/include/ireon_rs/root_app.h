/**
 * @file root_app.h
 * Contains application class for root server
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: root_app.h 780 2006-10-07 16:01:08Z zak $

 *  See LICENSE for details
 */

#ifndef _ROOT_APP_H
#define _ROOT_APP_H

#include "common/generic_app.h"
#include "ireon_rs/xsd/config_fwd_xsd.h"
#include "common/log_mixin.h"
#include <memory>

template <class T>
class CServerConnectionsManager;
class CRSWSConnectionBuilder;
class CRSPCConnectionBuilder;
class CNetConnection;

class CRootApp : private CGenericApp, private CLogMixin
{
public:
	class EInitError {};

	CRootApp(String& configPath);
	~CRootApp();
	static CRootApp* instance() {return m_singleton;}

	void go();
	void shutdown() { m_run = false; }

	ireon::rs::CRSConfig * getConfig() { return m_config.get(); }

	CNetConnection& getWSConnection(ushort wsid);
	CNetConnection& getPlayerConnection(uint playerid);

private:
	bool	m_run;

	std::auto_ptr<ireon::rs::CRSConfig> m_config;
	static CRootApp* m_singleton;

	std::auto_ptr<CServerConnectionsManager<CRSPCConnectionBuilder> >	m_playerConnectionsManager;
	std::auto_ptr<CServerConnectionsManager<CRSWSConnectionBuilder> >	m_wsConnectionsManager;

	// some utility functions
	void connectToDB();
	void startWSConnectionsManager();
	void startPlayerConnectionsManager();
};

#endif
