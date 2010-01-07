/* Copyright (C) 2005 ireon.org developers council
 * $Id: event_manager.h 510 2006-02-26 21:09:40Z zak $

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
 * @file script_manager.h
 * Script manager
 */
#ifndef _SCRIPT_MANAGER_H
#define _SCRIPT_MANAGER_H

#include "OgreInput.h"

///Handler for CEGUI events
struct WindowEvent
{
	virtual bool operator()(const CEGUI::EventArgs& arg) const;

	///Window's name
	String m_window;
	///Handler
	String m_handler;
};

///Handler for CEGUI key events
struct WindowKeyEvent : public WindowEvent
{
	virtual bool operator()(const CEGUI::EventArgs& arg) const;
};

///Argument for event
struct EventArg
{
	virtual ~EventArg() {}
};

struct WindowEventArg : public EventArg
{
	WindowEventArg() {}
	WindowEventArg(String s): m_window(s) {}
	///Window's name
	String m_window;
};

struct WindowKeyEventArg : public WindowEventArg
{
	KeyCode m_key;
};


struct FrameEventArg : public EventArg
{
	Real m_time;
};

struct CharacterEventArg : public EventArg
{
	CharacterPtr m_ch;
};

struct CharUpdateEventArg : public EventArg
{
	CharacterPtr m_ch;
	Real m_time;
};

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

class CEventManager
{
protected:
	CEventManager();

public:


	static CEventManager* instance();

	///Execute event with specified id
	bool execute(uint id, EventArg *arg);
	///Execute event handler with specified name
	bool execute(const String& name,EventArg *arg);

	///Key events
	bool keyPressed(KeyCode code);
	bool keyReleased(KeyCode code);
	bool keyClicked(KeyCode code);

	void addEvent(uint id, const String& handler) {m_handlers[id].push_back(handler);}
	void removeEvent(uint id, const String& handler);

	void addKeyPressedEvent(KeyCode code, const String& handler) {m_keyPressedHandlers[code].push_back(handler);}
	void addKeyReleasedEvent(KeyCode code, const String& handler) {m_keyReleasedHandlers[code].push_back(handler);}
	void addKeyClickedEvent(KeyCode code, const String& handler) {m_keyClickedHandlers[code].push_back(handler);}

	void removeKeyPressedEvent(KeyCode code, const String& handler);
	void removeKeyReleasedEvent(KeyCode code, const String& handler);
	void removeKeyClickedEvent(KeyCode code, const String& handler);


protected:
	///Event handlers (names of python functions)
	std::map<uint, StringVector> m_handlers;

	std::map<KeyCode, StringVector> m_keyPressedHandlers;
	std::map<KeyCode, StringVector> m_keyReleasedHandlers;
	std::map<KeyCode, StringVector> m_keyClickedHandlers;

	static CEventManager* m_singleton;

	/// Currently executed event
	uint m_running;
};

#endif