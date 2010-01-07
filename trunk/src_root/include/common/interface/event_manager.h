/* Copyright (C) 2005 ireon.org developers council
 * $Id: event_manager.h 844 2007-01-03 13:28:38Z mip $

 *  See LICENSE for details
 */
 
/**
* @file script_manager.h
* Script manager
*/
#ifndef _SCRIPT_MANAGER_H
#define _SCRIPT_MANAGER_H
#pragma warning(push, 1)
#include "boost/shared_ptr.hpp"
#include "Ogre/main/OgreNoMemoryMacros.h"
#include "CEGUI/CEGUIForwardRefs.h"
#include "CEGUI/CEGUIString.h"
#include "Ogre/main/OgreMemoryMacros.h"
#include "Ogre/main/Ogre.h"
#include "Ogre/main/OgreInput.h"
#pragma warning(pop)
#include "OIS/OISKeyboard.h"
class CDynamicObject;
typedef boost::shared_ptr<CDynamicObject> CharacterPtr;


// ///Handler for CEGUI events
//struct WindowEvent
//{
//	virtual bool operator()(const CEGUI::EventArgs& arg) const;
// 
// 	///Window's name
// 	String m_window;
// 	///Handler
// 	String m_handler;
//};
// 
// ///Handler for CEGUI key events
// struct WindowKeyEvent : public WindowEvent
// {
// 	virtual bool operator()(const CEGUI::EventArgs& arg) const;
// };
// 
#ifndef SWIG
typedef void *(*swigConverterFunc)(void *);
typedef struct swigTypeInfo *(*swigDycastFunc)(void **);
/* Structure to store inforomation on one type */
typedef struct swigTypeInfo {
	const char             *name;			/* mangled name of this type */
	const char             *str;			/* human readable name of this type */
	swigDycastFunc        dcast;		/* dynamic cast function down a hierarchy */
	struct swigCastInfo  *cast;			/* linked list of types that can cast into this type */
	void                   *clientdata;		/* language specific type data */
	int                    owndata;		/* flag if the structure owns the clientdata */
} swigTypeInfo;
/* Structure to store a type and conversion function used for casting */
typedef struct swigCastInfo {
	swigTypeInfo         *type;			/* pointer to type that is equivalent to this type */
	swigConverterFunc     converter;		/* function to cast the void pointers */
	struct swigCastInfo  *next;			/* pointer to next cast in linked list */
	struct swigCastInfo  *prev;			/* pointer to the previous cast */
} swigCastInfo;
#endif 


struct EventArg;


enum Event
{
	///No event (for m_running)
	NONE,
	///Key is pressed
	ON_KEY_PRESSED,
	///Key is released
	ON_KEY_RELEASED,
	///Key is clicked
	ON_KEY_CLICKED,
	///New frame
	ON_FRAME,
	///Character inserted to world
	ON_CHAR_TO_WORLD,
	///Character removed from world
	ON_CHAR_FROM_WORLD,
	///Character update
	ON_CHAR_UPDATE,
	/// Char died
	ON_CHAR_DIE,
	/// Amount of events
	EVENT_COUNT
};

struct StringLess
{
	bool operator() (const String& arg1, const String& arg2)
	{
		if ( strcmp(arg1.c_str(), arg2.c_str()) < 0 )
			return true;
		else 
			return false;
	}
};

class CEventManager: protected CLogMixin
{
protected:
	CEventManager();

public:
	static CEventManager* instance();
	
	void registerSwigType(swigTypeInfo* arg, String& eventName);
	swigTypeInfo* getSwigType(String& eventName);
	///Execute event with specified id
	bool execute(uint id, EventArg *arg);
	///Execute event handler with specified name
	bool execute(const String& name,EventArg *arg);

	///Key events
	bool keyPressed(OIS::KeyCode code);
	bool keyReleased(OIS::KeyCode code);
	bool keyClicked(OIS::KeyCode code);

	void addEvent(uint id, const String& handler) {m_handlers[id].push_back(handler);}
	void removeEvent(uint id, const String& handler);

	void addKeyPressedEvent(OIS::KeyCode code, const String& handler) {m_keyPressedHandlers[code].push_back(handler);}
	void addKeyReleasedEvent(OIS::KeyCode code, const String& handler) {m_keyReleasedHandlers[code].push_back(handler);}
	void addKeyClickedEvent(OIS::KeyCode code, const String& handler) {m_keyClickedHandlers[code].push_back(handler);}

	void removeKeyPressedEvent(OIS::KeyCode code, const String& handler);
	void removeKeyReleasedEvent(OIS::KeyCode code, const String& handler);
	void removeKeyClickedEvent(OIS::KeyCode code, const String& handler);


protected:
	///Event handlers (names of python functions)
	std::map<uint, StringVector> m_handlers;

	std::map<OIS::KeyCode, StringVector> m_keyPressedHandlers;
	std::map<OIS::KeyCode, StringVector> m_keyReleasedHandlers;
	std::map<OIS::KeyCode, StringVector> m_keyClickedHandlers;

	std::map<String, swigTypeInfo*, StringLess> swigTypeInfoMap_;
	static CEventManager* m_singleton;

	/// Currently executed event
	uint m_running;
};

#define EVENT_ARG_HEAD(eventType)   \
static void registerSwigType(swigTypeInfo *typeInfo, eventType*)	\
{																		\
	String argType = #eventType ; \
	CEventManager::instance()->registerSwigType(typeInfo, argType );\
};																		\
virtual swigTypeInfo* getSwigType() \
{									\
	String argType = #eventType ; \
	return CEventManager::instance()->getSwigType(argType); \
}
///Argument for event
struct EventArg
{
	virtual swigTypeInfo* getSwigType()=0;
	virtual ~EventArg() {}
};

//struct WindowEventArg : public EventArg
//{

//WindowEventArg() {}
//	WindowEventArg(String s): m_window(s) {}
//virtual static void register()
//{ 
//	CEventManager::instance()->register();

//};
/////Window's name
//	String m_window;
//};
//
//struct WindowKeyEventArg : public WindowEventArg
//{
// Ogre::KeyCode m_key;
//};
//


struct FrameEventArg : public EventArg
{
	EVENT_ARG_HEAD(FrameEventArg);
	Real m_time;
};

//void testPyEvent(EventArg *arg1, EventArg *arg2);

struct CharacterEventArg : public EventArg
{
	EVENT_ARG_HEAD(CharacterEventArg);
	CharacterPtr m_ch;
};

struct CharUpdateEventArg : public EventArg
{
	EVENT_ARG_HEAD(CharUpdateEventArg);
	CharacterPtr m_ch;
	Real m_time;
};
#endif
