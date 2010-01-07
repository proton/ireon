/* Copyright (C) 2005 ireon.org developers council
 * $Id: ws_client.cpp 358 2005-12-07 21:25:22Z llyeli $

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
 * @file ws_client.cpp
 * World server network client (connecting to cluster manager)
 */

#include "stdafx.h"
#include "world_app.h"
#include "net/ws_client.h"
#include "db/account.h"
#include "db/client_char_data.h"
#include "db/world_db.h"
#include "net/ws_player_context.h"
#include "net/world_server.h"
#include "world/world_char_player.h"

void CWSClient::initSignals()
{
	m_signals.clear();
	m_signals.push_back(&CWSClient::onLogin);
	m_signals.push_back(&CWSClient::onSendAccount);
	m_signals.push_back(&CWSClient::onGetChar);
	m_signals.push_back(&CWSClient::onNewChar);
	m_signals.push_back(&CWSClient::onRemoveChar);
};

bool CWSClient::processCommand(CData &data, byte id)
{
	if( id >= m_signals.size() )
		return false;
	(this->*m_signals[id])(data);
	if( data.readEmpty() )
		return false;
	return true;
};

/* Network commands */

void CWSClient::login()
{
	CData data;
	data.wrt((byte)SIGNALS_START);
	data.wrt(CWorldApp::instance()->getId());
	data.wrt(CWorldApp::instance()->getPort());
	write(data);
};

void CWSClient::getAccount(const char* name)
{
	CData data;
	data.wrt((byte)(SIGNALS_START + 1));
	data << name;
	AccPtr a = CWorldDB::instance()->findAccount(name);
	data.wrt(a.get() ? a->getSync() : 0);
	write(data);
};

/* End network commands */

/* Signal handlers */

void CWSClient::onLogin(CData& data)
{
	byte res;
	data >> res;
	if( res )
		CLog::instance()->log(CLog::msgLvlInfo,_("Logged in to cluster manager.\n"));
	else
	{
		CLog::instance()->log(CLog::msgLvlError,_("Login to cluster manager failed.\n"));
		CWorldApp::instance()->shutdown();
	};
};

void CWSClient::onSendAccount(CData& data)
{
	uint id;
	data >> id;
	AccPtr a;
	String name;
	if( id ) ///If account exists
	{
		CLog::instance()->log(CLog::msgLvlInfo,_("Received data for account %d.\n"),id);
		byte upToDate;
		data >> upToDate;
		a = CWorldDB::instance()->findAccount(id);
		if( !upToDate )
		{
			a.reset(new CAccount);
			a->serialize(data);
			a = CWorldDB::instance()->updateAccount(a);
			CLog::instance()->log(CLog::msgLvlInfo,_("Updating account %s.\n"),a->getName());
		} else if( !a.get() )
		{
			CLog::instance()->log(CLog::msgLvlError,_("Can't find account, but cluster manager says it's up to date. Id = %d.\n"),id);
			return;
		} else
			CLog::instance()->log(CLog::msgLvlInfo,_("Account %s is up to date.\n"),a->getName());
		name = a->getName();
	} else
	{
		data >> name;
	};
	std::vector<WaitLoginStruct>::iterator i;
	for( i = CWorldDB::instance()->m_waitLogin.begin(); i != CWorldDB::instance()->m_waitLogin.end(); i++ )
		if( (*i).name == name )
		{
			((CWSPlayerContext*)(*i).context)->enterGame(a);
			CWorldDB::instance()->removeWaitLogin((*i).context);
			break;
		};
};

void CWSClient::onGetChar(CData& data)
{
};

void CWSClient::onNewChar(CData& data)
{
	ClientOwnCharData d;
	uint acc;
	d.serialize(data);
	data >> acc;
	CData answer;
	answer.wrt((byte)(SIGNALS_START+3));
	CharPlayerPtr ch(new CWorldCharPlayer);
	ch->fromClientOwnCharData(d);
	ch->init();
	ch->setAccountId(acc);
	byte res = CWorldDB::instance()->updateCharPlayer(ch);
	answer << res;
	if( res != CCE_NONE )
	{
		answer << d.m_id;
	}
	else
	{
		ch->toClientOwnCharData(d);
		d.serialize(answer);
	}
	write(answer);
};

void CWSClient::onRemoveChar(CData& data)
{
	uint id;
	data >> id;
	CWorldDB::instance()->removeCharPlayer(id);
	CData answer;
	answer.wrt((byte)(SIGNALS_START + 4));
	answer << id;
	answer.wrt((byte)1);
	write(answer);
};
/* End signal handlers */

