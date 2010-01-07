/* Copyright (C) 2005 ireon.org developers council
 * $Id: py_events.cpp 510 2006-02-26 21:09:40Z zak $

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
 * @file py_events.cpp
 * Export events to python
 */

#include "stdafx.h"

#include <OgreNoMemoryMacros.h>
#include <CEGUI/elements/CEGUIPushButton.h>
#include <CEGUI/elements/CEGUIMultiColumnList.h>
#include <CEGUI/elements/CEGUIListBox.h>
#include <OgreMemoryMacros.h>

#define BOOST_PYTHON_STATIC_MODULE
#include "boost/python.hpp"
using namespace boost::python;

bool (CEventManager::*ex)(uint, EventArg*) = &CEventManager::execute;


BOOST_PYTHON_MODULE(Event)
{
	class_< CEGUI::String , boost::noncopyable>
		("CEString",no_init);

	class_< WindowEvent >
		("WindowEvent")
		.def_readwrite("window",&WindowEvent::m_window)
		.def_readwrite("handler",&WindowEvent::m_handler);

	class_< WindowKeyEvent, bases<WindowEvent> >
		("WindowKeyEvent");

	class_< EventArg >
		("EventArg");

	class_< WindowEventArg, bases<EventArg> >
		("WindowEventArg")
		.def_readwrite("window",&WindowEventArg::m_window);

	class_< WindowKeyEventArg, bases<WindowEventArg> >
		("WindowKeyEventArg")
		.def_readwrite("key",&WindowKeyEventArg::m_key);

	class_< FrameEventArg, bases<EventArg> >
		("FrameEventArg")
		.def_readwrite("time",&FrameEventArg::m_time);

	class_< CharacterEventArg, bases<EventArg> >
		("CharacterEventArg")
		.def_readwrite("ch",&CharacterEventArg::m_ch);

	class_< CharUpdateEventArg, bases<EventArg> >
		("CharUpdateEventArg")
		.def_readwrite("ch",&CharUpdateEventArg::m_ch)
		.def_readwrite("time",&CharUpdateEventArg::m_time);

	enum_< Event >
		("CommonEventID")
		.value("CHAR_DIE",ON_CHAR_DIE)
		.value("CHAR_FROM_WORLD",ON_CHAR_FROM_WORLD)
		.value("CHAR_TO_WORLD",ON_CHAR_TO_WORLD)
		.value("CHAR_UPDATE",ON_CHAR_UPDATE)
		.value("FRAME",ON_FRAME);

	/// Key codes. For details see OgreInput.h
	enum_< KeyCode >
		("KeyCode")
		.value("KC_ESCAPE",KC_ESCAPE)
		.value("KC_1",KC_1)
		.value("KC_2",KC_2)
		.value("KC_3",KC_3) 
		.value("KC_4",KC_4)
		.value("KC_5",KC_5) 
		.value("KC_6",KC_6)
		.value("KC_7",KC_7)
		.value("KC_8",KC_8)
		.value("KC_9",KC_9)
		.value("KC_0",KC_0)
		.value("KC_MINUS",KC_MINUS)
		.value("KC_EQUALS",KC_EQUALS)
		.value("KC_BACK",KC_BACK)  
		.value("KC_TAB",KC_TAB)   
		.value("KC_Q",KC_Q)
		.value("KC_W",KC_W)
		.value("KC_E",KC_E)
		.value("KC_R",KC_R)
		.value("KC_T",KC_T)
		.value("KC_Y",KC_Y)
		.value("KC_U",KC_U)
		.value("KC_I",KC_I)
		.value("KC_O",KC_O)
		.value("KC_P",KC_P)
		.value("KC_LBRACKET",KC_LBRACKET)
		.value("KC_RBRACKET",KC_RBRACKET)
		.value("KC_RETURN",KC_RETURN)
		.value("KC_LCONTROL",KC_LCONTROL)
		.value("KC_A",KC_A)
		.value("KC_S",KC_S)
		.value("KC_D",KC_D)
		.value("KC_F",KC_F)
		.value("KC_G",KC_G)
		.value("KC_H",KC_H)
		.value("KC_J",KC_J)
		.value("KC_K",KC_K)
		.value("KC_L",KC_L)
		.value("KC_SEMICOLON",KC_SEMICOLON)
		.value("KC_APOSTROPHE",KC_APOSTROPHE)
		.value("KC_GRAVE",KC_GRAVE)
		.value("KC_LSHIFT",KC_LSHIFT)
		.value("KC_BACKSLASH",KC_BACKSLASH)
		.value("KC_Z",KC_Z)
		.value("KC_X",KC_X)
		.value("KC_C",KC_C)
		.value("KC_V",KC_V)
		.value("KC_B",KC_B)
		.value("KC_N",KC_N)
		.value("KC_M",KC_M)
		.value("KC_COMMA",KC_COMMA)
		.value("KC_PERIOD",KC_PERIOD)
		.value("KC_SLASH",KC_SLASH)
		.value("KC_RSHIFT",KC_RSHIFT)
		.value("KC_MULTIPLY",KC_MULTIPLY)
		.value("KC_LMENU",KC_LMENU)
		.value("KC_SPACE",KC_SPACE)
		.value("KC_CAPITAL",KC_CAPITAL)
		.value("KC_F1",KC_F1)
		.value("KC_F2",KC_F2)
		.value("KC_F3",KC_F3)
		.value("KC_F4",KC_F4)
		.value("KC_F5",KC_F5)
		.value("KC_F6",KC_F6)
		.value("KC_F7",KC_F7)
		.value("KC_F8",KC_F8)
		.value("KC_F9",KC_F9)
		.value("KC_F10",KC_F10)
		.value("KC_NUMLOCK",KC_NUMLOCK)
		.value("KC_SCROLL",KC_SCROLL)
		.value("KC_NUMPAD7",KC_NUMPAD7)
		.value("KC_NUMPAD8",KC_NUMPAD8)
		.value("KC_NUMPAD9",KC_NUMPAD9)
		.value("KC_SUBTRACT",KC_SUBTRACT)
		.value("KC_NUMPAD4",KC_NUMPAD4)
		.value("KC_NUMPAD5",KC_NUMPAD5)
		.value("KC_NUMPAD6",KC_NUMPAD6)
		.value("KC_ADD",KC_ADD)
		.value("KC_NUMPAD1",KC_NUMPAD1)
		.value("KC_NUMPAD2",KC_NUMPAD2)
		.value("KC_NUMPAD3",KC_NUMPAD3)
		.value("KC_NUMPAD0",KC_NUMPAD0)
		.value("KC_DECIMAL",KC_DECIMAL)
		.value("KC_OEM_102",KC_OEM_102)
		.value("KC_F11",KC_F11)
		.value("KC_F12",KC_F12)
		.value("KC_F13",KC_F13)
		.value("KC_F14",KC_F14)
		.value("KC_F15",KC_F15)
		.value("KC_KANA",KC_KANA)
		.value("KC_ABNT_C1",KC_ABNT_C1)
		.value("KC_CONVERT",KC_CONVERT)
		.value("KC_NOCONVERT",KC_NOCONVERT)
		.value("KC_YEN",KC_YEN)
		.value("KC_ABNT_C2",KC_ABNT_C2)
		.value("KC_NUMPADEQUALS",KC_NUMPADEQUALS)
		.value("KC_PREVTRACK",KC_PREVTRACK)
		.value("KC_AT",KC_AT)
		.value("KC_COLON",KC_COLON)
		.value("KC_UNDERLINE",KC_UNDERLINE)
		.value("KC_KANJI",KC_KANJI)
		.value("KC_STOP",KC_STOP)
		.value("KC_AX",KC_AX)
		.value("KC_UNLABELED",KC_UNLABELED)
		.value("KC_NEXTTRACK",KC_NEXTTRACK)
		.value("KC_NUMPADENTER",KC_NUMPADENTER)
		.value("KC_RCONTROL",KC_RCONTROL)
		.value("KC_MUTE",KC_MUTE)
		.value("KC_CALCULATOR",KC_CALCULATOR)
		.value("KC_PLAYPAUSE",KC_PLAYPAUSE)
		.value("KC_MEDIASTOP",KC_MEDIASTOP)
		.value("KC_VOLUMEDOWN",KC_VOLUMEDOWN)
		.value("KC_VOLUMEUP",KC_VOLUMEUP)
		.value("KC_WEBHOME",KC_WEBHOME)
		.value("KC_NUMPADCOMMA",KC_NUMPADCOMMA)
		.value("KC_DIVIDE",KC_DIVIDE)
		.value("KC_SYSRQ",KC_SYSRQ)
		.value("KC_RMENU",KC_RMENU)
		.value("KC_PAUSE",KC_PAUSE)
		.value("KC_HOME",KC_HOME)
		.value("KC_UP",KC_UP)
		.value("KC_PGUP",KC_PGUP)
		.value("KC_LEFT",KC_LEFT)
		.value("KC_RIGHT",KC_RIGHT)
		.value("KC_END",KC_END)
		.value("KC_DOWN",KC_DOWN)
		.value("KC_PGDOWN",KC_PGDOWN)
		.value("KC_INSERT",KC_INSERT)
		.value("KC_DELETE",KC_DELETE)
		.value("KC_LWIN",KC_LWIN)
		.value("KC_RWIN",KC_RWIN)
		.value("KC_APPS",KC_APPS)
		.value("KC_POWER",KC_POWER)
		.value("KC_SLEEP",KC_SLEEP)
		.value("KC_WAKE",KC_WAKE)
		.value("KC_WEBSEARCH",KC_WEBSEARCH)
		.value("KC_WEBFAVORITES",KC_WEBFAVORITES)
		.value("KC_WEBREFRESH",KC_WEBREFRESH)
		.value("KC_WEBSTOP",KC_WEBSTOP)
		.value("KC_WEBFORWARD",KC_WEBFORWARD)
		.value("KC_WEBBACK",KC_WEBBACK)
		.value("KC_MYCOMPUTER",KC_MYCOMPUTER)
		.value("KC_MAIL",KC_MAIL)
		.value("KC_MEDIASELECT",KC_MEDIASELECT);


	class_< CEventManager, boost::noncopyable >
		("EventManager",no_init)
		.def("addEvent",&CEventManager::addEvent)
		.def("removeEvent",&CEventManager::removeEvent)
		.def("addKeyPressedEvent",&CEventManager::addKeyPressedEvent)
		.def("addKeyReleasedEvent",&CEventManager::addKeyReleasedEvent)
		.def("addKeyClickedEvent",&CEventManager::addKeyClickedEvent)
		.def("removeKeyPressedEvent",&CEventManager::removeKeyPressedEvent)
		.def("removeKeyReleasedEvent",&CEventManager::removeKeyReleasedEvent)
		.def("removeKeyClickedEvent",&CEventManager::removeKeyClickedEvent)
		.def("execute",ex);

	class_< CEGUI::Window, boost::noncopyable >
		("Window",no_init)
		.def_readonly("EventKeyDown",CEGUI::Window::EventKeyDown)
		.def_readonly("EventSized",CEGUI::Window::EventSized);

	class_< CEGUI::PushButton, boost::noncopyable >
		("PushButton",no_init)
		.def_readonly("EventClicked",CEGUI::PushButton::EventClicked);

	class_< CEGUI::MultiColumnList,boost::noncopyable >
		("MultiList", no_init )
		.def_readonly("EventSelectionChanged",CEGUI::MultiColumnList::EventSelectionChanged);

	class_< CEGUI::Listbox,boost::noncopyable >
		("List", no_init )
		.def_readonly("EventSelectionChanged",CEGUI::Listbox::EventSelectionChanged);

	def("get",&CEventManager::instance,return_value_policy<reference_existing_object>());
};

void initEventModule()
{
	initEvent();
};