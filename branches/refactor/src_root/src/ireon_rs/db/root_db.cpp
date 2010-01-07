/**
 * @file root_db.cpp
 * Root database
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: root_db.cpp 433 2005-12-20 20:19:15Z zak $
 
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
#include "net/rs_player_context.h"
#include "net/rs_world_context.h"
#include "net/root_eserver.h"
#include "net/root_iserver.h"
#include "db/account.h"
#include "db/client_char_data.h"
#include "db/root_db.h"
#include "file/file.h"

CRootDB* CRootDB::m_singleton = 0;


void RootCharPlayerData::serialize(CData& d)
{
	if( d.store() )
	{
		d << m_account;
		d << m_cluster;
		d << m_name;
	} else
	{
		d >> m_account;
		d >> m_cluster;
		d >> m_name;
	};
};

bool CRootDB::init()
{
	assert(m_singleton == 0);
	m_singleton = new CRootDB;
	if (!m_singleton->load())
		return false;
	return true;
};

bool CRootDB::load()
{
	FilePtr f(new CFile);
	f->open("pdb.dat","r");	

	if( !f->isOpen() )
		return true;

	char* data = new char[f->getSize()];

	int num,i;
	f->read(data,f->getSize());
	CData d(data,f->getSize(),false);

	d >> num;

	///Loading accounts
	AccPtr ptr;
	for( i = 0; i < num; i++ )
	{
		ptr.reset( new CAccount );
		ptr->serialize(d);
		m_accounts.push_back(ptr);
	}
	///Loading player's data
	d >> num;
	uint id;
	RootCharPlayerData cpd;
	for( i = 0; i < num; i++ )
	{
		d >> id;
		cpd.serialize(d);
		m_players.insert(std::pair<uint,RootCharPlayerData>(id,cpd));
	};
	return true;
}

bool CRootDB::save()
{
	g_log("Saving db... ");
	CData d;

	//Saving accounts
	d.wrt((int)m_accounts.size());
	std::vector<AccPtr>::iterator i;
	for( i = m_accounts.begin(); i != m_accounts.end(); i++ )
		(*i)->serialize(d);

	d.wrt((int)m_players.size());
	std::map<uint,RootCharPlayerData>::iterator j;
	for( j = m_players.begin(); j != m_players.end(); j++ )
	{
		d.wrt((*j).first);
		(*j).second.serialize(d);
	};

	//Saving to file
	FilePtr f(new CFile);
	f->open("pdb.dat","w");
	f->write(d.data(),d.length());
	g_log("done.\n");
	return true;
};

AccPtr CRootDB::createAccount(const String& name)
{
	int maxid = 0;
	std::vector<AccPtr>::iterator i;
	for( i = m_accounts.begin(); i != m_accounts.end(); i++ )
	{
		if( (*i)->m_id > maxid )
			maxid = (*i)->m_id;
		if( (*i)->getName() == name )
			return AccPtr();
	}
	maxid++;
	AccPtr ptr(new CAccount);
	ptr->m_name = name;
	ptr->m_id = maxid;
	m_accounts.push_back(ptr);
	return ptr;
};

AccPtr CRootDB::findAccount(String name)
{
	std::vector<AccPtr>::iterator i;
	for( i = m_accounts.begin(); i != m_accounts.end(); i++ )
		if( (*i)->getName() == name )
			return *i;
	return AccPtr();
};

AccPtr CRootDB::findAccount(uint id)
{
	std::vector<AccPtr>::iterator i;
	for( i = m_accounts.begin(); i != m_accounts.end(); i++ )
		if( (*i)->getId() == id )
			return *i;
	return AccPtr();
};

uint CRootDB::getCluster(uint id)
{
	return (m_players.find(id) != m_players.end()) ? m_players[id].m_cluster : 0;
};

byte CRootDB::addCharacter(RootCharPlayerData *ch, uint id)
{
	assert(ch);
	if( !ch )
		return false;

	if( m_players.find(id) != m_players.end() )
		return false;

	m_players.insert(std::pair<uint,RootCharPlayerData>(id,*ch));
	deleteCreating(id);

	return true;
};

byte CRootDB::addCreatingCharacter(ClientOwnCharData *ch, uint acc)
{
	if( !ch )
		return CCE_OTHER;

	RootCharPlayerData d;
	d.m_account = acc;
	d.m_cluster = getBirthCluster(ch);
	d.m_name = ch->m_name;
	uint max = 0;
	std::map<uint,RootCharPlayerData>::iterator i;
	for( i = m_players.begin(); i != m_players.end(); i++ )
	{
		if( i->first > max )
			max = i->first;
		if( i->second.m_name == ch->m_name )
			return CCE_EXISTS; ///Character with this name already exists
	}
	for( i = m_creating.begin(); i != m_creating.end(); i++ )
	{
		if( i->first > max )
			max = i->first;
		if( i->second.m_name == ch->m_name )
			return CCE_EXISTS; ///Character with this name already exists
	}
	ch->m_id = max + 1;
	CRSWorldContext* con = CRootApp::instance()->iserver()->findContext(d.m_cluster);
	if( con )
	{
		m_creating.insert(std::pair<uint,RootCharPlayerData>(ch->m_id,d));
		con->sendChar(ch,acc);
	} else
	{
		CLog::instance()->log(CLog::msgLvlError,_("Can't find cluster for char with id %d (new).\n"),ch->m_id);
		return CCE_OTHER;
	};
	return CCE_NONE;
};

uint CRootDB::getBirthCluster(ClientOwnCharData* ch)
{
	return 1;
};

void CRootDB::deleteCharPlayer(uint id)
{
	m_players.erase(id);
};

void CRootDB::deleteCreating(uint id)
{
	m_creating.erase(id);
};

void CRootDB::selectCharactersByAccount(uint id, std::map<uint,RootCharPlayerData> &chars)
{
	std::map<uint,RootCharPlayerData>::iterator it;
	for( it = m_players.begin(); it != m_players.end(); ++it )
		if( (*it).second.m_account == id )
			chars.insert(*it);
};
