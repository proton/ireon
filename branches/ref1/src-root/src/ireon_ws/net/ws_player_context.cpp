/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: ws_player_context.cpp 610 2006-04-20 11:04:59Z zak $

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
#include "world_app.h"
#include "net/ws_player_context.h"
#include "net/world_server.h"
#include "db/world_db.h"
#include "db/account.h"
#include "db/client_char_data.h"
#include "world/ws_world.h"
#include "world/world_char_player.h"
#include "world/world_char_mob.h"

std::vector<CWSSig> CWSPlayerContext::m_signals;

CWSPlayerContext::CWSPlayerContext(TCPsocket sock, SDLNet_SocketSet socketSet):
CNetDataProcessor(sock,socketSet)
{
};

void CWSPlayerContext::initSignals()
{
	m_signals.push_back(&CWSPlayerContext::onLogin);
	m_signals.push_back(&CWSPlayerContext::onSendPos);
	m_signals.push_back(&CWSPlayerContext::onChatCmd);
	m_signals.push_back(&CWSPlayerContext::onFightCmd);
	m_signals.push_back(&CWSPlayerContext::onResurrect);
};

bool CWSPlayerContext::processCommand(CData& data, byte id)
{
	if( id >= m_signals.size() )
		return false;
	if( !m_account && id > 0 )
		return true; /// We're not logged in, but id is more then 0 (login)
	else if( id == 0 && m_account.get() )
		return true; /// We're logged in but obtained login command
	(this->*m_signals[id])(data);
	if( data.readEmpty() )
		return false;
	return true;
};

void CWSPlayerContext::onClose()
{
	///Remove it from wait login query
	CWorldDB::instance()->removeWaitLogin(this);
	if( getCharacter() )
	{
		getCharacter()->setContext(NULL);
		m_character.reset();
	}
};

/* Network commands */

void CWSPlayerContext::enterGame(AccPtr a)
{
	CData answer;
	answer.wrt((byte)SIGNALS_START);
	if( a.get() && a->getPasswd() == m_password && a->getSelChar())
	{
		CLog::instance()->log(CLog::msgLvlInfo,_("Account %s logged in from %s.\n"),a->getName(),getHost());
		answer.wrt((byte)1);
		///Closing all connections to same account
		CWSPlayerContext* c;
		while( c = CWorldApp::instance()->server()->findContext(a->getId()) )
			CWorldApp::instance()->server()->closeContext(c);
		m_account = a;
		m_character = CWorld::instance()->findCharPlayer(a->getSelChar());
		if( !m_character )
		{
			///Character isn't in world
			m_character = CWorldDB::instance()->findCharPlayer(a->getSelChar());
			if( !m_character )
			{
				/// Data MUST be there. If not, it's error in database of some server
				CLog::instance()->log(CLog::msgLvlCritical,_("Enter world: can't find character with id %d.\n"),a->getSelChar());
				answer.wrt((byte)0);
				write(answer);
				return;
			};
			CWorld::instance()->insertCharPlayer(m_character);
		};
		m_character->setContext(this);
		sendInitialInfo(answer);
	} else
	{
		answer.wrt((byte)0);
		CLog::instance()->log(CLog::msgLvlInfo,_("Login from %s failed.\n"),getHost());
	}
	write(answer);
};

void CWSPlayerContext::sendInitialInfo(CData& data)
{
	if( !m_character|| !m_account )
		return;
	m_character->m_playersIn.clear();
	m_character->m_playersOut.clear();
	m_character->m_playersPos.clear();
	m_character->m_playersFight.clear();
	m_character->m_playersStopFight.clear();
	/// Notice client about all characters, that are visible for us
	for( std::vector<CharacterPtr>::iterator it = m_character->m_iSee.begin(); it != m_character->m_iSee.end(); ++it )
		m_character->noticeCharIn(*it);
	if( m_character->isFighting() )
		m_character->noticeCharFight(m_character->getId(),m_character->getEnemy()->getId(),m_character->inCloseCombat());
	ClientOwnCharData cdata;
	m_character->toClientOwnCharData(cdata);
	data << m_character->getPos();
	data.wrt(m_character->getRotation());
	cdata.serialize(data);
};

void CWSPlayerContext::sendCharactersPositions()
{
	CData data;
	data.wrt((byte)(SIGNALS_START + 1));
	data.wrt(CWorld::m_pulse);
	data << m_character->getDifference();
	std::map<uint,CharacterPtr>::iterator it1;
	for( it1 = m_character->m_playersIn.begin(); it1 != m_character->m_playersIn.end(); ++it1 )
	{
		data.wrt((*it1).first);
		ClientCharDataPtr d;
		if( (*it1).second->player() )
		{
			d.reset(new ClientCharPlayerData);
			(*it1).second->player()->toClientCharPlayerData(*(ClientCharPlayerData*)(d.get()));
		} else
		{
			d.reset(new ClientCharMobData);
			(*it1).second->mob()->toClientCharMobData(*(ClientCharMobData*)(d.get()));
		}
		d->serialize(data);
	};
	data.wrt((uint)0);
	m_character->m_playersIn.clear();
	std::set<uint>::iterator it2;
	for( it2 = m_character->m_playersOut.begin(); it2 != m_character->m_playersOut.end(); ++it2 )
		data.wrt((*it2));
	data.wrt((uint)0);
	m_character->m_playersOut.clear();
	std::map<uint,WorldPosition>::iterator it3;
	for( it3 = m_character->m_playersPos.begin(); it3 != m_character->m_playersPos.end(); ++it3)
	{
		data.wrt((*it3).first);
		data << (*it3).second.m_pos;
		data << (*it3).second.m_rotation;
	};
	data.wrt((uint)0);
	m_character->m_playersPos.clear();
	std::set<uint>::iterator it4;
	for( it4 = m_character->m_playersStopFight.begin(); it4 != m_character->m_playersStopFight.end(); ++it4 )
		data.wrt(*it4);
	m_character->m_playersStopFight.clear();
	data.wrt((uint)0);
	std::map<uint,FightPos>::iterator it5;
	for( it5 = m_character->m_playersFight.begin(); it5 != m_character->m_playersFight.end(); ++it5 )
	{
		data.wrt((*it5).first);
		data.wrt((*it5).second.m_enemyId);
		data.wrt((*it5).second.m_closeCombat);
	};
	data.wrt((uint)0);
	m_character->m_playersFight.clear();
	write(data);
};

void CWSPlayerContext::sendChatCommand(ChatCommand cmd, const String& arg1, const String& arg2, const String& arg3)
{
	CData data;
	data.wrt((byte)(SIGNALS_START + 2));
	data.wrt((byte)cmd);
	data << arg1 << arg2 << arg3;
	write(data);
};

void CWSPlayerContext::sendHit(uint chId, uint enemyId,byte percentHP, byte flags)
{
	CData data;
	data.wrt((byte)(SIGNALS_START + 3));
	data.wrt((byte)(FIGHT_HIT));
	data << chId;
	data << enemyId;
	data << percentHP;
	data << flags;
	write(data);
};

void CWSPlayerContext::sendDeath(uint ch, uint enemy)
{
	CData data;
	data.wrt((byte)(SIGNALS_START + 3));
	data.wrt((byte)(FIGHT_DEATH));
	data << ch;
	data << enemy;
	write(data);
};

void CWSPlayerContext::sendResurrect()
{
	CData data;
	data.wrt((byte)(SIGNALS_START + 4));
	sendInitialInfo(data);
	write(data);
};
/* End network commands*/

/* Signal handlers */
void CWSPlayerContext::onLogin(CData& data)
{
	CLog::instance()->log(CLog::msgLvlInfo,_("Login attempt from %s.... "),getHost());
	String login;
	String passwd;
	data >> login >> passwd;
	m_login = login;
	m_password = passwd;
	CWorldDB::instance()->checkLogin(login,passwd,this);
};

void CWSPlayerContext::onSendPos(CData& data)
{
	if( !m_character )
		return;
	WayPoint wp;
	data >> wp.m_pos;
	data >> wp.m_rotation;
	data >> wp.m_time;
	m_character->addWP(wp);
};

void CWSPlayerContext::onChatCmd(CData& data)
{
	if( !getCharacter() )
		return;
	byte command;
	data >> command;
	String arg1, arg2, arg3;
	data >> arg1 >> arg2 >> arg3;
	ChatCommand cmd = (ChatCommand)command;
	switch( command )
	{
		case CHAT_MSG:
		{
			if( arg1 == BLANK_STRING ) /// Local chat
				for( std::vector<CharacterPtr>::iterator it = getCharacter()->m_seeMe.begin(); it != getCharacter()->m_seeMe.end(); ++it )
					if( (*it)->player() && (*it)->player()->getContext() )
						(*it)->player()->getContext()->sendChatCommand(CHAT_MSG,BLANK_STRING,getCharacter()->getName(),arg2);
			break;
		}
	};
};

void CWSPlayerContext::onFightCmd(CData &data)
{
	byte cmd;
	data >> cmd;
	switch( (FightCommand)cmd )
	{
		case FIGHT_ATTACK:
			{
				uint enemy;
				bool closeCombat;
				data >> enemy >> closeCombat;
				CharacterPtr ch = CWorld::instance()->findCharacter(enemy);
				if( !ch )
					CLog::instance()->log(CLog::msgLvlInfo,"FIGHT: Can't find enemy with id %d for character %s(%d).\n",enemy,m_character->getName(),m_character->getId());
				else
					m_character->startFight(ch,closeCombat);
				break;
			};
		case FIGHT_STOP:
			{
				if( m_character->isFighting() )
					m_character->stopFight();
				break;
			};
	};
};

void CWSPlayerContext::onResurrect(CData& data)
{
	if( m_character->inWorld() )
		return;
	CData answer;
	m_character = CWorld::instance()->findCharPlayer(m_account->getSelChar());
	if( !m_character )
	{
		///Character isn't in world
		m_character = CWorldDB::instance()->findCharPlayer(m_account->getSelChar());
		if( !m_character )
		{
			/// Data MUST be there. If not, it's error in database of some server
			CLog::instance()->log(CLog::msgLvlCritical,_("Enter world: can't find character with id %d.\n"),m_account->getSelChar());
			return;
		};
		CWorld::instance()->insertCharPlayer(m_character);
	};
	m_character->setContext(this);
	sendResurrect();
};
/* End signal handlers */
