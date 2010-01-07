/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: cm_db.cpp 579 2006-03-26 12:58:49Z zak $
 
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
 * @file cm_db.cpp
 * Cluster manager's database
 */

#include "stdafx.h"
#include "cm_app.h"
#include "db/cm_db.h"
#include "db/account.h"
#include "db/client_char_data.h"
//#include "net/cm_world_context.h"
//#include "net/cm_server.h"
#include "file/file.h"
#include "common/net/signal_codes/cmwc_codes.h"

CCMDB* CCMDB::m_singleton = 0;

bool CCMDB::init()
{
	assert(m_singleton == 0);
	m_singleton = new CCMDB;
	if (!m_singleton->load())
		return false;
	return true;
};

bool CCMDB::load()
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
	uint id,world;
	for( i = 0; i < num; i++ )
	{
		d >> id;
		d >> world;
		m_players.insert(std::pair<uint,uint>(id,world));
	};
	return true;
}

bool CCMDB::save()
{
	g_log("Saving db... ");
	CData d;

	//Saving accounts
	d.wrt((int)m_accounts.size());
	std::vector<AccPtr>::iterator i;
	for( i = m_accounts.begin(); i != m_accounts.end(); i++ )
		(*i)->serialize(d);

	d.wrt((int)m_players.size());
	std::map<uint,uint>::iterator j;
	for( j = m_players.begin(); j != m_players.end(); j++ )
	{
		d.wrt((*j).first);
		d.wrt((*j).second);
	};

	//Saving to file
	FilePtr f(new CFile);
	f->open("pdb.dat","w");
	f->write(d.data(),d.length());
	g_log("done.\n");
	return true;
};

byte CCMDB::newCharacter(ClientOwnCharData *ch, uint acc)
{
	assert(ch);
	if( !ch )
		return CCE_OTHER;
	std::pair<uint,uint> p;
	p.first = ch->m_id;
	p.second = getBirthWorld(ch);///Newchar
	//CCMWorldContext* con = CCMApp::instance()->server()->findContext(p.second);
	CNetConnection* con = CCMApp::instance()->getWSConnectionsManager()->findConnection(p.second);
	if( con )
	{
		m_players.insert(p);
		CData	request;
		ch->serialize(request);
		request << acc;
		con->processCommandEx(ireon::net::cmwcCodes::iscMainSendNewChar, request, ireon::net::netAddress::atInternalAddress);
	} else
	{
		CLog::instance()->log(CLog::msgLvlError,_("Can't find cluster for char with id %d (new).\n"),ch->m_id);
		return CCE_OTHER;
	};
	return CCE_NONE;
}

uint CCMDB::getBirthWorld(ClientOwnCharData *ch)
{
	// TODO
	return 1;
	ch;
};

uint CCMDB::getWorld(uint id)
{
	// TODO
	return 1;
	id;
};

AccPtr CCMDB::findAccount(String name)
{
	std::vector<AccPtr>::iterator i;
	for( i = m_accounts.begin(); i != m_accounts.end(); i++ )
		if( (*i)->getName() == name )
			return *i;
	return AccPtr();
};

AccPtr CCMDB::findAccount(uint id)
{
	std::vector<AccPtr>::iterator i;
	for( i = m_accounts.begin(); i != m_accounts.end(); i++ )
		if( (*i)->getId() == id )
			return *i;
	return AccPtr();
};

void CCMDB::removeAccount(uint id)
{
	std::vector<AccPtr>::iterator i;
	for( i = m_accounts.begin(); i != m_accounts.end(); i++ )
		if( (*i)->getId() == id )
		{
			m_accounts.erase(i);
			return;
		}
};

AccPtr CCMDB::updateAccount(AccPtr acc)
{
	removeAccount(acc->getId());
	m_accounts.push_back(acc);
	return acc;
};

void CCMDB::removeWaitAccount(CCMWSConnection* context)
{
	std::vector<WaitAccountStruct>::iterator i,j;
	for ( i = m_waitAccount.begin(); i < m_waitAccount.end(); i++ )
		if( (*i).context == context )
		{
			j = i;
			i--;
			m_waitAccount.erase(j);
		};
};

void CCMDB::removeCharacter(uint id)
{
	std::map<uint,uint>::iterator it = m_players.find(id);
	if( it != m_players.end() )
		m_players.erase(it);
};
