/**
  * @file client_zone.h
  * Client-side zone class
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: client_zone.h 857 2007-05-18 12:56:16Z mip $
 
  *  See LICENSE for details
  */
 
#ifndef _CLIENT_ZONE_H
#define _CLIENT_ZONE_H
#include <Ogre/main/OgreNoMemoryMacros.h>
#include "ireon_client/xsd/zone_xsd.h"
#include <Ogre/main/OgreMemoryMacros.h>

namespace ireon
{
	namespace nav
	{
		class Mesh;
	}
}

class CClientZone: private CLogMixin
{

public:

	CClientZone(const String& resourceName);
 	~CClientZone();
 
	ireon::nav::Mesh* getNavigationMesh(){return navigationMesh_.get();};
 
protected:

	std::auto_ptr<ireon::nav::Mesh> navigationMesh_;
 };
 
 #endif
