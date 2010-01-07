/**
  * @file editor_world.cpp
  * Editor world manager
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: editor_world.cpp 752 2006-09-23 19:12:44Z zak $
 
  *  See LICENSE for details
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

	 String setting;
	//read ambient light rgb
	Ogre::ColourValue ambientLight(0,0,0,0);
	if ( (setting = I_APP->getSetting("/config/world/light/AmbientLight") ) != "")
		ambientLight += StringConverter::parseColourValue(setting);
	else
		ambientLight += Ogre::ColourValue(0.5, 0.5, 0.5, 1);
	//read main light rgb
	Ogre::ColourValue mainLight(0,0,0,0);
	if ( (setting = I_APP->getSetting("/config/world/light/MainLight") ) != "")
		mainLight += StringConverter::parseColourValue(setting);
	else
		mainLight += Ogre::ColourValue(0.2, 0.18, 0.2);

	bool shadows;
	if ( (setting = I_APP->getSetting("/config/world/shadows") ) != "")
		shadows = StringConverter::parseBool(setting);
	else
		shadows = true;

	bool light;
	if ( (setting = I_APP->getSetting("/config/world/light/enableMainLight") ) != "")
		light = StringConverter::parseBool(setting);
	else
		light = true;
	//--------------------------------------------------
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
 
 	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,"Loading world...\n");
 	// Set ambient light
 	sceneManager()->setAmbientLight(ambientLight);
 
    // Create a light
     Light* l = sceneManager()->createLight("MainLight");
 
     l->setPosition(0,800,-600);
 	l->setSpecularColour(mainLight);
 	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,"Created light.\n");
 
 	l->setCastShadows(shadows);
 
 	// Create a skydome
     sceneManager()->setSkyDome(true, "Examples/CloudySky", 5, 8);
 
 	sceneManager()->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
 	sceneManager()->setShadowColour(ColourValue(0.5,0.5,0.5));
 	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,"Scene initialization completed.\n");
 
 	sceneManager()->setWorldGeometry("../data/world.ini");
 	PagingLandscapeListenerManager::getSingleton().addListener(this);
 	vp->setBackgroundColour(ColourValue(0.93, 0.86, 0.76));
 
 //	if( !loadZones() )
 //		return false;
 
 	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,"Loading world complete.\n");
 
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