

#ifndef _KEY_PROCCESS_LAYER_
#define _KEY_PROCCESS_LAYER_
#pragma warning(push, 1)
#include <Ogre/main/OgreNoMemoryMacros.h>
#include "OIS/OIS.h"
#include "Ogre.h"

#include <CEGUI/CEGUIImageset.h>
#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUILogger.h>
#include <CEGUI/CEGUISchemeManager.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/CEGUIFontManager.h>
#include <CEGUI/CEGUIFont.h>
#include <CEGUI/CEGUIWindow.h>
#include <Ogre/main/OgreMemoryMacros.h>

#pragma warning(pop)
#include "common/interface/event_manager.h"
#include "common/interface/interface.h"
#include "common/world/managers/dispatcher.h"
#include "common/interface/keys.h"

//PL - ProcessLayer


template <typename T>
struct InputSystemEventPL
{
	void setNextLayer(T* layer)
	{
		m_nextLayer.reset(layer);
	};
protected:
	std::auto_ptr<T> m_nextLayer;
};

//---------------------------------------------------------------------------

struct KeyEventPL: public InputSystemEventPL<KeyEventPL>
{
	virtual void putPress(const OIS::KeyEvent &arg)=0;
	virtual void putRelease(const OIS::KeyEvent &arg)=0;
	virtual void pulse()=0;
};

struct RepeatKeyEventPL: public KeyEventPL
{
	void putPress(const OIS::KeyEvent &arg);
	void putRelease(const OIS::KeyEvent &arg);
	void pulse();

private:
	/// time  between press and token time
	ulong m_firstRepeatTime;
	/// time between token time and send char
	/// etc time send char = time_press( +) + m_first_repeat_time + m_second_repeat_time
	ulong m_secondRepeatTime;
	ulong m_time;		
	std::auto_ptr<OIS::KeyEvent> m_repeatKey; 
};

struct CEGUIKeyEventPL: public KeyEventPL
{
	void putPress(const OIS::KeyEvent &arg);
	void putRelease(const OIS::KeyEvent &arg);
	void pulse();
};

struct EventKeyEventPL: public KeyEventPL
{
	void putPress(const OIS::KeyEvent &arg);
	void putRelease(const OIS::KeyEvent &arg);
	void pulse();
};

struct ActionKeyEventPL: public KeyEventPL
{
	void putPress(const OIS::KeyEvent &arg);
	void putRelease(const OIS::KeyEvent &arg);
	void pulse();
};

struct KillKeyEventPL: public KeyEventPL
{
	void putPress(const OIS::KeyEvent&)	{ }
	void putRelease(const OIS::KeyEvent&)	{ };
	void pulse()							{ };
};

// TODO: may be merge with CRepeatKeyProcessLayer ?
struct PulseRepeatKeyEventPL: public KeyEventPL
{
	void putPress(const OIS::KeyEvent &arg);
	void putRelease(const OIS::KeyEvent &arg);
	void pulse();

private:
	std::auto_ptr<OIS::KeyEvent> m_repeatKey;
};


//----------------------------------------------------------------------------

struct MouseEventPL: public InputSystemEventPL<MouseEventPL>
{
	virtual void putPress(const OIS::MouseEvent &arg, OIS::MouseButtonID id)=0;
	virtual void putRelease(const OIS::MouseEvent &arg, OIS::MouseButtonID id)=0;
	virtual void putMove(const OIS::MouseEvent &arg)=0;
	virtual void pulse()=0;
};

struct CEGUIMouseEventPL: public MouseEventPL
{
	virtual void putPress(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual void putRelease(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual void putMove(const OIS::MouseEvent &arg);
	void pulse();
};


struct ActionMouseEventPL: public MouseEventPL
{
	virtual void putPress(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual void putRelease(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual void putMove(const OIS::MouseEvent &arg);
	void pulse();
};

struct KillMouseEventPL: public MouseEventPL
{
	virtual void putPress(const OIS::MouseEvent &arg, OIS::MouseButtonID id){};
	virtual void putRelease(const OIS::MouseEvent &arg, OIS::MouseButtonID id){};
	virtual void putMove(const OIS::MouseEvent &arg){};
	void pulse(){};
};

#endif