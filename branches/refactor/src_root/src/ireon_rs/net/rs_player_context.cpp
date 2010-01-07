/**
 * @file rs_player_context.cpp
 * Player connection context on root server
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: rs_player_context.cpp 433 2005-12-20 20:19:15Z zak $

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

#include "stdafx.h"
#include "root_app.h"
#include "db/client_char_data.h"
#include "db/root_db.h"
#include "net/rs_player_context.h"
#include "net/rs_world_context.h"
#include "net/root_eserver.h"
#include "net/root_iserver.h"

#include "ireon_rs/net/rspc/rspc_hello_state.h"
#include "ireon_rs/net/rspc/rspc_character_selection_state.h"

std::vector<CRSESig> CRSPlayerContext::m_signals;

CRSPlayerContext::CRSPlayerContext(TCPsocket sock, SDLNet_SocketSet socketSet):
CNetClient(sock,socketSet),
m_state(STATE_NONE),
m_contextState(NULL)
{
	setState(csHello);
};

void CRSPlayerContext::initSignals()
{
	m_signals.clear();
	m_signals.push_back((CRSESig)&CRSPlayerContext::onLogin); /// 0
	m_signals.push_back((CRSESig)&CRSPlayerContext::onSelectChar);
	m_signals.push_back((CRSESig)&CRSPlayerContext::onCreateChar);
	m_signals.push_back((CRSESig)&CRSPlayerContext::onRemoveChar);
};

// -----------------------------------------------------------------------------------

bool CRSPlayerContext::setState(byte newState)
{
	switch (newState)
	{
		case csHello:
		{
			if (m_contextState)
				delete m_contextState;
			m_contextState = new CRSPCHelloState(this);
			return true;
		}
		case csCharacterSelection:
		{
			if (m_contextState)
				delete m_contextState;
			m_contextState = new CRSPCCharacterSelectionState(this);
			return true;
		}
	}
	return false;
}

// -----------------------------------------------------------------------------------

bool CRSPlayerContext::processCommand(CData& data, ireon::net::commandId id)
{
	m_contextState->processRequest(0, data);
	if( id >= m_signals.size() )
		return false;
	if( id > 0 && !m_account.get() )  ///Must be logged in to send commands with id > 0
		return true;
	else if( id == 0 && m_account.get() )
		return true; /// We're logged in but obtained login command
	(this->*m_signals[id])(data);
	return true;
};

// -----------------------------------------------------------------------------------

void CRSPlayerContext::sendWorld(const char* host, uint port)
{
	CLog::instance()->log(CLog::msgLvlInfo,_("Sending world server %s:%d to account %d.\n"),host,port,m_account->getId());
	CData data;
	data.wrt((byte)(SIGNALS_START + 1));
	data << host << port;
	write(data);
};

void CRSPlayerContext::sendCreatedNewChar(byte res, ClientOwnCharData& data)
{
	CData answer;
	answer.wrt((byte)(SIGNALS_START + 2));
	answer << res;
	if( res == CCE_NONE )
		data.serialize(answer);
	write(answer);
};

void CRSPlayerContext::removeChar(uint id, byte res)
{
	CData answer;
	answer.wrt((byte)(SIGNALS_START + 3));
	answer << id << res;
	write(answer);
};

/*======= Command handlers ========*/

void CRSPlayerContext::onLogin(CData &data)
{
	String login;
	String passwd;
	bool create;
	data >> login >> passwd >> create;
	AccPtr a = CRootDB::instance()->findAccount(login);
	CData answer;
	answer.wrt((byte)SIGNALS_START);
	if( create )
	{
		/// Creating account
		CLog::instance()->log(CLog::msgLvlInfo,_("New account creating. Login = '%s'...  "),login.c_str());
		a = CRootDB::instance()->createAccount(login);
		if( a )
			a->setPasswd(passwd);
	} else
		CLog::instance()->log(CLog::msgLvlInfo,_("Login attempt from %s.... "),getHost());
	if( a.get() && a->getPasswd() == passwd )
	{
	   	CLog::instance()->log(CLog::msgLvlInfo,_("logged in.\n"));
		answer.wrt((byte)1);
		///Closing all connections to same account
		CRSPlayerContext* c;
		while( c = CRootApp::instance()->eserver()->findContext(a->getId()) )
			CRootApp::instance()->eserver()->closeContext(c);
		m_account = a;
		///Sending player list
		std::map<uint,RootCharPlayerData> chars;
		std::map<uint,RootCharPlayerData>::iterator i;
		CRootDB::instance()->selectCharactersByAccount(a->getId(),chars);
		for( i = chars.begin(); i != chars.end(); ++i )
		{
			answer.wrt((*i).first);
			answer << (*i).second.m_name;
		};
		answer.wrt((uint)0);
	}
	else
	{
		answer.wrt((byte)0);
	   	CLog::instance()->log(CLog::msgLvlInfo,_("failed.\n"));
	}
	write(answer);
};

void CRSPlayerContext::onSelectChar(CData &data)
{
	int id;
	data >> id;
	CLog::instance()->log(CLog::msgLvlInfo,_("Player %s selected char with id %d.\n"),m_account->getName(),id);
	if(CRootDB::instance()->findPlayer(id) && CRootDB::instance()->findPlayer(id)->m_account == m_account->getId())
	{
		m_account->setSelChar(id);
		uint cluster = CRootDB::instance()->getCluster(id);
		CRSWorldContext* con = CRootApp::instance()->iserver()->findContext(cluster);
		if( con )
		{
			m_state = STATE_WAIT_WORLD;
			con->getWorld(id);
			return;
		}
	}
	CLog::instance()->log(CLog::msgLvlError,_("Can't find cluster for char with id %d.\n"),id);
};

void CRSPlayerContext::onCreateChar(CData& data)
{
	ClientOwnCharData d;
	d.serialize(data);
	byte res = CRootDB::instance()->addCreatingCharacter(&d,m_account->getId());
	if( !res )
		CLog::instance()->log(CLog::msgLvlInfo,_("Created new character, id = %d, accid = %d.\n"),d.m_id,m_account->getId());
	else
	{
		///If not created, send answer immedeately
		sendCreatedNewChar(res,d);
	}
};

void CRSPlayerContext::onRemoveChar(CData& data)
{
	uint id;
	data >> id;
	uint cluster = CRootDB::instance()->getCluster(id);
	CRSWorldContext* con = CRootApp::instance()->iserver()->findContext(cluster);
	if( !con )
		removeChar(id, 0);
	else
		con->removeChar(id);
};
/*======= End command handlers =======*/
