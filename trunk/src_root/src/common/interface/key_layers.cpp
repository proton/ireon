#include "stdafx.h"
#include "common/interface/key_layers.h"
#include "common/interface/keys.h"
void RepeatKeyEventPL::putPress(const OIS::KeyEvent &arg)
{
	OIS::KeyEvent* key = new OIS::KeyEvent(arg);
	m_repeatKey.reset(key); // save key for lated repeat
	m_time = 0;				// set the timer to 0
	m_nextLayer->putPress(arg);
}
void RepeatKeyEventPL::putRelease(const OIS::KeyEvent &arg)
{
	m_nextLayer->putRelease(arg);
	if (m_repeatKey.get() && arg.key == m_repeatKey->key)
		m_repeatKey.reset();
}
void RepeatKeyEventPL::pulse()
{
	CInterface::getRepeatKeyTime(m_firstRepeatTime, m_secondRepeatTime);
	m_nextLayer->pulse();
	if(m_repeatKey.get())	//if we need in repeat
	{
		Ogre::Timer* timer = Ogre::Root::getSingletonPtr()->getTimer();
		ulong nowTime = timer->getMillisecondsCPU();
		if ( m_time == 0)	// first pulse after pressing key
			m_time = nowTime + m_firstRepeatTime;
		else if(  long(nowTime - m_time) > long(m_secondRepeatTime)) // time when we must be repeat key and char
		{
			m_nextLayer->putPress( *(m_repeatKey.get()) );
			m_time = nowTime;
		};

	};
}

//------------------------------------------------------------------

void CEGUIKeyEventPL::putPress(const OIS::KeyEvent &arg)
{
	bool handled = CEGUI::System::getSingleton().injectKeyDown( arg.key );
	handled		|= CEGUI::System::getSingleton().injectChar( arg.text );
	if ( !handled )
		m_nextLayer->putPress(arg);
}
void CEGUIKeyEventPL::putRelease(const OIS::KeyEvent &arg)
{
	CEGUI::System::getSingleton().injectKeyUp( arg.key );
	m_nextLayer->putRelease(arg);
};
void CEGUIKeyEventPL::pulse()
{
	m_nextLayer->pulse();
};

//--------------------------------------------------------------

void EventKeyEventPL::putPress(const OIS::KeyEvent &arg)
{
	bool handled = CEventManager::instance()->keyPressed(arg.key);
	if ( !handled )
		m_nextLayer->putPress(arg);
}
void EventKeyEventPL::putRelease(const OIS::KeyEvent &arg)
{
	CEventManager::instance()->keyReleased(arg.key);
	m_nextLayer->putRelease(arg);
};
void EventKeyEventPL::pulse()
{
	m_nextLayer->pulse();
};

//---------------------------------------------------------------

void ActionKeyEventPL::putPress(const OIS::KeyEvent &arg)
{
	KeyboardActionMap::actionMapType::iterator it =
		CInterface::getKeyboardMap()->getAction(arg.key);
	if ( it != CInterface::getKeyboardMap()->end() )
		CDispatcher::getSingletonPtr()->executeNow((*it).second);	
}
void ActionKeyEventPL::putRelease(const OIS::KeyEvent &arg)
{
	m_nextLayer->putRelease(arg);
};
void ActionKeyEventPL::pulse()
{
	m_nextLayer->pulse();
};

//----------------------------------------------------------------

void PulseRepeatKeyEventPL::putPress(const OIS::KeyEvent &arg)
{
	OIS::KeyEvent* key = new OIS::KeyEvent(arg);
	m_repeatKey.reset(key); // save key for lated repeat
	m_nextLayer->putPress(arg);

}
void PulseRepeatKeyEventPL::putRelease(const OIS::KeyEvent &arg)
{
	m_nextLayer->putRelease(arg);
	if (m_repeatKey.get() && arg.key == m_repeatKey->key)
		m_repeatKey.reset();
};
void PulseRepeatKeyEventPL::pulse()							
{
	if (m_repeatKey.get())
		m_nextLayer->putPress( *m_repeatKey );
};
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------

void CEGUIMouseEventPL::putPress(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	bool handled = CEGUI::System::getSingleton().injectMouseButtonDown((CEGUI::MouseButton)id);
	if ( !handled )
		m_nextLayer->putPress(arg, id);
}
void CEGUIMouseEventPL::putRelease(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	bool handled = CEGUI::System::getSingleton().injectMouseButtonUp((CEGUI::MouseButton)id);
	if ( !handled )
		m_nextLayer->putRelease(arg, id);
};
void CEGUIMouseEventPL::putMove(const OIS::MouseEvent &arg)
{
	bool handled = CEGUI::System::getSingleton().injectMouseMove( (float)arg.state.relX, (float)arg.state.relY );
	if ( !handled )
		m_nextLayer->putMove(arg);
}
void CEGUIMouseEventPL::pulse()
{
	m_nextLayer->pulse();
};

//--------------------------------------------------------------------

void ActionMouseEventPL::putPress(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	MouseActionMap::actionMapType::iterator it =
		CInterface::getMouseMap()->getAction(id);
	if ( it != CInterface::getMouseMap()->end() )
		CDispatcher::getSingletonPtr()->executeNow((*it).second);	
}

void ActionMouseEventPL::putRelease(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	m_nextLayer->putRelease(arg, id);
};

void ActionMouseEventPL::putMove(const OIS::MouseEvent &arg)
{
	int mouseCode1 = 0;
	int mouseCode2 = 0;

	if(arg.state.relY>0)
		mouseCode1 = OIS::MC_DOWN;
	if(arg.state.relY<0)
		mouseCode1 = OIS::MC_UP;

	if(arg.state.relX>0)
		mouseCode2 = OIS::MC_RIGHT;
	if(arg.state.relX<0)
		mouseCode2 = OIS::MC_LEFT;
	
	if (mouseCode1)
	{
		MouseActionMap::actionMapType::iterator it =
			CInterface::getMouseMap()->getAction(mouseCode1);
		if ( it != CInterface::getMouseMap()->end() )
			CDispatcher::getSingletonPtr()->executeNow((*it).second);	
	}
	if (mouseCode2)
	{
		MouseActionMap::actionMapType::iterator it =
			CInterface::getMouseMap()->getAction(mouseCode2);
		if ( it != CInterface::getMouseMap()->end() )
			CDispatcher::getSingletonPtr()->executeNow((*it).second);	
	}

	m_nextLayer->putMove(arg);
};

void ActionMouseEventPL::pulse()
{
	m_nextLayer->pulse();
};
