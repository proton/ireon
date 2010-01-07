/**
  * @file prototype_manager.cpp
  * manager templates of object 
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: prototype_manager.cpp 811 2006-11-14 14:15:56Z mip $
 
  *  See LICENSE for details
  */
#include "stdafx.h"
#include "common/world/managers/prototype_manager.h"
#include "common/world/static_object/static_prototype.h"
#include "common/world/dynamic_object/character_prototype.h"

#include <Ogre/main/OgreNoMemoryMacros.h>
#include "ireon_client/xsd/config_xsd.h"
#include "ireon_client/xsd/statics_xsd.h"
#include <Ogre/main/OgreMemoryMacros.h>

#include "common/file/directory.h"
#include "boost/algorithm/string.hpp"
#include "ireon_client/client_app.h"

CPrototypeManager::CPrototypeManager():
CGenericCollection<CPrototypeManager, CPrototype>("PrototypeManager")
{

}

bool CPrototypeManager::loadStaticPrototypes()
{

	_log(_info,_loc,gtc_("Start loading static prototypes\n"));

	StringVector files;

	String pathToProto = getPrototypesDirectory();
	String pathToStaticsDirectory = pathToProto.append("statics");
	String pathToStaticsConfig = pathToProto.append("statics.xml");

	if (isLoadAll())
	{
		_log(_info,_loc,gtc_("Loading All static prototypes"));
		files = getFilesInDirectory(pathToProto, ".static");
	}
	else
	{
		_log(_info,_loc,gtc_("For loading static prototypes used config file '%s'"),pathToStaticsConfig.c_str() );
		std::auto_ptr<ireon::client::CPCStatics> config;
		config = ireon::client::statics(pathToStaticsConfig);

		for ( ireon::client::CPCStatics::prototype::iterator 
			it = config->prototype().begin();
			it != config->prototype().end();
			++it )
				files.push_back(*it);			
	}
		
	for( StringVector::iterator it = files.begin(); it != files.end(); ++it )
	{
		CStaticPrototype* staticProto =  new CStaticPrototype();
		if (staticProto->load(*it))
			addObject(staticProto);
	}

	_log(_info,_loc,gtc_("Static prototypes loaded"));

	return true;
};

bool CPrototypeManager::loadOwnPlayerPrototypes()
{
	_log(_info,_loc,gtc_("Start loading own player prototype"));
	
	COwnPlayerPrototype* protoPlayer =  new COwnPlayerPrototype();
	protoPlayer->load("");
	addObject(protoPlayer);

	return true;
};
bool CPrototypeManager::loadCharacterPrototypes()
{
	return true;
};
bool CPrototypeManager::loadMobPrototypes()
{
	return true;
};
bool CPrototypeManager::isLoadAll()
{
	bool isLoadAll;
	if ( I_APP->getConfig()->world().present() &&
		I_APP->getConfig()->world().get().proto().present() &&
		I_APP->getConfig()->world().get().proto().get().loadAll().present())
		isLoadAll = I_APP->getConfig()->world().get().proto().get().loadAll().get();
	else
		isLoadAll = ireon::client::CProtoSettings::loadAll::default_value();
	return isLoadAll;

};

StringVector CPrototypeManager::getFilesInDirectory(const String& directory, const String& fileType)
{
	assert(directory.empty()&& "Directory name is Empty");
	assert(fileType.empty()&& "File type is Empty");
	
	StringVector files;
	std::auto_ptr<CDirectory> dir = new CDirectory();
	
	dir->open( directory.c_str() );

	if( dir->isOpen() )
	{
		bool res = dir->setToFirstEntry();
		while( res )
		{
			if( dir->getEntryType() == CDirectory::FILE )
				if(boost::algorithm::ends_with(dir->getEntryName(), fileType.c_str() ))
				{
					String path = dir->getEntryName();
					boost::algorithm::erase_head(path,path.find_last_of('/')+1);
					files.push_back(path);	
				};
			res = dir->setToNextEntry();
		};
	};
	dir->close();

	return files;
}

String CPrototypeManager::getPrototypesDirectory()
{
	if ( I_APP->getConfig()->world().present() &&
		I_APP->getConfig()->world().get().proto().present() &&
		I_APP->getConfig()->world().get().proto().get().directory().present())
		return I_APP->getConfig()->world().get().proto().get().directory().get();
	else
		return ireon::client::CProtoSettings::directory::default_value();

}