/**
 * @file client_world.cpp
 * Client world manager
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: world.cpp 510 2006-02-26 21:09:40Z zak $

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

#include "config.h"
#include "world/client_world.h"
#include "world/char_player.h"
#include "world/client_zone.h"

CClientWorld* CClientWorld::m_singleton = 0;

CClientWorld::CClientWorld()
{
};

CClientWorld::~CClientWorld()
{
};

bool CClientWorld::init()
{

	CWorld::init();

	return true;
};

bool CClientWorld::createScene()
{
	if( !CWorld::createScene() )
		return false;

    // Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	Camera* cam = sceneManager()->createCamera("PlayerCam");
	// Position it at 500 in Z direction
	cam->setPosition(Ogre::Vector3(-50,300,500));
	
	cam->setNearClipDistance(1);

	Ogre::Viewport* vp = CClientApp::instance()->getRenderWindow()->addViewport(cam);
	// Alter the camera aspect ratio to match the viewport
	cam->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));


	sceneManager()->setSkyBox(true,"Examples/MorningSkyBox");

	return true;
};

bool CClientWorld::loadZones()
{
	CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlInfo,"Start loading zones\n");
	CConfig cf;
	cf.load(CClientApp::instance()->getSetting("/config/world/ZonesDir") + "zones.ini");
	StringVector vec = cf.getMultiSetting("Zone");
	CClientZone zone;
	for( StringVector::iterator it = vec.begin(); it != vec.end(); ++it )
	{
		zone.load(*it);
	}
	CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlInfo,"Zones loaded\n");
	return true;
};

bool CClientWorld::load()
{

	CLog::instance()->log(CLog::msgLvlInfo,"Loading world...\n");
	// Set ambient light
	sceneManager()->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

   // Create a light
    Light* l = sceneManager()->createLight("MainLight");

    l->setPosition(0,800,-600);
	l->setSpecularColour(0.2,0.18,0.2);
	CLog::instance()->log(CLog::msgLvlInfo,"Created light.\n");

	l->setCastShadows(true);

	// Create a skydome
	sceneManager()->setSkyBox(false,"");
    sceneManager()->setSkyDome(true, "Examples/CloudySky", 5, 8);

	sceneManager()->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
	sceneManager()->setShadowColour(ColourValue(0.5,0.5,0.5));
	CLog::instance()->log(CLog::msgLvlInfo,"Scene initialization completed.\n");

	sceneManager()->setWorldGeometry("../data/world.ini");
	PagingLandscapeListenerManager::getSingleton().addListener(this);

	if( !loadZones() )
		return false;

	CLog::instance()->log(CLog::msgLvlInfo,"Loading world complete.\n");

	return true;
}

void CClientWorld::terrainReady()
{
	if( !loaded() )
	{
		CLog::instance()->log(CLog::msgLvlInfo,"Terrain ready.\n");
		m_loaded = true;
		m_ownChar->setPosition(Vector2(m_ownChar->getPosition().x,m_ownChar->getPosition().z));
		for( std::map<uint,CharacterPtr>::iterator it = m_characters.begin(); it != m_characters.end(); ++it )
			(*it).second->setPosition(Vector2((*it).second->getPosition().x,(*it).second->getPosition().z));
	}
}

CharPlayerPtr CClientWorld::addPlayer(ClientCharPlayerData* data)
{
	assert(data);
	if( findCharacter(data->m_id) )
		return CharPlayerPtr();
	CharPlayerPtr ch(new CCharPlayer(data,"Ninja.mesh"));
	addCharacter(ch);
	return ch;
};

void CClientWorld::clear()
{
	CWorld::clear();
	m_ownChar.reset();
};

OwnCharPlayerPtr CClientWorld::createOwnCharacter(ClientOwnCharData *data)
{
	m_ownChar.reset(new COwnCharPlayer(data,"Ninja.mesh"));
	/// Fire event
	CharacterEventArg arg;
	arg.m_ch = m_ownChar;
	CEventManager::instance()->execute(ON_CHAR_TO_WORLD,&arg);
	return m_ownChar;
};

void CClientWorld::update(Real time)
{
	CWorld::update(time);
	if( m_ownChar )
		m_ownChar->update(time);
}

void CClientWorld::updateCharactersInfo()
{
	for( std::map<uint,CharacterPtr>::iterator it = m_characters.begin(); it != m_characters.end(); ++it )
	{
		CharacterEventArg arg;
		arg.m_ch = (*it).second;
		CEventManager::instance()->execute(ON_CHAR_INFO_UPDATE,&arg);
	};
	CharacterEventArg arg;
	arg.m_ch = m_ownChar;
	CEventManager::instance()->execute(ON_CHAR_INFO_UPDATE,&arg);
};

bool characterLessByScreenCoordinates(CharacterPtr ch1, CharacterPtr ch2)
{
	return CClientWorld::instance()->translateTo2d(ch1->getPosition()).x < CClientWorld::instance()->translateTo2d(ch2->getPosition()).x;
};

void CClientWorld::makeVisibleList(std::vector<CharacterPtr> &list, bool sorted)
{
	list.clear();
	std::map<uint,CharacterPtr>::iterator it;
	for( it = m_characters.begin(); it != m_characters.end(); ++it )
		if( (*it).second->visible() )
			list.push_back((*it).second);
	if( sorted )
		std::sort(list.begin(),list.end(),characterLessByScreenCoordinates);
};

CharacterPtr CClientWorld::findCharacter(uint id)
{
	if( m_ownChar && m_ownChar->getId() == id )
		return m_ownChar;
	return CWorld::findCharacter(id);
};
