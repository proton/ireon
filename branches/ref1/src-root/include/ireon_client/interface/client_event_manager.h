/**
 * @file script_manager.h
 * Client event manager
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id$

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
#ifndef _CLIENT_EVENT_MANAGER_H
#define _CLIENT_EVENT_MANAGER_H

#include "interface/event_manager.h"
#include "client_app.h"

enum ClientEvent
{
	///Application changed state
	ON_APP_CHANGES_STATE = EVENT_COUNT,
	///Char created/removed etc
	ON_CHAR_LIST_CHANGED,
	///Root menu must be updated
	ON_ROOT_MENU_UPDATED,
	///Character created result obtained
	ON_CHAR_CREATED,
	///Character removed result obtained
	ON_CHAR_REMOVED,
	ON_VERSION_CHECK_FAILED,				//< version check on root server failed
	///Login to root or world server failed
	ON_LOGIN_FAILED,
	///Connection attempt failed
	ON_CONN_FAILED,
	///Chat command obtained
	ON_CHAT,
	///Update character's info
	ON_CHAR_INFO_UPDATE
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
#endif