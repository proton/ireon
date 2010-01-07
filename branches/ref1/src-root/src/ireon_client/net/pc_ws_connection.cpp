/**
 * @file ireon_client\net\pc_ws_connection.cpp
 * player client connection to world server
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id$

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


//#include "ireon_client/stdafx.h"
//#include "interface/interface.h"
//#include "client_app.h"
//#include "db/client_char_data.h"
//#include "world/world.h"
//#include "world/char_player.h"
//#include "net/player_client.h"
#include "common/net/signal_codes/rspc_codes.h"


/*
void CPlayerClient::initRootSignals()
{
	CLog::instance()->log(CLog::msgLvlInfo,_("Initializing root signals\n"));
	m_slots.clear();

	this->registerSlot(ireon::net::rspcCodes::rcRegistrationOk, (CGenericClientSlot) &CPlayerClient::onRootRegistrationOk);
	this->registerSlot(ireon::net::rspcCodes::rcRegistrationFailed, (CGenericClientSlot) &CPlayerClient::onRootRegistrationFailed);

	this->registerSlot(ireon::net::rspcCodes::rcLoginOk, (CGenericClientSlot) &CPlayerClient::onRootLoginOk);
	this->registerSlot(ireon::net::rspcCodes::rcLoginFailed, (CGenericClientSlot) &CPlayerClient::onRootLoginFailed);

	this->registerSlot(ireon::net::rspcCodes::rcCharListOk, (CGenericClientSlot) &CPlayerClient::onRootCharList);
	this->registerSlot(ireon::net::rspcCodes::rcCharCreated, (CGenericClientSlot) &CPlayerClient::onCreateChar);
	this->registerSlot(ireon::net::rspcCodes::rcCharCreationFailed, (CGenericClientSlot) &CPlayerClient::onCharCreationFailed);

	this->registerSlot(ireon::net::rspcCodes::rcCharRemoveOk, (CGenericClientSlot) &CPlayerClient::onRemoveChar);

	this->registerSlot(100, (CGenericClientSlot) &CPlayerClient::onSendWorld);
};

void CPlayerClient::initWorldSignals()
{
	CLog::instance()->log(CLog::msgLvlInfo,_("Initializing world signals\n"));
	m_slots.clear();
	this->registerSlot(0, (CGenericClientSlot) &CPlayerClient::onWorldLogin);
	this->registerSlot(1, (CGenericClientSlot) &CPlayerClient::onSendCoords);
	this->registerSlot(2, (CGenericClientSlot) &CPlayerClient::onChatCmd);
	this->registerSlot(3, (CGenericClientSlot) &CPlayerClient::onFightCmd);
	this->registerSlot(4, (CGenericClientSlot) &CPlayerClient::onResurrect);
};

// -----------------------------------------------------------------------

bool CPlayerClient::registerSlot(const ireon::net::commandId requestId, const CGenericClientSlot slot)
{
	assert(slot);
	m_slots[requestId] = slot;
	return true;
}

// -----------------------------------------------------------------------

bool CPlayerClient::processCommand(CData &data, ireon::net::commandId requestId)
{
	if (m_slots.find(requestId) != m_slots.end())
	{
		(this->*m_slots[requestId])(data);	
		return true;
	} 
	else 
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError, _("Could not find signal with id %u.\n"), requestId);
		return false;
	}
/*
	if( id >= m_signals.size() )
		return false;
	(this->*m_signals[id])(data);
	return true;
*/
/*
};

// -----------------------------------------------------------------------

void CPlayerClient::reconnect()
{
	if( m_world && CClientApp::instance()->getState() == CClientApp::AS_PLAY )
		CClientApp::instance()->setState(CClientApp::AS_CONNECTING_WORLD);
	if( m_world && CClientApp::instance()->getState() == CClientApp::AS_ROOT )
		CClientApp::instance()->setState(CClientApp::AS_CONNECTING_ROOT);
	CNetDataProcessor::reconnect();
};

void CPlayerClient::connectionFailed(uint error)
{
	ConnectFailedEventArg arg;
	arg.m_host =  getHost();
	arg.m_port = getPort();
	switch(error)
	{
	case 1:
		{
			arg.m_error = _("Can't resolve host.");
			break;
		}
	case 2:
		{
			arg.m_error = _("Can't establish connection.");
			break;
		}
	default:
		{
			break;
		}
	};
	CEventManager::instance()->execute(CEventManager::ON_CONN_FAILED,&arg);
};

//=======  Network commands =========

// TODO change function name, move this logic to scripts
void CPlayerClient::login(const String& login, const String& password)
{
	if( CClientApp::instance()->getState() == CClientApp::AS_CONNECTING_ROOT )
	{
		if (CClientApp::instance()->getSetting("/temp/rootConnection/createAccount") == "1")
			registerToRoot(login, password);
		else 
			loginToRoot(login, password);
	}
	else
		loginSomewhere(login, password);
};

// ---------------------------------------------------------

void CPlayerClient::loginToRoot(const String& login, const String& password)
{
	CLog::instance()->log(CLog::msgLvlInfo,_("Sending login request to root server.\n"));
	CData data;
	data.wrt((ireon::net::commandId)ireon::net::rspcCodes::scHelloLogin);
	data << login << password;
	write(data);
};

// ---------------------------------------------------------

void CPlayerClient::registerToRoot(const String& login, const String& password)
{
	CLog::instance()->log(CLog::msgLvlInfo,_("Sending register request to root server.\n"));
	CData data;
	data.wrt((ireon::net::commandId)ireon::net::rspcCodes::scHelloRegister);
	data << login << password;
	write(data);
};

// ---------------------------------------------------------

// TODO refactor this, change function name
void CPlayerClient::loginSomewhere(const String& login, const String& password)
{
	CLog::instance()->log(CLog::msgLvlInfo,_("Sending login request to somewhere.\n"));
	CData data;
	data.wrt((byte)SIGNALS_START); //Command id
	data << login << password;
	write(data);
};

// ---------------------------------------------------------

void CPlayerClient::getCharsList()
{
	CLog::instance()->log(CLog::msgLvlInfo,_("Sending chars listing request to root server.\n"));
	CData data;
	data.wrt((ireon::net::commandId)ireon::net::rspcCodes::scCharSelList);
	write(data);
};

// ---------------------------------------------------------

void CPlayerClient::selectChar(uint id)
{
	CLog::instance()->log(CLog::msgLvlInfo,_("Select character with id %d.\n"),id);
	CData data;
	data.wrt((ireon::net::commandId)ireon::net::rspcCodes::scCharSelSelect);
	data << id;
	write(data);
};

// ---------------------------------------------------------

void CPlayerClient::createChar(ClientCharRegistrationData& ch)
{
	CLog::instance()->log(CLog::msgLvlInfo,_("Sending new char data.\n"));
	CData data;
	data.wrt((ireon::net::commandId)ireon::net::rspcCodes::scCharSelCreate);
	ch.serialize(data);
	write(data);
};

// ---------------------------------------------------------

void CPlayerClient::removeChar(uint id)
{
	CLog::instance()->log(CLog::msgLvlInfo,_("Sending remove char command.\n"));
	CData data;
	data.wrt((ireon::net::commandId)ireon::net::rspcCodes::scCharSelRemove);
	data << id;
	write(data);
};

// =======  End network commands =========

// =======  Root signals handlers =========

void CPlayerClient::onRootRegistrationOk(CData& data)
{
	CLog::instance()->log(CLog::msgLvlInfo,_("Registered account on root server.\n"));
	CClientApp::instance()->setState(CClientApp::AS_MENU);
};

// ----------------------------------------------------------

void CPlayerClient::onRootRegistrationFailed(CData& data)
{
	byte answer;
	data >> answer;
	CLog::instance()->log(CLog::msgLvlError,_("Registration on root server failed.\n"));
	switch(answer)
	{
	case ireon::net::rspcCodes::rfAlreadyRegistered:
		CLog::instance()->log(CLog::msgLvlError,_("Server returned error: account with such name already registered.\n"));
		break;
	case ireon::net::rspcCodes::rfUnknown:
		CLog::instance()->log(CLog::msgLvlError,_("Server returned error: unknown account db error.\n"));
		break;
	}
	CClientApp::instance()->setState(CClientApp::AS_MENU);
	CEventManager::instance()->execute(CEventManager::ON_LOGIN_FAILED,NULL);		
};

// ----------------------------------------------------------

void CPlayerClient::onRootLoginOk(CData& data)
{
	CLog::instance()->log(CLog::msgLvlInfo,_("Logged in on root server.\n"));
//	CClientApp::instance()->setState(CClientApp::AS_MENU);
//	CEventManager::instance()->execute(CEventManager::ON_LOGIN_FAILED,NULL);		
	getCharsList();
};

// ----------------------------------------------------------

void CPlayerClient::onRootLoginFailed(CData& data)
{
	byte answer;
	data >> answer;
	CLog::instance()->log(CLog::msgLvlError,_("Login to root server failed.\n"));
	switch(answer)
	{
	case ireon::net::rspcCodes::lfAccountNotFound:
		CLog::instance()->log(CLog::msgLvlError,_("Server returned error: account not found.\n"));
		break;
	case ireon::net::rspcCodes::lfAlreadyLoggedIn:
		CLog::instance()->log(CLog::msgLvlError,_("Server returned error: account already logged in.\n"));
		break;
	case ireon::net::rspcCodes::lfWrongPassword:
		CLog::instance()->log(CLog::msgLvlError,_("Server returned error: wrong password.\n"));
		break;
	}
	CClientApp::instance()->setState(CClientApp::AS_MENU);
	CEventManager::instance()->execute(CEventManager::ON_LOGIN_FAILED,NULL);		
};

// ----------------------------------------------------------

void CPlayerClient::onRootCharList(CData& data)
{
	CLog::instance()->log(CLog::msgLvlInfo,_("Got account characters list.\n"));
	CClientApp::instance()->m_characters.clear();

	ClientCharRegistrationData	chData;
	ClientOwnCharData ch;

	chData.serialize(data);
	ch.m_id = chData.m_id;
	ch.m_name = chData.m_name;

	while(ch.m_id)
	{
		CClientApp::instance()->m_characters.push_back(ch);
		chData.serialize(data);
		ch.m_id = chData.m_id;
		ch.m_name = chData.m_name;
	};

	CClientApp::instance()->setState(CClientApp::AS_ROOT);
	CEventManager::instance()->execute(CEventManager::ON_CHAR_LIST_CHANGED,NULL);	
};

// ----------------------------------------------------------

/*
void CPlayerClient::onRootCharListEmpty(CData& data)
{
	// log
};
*/
/*
// ----------------------------------------------------------

void CPlayerClient::onSendWorld(CData& data)
{
	String ip;
	uint port;
	data >> ip;
	data >> port;
	if( ip == BLANK_STRING )
	{
		CLog::instance()->log(CLog::msgLvlInfo,_("World server for char not found.\n"));
		return;
	};
	CLog::instance()->log(CLog::msgLvlInfo,_("Obtained world server %s:%d.\n"),ip.c_str(),port);
	CLog::instance()->log(CLog::msgLvlInfo,_("Connecting to world server (%s:%d).\n"),ip.c_str(),port);
	m_refactoredNetwork = false;
	if(CClientApp::instance()->getWorldConn()->connect(ip.c_str(),port,CClientApp::instance()->getSocketSet()))
		CClientApp::instance()->setState(CClientApp::AS_CONNECTING_WORLD);
	else
	{
		CLog::instance()->log(CLog::msgLvlError,_("Can't connect to world server(%s:%d).\n"),ip.c_str(),port);
		ConnectFailedEventArg arg;
		arg.m_error = _("Can't create connectioin to world server.");
		arg.m_host = ip;
		arg.m_port = port;
		CEventManager::instance()->execute(CEventManager::ON_CONN_FAILED,&arg);
	}
};

void CPlayerClient::onCharCreationFailed(CData& data)
{
	// FIXME
	CLog::instance()->log(CLog::msgLvlError,_("Char creation failed.\n"));
	CharCreatedEventArg arg;
	arg.m_d = NULL;
	arg.m_res = CCE_EXISTS;
	CEventManager::instance()->execute(CEventManager::ON_CHAR_CREATED,&arg);
}


void CPlayerClient::onCreateChar(CData& data)
{
	// refactor this getCharsList() takes care of updating char listing
	ClientCharRegistrationData chData;
	chData.serialize(data);
	ClientOwnCharData d;
	d.m_id = chData.m_id;
	d.m_name = chData.m_name;
	CClientApp::instance()->m_characters.push_back(d);
	CharCreatedEventArg arg;
	arg.m_d = &d;
	arg.m_res = CCE_NONE;
	CEventManager::instance()->execute(CEventManager::ON_CHAR_CREATED,&arg);
	CEventManager::instance()->execute(CEventManager::ON_CHAR_LIST_CHANGED,&arg);
	CLog::instance()->log(CLog::msgLvlInfo,_("New char with id %d created.\n"),d.m_id);
	getCharsList();
};

void CPlayerClient::onRemoveChar(CData &data)
{
	uint id;
	byte res;
	data >> id >> res;
	CharRemovedEventArg arg;
	arg.m_id = id;
	arg.m_res = res;
	CEventManager::instance()->execute(CEventManager::ON_CHAR_REMOVED,&arg);
};

// =======  End root signals handlers =========

// ======= World server commands ===========

void CPlayerClient::sendCoords(Vector2 coords, Degree rotation, uint time)
{
	CData data;
	data.wrt((byte)(SIGNALS_START + 1));
	data << coords;
	data.wrt(degreeToByte(rotation.valueDegrees()));
	data << time;
	write(data);
};

void CPlayerClient::sendChatCmd(ChatCommand cmd,const String& arg1, const String& arg2, const String& arg3)
{
	CData data;
	data.wrt((byte)(SIGNALS_START + 2));
	data.wrt((byte)cmd);
	data << arg1 << arg2 << arg3;
	write(data);
};

void CPlayerClient::sendFightCmd(FightCommand cmd, uint enemy )
{
	CData data;
	data.wrt((byte)(SIGNALS_START + 3));
	data.wrt((byte)cmd);
	if( cmd == FIGHT_ATTACK )
	{
		data << enemy;
		data.wrt((byte)1);
	}
	write(data);
};

void CPlayerClient::sendResurrect()
{
	CData data;
	data.wrt((byte)(SIGNALS_START + 4));
	write(data);
};
// =======  World signals handlers =========

void CPlayerClient::onWorldLogin(CData& data)
{
 byte answer;
 data >> answer;
 if( answer )
 {
	 ///Logged in
 	CLog::instance()->log(CLog::msgLvlInfo,_("Logged in to world server.\n"));
	CWorld::instance()->load();
	getInitialInfo(data);
	CClientApp::instance()->setState(CClientApp::AS_PLAY);
 } else
 {
 	CLog::instance()->log(CLog::msgLvlInfo,_("Login failed.\n"));
	CClientApp::instance()->setState(CClientApp::AS_ROOT);
	CEventManager::instance()->execute(CEventManager::ON_LOGIN_FAILED,NULL);
 }
};

void CPlayerClient::getInitialInfo(CData& data)
{
	ClientOwnCharData* d = new ClientOwnCharData;
	Vector2 pos;
	byte rot;
	data >> pos;
	data >> rot;
	Radian rotation(Degree(byteToDegree(rot)));
	d->serialize(data);
	CWorld::instance()->createOwnCharacter(d);
	CWorld::instance()->getOwnChar()->setPosition(pos);
	CWorld::instance()->getOwnChar()->setRotation(rotation);
	CWorld::instance()->getOwnChar()->setDifference(Vector2::ZERO);
};

void CPlayerClient::onSendCoords(CData& data)
{
	int64 pulse;
	data >> pulse;
	Vector2 dif;
	data >> dif;
	CWorld::instance()->getOwnChar()->setDifference(dif);
	uint id;
	data >> id ;
	while (id )
	{
		if( id & MOB_ID_FLAG )
		{
			ClientCharMobData* d = new ClientCharMobData;
			d->serialize(data);
			CLog::instance()->log(CLog::msgLvlInfo,_("Adding mob with id %d.\n"),d->m_id);
			CWorld::instance()->addMob(d);
		} else
		{
			ClientCharPlayerData* d = new ClientCharPlayerData;
			d->serialize(data);
			CLog::instance()->log(CLog::msgLvlInfo,_("Adding character with id %d.\n"),d->m_id);
			CWorld::instance()->addPlayer(d);
		}
		data >> id;
	};
	data >> id;
	while ( id )
	{
		CLog::instance()->log(CLog::msgLvlInfo,_("Removing character with id %d.\n"),id);
		CWorld::instance()->removeCharacter(id);
		data >> id;
	};
	data >> id;
	while( id )
	{
		Vector2 pos;
		byte rot;
		data >> pos;
		data >> rot;
		Radian rotation(Degree(byteToDegree(rot)));
		CharacterPtr p = CWorld::instance()->findCharacter(id);
		if( p )
		{
			if( p->getPosition().x == 0 && p->getPosition().z == 0 )
			{
				p->setPosition(pos);
				p->setRotation(rotation);
			}
			else
				p->addWayPoint(pulse,pos,rotation);
		}
		data >> id;
	};
	data >> id;
	while( id )
	{
		CharacterPtr ch = CWorld::instance()->findCharacter(id);
		if( !ch )
			CLog::instance()->log(CLog::msgLvlError,"Can't find character for command FIGHT_STOP with id %d.\n",id);
		else
			ch->stopFight();
		data >> id;
	};
	data >> id;
	while( id )
	{
		uint enemyId;
		bool closeCombat;
		data >> enemyId >> closeCombat;
		CharacterPtr ch = CWorld::instance()->findCharacter(id);
		if( !ch )
			CLog::instance()->log(CLog::msgLvlError,"Can't find character for command FIGHT_ATTACK with id %d.\n",id);
		else
			ch->startFight(enemyId, closeCombat);
		data >> id;
	}
};

void CPlayerClient::onChatCmd(CData& data)
{
	byte cmd;
	data >> cmd;
	String arg1,arg2,arg3;
	data >> arg1 >> arg2 >> arg3;
	ChatEventArg eventArg;
	eventArg.m_command = (ChatCommand)cmd;
	eventArg.m_arg1 = arg1;
	eventArg.m_arg2 = arg2;
	eventArg.m_arg3 = arg3;
	CEventManager::instance()->execute(CEventManager::ON_CHAT,&eventArg);
};

void CPlayerClient::onFightCmd(CData& data)
{
	byte cmd;
	data >> cmd;
	switch( (FightCommand)cmd )
	{
		case FIGHT_HIT:
			{
				uint chId;
				uint enemyId;
				byte percent;
				byte flags;
				data >> chId >> enemyId >> percent >> flags;
				CharacterPtr enemy = CWorld::instance()->findCharacter(enemyId);
				if( enemy )
					enemy->setPercentHP(percent);
				break;
			};
		case FIGHT_MSG:
			{
				break;
			};
		case FIGHT_DEATH:
			{
				uint chId,enemyId;
				data >> chId >> enemyId;
				CharacterPtr ch = CWorld::instance()->findCharacter(chId);
				if( ch )
					ch->die();
				else
					CLog::instance()->log(CLog::msgLvlInfo,"Can't find character that dying (%d).\n",chId);
				break;
			};
	};
};

void CPlayerClient::onResurrect(CData& data)
{
	CWorld::instance()->clear();
	CClientApp::instance()->setState(CClientApp::AS_PLAY);
	getInitialInfo(data);
};

/*=======  End world signals handlers =========*/
