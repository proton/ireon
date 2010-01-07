/* Copyright (C) 2005 ireon.org developers council
 * $Id: cm_client.cpp 314 2005-11-27 19:54:19Z zak $

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
 * @file cm_client.cpp
 * Cluster manager network client (connecting to root server)
 */

#include "stdafx.h"
#include "cm_app.h"
#include "db/cm_db.h"
#include "db/account.h"
#include "db/client_char_data.h"
#include "net/cm_client.h"
#include "net/cm_world_context.h"
#include "net/cm_server.h"


void CCMClient::initSignals()
{
	m_signals.clear();
	m_signals.push_back(&CCMClient::onLogin);
	m_signals.push_back(&CCMClient::onSendAccount);
	m_signals.push_back(&CCMClient::onGetChar);
	m_signals.push_back(&CCMClient::onGetWorld);
	m_signals.push_back(&CCMClient::onNewChar);
	m_signals.push_back(&CCMClient::onRemoveChar);
};

bool CCMClient::processCommand(CData &data, byte id)
{
	if( id >= m_signals.size() )
		return false;
	(this->*m_signals[id])(data);
	return true;
};

/* Network commands */

void CCMClient::login()
{
	CData data;
	data.wrt((byte)SIGNALS_START);
	data.wrt(CCMApp::instance()->getId());
	write(data);
};

void CCMClient::getAccount(const char* name, int64 sync)
{
	CData data;
	data.wrt((byte)(SIGNALS_START + 1));
	data << name;
	data << sync;
	write(data);
};

void CCMClient::sendNewChar(byte res, uint id, ClientOwnCharData *data)
{
	CData answer;
	answer.wrt((byte)(SIGNALS_START + 4));
	answer << res;
	if( res != CCE_NONE )
	{
		answer.wrt(id);
	} else
	{
		if( !data )
			return;
		data->serialize(answer);
	}
	write(answer);
};

void CCMClient::removeChar(byte res, uint id)
{
	CData answer;
	answer.wrt((byte)(SIGNALS_START + 5));
	answer << res << id;
	write(answer);
};

/* End network commands*/

/* Network signals */

void CCMClient::onLogin(CData& data)
{
	byte res;
	data >> res;
	if( res )
		CLog::instance()->log(CLog::msgLvlInfo,_("Logged in to root server.\n"));
	else
	{
		CLog::instance()->log(CLog::msgLvlError,_("Login to root server failed.\n"));
		CCMApp::instance()->shutdown();
	};
};

void CCMClient::onSendAccount(CData& data)
{
	uint id;
	data >> id;
	AccPtr a;
	String name;
	if( id )
	{
		a = CCMDB::instance()->findAccount(id);
		byte upToDate;
		data >> upToDate;
		if( !upToDate )
		{
			if( !a.get() )
				a.reset(new CAccount);
			a->serialize(data);
			a = CCMDB::instance()->updateAccount(a);
		} else
			if( !a.get() )
			{
				CLog::instance()->log(CLog::msgLvlError,_("Can't find account in database, but root server says it's up to date. Id = %d\n"),id);
				return;
			};

		name = a->getName();
	} else
	{
		data >> name;
	}
	std::vector<WaitAccountStruct>::iterator i;
	for( i = CCMDB::instance()->m_waitAccount.begin(); i != CCMDB::instance()->m_waitAccount.end(); i++ )
		if( (*i).name == name )
		{
			(*i).context->sendAccount(a.get(),name.c_str(),(*i).sync);
			CCMDB::instance()->removeWaitAccount((*i).context);
			break;
		};
};

void CCMClient::onGetChar(CData& data)
{
};

void CCMClient::onGetWorld(CData& data)
{
	uint id;
	data >> id;
	uint world = CCMDB::instance()->getWorld(id);
	
	CCMWorldContext* con = CCMApp::instance()->server()->findContext(world);
	const char* ip;
	uint port;
	if( con )
	{
		ip = con->getHost();
		port = con->getWorldPort();
	} else
	{
		ip = "";
		port = 0;
	}
	CData answer;
	answer.wrt((byte)(SIGNALS_START+3));
	answer << id;
	answer << ip;
	answer << port;
	write(answer);
};

void CCMClient::onNewChar(CData& data)
{
	ClientOwnCharData ch;
	uint acc;
	ch.serialize(data);
	data >> acc;
	byte res = CCMDB::instance()->newCharacter(&ch,acc);
	if( res != CCE_NONE )
		sendNewChar(res,ch.m_id,NULL);
};

void CCMClient::onRemoveChar(CData& data)
{
	uint id;
	data >> id;
	uint world = CCMDB::instance()->getWorld(id);
	CCMWorldContext* con = CCMApp::instance()->server()->findContext(world);
	if( !con )
		removeChar(id, 0);
	else
		con->removeChar(id);
};
/* End network signals */
