/* Copyright (C) 2005 ireon.org developers council
 * $Id: event_manager.h 433 2005-12-20 20:19:15Z zak $

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

#include "client_app.h"
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

struct AppChangeStateEventArg : public EventArg
{
	CClientApp::State m_oldState;
	CClientApp::State m_newState;
};

struct CharCreatedEventArg : public EventArg
{
	byte m_res;
	ClientOwnCharData* m_d;
};

struct CharRemovedEventArg : public EventArg
{
	byte m_res;
	uint m_id;
};

struct ConnectFailedEventArg : public EventArg
{
	String m_host;
	ushort m_port;
	String m_error;
};

struct ChatEventArg : public EventArg
{
	ChatCommand m_command;
	String m_arg1;
	String m_arg2;
	String m_arg3;
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

class CEventManager
{
protected:
	CEventManager();

public:
	enum EventID
	{
		///No event (for m_running)
		NONE,
		///Application changed state
		ON_APP_CHANGES_STATE,
		///Char created/removed etc
		ON_CHAR_LIST_CHANGED,
		///Root menu must be updated
		ON_ROOT_MENU_UPDATED,
		///Character created result obtained
		ON_CHAR_CREATED,
		///Character removed result obtained
		ON_CHAR_REMOVED,
		///Login to root or world server failed
		ON_LOGIN_FAILED,
		///Connection attempt failed
		ON_CONN_FAILED,
		///Chat command obtained
		ON_CHAT,
		///Key is pressed
		ON_KEY_PRESSED,
		///Key is released
		ON_KEY_RELEASED,
		///Key is clicked
		ON_KEY_CLICKED,
		///Character inserted to world
		ON_CHAR_TO_WORLD,
		///Character removed from world
		ON_CHAR_FROM_WORLD,
		///Character update
		ON_CHAR_UPDATE,
		///Update character's info
		ON_CHAR_INFO_UPDATE
	};

	static CEventManager* instance();

	///Execute event with specified id
	bool execute(EventID id, EventArg *arg);
	///Execute event handler with specified name
	bool execute(const String& name,EventArg *arg);

	///Key events
	bool keyPressed(KeyCode code);
	bool keyReleased(KeyCode code);
	bool keyClicked(KeyCode code);

	void addEvent(EventID id, const String& handler) {m_handlers[id].push_back(handler);}
	void removeEvent(EventID id, const String& handler);

	void addKeyPressedEvent(KeyCode code, const String& handler) {m_keyPressedHandlers[code].push_back(handler);}
	void addKeyReleasedEvent(KeyCode code, const String& handler) {m_keyReleasedHandlers[code].push_back(handler);}
	void addKeyClickedEvent(KeyCode code, const String& handler) {m_keyClickedHandlers[code].push_back(handler);}

	void removeKeyPressedEvent(KeyCode code, const String& handler);
	void removeKeyReleasedEvent(KeyCode code, const String& handler);
	void removeKeyClickedEvent(KeyCode code, const String& handler);

protected:
	///Event handlers (names of python functions)
	std::map<EventID, StringVector> m_handlers;

	std::map<KeyCode, StringVector> m_keyPressedHandlers;
	std::map<KeyCode, StringVector> m_keyReleasedHandlers;
	std::map<KeyCode, StringVector> m_keyClickedHandlers;

	static CEventManager* m_singleton;

	/// Currently executed event
	uint m_running;
};

#endif