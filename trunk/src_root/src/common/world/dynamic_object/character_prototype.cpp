/**
* @file client_static.cpp
* Static object
*/

/* Copyright (C) 2005 ireon.org developers council
* $Id: client_static.cpp 829 2006-11-30 18:39:37Z mip $

*  See LICENSE for details
*/
#include "ireon_client/stdafx.h"
#include "common/world/dynamic_object/character_prototype.h"
#include "ireon_client/client_app.h"
#include <Ogre/main/OgreNoMemoryMacros.h>
#include "ireon_client/xsd/config_xsd.h"
#include <Ogre/main/OgreMemoryMacros.h>

CCharacterPrototype::CCharacterPrototype()
{
};

CCharacterPrototype::~CCharacterPrototype()
{
};

bool CCharacterPrototype::load(const String&)
{
	return true;
}

COwnPlayerPrototype::COwnPlayerPrototype()
{
};

COwnPlayerPrototype::~COwnPlayerPrototype()
{
};

bool COwnPlayerPrototype::load(const String& )
{
	String playerMesh;
	if ( I_APP->getConfig()->world().present() &&
		I_APP->getConfig()->world().get().player().present() &&
		I_APP->getConfig()->world().get().player().get().mesh().present())
		playerMesh = I_APP->getConfig()->world().get().player().get().mesh().get();
	else
		playerMesh = ireon::client::CPlayerSettings::mesh::default_value();
	_log(_info, _loc, gtc_("own player mesh: '%s'"),playerMesh.c_str());
	
	name_ = "ownPlayer";
	meshName_ = playerMesh;
	
	return true;
};
