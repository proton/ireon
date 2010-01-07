/* Copyright (C) 2005 ireon.org developers council
 * $Id: event_manager.cpp 350 2005-12-05 22:03:56Z llyeli $

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
 * @file event_manager.cpp
 * Event manager
 */

#include "stdafx.h"
#include "interface/event_manager.h"
#include "CEGUIInputEvent.h"

#define BOOST_PYTHON_STATIC_MODULE
#include "boost/python.hpp"
using namespace boost::python;

CEventManager* CEventManager::m_singleton = 0;

inline CEventManager* CEventManager::instance()
{
	if( !m_singleton )
		m_singleton = new CEventManager;
	return m_singleton;
}

CEventManager::CEventManager():
m_running(NONE)
{
};

void CEventManager::removeEvent(EventID id, const String& handler)
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

void CEventManager::removeKeyPressedEvent(KeyCode code, const String& handler)
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

void CEventManager::removeKeyReleasedEvent(KeyCode code, const String& handler)
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

void CEventManager::removeKeyClickedEvent(KeyCode code, const String& handler)
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


bool CEventManager::execute(EventID id, EventArg* arg)
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

bool CEventManager::keyPressed(KeyCode code)
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

bool CEventManager::keyReleased(KeyCode code)
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

bool CEventManager::keyClicked(KeyCode code)
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
//	CLog::instance()->log(CLog::msgFlagPython,CLog::msgLvlInfo,"Executing handler '%s'... \n",name.c_str());
	object module(handle<>(borrowed(PyImport_AddModule("__main__"))));
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
		else
			CLog::instance()->log(CLog::msgFlagPython,CLog::msgLvlError,_("Handler does not exitst: '%s'.\n"),name.c_str());
	} catch (error_already_set)
	{
		CLog::instance()->log(CLog::msgLvlError,_("Error in python script. (Function '%s')\n"),name.c_str());
		PyErr_Print();
	};
//	CLog::instance()->log(CLog::msgFlagPython,CLog::msgLvlInfo,"Execution completed.\n");
	return true;
};

bool WindowEvent::operator ()(const CEGUI::EventArgs &carg) const
{
	WindowEventArg arg;
	arg.m_window = m_window;
	CEventManager::instance()->execute(m_handler,&arg);
	return true;
};

bool WindowKeyEvent::operator ()(const CEGUI::EventArgs &carg) const
{
	WindowKeyEventArg arg;
	arg.m_key = (KeyCode)((CEGUI::KeyEventArgs&)carg).scancode;
	arg.m_window = m_window;
	CEventManager::instance()->execute(m_handler,&arg);
	return true;
};