/**
 * @file script_manager.h
 * Client event manager
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: client_event_manager.h 845 2007-01-04 21:48:12Z mip $

 *  See LICENSE for details
 */
#ifndef _CLIENT_EVENT_MANAGER_H
#define _CLIENT_EVENT_MANAGER_H

#include "common/interface/event_manager.h"
#include "ireon_client/client_app.h"

enum ClientEvent
{
	///Application changed state
	ON_APP_CHANGES_STATE = EVENT_COUNT,
	///Char created/removed etc
	ON_CHAR_LIST_CHANGED,
	///Char added in list
	ON_CHAR_ADD_LIST,
	///Root menu must be updated
	ON_ROOT_MENU_UPDATED,
	///Character created result obtained
	ON_CHAR_CREATED,
	///Character removed result obtained
	ON_CHAR_REMOVED,
	/// Root server receipt request for select char and processing this
	ON_CHAR_SELECT_ACK,
	/// Failed select ack char
	ON_CHAR_SELECT_ACK_FAILED,
	/// Root server processing request for select char and send ws address and etc. 
	ON_CHAR_SELECT,
	/// Root server failed processing request for select char
	ON_CHAR_SELECT_FAILED,

	///Connection attempt failed
	ON_CONN_FAILED,
	/// Version of client incorrect
	ON_CHECK_VERSION_FAILED,
	///Login to root or world server failed
	ON_LOGIN_FAILED,
	///Chat command obtained
	ON_CHAT,
	///Update character's info
	ON_CHAR_INFO_UPDATE
};

struct AppChangeStateEventArg : public EventArg
{
	EVENT_ARG_HEAD(AppChangeStateEventArg);
	CClientApp::State m_oldState;
	CClientApp::State m_newState;
};

struct CharCreatedEventArg : public EventArg
{
	EVENT_ARG_HEAD(CharCreatedEventArg);
	byte m_res;
};

struct CharRemovedEventArg : public EventArg
{
	EVENT_ARG_HEAD(CharRemovedEventArg);
	byte m_res;
	uint m_id;
};

struct CharAddListArg : public EventArg
{
	EVENT_ARG_HEAD(CharAddListArg);
	uint m_id;
	String m_name;
};

struct CharSelectAckArg : public EventArg
{
	EVENT_ARG_HEAD(CharSelectAckArg);
	byte m_res;
};
// TODO: refactor this ALL
struct CharSelectAckFailedArg : public EventArg
{
	EVENT_ARG_HEAD(CharSelectAckFailedArg);
	byte m_res;
};

struct CharSelectArg : public EventArg
{
	EVENT_ARG_HEAD(CharSelectArg);
	String m_WSaddress;
	String m_token;
};
// TODO: refactor this ALL
struct CharSelectFailedArg : public EventArg
{
	EVENT_ARG_HEAD(CharSelectFailedArg);
	byte m_res;
	int m_charInGame;
};

struct ConnectFailedEventArg : public EventArg
{
	EVENT_ARG_HEAD(ConnectFailedEventArg);
	String m_host;
	ushort m_port;
	String m_error;
};

struct CheckVersionFailedEventArg : public EventArg
{
	EVENT_ARG_HEAD(CheckVersionFailedEventArg);
	String m_host;
	ushort m_port;
	std::vector<ushort> m_serverVersion;
};

struct ChatEventArg : public EventArg
{
	EVENT_ARG_HEAD(ChatEventArg);
	ChatCommand m_command;
	String m_arg1;
	String m_arg2;
	String m_arg3;
};
#endif
