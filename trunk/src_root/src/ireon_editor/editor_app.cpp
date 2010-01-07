/**
  * @file ireon_client/editor_app.cpp
  * Contains CEditorApp class.
  * CEditorApp class source file.
  */
 
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: editor_app.cpp 752 2006-09-23 19:12:44Z zak $
 
  *  See LICENSE for details
  */
 
 
 #include "stdafx.h"
 #include "config.h"
 #include "editor_app.h"
 #include "editor_frame_listener.h"
 #include "world/world.h"
 #include "interface/interface.h"
 #include "resource/resource_manager.h"
 
 CEditorApp* CEditorApp::m_singleton = 0;
 
 // ---------------------------------------------------------------
 
 CEditorApp::CEditorApp():
   m_ogreRoot(NULL),
   m_frameListener(NULL),
   m_window(NULL)
 {
 	assert(m_singleton == 0);
 
 	m_singleton = this;
 };
 
 // ---------------------------------------------------------------
 
 CEditorApp::~CEditorApp()
 {
 	assert(m_singleton);
 
 	m_singleton = 0;
 	if( m_frameListener )
 		delete m_frameListener;
 };
 
 // ---------------------------------------------------------------
 
 extern void exportAllToPython();
 extern void loadScripts();
 
 // ---------------------------------------------------------------
 
 bool CEditorApp::init()
 {
 	CGenericApp::setAppName("ireon_editor");
 //	FIXME -- find out wtf
 //	do not uncomment this, or gui will not work!!!
 //	CGenericApp::initI10N();
 	CGenericApp::printPIDToFile();
 	CEditorApp::initLogSystem();
 
 	if (!loadXMLConfigFile("../config/defaults/config.xml", true)) 
 	{
 		CLog::instance()->log(CLog::msgLvlCritical,__FUNCTION__,_("Default configuration file loading failure!\n"));
 		shutdown();
 	}
 	// we have defaults, so it's ok if config.xml is missing
 	loadXMLConfigFile("../config/config.xml");
 	if (!checkRequiredConfigOptions()) 
 	{
 		CLog::instance()->log(CLog::msgLvlCritical,__FUNCTION__,_("One or more required options not found!\n"));
 		shutdown();
 	}
 
 
 	m_ogreRoot = new Ogre::Root("../config/plugins.cfg","../config/ogre.cfg","../logs/ogre.log");
 
 	if( !setupResources() )
 		return false;
 	if( !configure() )
 		return false;
 	if( !loadResources() )
 		return false;
 
 	//=== Init world ===
 	if( !CEditorWorld::instance()->init() )
 		return false;
 
 	m_frameListener = new CEditorFrameListener();
 	m_ogreRoot->addFrameListener(m_frameListener);
 
 	//=== Init interface ===
 	if( !CEditorInterface::instance()->init(m_window) )
 		return false;
 
 	exportAllToPython();
 	loadScripts();
 
 	return true;
 }
 
 // ---------------------------------------------------------------
 
 bool CEditorApp::checkRequiredConfigOptions()
 {
 	StringVector requiredOptions;
 	requiredOptions += "/config/interface/dir", "/config/world/config", "/config/interface/symbols";
 	if (!m_config->checkRequiredOptions(requiredOptions))
 		return false;
 	return true;
 }
 	
 // ---------------------------------------------------------------
 
 bool CEditorApp::initLogSystem()
 {
 	CGenericApp::initLogSystem();
 	CLog::instance()->regListener(new CFileLogListener("../logs/log_editor_error.txt",CLog::msgFlagALL,CLog::msgLvlError));
 	CLog::instance()->regListener(new CFileLogListener("../logs/log_editor_python.txt",CLog::msgFlagPython));
 	CLog::instance()->regListener(new CFileLogListener("../logs/log_editor_resources.txt",CLog::msgFlagResources));
 	CLog::instance()->timeStamp();
 	return true;
 }
 
 // ---------------------------------------------------------------
 
 void CEditorApp::go()
 {
 	/// Create math object to create angle tables
 	Math math;
 	m_ogreRoot->startRendering();
 }
 
 // ---------------------------------------------------------------
 
 bool CEditorApp::pulse(Real time)
 {
 	CEditorInterface::instance()->pulseUpdate(time);
 	return true;
 };
 
 // ---------------------------------------------------------------
 
 void CEditorApp::shutdown()
 {
 	if( m_config )
 		delete m_config;
 	ExitProcess(0);
 };
 
 // ---------------------------------------------------------------
 
 bool CEditorApp::setupResources()
 {
 	// Load resource paths from config file
 	CConfig cf;
 	cf.load("../config/resources.cfg");
 
 	// Go through all sections & settings in the file
 	CConfig::SectionIterator* seci = cf.getFirstSection();
 
 	String secName, typeName, archName;
 	while (seci)
 	{
 		secName = (*seci)->first;
 		CConfig::SettingsMultiMap *settings = (*seci)->second;
 		CConfig::SettingsMultiMap::iterator i;
 		for (i = settings->begin(); i != settings->end(); ++i)
 		{
 			typeName = i->first;
 			archName = i->second;
 			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
 				archName, typeName, secName);
 		}
 		seci = cf.getNextSection();
 	}
 
 	// Load resource paths from config file
 	cf.clear();
 	cf.load("../config/ireon_resources.ini");
 
 	// Go through all sections & settings in the file
 	seci = cf.getFirstSection();
 
 	while (seci)
 	{
 		secName = (*seci)->first;
 		CConfig::SettingsMultiMap *settings = (*seci)->second;
 		CConfig::SettingsMultiMap::iterator i;
 		for (i = settings->begin(); i != settings->end(); ++i)
 		{
 			typeName = i->first;
 			archName = i->second;
 			CResourceManager::instance()->addLocation(archName,typeName);
 		}
 		seci = cf.getNextSection();
 	}
 return true;
 };
 
 // ---------------------------------------------------------------
 
 bool CEditorApp::loadResources()
 {
 	// Initialise, parse scripts etc
 	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
 
 	return true;
 };
 
 // ---------------------------------------------------------------
 
 bool CEditorApp::configure()
 {
 	if( !m_ogreRoot )
 		return false;
 	if(m_ogreRoot->showConfigDialog())
 	{
 		// If returned true, user clicked OK so initialise
 		// Here we choose to let the system create a default rendering window by passing 'true'
 		m_window = m_ogreRoot->initialise(true,"Ireon: The real One");
 		return true;
 	}
 	else
 	{
 		return false;
 	}
 	return true;
 };
 
 // ---------------------------------------------------------------
 
 String CEditorApp::getSetting(const String& key)
 {
 	return m_config->getFirstValue(key);
 };
 
 // ---------------------------------------------------------------
 
 void CEditorApp::setSetting(const String &key, const String &value)
 {
 	m_config->setFirstValue(key,value);
 };
 
 Real CEditorApp::getFPS()
 {
 	return m_window->getLastFPS();
 };
 
 uint CEditorApp::getTriCount()
 {
 	return (uint)m_window->getTriangleCount();
 };
 std::vector<byte> CEditorApp::getVersion()
 {
 	byte bVersion[] = {PRODUCT_VERSION};
 	std::vector<byte> vVersion;
 	vVersion.assign(bVersion,&bVersion[sizeof(bVersion)]);
 	return vVersion;
 }