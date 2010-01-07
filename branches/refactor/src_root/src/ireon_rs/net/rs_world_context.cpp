/* Copyright (C) 2005 ireon.org developers council
 * $Id: rs_world_context.cpp 433 2005-12-20 20:19:15Z zak $

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
 * @file rs_world_context.cpp
 * World server connection context on root server
 */

#include "stdafx.h"
#include "root_app.h"
#include "net/rs_world_context.h"
#include "net/rs_player_context.h"
#include "net/root_eserver.h"
#include "net/root_iserver.h"
#include "db/root_db.h"
#include "db/client_char_data.h"

std::vector<CRSISig> CRSWorldContext::m_signals;

CRSWorldContext::CRSWorldContext(TCPsocket sock, SDLNet_SocketSet socketSet):
CNetClient(sock,socketSet),
m_clusterId(0)
{
};

void CRSWorldContext::initSignals()
{
	m_signals.clear();
	m_signals.push_back((CRSISig)&CRSWorldContext::onLogin); //0
	m_signals.push_back((CRSISig)&CRSWorldContext::onGetAccount);
	m_signals.push_back((CRSISig)&CRSWorldContext::onSendChar);
	m_signals.push_back((CRSISig)&CRSWorldContext::onSendWorld);
	m_signals.push_back((CRSISig)&CRSWorldContext::onNewChar);
	m_signals.push_back((CRSISig)&CRSWorldContext::onRemoveChar);
};

bool CRSWorldContext::processCommand(CData& data, byte id)
{
	if( id >= m_signals.size() )
		return false;
	if( id > 0 && !m_clusterId )
		return true; /// We're not logged in
	(this->*m_signals[id])(data);
	return true;
};

/* Network commands */

void CRSWorldContext::getWorld(uint id)
{
	CData data;
	data.wrt((byte)(SIGNALS_START + 3));
	data << id;
	write(data);
	CLog::instance()->log(CLog::msgLvlInfo,_("Sending request for world for char with id %d.\n"),id);
};

void CRSWorldContext::sendChar(ClientOwnCharData *c, uint acc)
{
	CData data;
	data.wrt((byte)(SIGNALS_START+4));
	c->serialize(data);
	data << acc;
	write(data);
};

void CRSWorldContext::removeChar(uint id)
{
	CData data;
	data.wrt((byte)(SIGNALS_START+5));
	data << id;
	write(data);
};

/* End network commands */

/* Signal handlers */

void CRSWorldContext::onLogin(CData& data)
{
	uint clusterId;
	data >> clusterId;
	CData answer;
	answer.wrt((byte)SIGNALS_START);
	if( CRootApp::instance()->findCluster(getHost()) )
	{
		if( CRootApp::instance()->iserver()->findContext(clusterId) )
		{
			CLog::instance()->log(CLog::msgLvlInfo,_("Try to log in second cluster manager with id %d.\n"),clusterId);
			answer.wrt((byte)0);
		} else
		{
			m_clusterId = clusterId;
			CLog::instance()->log(CLog::msgLvlInfo,_("Cluster manager with id %d logged in.\n"),m_clusterId);
			answer.wrt((byte)1);
		}
	} else
	{
		CLog::instance()->log(CLog::msgLvlInfo,_("Cluster manager with id %d failed login.\n"),clusterId);
		answer.wrt((byte)0);
	}
	write(answer);
};

void CRSWorldContext::onGetAccount(CData& data)
{
	String name;
	int64 sync;
	data >> name;
	data >> sync;
	CData answer;
	answer.wrt((byte)(SIGNALS_START + 1));
	AccPtr a = CRootDB::instance()->findAccount(name);
	if( !a.get() )
	{
		answer.wrt((uint)0);
		answer << name;
	} else
	{
		answer.wrt(a->getId());
		if(a->checkSync(sync))
		{
			answer.wrt((byte)0); ///Account isn't up to date
			a->serialize(answer);
		} else
			answer.wrt((byte)1); ///Account up to date
		a->needSync();
	}
	CLog::instance()->log(CLog::msgLvlInfo,_("Sending data for account '%s'.\n"),name.c_str());
	write(answer);
};

void CRSWorldContext::onSendChar(CData& data)
{
};

void CRSWorldContext::onSendWorld(CData& data)
{
	String ip;
	uint port, id;
	data >> id;
	data >> ip;
	data >> port;
	CRSPlayerContext* con = CRootApp::instance()->eserver()->findContext(CRootDB::instance()->findPlayer(id) ? CRootDB::instance()->findPlayer(id)->m_account : 0);
	if( con )
		con->sendWorld(ip.c_str(),port);
};

void CRSWorldContext::onNewChar(CData& data)
{
	byte res;
	ClientOwnCharData d;
	data >> res;
	uint id;
	CRSPlayerContext* con;
	if( !res )
	{
		d.serialize(data);
		CLog::instance()->log(CLog::msgLvlInfo,_("New character with id %d created sucsessfully.\n"),d.m_id);
		id = d.m_id;
		RootCharPlayerData* rootData = CRootDB::instance()->findCreating(id);
		if( !rootData )
			res = CCE_OTHER;
		else if( !CRootDB::instance()->addCharacter(rootData,id) )
			res = CCE_OTHER;
	}
	if( res )
	{
		data >> id;
		CLog::instance()->log(CLog::msgLvlError,_("New character with id %d wasn't created!\n"),id);
		con = CRootDB::instance()->findCreating(id) ? 
			CRootApp::instance()->eserver()->findContext(CRootDB::instance()->findCreating(id)->m_account) : 0;
		CRootDB::instance()->deleteCreating(id);
	} else
	{
		con = CRootDB::instance()->findPlayer(id) ?
			CRootApp::instance()->eserver()->findContext(CRootDB::instance()->findPlayer(id)->m_account) : 0;
	}
	if( con )
		con->sendCreatedNewChar(res,d);
};

void CRSWorldContext::onRemoveChar(CData& data)
{
	uint id;
	byte res;
	data >> id >> res;
	RootCharPlayerData* d = CRootDB::instance()->findPlayer(id);
	if( d )
	{
		CRSPlayerContext* con = CRootApp::instance()->eserver()->findContext(d->m_account);
		if( con )
			con->removeChar(id,res);
	}
	if( res == 1 )
		CRootDB::instance()->deleteCharPlayer(id);
};

/* End signal handlers */
