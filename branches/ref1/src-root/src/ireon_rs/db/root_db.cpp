/**
 * @file root_db.cpp
 * Root database
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: root_db.cpp 565 2006-03-22 16:50:54Z zak $
 
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

#include "ireon_rs/stdafx.h"
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
		d << m_id;
	} else
	{
		d >> m_account;
		d >> m_cluster;
		d >> m_name;
		d >> m_id;
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
		m_chars.insert(std::pair<uint,RootCharPlayerData>(id,cpd));
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

	d.wrt((int)m_chars.size());
	std::map<uint,RootCharPlayerData>::iterator j;
	for( j = m_chars.begin(); j != m_chars.end(); j++ )
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
	uint maxid = 0;
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

uint CRootDB::getCluster(uint charId)
{
	return (m_chars.find(charId) != m_chars.end()) ? m_chars[charId].m_cluster : 0;
};

byte CRootDB::addCharacter(RootCharPlayerData *ch)
{
	assert(ch);
	uint lastCharId = (uint) m_chars.size();
	if (lastCharId == MAX_INT)
	{
		CLog::instance()->log(CLog::msgFlagAccChar,CLog::msgLvlError,_("Can't create character (acc: %d, name: %s) -- maximum char id number reached"), ch->m_account, ch->m_name.c_str());
		return false;
	}
	CLog::instance()->log(CLog::msgFlagAccChar,CLog::msgLvlVerbose,_("Adding character (acc: %d, name: %s)"), ch->m_account, ch->m_name.c_str());
	ch->m_id = lastCharId + 1;
	m_chars.insert(std::pair<uint,RootCharPlayerData>(ch->m_id,*ch));
	return true;
};


void CRootDB::deleteCharacter(uint id)
{
	m_chars.erase(id);
};


void CRootDB::selectCharactersByAccount(uint accId, std::map<uint,RootCharPlayerData> &chars)
{
	std::map<uint,RootCharPlayerData>::iterator it;
	for( it = m_chars.begin(); it != m_chars.end(); ++it )
		if( (*it).second.m_account == accId )
			chars.insert(*it);
};

uint CRootDB::getMaxCharacterId()
{
/*	uint max = 0;
	std::map<uint,RootCharPlayerData>::iterator i;
	for( i = m_chars.begin(); i != m_chars.end(); i++ )
	{
		if( i->first > max )
			max = i->first;
	}
	return max;
*/
	return (uint) m_chars.size();
};

RootCharPlayerData* CRootDB::findCharacter(String name)
{
	std::map<uint,RootCharPlayerData>::iterator i;
	for( i = m_chars.begin(); i != m_chars.end(); i++ )
		if( i->second.m_name == name )
			return &(i->second);
	return 0;
}
