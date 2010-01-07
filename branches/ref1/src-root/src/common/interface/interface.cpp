/* Copyright (C) 2005 ireon.org developers council
 * $Id: interface.cpp 510 2006-02-26 21:09:40Z zak $

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

/**
 * @file interface.cpp
 * Interface manager
 */

#include "stdafx.h"

#include <OgreNoMemoryMacros.h>
#include <CEGUI/CEGUIImageset.h>
#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUILogger.h>
#include <CEGUI/CEGUISchemeManager.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/CEGUIFontManager.h>
#include <CEGUI/CEGUIFont.h>
#include <OgreMemoryMacros.h>
#include <CEGUI/Renderers/OgreGUIRenderer/OgreCEGUIRenderer.h>
#include <CEGUI/Renderers/OgreGUIRenderer/OgreCEGUIResourceProvider.h>
#include "file/directory.h"
#include "OgreCursor.h"
#include "OgrePagingLandScapeRaySceneQuery.h"

#include "common/misc/boost_string_algo_wrapper.h"

//#include "OgreInput.h"
//#include "OgreMath.h"
//#include "paging_landscape/OgrePagingLandScapeOptions.h"
//#include "paging_landscape/OgrePagingLandScapeSceneManager.h"

#include "interface/window.h"

static std::map<CEGUI::utf32,CEGUI::utf32> convertMap;

#define ADD_CONVERT(from,to) convertMap.insert(std::pair<CEGUI::utf32,CEGUI::utf32>((from),CEGUI::String((CEGUI::utf8*)to).at(0)));

void CInterface::initConvert()
{
	ADD_CONVERT('q',"й");
	ADD_CONVERT('w',"ц");
	ADD_CONVERT('e',"у");
	ADD_CONVERT('r',"к");
	ADD_CONVERT('t',"е");
	ADD_CONVERT('y',"н");
	ADD_CONVERT('u',"г");
	ADD_CONVERT('i',"ш");
	ADD_CONVERT('o',"щ");
	ADD_CONVERT('p',"з");
	ADD_CONVERT('[',"х");
	ADD_CONVERT(']',"ъ");
	ADD_CONVERT('a',"ф");
	ADD_CONVERT('s',"ы");
	ADD_CONVERT('d',"в");
	ADD_CONVERT('f',"а");
	ADD_CONVERT('g',"п");
	ADD_CONVERT('h',"р");
	ADD_CONVERT('j',"о");
	ADD_CONVERT('k',"л");
	ADD_CONVERT('l',"д");
	ADD_CONVERT(';',"ж");
	ADD_CONVERT('\'',"э");
	ADD_CONVERT('z',"я");
	ADD_CONVERT('x',"ч");
	ADD_CONVERT('c',"с");
	ADD_CONVERT('v',"м");
	ADD_CONVERT('b',"и");
	ADD_CONVERT('n',"т");
	ADD_CONVERT('m',"ь");
	ADD_CONVERT(',',"б");
	ADD_CONVERT('.',"ю");
	ADD_CONVERT('/',".");
	ADD_CONVERT('Q',"Й");
	ADD_CONVERT('W',"Ц");
	ADD_CONVERT('E',"У");
	ADD_CONVERT('R',"К");
	ADD_CONVERT('T',"Е");
	ADD_CONVERT('Y',"Н");
	ADD_CONVERT('U',"Г");
	ADD_CONVERT('I',"Ш");
	ADD_CONVERT('O',"Щ");
	ADD_CONVERT('P',"З");
	ADD_CONVERT('{',"Х");
	ADD_CONVERT('}',"Ъ");
	ADD_CONVERT('A',"Ф");
	ADD_CONVERT('S',"Ы");
	ADD_CONVERT('D',"В");
	ADD_CONVERT('F',"А");
	ADD_CONVERT('G',"П");
	ADD_CONVERT('H',"Р");
	ADD_CONVERT('J',"О");
	ADD_CONVERT('K',"Л");
	ADD_CONVERT('L',"Д");
	ADD_CONVERT(':',"Ж");
	ADD_CONVERT('"',"Э");
	ADD_CONVERT('Z',"Я");
	ADD_CONVERT('X',"Ч");
	ADD_CONVERT('C',"С");
	ADD_CONVERT('V',"М");
	ADD_CONVERT('B',"И");
	ADD_CONVERT('N',"Т");
	ADD_CONVERT('M',"Ь");
	ADD_CONVERT('<',"Б");
	ADD_CONVERT('>',"Ю");
	ADD_CONVERT('?',".");
	ADD_CONVERT('@',"\"");
	ADD_CONVERT('#',"№");
	ADD_CONVERT('$',";");
	ADD_CONVERT('^',":");
	ADD_CONVERT('&',"?");
	ADD_CONVERT('|',"/");
	ADD_CONVERT('`',"ё");
	ADD_CONVERT('~',"Ё");
};

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

CInterface::CInterface():
m_eventProcessor(NULL),
m_GUIRenderer(NULL),
m_playerCam(NULL),
m_window(NULL),
m_query(NULL),
m_GUISystem(NULL),
m_shutdown(false),
m_altLayout(false)
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
};

bool CInterface::init(Ogre::RenderWindow* win)
{
	CLog::instance()->log(CLog::msgLvlInfo,"Interface initialization...\n");
	initConvert();

	m_window = win;

	m_playerCam = I_WORLD->sceneManager()->getCamera("PlayerCam");

	m_rotateSpeed = 90;

	m_eventProcessor = new Ogre::EventProcessor();
	m_eventProcessor->initialise(win);
	m_eventProcessor->startProcessingEvents();
	m_eventProcessor->addKeyListener(this);
	m_eventProcessor->addMouseMotionListener(this);
	m_eventProcessor->addMouseListener(this);

	Ray ray;
	m_query = I_WORLD->sceneManager()->createRayQuery(ray);
	
	m_rotScale = 0.0f;

	m_GUIRenderer = new CEGUI::OgreCEGUIRenderer(m_window, 
		Ogre::RENDER_QUEUE_OVERLAY, false, 3000, I_WORLD->sceneManager() );

	m_GUISystem = new CEGUI::System(m_GUIRenderer,(CEGUI::utf8 *) "../logs/cegui.log");

	CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);

	// Loading schemes
	DirPtr dir(new CDirectory());
	dir->open((I_APP->getSetting("/config/interface/dir") + "schemes").c_str());
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
					CLog::instance()->log(CLog::msgLvlInfo,_("Loading scheme '%s'.\n"),path.c_str());
				};
			res = dir->setToNextEntry();
		};
	};

	dir->close();
	dir->open((I_APP->getSetting("/config/interface/dir") + "layouts").c_str());
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
						CLog::instance()->log(CLog::msgLvlTrace,_("Loading layout '%s'.\n"),path.c_str());
						CEGUI::WindowManager::getSingleton().loadWindowLayout(
							(CEGUI::utf8*)path.c_str());
					}
				};
			res = dir->setToNextEntry();
		};
	};
	dir->close();

	m_GUISystem->setDefaultMouseCursor(
		(CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
	CEGUI::FontManager::getSingleton().createFont("Tahoma-12.font");
	m_GUISystem->setDefaultFont((CEGUI::utf8*)"Tahoma-12");
	CEGUI::Font *f = CEGUI::FontManager::getSingleton().getFont((CEGUI::utf8*)"Tahoma-12");
	f->defineFontGlyphs((CEGUI::utf8*)I_APP->getSetting("/config/interface/symbols").c_str());


//	m_GUISystem->setMouseMoveScaling(2);
	m_eventProcessor->getInputReader()->getCursor()->setScale(2);

	m_mainWindow.reset(newWindowObject(BASIC,CEGUI::WindowManager::getSingleton().loadWindowLayout(
		(CEGUI::utf8*)"root.layout")));
	if( !m_mainWindow )
	{
		CLog::instance()->log(CLog::msgLvlError,_("Can't find root layout in layouts.\n"));
		return false;
	}
	m_GUISystem->setGUISheet(m_mainWindow->m_window);
	m_mainWindow->init();

	Ogre::Root::getSingleton().addFrameListener(this);

	m_rotX = 0;
	m_rotY = 0;

	CLog::instance()->log(CLog::msgLvlInfo,"Interface initialization complete.\n");
	return true;
};


bool CInterface::frameStarted(const Ogre::FrameEvent &evt)
{
	return true;
};

bool CInterface::frameEnded(const Ogre::FrameEvent& evt)
{
	if (m_shutdown)
		return false;
	return true;
}

void CInterface::mouseMoved (Ogre::MouseEvent *e)
{
	CEGUI::System::getSingleton().injectMouseMove(
		e->getRelX() * m_GUIRenderer->getWidth(), 
		e->getRelY() * m_GUIRenderer->getHeight());
	e->consume();
}

void CInterface::mouseDragged (Ogre::MouseEvent *e) 
{ 
	mouseMoved(e);
}

void CInterface::mousePressed (Ogre::MouseEvent *e)
{
	bool handled = CEGUI::System::getSingleton().injectMouseButtonDown(
	convertOgreButtonToCegui(e->getButtonID()));
	if( !handled )
	{
		mousePressedNoGUI(e);
		switch (e->getButtonID())
		{
			case e->BUTTON0_MASK:
			{
				m_mousePressed.insert(LBUTTON);
				break;
			}
			case e->BUTTON1_MASK:
				m_mousePressed.insert(RBUTTON);
				break;
			case e->BUTTON2_MASK:
				m_mousePressed.insert(MBUTTON);
				break;
		}
	}
    e->consume();
}

void CInterface::mouseReleased (Ogre::MouseEvent *e)
{
	bool handled = CEGUI::System::getSingleton().injectMouseButtonUp(
	convertOgreButtonToCegui(e->getButtonID()));
	if( !handled )
	{
		switch (e->getButtonID())
		{
			case e->BUTTON0_MASK:
				m_mousePressed.erase(LBUTTON);
				break;
			case e->BUTTON1_MASK:
				m_mousePressed.erase(RBUTTON);
				break;
			case e->BUTTON2_MASK:
				m_mousePressed.erase(MBUTTON);
				break;
		}
	}
	e->consume();
}

void CInterface::mouseClicked(Ogre::MouseEvent* e) {}
void CInterface::mouseEntered(Ogre::MouseEvent* e) {}
void CInterface::mouseExited(Ogre::MouseEvent* e) {}

void CInterface::keyPressed(Ogre::KeyEvent* e)
{
	if(e->getKey() == Ogre::KC_F4  && isKeyPressed(Ogre::KC_LMENU))
	{
			m_shutdown = true;
			e->consume();
			return;
	}
	if(e->getKey() == Ogre::KC_SYSRQ)
	{
		m_window->writeContentsToFile("screen.jpg");
	};

	bool handled = CEGUI::System::getSingleton().injectKeyDown(e->getKey());
	CEGUI::utf32 ch = e->getKeyChar();
	if( m_altLayout )
	{
		std::map<CEGUI::utf32,CEGUI::utf32>::iterator it = convertMap.find(ch);
		if( it != convertMap.end() )
			ch = (*it).second;
	};
	handled |= CEGUI::System::getSingleton().injectChar(ch);
	if( !handled || (!ch && e->getKey() != KC_RETURN) )
	{
		m_keysPressed.insert((Ogre::KeyCode)e->getKey());
		CEventManager::instance()->keyPressed((KeyCode)e->getKey());
		keyPressedNoGUI(e);
	}
	e->consume();
}

void CInterface::keyReleased(Ogre::KeyEvent* e)
{
	bool handled = CEGUI::System::getSingleton().injectKeyUp(e->getKey());

	CEventManager::instance()->keyReleased((KeyCode)e->getKey());

	m_keysPressed.erase((Ogre::KeyCode)e->getKey());

	e->consume();
}
void CInterface::keyClicked(Ogre::KeyEvent* e) 
{
	CEventManager::instance()->keyClicked((KeyCode)e->getKey());
	e->consume();
}

WndPtr CInterface::findWindow(const String& name, WndPtr root)
{
	if( !root )
		root = m_mainWindow;
	for( uint i = 0; i < root->getChildCount(); i++ )
	{
		if( name == root->getChildAtIdx(i)->m_window->getName().c_str() )
			return root->getChildAtIdx(i);
		if( WndPtr win = findWindow(name,root->getChildAtIdx(i)) )
			return win;
	}
	return WndPtr();
};

WndPtr CInterface::getWindow(const String& name, WinType type)
{
	WndPtr w = findWindow(name);
	if( w )
		return w;
	if( !CEGUI::WindowManager::getSingleton().isWindowPresent((CEGUI::utf8*)name.c_str()) )
	{
		CLog::instance()->log(CLog::msgLvlError,_("GetWindow: '%s' failed.\n"),name.c_str());
		return WndPtr();
	}
	CEGUI::Window* cwin =  CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)name.c_str());
	w = WndPtr(newWindowObject(type,cwin));
	return w;
};

WndPtr CInterface::createWindow(const String& type, const String& name, WinType wtype)
{
	CEGUI::Window* cwin =  CEGUI::WindowManager::getSingleton().createWindow((CEGUI::utf8*)type.c_str(),(CEGUI::utf8*)name.c_str());
	if( !cwin )
		return WndPtr();
	WndPtr w(newWindowObject(wtype,cwin));
	w->m_created = true;
	return w;
};


void CInterface::destroyWindow(const WndPtr& window, WndPtr root)
{
	if( !window )
		return;
	if( !root )
		root = m_mainWindow;
	if( !window->m_created ) ///If window wasn't created, it can't be destroyed
		return;
	uint i;
	for( i = 0; i < root->getChildCount(); i++ )
	{
		if( root->getChildAtIdx(i) == window )
		{
			root->removeChild(i);
			break;
		} else
			destroyWindow(window,root->getChildAtIdx(i));
	};
	for( i = 0; i < window->getChildCount(); i++ )
		destroyWindow(window->getChildAtIdx(i),window);
	window->deInit();
	CEGUI::WindowManager::getSingleton().destroyWindow(window->m_window);
	return;
};

WndPtr CInterface::unlinkWindow(const WndPtr& window, WndPtr root)
{
	if( !window )
		return WndPtr();
	if( !root )
		root = m_mainWindow;
	WndPtr w;
	for( uint i = 0; i < root->getChildCount(); i++ )
	{
		if( root->getChildAtIdx(i) == window )
		{
			w = root->getChildAtIdx(i);
			w->deInit();
			root->removeChild(i);
			return w;
		} else
			w = unlinkWindow(window,root->getChildAtIdx(i));
	};
	return w;
};

bool CInterface::hasChildren(const CWindow* win, const CWindow* root)
{
	bool res = false;
	if( !root || !win )
		return false;
	for( uint i = 0; i < root->getChildCount(); i++ )
	{
		if( root->getChildAtIdx(i).get() == win )
			return true;
		res = res || hasChildren(win,root->getChildAtIdx(i).get());
	}
	return res;
};
void CInterface::disableWindows(const CWindow* except, CWindow* root)
{
	if( !root )
		root = m_mainWindow.get();
	if( root == except )
		return;
	if( !hasChildren(except,root) )
		root->disable();
	else
		for( uint i = 0; i < root->getChildCount(); i++ )
			disableWindows(except,root->getChildAtIdx(i).get());
};

void CInterface::enableWindows(WndPtr root)
{
	if( !root )
		root = m_mainWindow;
	root->enable();
	for( uint i = 0; i < root->getChildCount(); i++ )
		enableWindows(root->getChildAtIdx(i));
};

bool CInterface::linkWindow(const WndPtr& win, WndPtr parent)
{
	if( !win )
		return false;
	if( !parent )
		parent = m_mainWindow;
	if( hasChildren(win.get(),m_mainWindow.get()) )
		return false;
	parent->addChildWindow(win);
	if( parent->active() )
		win->init();
	return true;
};


CWindow* CInterface::newWindowObject(WinType type, CEGUI::Window* win)
{
	switch(type)
	{
		case BASIC:
			return new CWindow(win);
		case FRAME:
			return new CFrameWindow(win);
		case STATIC_TEXT:
			return new CStaticTextWindow(win);
		case STATIC_IMAGE:
			return new CStaticImage(win);
		case MULTI_LIST:
			return new CMultiListWindow(win);
		case CHECK_BOX:
			return new CCheckBox(win);
		case LIST:
			return new CListBox(win);
		case SCROLL_BAR:
			return new CScrollBar(win);
		case PROGRESS_BAR:
			return new CProgressBar(win);
	};
	return NULL;
		
};

float CInterface::getFontHeight(const String& font)
{
	CEGUI::Font* f = 0;
	if( font != BLANK_STRING )
		f = CEGUI::FontManager::getSingleton().getFont((CEGUI::utf8*)font.c_str());
	if( !f )
		f = CEGUI::System::getSingleton().getDefaultFont();
	return f->getFontHeight();
};
float CInterface::getTextExtent(const String& text, const String& font)
{
	CEGUI::Font* f = 0;
	if( font != BLANK_STRING )
		f = CEGUI::FontManager::getSingleton().getFont((CEGUI::utf8*)font.c_str());
	if( !f )
		f = CEGUI::System::getSingleton().getDefaultFont();
	return f->getTextExtent((CEGUI::utf8*)text.c_str());
};

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