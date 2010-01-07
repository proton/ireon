/**
 * @file editor_world.cpp
 * Editor world manager
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

#include "world/editor_world.h"
#include "world/client_zone.h"
#include "config.h"

CEditorWorld* CEditorWorld::m_singleton = 0;

CEditorWorld::CEditorWorld():
m_zone(NULL)
{
	m_zone = new CClientZone();
};

CEditorWorld::~CEditorWorld()
{
	if( m_zone )
		delete m_zone;
};

bool CEditorWorld::init()
{

	CWorld::init();

	// Get zone list from file
	CConfig cf;
	cf.load(CEditorApp::instance()->getSetting("/config/world/ZonesDir") + "zones.ini");
	m_zoneList = cf.getMultiSetting("Zone");

	if( !m_zoneList.empty() )
		m_zone->load(m_zoneList[0]);
		
	return true;
};

bool CEditorWorld::createScene()
{
    // Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	Camera* cam = sceneManager()->createCamera("PlayerCam");

	cam->setPosition(Ogre::Vector3(0,200,0));
	cam->lookAt(Vector3(0,0,0));
	
	cam->setNearClipDistance(1);

	Ogre::Viewport* vp = CEditorApp::instance()->getRenderWindow()->addViewport(cam);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	// Alter the camera aspect ratio to match the viewport
	cam->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

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
    //sceneManager()->setSkyDome(true, "Examples/CloudySky", 5, 8);

	sceneManager()->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
	sceneManager()->setShadowColour(ColourValue(0.5,0.5,0.5));
	CLog::instance()->log(CLog::msgLvlInfo,"Scene initialization completed.\n");

	sceneManager()->setWorldGeometry("../data/world.ini");
	PagingLandscapeListenerManager::getSingleton().addListener(this);
	vp->setBackgroundColour(ColourValue(0.93, 0.86, 0.76));

//	if( !loadZones() )
//		return false;

	CLog::instance()->log(CLog::msgLvlInfo,"Loading world complete.\n");

	return true;
};


void CEditorWorld::update(Real time)
{
	CWorld::update(time);
};

void CEditorWorld::clear()
{
	CWorld::clear();
};