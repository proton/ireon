/**
  * @file ireon_client/editor_app.h
  * Application class header file.
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: editor_app.h 693 2006-08-31 08:02:35Z zak $
 
  *  See LICENSE for details
  */
 
 #ifndef _CLIENT_APP_H
 #define _CLIENT_APP_H
 
 #include "common/xmlconfig.h"
 #include "common/generic_app.h"
 
 class CEditorApp;
 class CConfig;
 namespace Ogre{
 class Root;
 class RenderWindow;
 };
 class CEditorFrameListener;
 
 class CEditorApp : private CGenericApp
 {
 public:
   /**
    * Initializes config path and positions application as "singleton".
    * @param config config file path.
    */
 	CEditorApp();
 	~CEditorApp();
 
 private:
 	virtual bool initLogSystem();
 	bool checkRequiredConfigOptions();
 	///Setup resources
 	bool setupResources();
 	///Configure ogre
 	bool configure();
 	///Load resources
 	bool loadResources();
 
 public:
 	/**
    * Main application initialization procedure.
    * It registers log listeners, that are used for saving application operations results,
    * loads config file, connects states with their handlers (CEditorApp::connectingHandler, CEditorApp::playHandler etc)
    * and creates connection object.
    * Also, it initializes gettext textdomain for translating string constants.
    */
 	bool init();
 
 	/**
    * Application running procedure.
    * It runs endless loop in that depending on state it calls someone of application handler.
    * When player successfully connected it periodically calls CEditorApp::playHandler.
    */
 	void go();
 
 
   /**
    * Application shutdown procedure. Deletes memory resources obtained by config file
    * and sends disconnect() message to connection object.
    */
 	void shutdown();
 
 	/**
    * Returning application instance.
    * As application positions itself as "singleton" there can be only one instance of it.
    * So that proc creates instance at first time calling and always returns current instance later.
    */
 	static CEditorApp* instance() {return m_singleton;}
 
 	String getSetting(const String& key);
 	void setSetting(const String& key, const String& value);
 
 	///Perform pulse
 	bool pulse(Real time);
 
 	Real getFPS();
 	uint getTriCount();
 	/// Get program version 
 	std::vector<byte> getVersion();
 
 	RenderWindow* getRenderWindow() const {return m_window;}
 private:
 	static CEditorApp* m_singleton; ///< Application single instance ("singleton").
 
 	Ogre::Root* m_ogreRoot;
 	CEditorFrameListener* m_frameListener;
 	Ogre::RenderWindow* m_window;
 };
 
 #endif
