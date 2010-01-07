/**
* @file prototype_manager.h
* Command
* 
*/

/* Copyright (C) 2005 ireon.org developers council
* $Id: prototype_manager.h 829 2006-11-30 18:39:37Z mip $

*  See LICENSE for details
*/

#ifndef _PROTOTYPE_MANAGER_H
#define _PROTOTYPE_MANAGER_H

#include "common/world/managers/generic_collection.h"
//#include "boost/shared_ptr.hpp"
//#include "client_app.h" // I_APP
//typedef boost::shared_ptr<CStaticPrototype> StaticPrototypePtr;

class CPrototype;

class CPrototypeManager: public CGenericCollection<CPrototypeManager, CPrototype>
{
public:
	CPrototypeManager();

	virtual ~CPrototypeManager(){};

	bool loadStaticPrototypes();

	bool loadOwnPlayerPrototypes();
	
	bool loadCharacterPrototypes();
	
	bool loadMobPrototypes();
protected:

	bool isLoadAll();
	StringVector getFilesInDirectory(const String& directory, const String& fileType);
	String getPrototypesDirectory();

};

#endif
