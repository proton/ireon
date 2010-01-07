/**
 * @file root_app.h
 * Contains application class for root server
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: root_app.h 561 2006-03-21 20:11:14Z zak $

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

#ifndef _ROOT_APP_H
#define _ROOT_APP_H

#include "common/xmlconfig.h"
#include "common/generic_app.h"

#include "common/net/connections_manager.h"
#include "ireon_rs/net/rs_player_connection.h"
#include "ireon_rs/net/rs_cm_connection.h"

class CRootApp : private CGenericApp
{
public:
	CRootApp();
	~CRootApp();

	///Initialize application
	void init();
	///Run application
	void go();
	///Get instance
	static CRootApp* instance() {return m_singleton;}

	///Find cluster in list
	bool findCluster(const String& s); 

	CIncomingConnectionsManager<CRSPlayerConnection>* getPlayerConnectionsManager() {return m_playerConnectionManager;} 
	CIncomingConnectionsManager<CRSCMConnection>* getCMConnectionsManager() {return m_cmConnectionManager;} 

	void shutdown();

private:
	static CRootApp* m_singleton;

	///Cluster adresses
	StringVector m_clusters;

	CIncomingConnectionsManager<CRSPlayerConnection>	*m_playerConnectionManager;
	CIncomingConnectionsManager<CRSCMConnection>		*m_cmConnectionManager;

	// some utility functions
	bool checkRequiredConfigOptions();
	bool initCMConnectionsManager();
	bool initPlayerConnectionsManager();
};

#endif
