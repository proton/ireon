/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: resource_manager.cpp 522 2006-03-06 16:49:30Z zak $

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
 * @file resource_manager.cpp
 * Resource manager
 */

#include "stdafx.h"
#include "resource/resource_manager.h"
#include "file/directory.h"

CResourceManager* CResourceManager::m_singleton = 0;

void CResourceManager::addLocation(const String& loc, const String& type)
{
	CLog::instance()->log(CLog::msgLvlTrace,_("Add resource location: '%s'.\n"),loc.c_str());
	CResourceLocation* lp =	NULL;
	if( type == "Dir" )
	{
		lp = new CDirectory(loc);
		lp->open();
	};
	if( !lp )
	{
		CLog::instance()->log(CLog::msgLvlError,_("Unknown resource type: '%s'.\n"),type.c_str());
		return;
	}
	StringVector vec;
	lp->list(&vec);
	StringVector::iterator it;
	for( it = vec.begin(); it != vec.end(); ++it )
		m_resources.insert(std::pair<String,CResourceLocation*>(*it,lp));
};

DataPtr CResourceManager::load(const String& name)
{
	CLog::instance()->log(CLog::msgLvlTrace,_("Loading resource: '%s'.\n"),name.c_str());
	std::map<String, CResourceLocation*>::iterator it = m_resources.find(name);
	if( it != m_resources.end() )
		return (*it).second->load((*it).first);
	CLog::instance()->log(CLog::msgLvlError,_("Error loading resource: '%s'.\n"),name.c_str());
	return DataPtr();
};
