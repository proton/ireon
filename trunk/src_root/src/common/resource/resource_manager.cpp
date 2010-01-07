/* Copyright (C) 2005 ireon.org developers council
 * $Id: resource_manager.cpp 787 2006-10-29 15:03:48Z mip $

 *  See LICENSE for details
 */

/**
 * @file resource_manager.cpp
 * Resource manager
 */

#include "common/common_stdafx.h"
#include "common/resource/resource_manager.h"
#include "common/file/directory.h"

CResourceManager* CResourceManager::m_singleton = 0;

void CResourceManager::addLocation(const String& type, const String& loc)
{
	_log ( _debug, _loc, gtc_("Adding resource location: '%s'."), loc.c_str() );
	CResourceLocation* lp =	NULL;
	if( type == "Dir" )
	{
		lp = new CDirectory(loc);
		lp->open();
	};
	if( !lp )
	{
		_log ( _error, _loc, gtc_("Unknown resource type: '%s'."), type.c_str() );
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
	_log ( _debug, _loc, gtc_("Loading resource: '%s'."), name.c_str() );
	std::map<String, CResourceLocation*>::iterator it = m_resources.find(name);
	if( it != m_resources.end() )
		return (*it).second->load((*it).first);

	_log ( _error, _loc, gtc_("Resource %s not found."), name.c_str() );
	return DataPtr();
};
