/* Copyright (C) 2005 ireon.org developers council
  * $Id: interface.cpp 855 2007-04-27 09:33:36Z mip $
 
  *  See LICENSE for details
  */
 
 /**
  * @file interface.cpp
  * Interface manager
  */
#include "stdafx.h"
#include "common/interface/interface.h"
#include "common/interface/window.h"
#include "common/interface/event_manager.h"
#include "ireon_client/world/client_world.h"
#include "ireon_client/client_app.h"
#include "boost/algorithm/string.hpp"
#pragma warning(push, 1)
#include <Ogre/main/OgreNoMemoryMacros.h>
#include <CEGUI/CEGUIImageset.h>
#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUILogger.h>
#include <CEGUI/CEGUISchemeManager.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/CEGUIFontManager.h>
#include <CEGUI/CEGUIFont.h>
#include <CEGUI/CEGUIWindow.h>
#include <Ogre/main/OgreMemoryMacros.h>

#include "CEGUI/renderers/OgreGUIRenderer/OgreCEGUIRenderer.h"
#include "CEGUI/renderers/OgreGUIRenderer/OgreCEGUIResourceProvider.h"
#include "Ogre/main/OgreCursor.h"
//#include "Ogre/paging_landscape/OgrePagingLandScapeRaySceneQuery.h"
#include "OIS/OIS.h"
#pragma warning(pop)
#include "Ogre/main/OgreMouseEvent.h"
//#include "OgreInput.h"
//#include "OgreMath.h"
//#include "paging_landscape/OgrePagingLandScapeOptions.h"
//#include "paging_landscape/OgrePagingLandScapeSceneManager.h"

#include "ireon_client/xsd/config_xsd.h"
#include "common/file/directory.h"


#include "common/interface/key_layers.h"
#include "common/interface/keys.h"

#include "ireon_client/python_script_module/python_script_module.h"
typedef boost::shared_ptr<CDirectory> DirPtr;

#include "ireon_client/interface/client_camera.h"
#include "common/world/managers/dynamic_collection.h"
//----------------------------------------------------------------//
CEGUI::MouseButton convertOISMouseButtonToCegui(int buttonID)
{
	switch (buttonID)
	{
	case 0: return CEGUI::LeftButton;
	case 1: return CEGUI::RightButton;
	case 2:	return CEGUI::MiddleButton;
	case 3: return CEGUI::X1Button;
	default: return CEGUI::LeftButton;
	}
}


CEGUI::MouseButton convertOgreButtonToCegui(int buttonID)
{
	switch (buttonID)
	{
	case Ogre::MouseEvent::BUTTON0_MASK:
		return CEGUI::LeftButton;
	case Ogre::MouseEvent::BUTTON1_MASK:
		return CEGUI::RightButton;
	case Ogre::MouseEvent::BUTTON2_MASK:
		return CEGUI::MiddleButton;
	case Ogre::MouseEvent::BUTTON3_MASK:
		return CEGUI::X1Button;
	default:
		return CEGUI::LeftButton;
	}
}

ulong CInterface::m_firstRepeatTime = 0;
ulong CInterface::m_secondRepeatTime = 0;
KeyboardActionMap* CInterface::keyboardMap_ = NULL;
MouseActionMap* CInterface::mouseMap_ = NULL;

CInterface::CInterface():
m_eventProcessor(NULL),
m_GUIRenderer(NULL),
m_playerCam(NULL),
m_window(NULL),
m_query(NULL),
m_GUISystem(NULL),
m_shutdown(false)
{
};

CInterface::~CInterface()
{
	if( m_eventProcessor )
		delete m_eventProcessor;
	if( m_GUIRenderer )
		delete m_GUIRenderer;
	if( m_playerCam )
		delete m_playerCam;
	if( m_GUISystem )
		delete m_GUISystem;
	if( m_query )
		I_WORLD->sceneManager()->destroyQuery(m_query);
	OIS::InputManager* im = OIS::InputManager::getSingletonPtr();
	if( im )
	{
		im->destroyInputObject( m_mouse );
		im->destroyInputObject( m_keyboard );
		im->destroyInputSystem();
		im = 0;
	}
	if (keyboardMap_)
		delete keyboardMap_;
};

CEGUI::System* CInterface::getGUISystemPtr()
{
	assert(m_GUISystem);
	return m_GUISystem;
}


CEGUI::WindowManager* CInterface::getGUIWinManagerPtr()
{
	assert(winMgr_);
	return winMgr_;	
}
CEGUI::Window* CInterface::getRootWindow()
{
	// if exception in init occured 
	assert(m_mainWindow);
	return m_mainWindow;	
}
bool CInterface::init(Ogre::RenderWindow* win)
{
	_log(_info,_loc,_("Interface initialization..."));

	try
	{
	Ray ray;
	m_window = win;
	m_playerCam = I_WORLD->sceneManager()->getCamera("PlayerCam");
	camera_ = new ClientCamera();
	camera_->setObserveMode(ClientCamera::THIRD_PERSON);
	camera_->watch(
		CDynamicCollection::getSingletonPtr()->getOwnPlayer()
		);
	rotateSpeed_ = 90;
	rotateScale_ = 0.0f;

	m_eventProcessor = new Ogre::EventProcessor();
	m_eventProcessor->initialise(win);
	m_eventProcessor->startProcessingEvents();


	
	m_query = I_WORLD->sceneManager()->createRayQuery(ray);
 
	m_GUIRenderer = new CEGUI::OgreCEGUIRenderer(
							m_window, 
							Ogre::RENDER_QUEUE_OVERLAY,
							false,
							3000,
							I_WORLD->sceneManager()
							);
#ifdef _DEBUG
	CEGUI::System::setDefaultXMLParserName("XercesParser-vs80-md-d-0_5_0.dll");
#elif defined NDEBUG
	CEGUI::System::setDefaultXMLParserName("XercesParser-vs80-md-0_5_0.dll");
#else 
#error _DEBUG or NDEBUG must be defined.
#endif
	m_GUISystem = new CEGUI::System(
							m_GUIRenderer,
							NULL,
							NULL
							,I_APP->getScriptModule(),
							"",
							"../logs/cegui.log"
							);

	CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);
	
	// instance singleton for right access from python
	winMgr_ = CEGUI::WindowManager::getSingletonPtr();
	imgMgr_ = CEGUI::ImagesetManager::getSingletonPtr();
	scmMgr_ = CEGUI::SchemeManager::getSingletonPtr();

	_log(_info,_loc,_("Inititalize Font"));
	CEGUI::Font* defaultFont = CEGUI::FontManager::getSingleton().createFont("tahoma-12.font");
	m_GUISystem->setDefaultFont(defaultFont);
	//CEGUI::Font *f = CEGUI::FontManager::getSingleton().getFont((CEGUI::utf8*)"Tahoma-12");
	//f->->defineFontGlyphs((CEGUI::utf8*)I_APP->getSetting("/config/interface/symbols").c_str());

	// Loading GUI schemes
	loadCeguiSchemes();
	
	
	_log(_info,_loc,_("Initialize input system scaling") );

	m_GUISystem->setDefaultMouseCursor("TaharezLook", "MouseArrow");
	m_GUISystem->setMouseMoveScaling(2);
 	m_eventProcessor->getInputReader()->setMouseScale(2);
	setRepeatKeyTime(400,100);

	_log(_info,_loc,"Create CEGUI mainWindow");

	CEGUI::Window* mainWindow;
	mainWindow = CEGUI::WindowManager::getSingleton().
			loadWindowLayout( (CEGUI::utf8*)"root.layout" ) ;
	m_GUISystem->setGUISheet(mainWindow);
	
	
 	m_mainWindow = mainWindow;

	/*mainWindow->show();
	CEGUI::Window* m = CEGUI::WindowManager::getSingleton().createWindow("ChatWindow/MainWindow");
 	m_GUISystem->getGUISheet()->addChildWindow(m);
	m->show();*/
	Ogre::Root::getSingleton().addFrameListener(this);
 
 	rotX_ = 0;
 	rotY_ = 0;
 
	initOIS();
 	
	}
	catch (const CEGUI::Exception& e)
	{
		_log(_error,_loc,_("Error in initialisation CEGUI: %s"),
		 (CEGUI::utf8*)  e.getMessage().c_str()  );
		return false;
	}
	_log(_info,_loc,_("Interface initialization complete."));
 	return true;
 };
 
 
bool CInterface::frameStarted(const Ogre::FrameEvent &evt)
{
	if(m_window->isClosed())
		return false;
	//Need to capture/update each device
	m_keyboard->capture();		
	m_mouse->capture();
	// synchronised frame and key process
	keyLayer_->pulse();
	mouseLayer_->pulse();

	return true;
};
 
bool CInterface::frameEnded(const Ogre::FrameEvent& evt)
{
 	if (m_shutdown)
 		return false;
 	return true;
 }
 //----------------------------------------------------------------//
 bool CInterface::mouseMoved( const OIS::MouseEvent &arg )
 {
	 mouseLayer_->putMove(arg);
	 return true;
 }

 //----------------------------------------------------------------//
 bool CInterface::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
 {
	 mouseLayer_->putPress(arg, id);
	 return true;
 }

 //----------------------------------------------------------------//
 bool CInterface::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
 {
	 mouseLayer_->putRelease(arg, id);
	 return true;
 }

bool CInterface::keyPressed( const OIS::KeyEvent &arg )
{
	keyLayer_->putPress(arg);
	return true;
}
 //----------------------------------------------------------------//
bool CInterface::keyReleased( const OIS::KeyEvent &arg )
{
	keyLayer_->putRelease(arg);
	return true;
}


void CInterface::initOIS()
{
	_log(_info,_loc,_("Init OIS"));
	
	bool bufferedKeys  = true;
	bool bufferedMouse = true;
	size_t windowHnd = 0;
	m_window->getCustomAttribute("HWND", &windowHnd);

	std::ostringstream windowHndStr;
	windowHndStr << windowHnd;
	
	OIS::ParamList pl;		
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	OIS::InputManager &im = *OIS::InputManager::createInputSystem( pl );

	m_keyboard = static_cast<OIS::Keyboard*>(im.createInputObject( OIS::OISKeyboard, bufferedKeys ));
	m_mouse = static_cast<OIS::Mouse*>(im.createInputObject( OIS::OISMouse, bufferedMouse ));
	
	unsigned int width, height, depth;
	int left, top;
	m_window->getMetrics(width, height, depth, left, top);

	//Set Mouse Region.. if window resizes, we should alter this to reflect as well
	const OIS::MouseState &ms = m_mouse->getMouseState();
	ms.width = width;
	ms.height = height;

	_log(_info,_loc,_("Set OIS callbacks"));
	
	m_keyboard->setEventCallback(this);
	m_mouse->setEventCallback(this);
	
	_log(_info,_loc,_("Initialize OIS completed") );
	
	_log(_info,_loc,_("Set Keyboard process layers..."));
	RepeatKeyEventPL*		repeatLayer	= new RepeatKeyEventPL;
	EventKeyEventPL*		eventLayer	= new EventKeyEventPL;
	CEGUIKeyEventPL*		ceguiLayer	= new CEGUIKeyEventPL;
	PulseRepeatKeyEventPL*	pulseLayer	= new PulseRepeatKeyEventPL;
	ActionKeyEventPL*		actionLayer	= new ActionKeyEventPL;
	KillKeyEventPL*			stopLayer	= new KillKeyEventPL;

	keyLayer_.reset(repeatLayer);
	repeatLayer->setNextLayer( eventLayer );
	eventLayer->setNextLayer( ceguiLayer );
	ceguiLayer->setNextLayer( pulseLayer );
	pulseLayer->setNextLayer(actionLayer);
	actionLayer->setNextLayer( stopLayer );

	_log(_info,_loc,_("Set Mouse process layers..."));
	
	CEGUIMouseEventPL*		ceguiMLayer	= new CEGUIMouseEventPL;
	ActionMouseEventPL*		actionMLayer	= new ActionMouseEventPL;
	KillMouseEventPL*		killMLayer	= new KillMouseEventPL;

	mouseLayer_.reset(ceguiMLayer);
	ceguiMLayer->setNextLayer(actionMLayer);
	actionMLayer->setNextLayer( killMLayer );


	keyboardMap_ = new KeyboardActionMap();
	keyboardMap_->setDefaultMap();
	mouseMap_ = new MouseActionMap();
	mouseMap_->setDefaultMap();
}
void CInterface::loadCeguiSchemes()
{
	_log(_info,_loc,_("Loading GUI schemes"));
	String pathToInterface;
	
	if ( I_APP->getConfig()->_interface().present() && I_APP->getConfig()->_interface().get().dir().present() )
		pathToInterface = I_APP->getConfig()->_interface().get().dir().get();			
	else
		pathToInterface = ireon::client::CInterfaceSettings::dir::default_value();
	String pathToSchemes = pathToInterface;
	String pathToLayouts = pathToInterface;

	pathToSchemes.append("/schemes");
	pathToLayouts.append("/layouts");

	DirPtr dir(new CDirectory());
	dir->open( pathToSchemes.c_str() );

	if( dir->isOpen() )
	{
		bool res = dir->setToFirstEntry();
		while( res )
		{
			if( dir->getEntryType() == CDirectory::FILE )
				if(boost::algorithm::ends_with(dir->getEntryName(),".scheme"))
				{
					String path = dir->getEntryName();
					boost::algorithm::erase_head(path,path.find_last_of('/')+1);
					CEGUI::SchemeManager::getSingleton().loadScheme(
						(CEGUI::utf8*)path.c_str());
					_log(_info,_loc,_("Loading scheme '%s'.\n"),path.c_str());
				};
			res = dir->setToNextEntry();
		};
	};
	dir->close();

	dir->open( pathToLayouts.c_str());
	if( dir->isOpen() )
	{
		bool res = dir->setToFirstEntry();
		while( res )
		{
			if( dir->getEntryType() == CDirectory::FILE )
				if(boost::algorithm::ends_with(dir->getEntryName(),".layout") )
				{
					String path = dir->getEntryName();
					boost::algorithm::erase_head(path,path.find_last_of('/')+1);
					if( path != "root.layout" )
					{
						_log(_info,_loc,_("Loading layout '%s'.\n"),path.c_str());
						CEGUI::WindowManager::getSingleton().loadWindowLayout(
							(CEGUI::utf8*)path.c_str());
					}
				};
			res = dir->setToNextEntry();
		};
	};
	dir->close();

}
 /*
 
 bool CInterface::isCursorShown()
 {
 	return CEGUI::MouseCursor::getSingleton().isVisible();
 };
 
 void CInterface::setShowCursor(bool value)
 {
 	if( value )
 		CEGUI::MouseCursor::getSingleton().show();
 	else
 		CEGUI::MouseCursor::getSingleton().hide();
 };
*/