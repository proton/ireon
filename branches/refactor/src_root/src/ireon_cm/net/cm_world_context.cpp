/* Copyright (C) 2005 ireon.org developers council
 * $Id: cm_world_context.cpp 433 2005-12-20 20:19:15Z zak $

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
 * @file ws_player_context.cpp
 * Player connection context on world server
 */

#include "stdafx.h"
#include "cm_app.h"
#include "db/client_char_data.h"
#include "db/account.h"
#include "db/cm_db.h"
#include "net/cm_world_context.h"
#include "net/cm_server.h"
#include "net/cm_client.h"

std::vector<CMWSig> CCMWorldContext::m_signals;

CCMWorldContext::CCMWorldContext(TCPsocket sock, SDLNet_SocketSet socketSet):
CNetClient(sock,socketSet),
m_worldId(0)
{
};

void CCMWorldContext::initSignals()
{
	m_signals.clear();
	m_signals.push_back(&CCMWorldContext::onLogin);
	m_signals.push_back(&CCMWorldContext::onGetAccount);
	m_signals.push_back(&CCMWorldContext::onSendPlayerChar);
	m_signals.push_back(&CCMWorldContext::onNewChar);
	m_signals.push_back(&CCMWorldContext::onRemoveChar);
};

bool CCMWorldContext::processCommand(CData& data, byte id)
{
	if( id >= m_signals.size() )
		return false;
	if( id > 0 && !m_worldId )
		return true; /// We're not logged in
	(this->*m_signals[id])(data);
	return true;
};

/* Network commands */

void CCMWorldContext::sendAccount(CAccount *acc, const char* name, int64 sync)
{
	CData data;
	data.wrt((byte)(SIGNALS_START+1));
	if( !acc )
	{
		data.wrt((byte)0);
		data << name;
	} else
	{
		data.wrt(acc->getId());
		if( acc->getSync() > sync )
		{
			data.wrt((byte)0); /// Isn't up to date
			acc->serialize(data);
		} else
			data.wrt((byte)1); /// Is up to date
	};
	CLog::instance()->log(CLog::msgLvlInfo,_("Sending account data to %s.\n"), getHost());
	write(data);
};

void CCMWorldContext::sendNewChar(ClientOwnCharData *ch,uint acc)
{
	CData data;
	data.wrt((byte)(SIGNALS_START+3));
	ch->serialize(data);
	data << acc;
	write(data);
};

void CCMWorldContext::removeChar(uint id)
{
	CData data;
	data.wrt((byte)(SIGNALS_START+4));
	data << id;
	write(data);
};

/* End network commands */

/* Signal handlers */

void CCMWorldContext::onLogin(CData& data)
{
	uint worldId;
	data >> worldId;
	CData answer;
	answer.wrt((byte)SIGNALS_START);
	if( CCMApp::instance()->findWorldHost(getHost()) )
	{
		if( CCMApp::instance()->server()->findContext(worldId) )
		{
			CLog::instance()->log(CLog::msgLvlInfo,_("Try to log in second world with id %d.\n"),worldId);
			answer.wrt((byte)0);
		} else
		{
			CLog::instance()->log(CLog::msgLvlInfo,_("World with id %d logged in.\n"),worldId);
			answer.wrt((byte)1);
			m_worldId = worldId;
			data >> m_worldPort;
		}
	} else
	{
		CLog::instance()->log(CLog::msgLvlInfo,_("World with id %d failed login.\n"),m_worldId);
		answer.wrt((byte)0);
	}
	write(answer);
};

void CCMWorldContext::onSendPlayerChar(CData &data)
{
};

void CCMWorldContext::onGetAccount(CData& data)
{
	String name;
	int64 sync;
	data >> name;
	data >> sync;
	WaitAccountStruct s;
	s.name = name;
	s.context = this;
	s.sync = sync;
	CCMDB::instance()->removeWaitAccount(this);
	CCMDB::instance()->m_waitAccount.push_back(s);
	AccPtr a = CCMDB::instance()->findAccount(name);
	CCMApp::instance()->client()->getAccount(name.c_str(),a.get() ? a->getSync() : 0);
};

void CCMWorldContext::onNewChar(CData& data)
{
	byte res;
	data >> res;
	if( res != CCE_NONE )
	{
		///It was error, delete char from database
		uint id;
		data >> id;
		CCMDB::instance()->removeCharacter(id);
		CCMApp::instance()->client()->sendNewChar(res,id,0);
	} else
	{
		ClientOwnCharData d;
		d.serialize(data);
		CCMApp::instance()->client()->sendNewChar(res,0,&d);
	};
};

void CCMWorldContext::onRemoveChar(CData& data)
{
	uint id;
	byte res;
	data >> res >> id;
	CCMApp::instance()->client()->removeChar(res,id);
	if( res == 1 )
 		CCMDB::instance()->removeCharacter(id);
};
/* End signal handlers */
