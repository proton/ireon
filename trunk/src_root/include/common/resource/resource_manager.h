/* Copyright (C) 2005 ireon.org developers council
 * $Id: resource_manager.h 787 2006-10-29 15:03:48Z mip $

 *  See LICENSE for details
 */

/**
 * @file resource_manager.h
 * Resource manager
 */
#ifndef _RESOURCE_MANAGER_H
#define _RESOURCE_MANAGER_H

class CResourceLocation;
#include "boost/shared_ptr.hpp"
class CData;
typedef boost::shared_ptr<CData> DataPtr;
#include "common/log_mixin.h"

class CResourceManager: private CLogMixin
{
public:
	static CResourceManager* instance() { if( !m_singleton ) m_singleton = new CResourceManager(); return m_singleton; }

	void registerCategoryLocation();
	void registerTypeLocation();

	void addLocation( const String& type, const String& loc );

	DataPtr load(const String& name);

private:
	CResourceManager(): CLogMixin("common.resources") {}

	/// List of resource locations
	std::vector<CResourceLocation*> m_locations;
	/// Vector of available resource names
	std::map<String,CResourceLocation*> m_resources;

	static CResourceManager* m_singleton;
};

#endif
