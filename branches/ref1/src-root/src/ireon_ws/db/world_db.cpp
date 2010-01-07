/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: world_db.cpp 610 2006-04-20 11:04:59Z zak $

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
 * @file world_db.cpp
 * World database
 */

#include "stdafx.h"
#include "world_app.h"
#include "db/world_db.h"
#include "db/account.h"
#include "net/ws_client.h"
#include "file/file.h"
#include "world/ws_world.h"
#include "world/world_char_player.h"
#include "world/world_char_mob.h"
#include "net/ws_player_context.h"
#include "net/world_server.h"

CWorldDB* CWorldDB::m_singleton = 0;

bool CWorldDB::init()
{
	assert(m_singleton == 0);
	m_singleton = new CWorldDB;
	if (!m_singleton->load())
		return false;
	return true;
};

bool CWorldDB::load()
{
	FilePtr f(new CFile);
	f->open("pdb.dat","r");	

	if( !f->isOpen() )
		return true;

	char* data = new char[f->getSize()];

	f->read(data,f->getSize());
	CData d(data,f->getSize(),false);

	int num,i;

	d >> CWorld::m_pulse;
	///Loading accounts
	d >> num;
	AccPtr ptr;
	for( i = 0; i < num; i++ )
	{
		ptr.reset( new CAccount );
		ptr->serialize(d);
		m_accounts.push_back(ptr);
	}
	///Loading player's data
	d >> num;
	CharPlayerPtr ch;
	for( i = 0; i < num; i++ )
	{
		ch.reset(new CWorldCharPlayer());
		ch->serialize(d);
		m_players.insert(std::pair<uint,CharPlayerPtr>(ch->getId(),ch));
	};
	return true;
}

bool CWorldDB::save()
{
	g_log("Saving db... ");
	CData d;

	d << CWorld::m_pulse;

	//Saving accounts
	d.wrt((int)m_accounts.size());
	std::vector<AccPtr>::iterator i;
	for( i = m_accounts.begin(); i != m_accounts.end(); i++ )
		(*i)->serialize(d);

	d.wrt((int)m_players.size());
	std::map<uint,CharPlayerPtr>::iterator j;
	for( j = m_players.begin(); j != m_players.end(); j++ )
	{
		(*j).second->serialize(d);
	};

	//Saving to file
	FilePtr f(new CFile);
	f->open("pdb.dat","w");
	f->write(d.data(),d.length());
	g_log("done.\n");
	return true;
};

void CWorldDB::checkLogin(String name, String passwd, CNetDataProcessor* context)
{
	removeWaitLogin(context);
	WaitLoginStruct s;
	s.context = context;
	s.name = name;
	s.password = passwd;
	m_waitLogin.push_back(s);
	CWorldApp::instance()->client()->getAccount(name.c_str());
};

void CWorldDB::removeWaitLogin(CNetDataProcessor* context)
{
	std::vector<WaitLoginStruct>::iterator i,j;
	for ( i = m_waitLogin.begin(); i < m_waitLogin.end(); i++ )
		if( (*i).context == context )
		{
			j = i;
			i--;
			m_waitLogin.erase(j);
		};
};

AccPtr CWorldDB::findAccount(String name)
{
	std::vector<AccPtr>::iterator i;
	for( i = m_accounts.begin(); i != m_accounts.end(); i++ )
		if( (*i)->getName() == name )
			return *i;
	return AccPtr();
};

AccPtr CWorldDB::findAccount(uint id)
{
	std::vector<AccPtr>::iterator i;
	for( i = m_accounts.begin(); i != m_accounts.end(); i++ )
		if( (*i)->getId() == id )
			return *i;
	return AccPtr();
};

void CWorldDB::removeAccount(uint id)
{
	std::vector<AccPtr>::iterator i;
	for( i = m_accounts.begin(); i != m_accounts.end(); i++ )
		if( (*i)->getId() == id )
		{
			m_accounts.erase(i);
			return;
		}
};

AccPtr CWorldDB::updateAccount(AccPtr acc)
{
	removeAccount(acc->getId());
	m_accounts.push_back(acc);
	return acc;
};

CharPlayerPtr CWorldDB::findCharPlayer(String name)
{
	std::map<uint,CharPlayerPtr>::iterator i;
	for( i = m_players.begin(); i != m_players.end(); i++ )
		if( (*i).second->getName() == name )
			return (*i).second;
	return CharPlayerPtr();
};

CharPlayerPtr CWorldDB::findCharPlayer(uint id)
{
	std::map<uint,CharPlayerPtr>::iterator i = m_players.find(id);
	if( i != m_players.end() )
		return (*i).second;
	return CharPlayerPtr();
};

void CWorldDB::removeCharPlayer(uint id)
{
	CLog::instance()->log(CLog::msgLvlInfo,"Removing player with id %d from database.\n",id);
	std::map<uint,CharPlayerPtr>::iterator i = m_players.find(id);
	if( i != m_players.end() )
	{
		CWorld::instance()->removeCharPlayer(id);
		if( (*i).second->getContext() )
			CWorldApp::instance()->server()->closeContext((*i).second->getContext());
		m_players.erase(i);
		CLog::instance()->log(CLog::msgLvlInfo,"Player with id %d removed.\n",id);
		return;
	}
	CLog::instance()->log(CLog::msgLvlInfo,"Player with id %d not found.\n",id);
};

byte CWorldDB::updateCharPlayer(CharPlayerPtr ch)
{
	removeCharPlayer(ch->getId());
	m_players.insert(std::pair<uint,CharPlayerPtr>(ch->getId(),ch));
	return CCE_NONE;
};
