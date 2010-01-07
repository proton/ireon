/* Copyright (C) 2005 ireon.org developers council
  * $Id: event_manager.cpp 844 2007-01-03 13:28:38Z mip $
 
  *  See LICENSE for details
  */
 
 /**
  * @file event_manager.cpp
  * Event manager
  */
 
#include "stdafx.h"
#include "common/interface/event_manager.h"
#include "ireon_client/client_app.h"
#pragma warning(push, 1)
#include "CEGUI/CEGUIInputEvent.h"
#include "OIS/OIS.h"
#pragma warning(pop)

#include "ireon_client/python_script_module/python_script_module.h" 
#define BOOST_PYTHON_STATIC_MODULE
//#pragma warning(push)
//#pragma warning(disable : 4100 4201 4244 4251 4267 4275 4512) 
//#include "boost/python.hpp"
//#pragma warning(pop)


//using namespace boost::python;
extern "C" struct swig_type_info;
#define swig_type_info swigTypeInfo 
extern "C" static swigTypeInfo _swigt__p_EventArg;
extern "C" static swigTypeInfo _swigt__p_FrameEventArg;
extern "C" static swigTypeInfo _swigt__p_CharUpdateEventArg;
extern "C" static swigTypeInfo _swigt__p_CharacterEventArg;

CEventManager* CEventManager::m_singleton = 0;
 
CEventManager* CEventManager::instance()
{
 	if( !m_singleton )
 		m_singleton = new CEventManager;
 	return m_singleton;
}
 
CEventManager::CEventManager():
m_running(NONE)
{
};

void CEventManager::registerSwigType(swigTypeInfo* arg, String& eventName)
{
	swigTypeInfoMap_.insert(std::pair<String, swigTypeInfo*>(eventName, arg) );
}
swigTypeInfo* CEventManager::getSwigType(String& eventName)
{
	std::map<String, swigTypeInfo*, StringLess>::iterator it = swigTypeInfoMap_.find(eventName);
	if (it != swigTypeInfoMap_.end())
		return (*it).second;
	return NULL;
}


void CEventManager::removeEvent(uint id, const String& handler)
{
	if( m_handlers.find(id) == m_handlers.end() )
		return;
	StringVector::iterator it;
	for( it = m_handlers[id].begin(); it != m_handlers[id].end(); ++it )
		if( (*it) == handler )
		{
			m_handlers[id].erase(it);
			break;
		}
};

void CEventManager::removeKeyPressedEvent(OIS::KeyCode code, const String& handler)
{
	if( m_keyPressedHandlers.find(code) == m_keyPressedHandlers.end() )
		return;
	StringVector::iterator it;
	for( it = m_keyPressedHandlers[code].begin(); it != m_keyPressedHandlers[code].end(); ++it )
		if( (*it) == handler )
		{
			m_keyPressedHandlers[code].erase(it);
			break;
		}

};
 
 void CEventManager::removeKeyReleasedEvent(OIS::KeyCode code, const String& handler)
 {
 	if( m_keyReleasedHandlers.find(code) == m_keyReleasedHandlers.end() )
 		return;
 	StringVector::iterator it;
 	for( it = m_keyReleasedHandlers[code].begin(); it != m_keyReleasedHandlers[code].end(); ++it )
 		if( (*it) == handler )
 		{
 			m_keyReleasedHandlers[code].erase(it);
 			break;
 		}
 };
 
 void CEventManager::removeKeyClickedEvent(OIS::KeyCode code, const String& handler)
 {
 	if( m_keyClickedHandlers.find(code) == m_keyClickedHandlers.end() )
 		return;
 	StringVector::iterator it;
 	for( it = m_keyClickedHandlers[code].begin(); it != m_keyClickedHandlers[code].end(); ++it )
 		if( (*it) == handler )
 		{
 			m_keyClickedHandlers[code].erase(it);
 			break;
 		}
 };
 
 
 
 bool CEventManager::execute(uint id, EventArg* arg)
 {
 	if( m_handlers.find(id) == m_handlers.end() )
 		return false;
 	if( m_running == (uint)id )
 		return false;
 	m_running = (uint)id; ///To avoid infinite loops
 	bool res = false;
 	for( StringVector::iterator it = m_handlers[id].begin(); it != m_handlers[id].end(); ++it)
 		res = res || execute(*it,arg);
 	m_running = NONE;
 	return res;
 };
 
 bool CEventManager::keyPressed(OIS::KeyCode code)
 {
 	if( m_keyPressedHandlers.find(code) == m_keyPressedHandlers.end() )
 		return false;
 	if( m_running == (uint)code )
 		return false;
 	m_running = (uint)code; ///To avoid infinite loops
 	bool res = false;
 	for( StringVector::iterator it = m_keyPressedHandlers[code].begin(); it != m_keyPressedHandlers[code].end(); ++it)
 		res = res || execute(*it,NULL);
 	m_running = NONE;
 	return res;
 };
 
 bool CEventManager::keyReleased(OIS::KeyCode code)
 {
 	if( m_keyReleasedHandlers.find(code) == m_keyReleasedHandlers.end() )
 		return false;
 	if( m_running == (uint)code )
 		return false;
 	m_running = (uint)code; ///To avoid infinite loops
 	bool res = false;
 	for( StringVector::iterator it = m_keyReleasedHandlers[code].begin(); it != m_keyReleasedHandlers[code].end(); ++it)
 		res = res || execute(*it,NULL);
 	m_running = NONE;
 	return res;
 };
 
 bool CEventManager::keyClicked(OIS::KeyCode code)
 {
 	if( m_keyClickedHandlers.find(code) == m_keyClickedHandlers.end() )
 		return false;
 	if( m_running == (uint)code )
 		return false;
 	m_running = (uint)code; ///To avoid infinite loops
 	bool res = false;
 	for( StringVector::iterator it = m_keyClickedHandlers[code].begin(); it != m_keyClickedHandlers[code].end(); ++it)
 		res = res || execute(*it,NULL);
 	m_running = NONE;
 	return res;
 };
 
 bool CEventManager::execute(const String& name, EventArg* arg)
 {
 //	CLog::instance()->log(CLog::msgFlagPython,CLog::msgLvlInfo,__FUNCTION__,"Executing handler '%s'... \n",name.c_str());
 
	I_APP->getScriptModule()->executeScriptGlobalWithParams(name, arg);
/*	object module(handle<>(borrowed(PyImport_AddModule("__main__"))));
 	object dictionary = module.attr("__dict__");
 	try{
 		object argument;
 		if( arg )
 		{
 			reference_existing_object::apply<EventArg*>::type converter;
 			PyObject* obj = converter( arg );
 			argument = object( handle<>( obj ) );
 			dictionary["arg"] = argument;
 		}
 		object func = dictionary[name.c_str()];
 		if (func.ptr() && PyCallable_Check(func.ptr()))
 			func(argument);
 		
 			
 	} catch (error_already_set)
 	{
 		_log(_error,_loc,_("Error in python script. (Function '%s')\n"),name.c_str());
 		PyErr_Print();
 	};
 */
	return true;
 };
 
 /*bool WindowEvent::operator ()(const CEGUI::EventArgs &carg) const
 {
 	WindowEventArg arg;
 	arg.m_window = m_window;
 	CEventManager::instance()->execute(m_handler,&arg);
 	return true;
 };
 
 bool WindowKeyEvent::operator ()(const CEGUI::EventArgs &carg) const
 {
 	WindowKeyEventArg arg;
 	arg.m_key = (Ogre::KeyCode)((CEGUI::KeyEventArgs&)carg).scancode;
 	arg.m_window = m_window;
 	CEventManager::instance()->execute(m_handler,&arg);
 	return true;
 };*/

