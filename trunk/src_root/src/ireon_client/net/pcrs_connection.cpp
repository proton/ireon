/**
* @file ireon_client/net/pcrs_connection.cpp
* client connection to root server
*/


/* Copyright (C) 2005 ireon.org developers council
* $Id: client_app.cpp 764 2006-09-25 13:33:36Z mip $

*  See LICENSE for details
*/

#include "ireon_client/stdafx.h"
#include "ireon_client/net/pcrs_connection.h"

CPCRSConnection::CPCRSConnection()
{
	switchStates();
};

// -----------------------------------------------------------------------
/*
void CRSConnection::connectionFailed(uint error)
{
	ConnectFailedEventArg arg;
	arg.m_host =  getHost();
	arg.m_port = getPort();
	switch(error)
	{
	case 1:
		{
			arg.m_error = _("Can't resolve host.");
			break;
		}
	case 2:
		{
			arg.m_error = _("Can't establish connection.");
			break;
		}
	default:
		{
			break;
		}
	};
	CEventManager::instance()->execute(CEventManager::ON_CONN_FAILED,&arg);
};

// ---------------------------------------------------------

void CRSConnection::loginToRoot(const String& login, const String& password)
{

};

// ---------------------------------------------------------

void CRSConnection::registerToRoot(const String& login, const String& password)
{
	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,_("Sending register request to root server.\n"));
	CData data;
	data.wrt((ireon::net::commandId)ireon::net::rspcCodes::scHelloRegister);
	data << login << password;
	write(data);
};

// ---------------------------------------------------------

void CRSConnection::getCharsList()
{
	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,_("Sending chars listing request to root server.\n"));
	CData data;
	data.wrt((ireon::net::commandId)ireon::net::rspcCodes::scCharSelList);
	write(data);
};

// ---------------------------------------------------------

void CRSConnection::selectChar(uint id)
{
	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,_("Select character with id %d.\n"),id);
	CData data;
	data.wrt((ireon::net::commandId)ireon::net::rspcCodes::scCharSelSelect);
	data << id;
	write(data);
};

// ---------------------------------------------------------

void CRSConnection::createChar(ClientCharRegistrationData& ch)
{
	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,_("Sending new char data.\n"));
	CData data;
	data.wrt((ireon::net::commandId)ireon::net::rspcCodes::scCharSelCreate);
	ch.serialize(data);
	write(data);
};

// ---------------------------------------------------------

void CRSConnection::removeChar(uint id)
{
	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,_("Sending remove char command.\n"));
	CData data;
	data.wrt((ireon::net::commandId)ireon::net::rspcCodes::scCharSelRemove);
	data << id;
	write(data);
};

void CRSConnection::onRootRegistrationOk(CData& data)
{
	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,_("Registered account on root server.\n"));
	CClientApp::instance()->setState(CClientApp::AS_MENU);
};

// ----------------------------------------------------------

void CRSConnection::onRootRegistrationFailed(CData& data)
{
	byte answer;
	data >> answer;
	CLog::instance()->log(CLog::msgLvlError,__FUNCTION__,_("Registration on root server failed.\n"));
	switch(answer)
	{
	case ireon::net::rspcCodes::rfAlreadyRegistered:
		CLog::instance()->log(CLog::msgLvlError,__FUNCTION__,_("Server returned error: account with such name already registered.\n"));
		break;
	case ireon::net::rspcCodes::rfUnknown:
		CLog::instance()->log(CLog::msgLvlError,__FUNCTION__,_("Server returned error: unknown account db error.\n"));
		break;
	}
	CClientApp::instance()->setState(CClientApp::AS_MENU);
	CEventManager::instance()->execute(CEventManager::ON_LOGIN_FAILED,NULL);		
};

// ----------------------------------------------------------

void CRSConnection::onRootLoginOk(CData& data)
{
	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,_("Logged in on root server.\n"));
	getCharsList();
};

// ----------------------------------------------------------

void CRSConnection::onRootLoginFailed(CData& data)
{
	byte answer;
	data >> answer;
	CLog::instance()->log(CLog::msgLvlError,__FUNCTION__,_("Login to root server failed.\n"));
	switch(answer)
	{
	case ireon::net::rspcCodes::lfAccountNotFound:
		CLog::instance()->log(CLog::msgLvlError,__FUNCTION__,_("Server returned error: account not found.\n"));
		break;
	case ireon::net::rspcCodes::lfAlreadyLoggedIn:
		CLog::instance()->log(CLog::msgLvlError,__FUNCTION__,_("Server returned error: account already logged in.\n"));
		break;
	case ireon::net::rspcCodes::lfWrongPassword:
		CLog::instance()->log(CLog::msgLvlError,__FUNCTION__,_("Server returned error: wrong password.\n"));
		break;
	}
	CClientApp::instance()->setState(CClientApp::AS_MENU);
	CEventManager::instance()->execute(CEventManager::ON_LOGIN_FAILED,NULL);		
};

// ----------------------------------------------------------

void CRSConnection::onRootCharList(CData& data)
{
	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,_("Got account characters list.\n"));
	CClientApp::instance()->m_characters.clear();

	ClientCharRegistrationData	chData;
	ClientOwnCharData ch;

	chData.serialize(data);
	ch.m_id = chData.m_id;
	ch.m_name = chData.m_name;

	while(ch.m_id)
	{
		CClientApp::instance()->m_characters.push_back(ch);
		chData.serialize(data);
		ch.m_id = chData.m_id;
		ch.m_name = chData.m_name;
	};

	CClientApp::instance()->setState(CClientApp::AS_ROOT);
	CEventManager::instance()->execute(CEventManager::ON_CHAR_LIST_CHANGED,NULL);	
};

// ----------------------------------------------------------

void CRSConnection::onSendWorld(CData& data)
{
	String ip;
	uint port;
	data >> ip;
	data >> port;
	if( ip == BLANK_STRING )
	{
		CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,_("World server for char not found.\n"));
		return;
	};
	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,_("Obtained world server %s:%d.\n"),ip.c_str(),port);
	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,_("Connecting to world server (%s:%d).\n"),ip.c_str(),port);
	if(CClientApp::instance()->getWorldConn()->connect(ip.c_str(),port,CClientApp::instance()->getSocketSet()))
		CClientApp::instance()->setState(CClientApp::AS_CONNECTING_WORLD);
	else
	{
		CLog::instance()->log(CLog::msgLvlError,__FUNCTION__,_("Can't connect to world server(%s:%d).\n"),ip.c_str(),port);
		ConnectFailedEventArg arg;
		arg.m_error = _("Can't create connectioin to world server.");
		arg.m_host = ip;
		arg.m_port = port;
		CEventManager::instance()->execute(CEventManager::ON_CONN_FAILED,&arg);
	}
};

void CRSConnection::onCharCreationFailed(CData& data)
{
	// FIXME full error reporting
	CLog::instance()->log(CLog::msgLvlError,__FUNCTION__,_("Char creation failed.\n"));
	CharCreatedEventArg arg;
	arg.m_d = NULL;
	arg.m_res = CCE_EXISTS;
	CEventManager::instance()->execute(CEventManager::ON_CHAR_CREATED,&arg);
}


void CRSConnection::onCreateChar(CData& data)
{
	// FIXME refactor this getCharsList() takes care of updating char listing
	ClientCharRegistrationData chData;
	chData.serialize(data);
	ClientOwnCharData d;
	d.m_id = chData.m_id;
	d.m_name = chData.m_name;
	CClientApp::instance()->m_characters.push_back(d);
	CharCreatedEventArg arg;
	arg.m_d = &d;
	arg.m_res = CCE_NONE;
	CEventManager::instance()->execute(CEventManager::ON_CHAR_CREATED,&arg);
	CEventManager::instance()->execute(CEventManager::ON_CHAR_LIST_CHANGED,&arg);
	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,_("New char with id %d created.\n"),d.m_id);
	getCharsList();
};

void CRSConnection::onRemoveChar(CData &data)
{
	uint id;
	byte res;
	data >> id >> res;
	CharRemovedEventArg arg;
	arg.m_id = id;
	arg.m_res = res;
	CEventManager::instance()->execute(CEventManager::ON_CHAR_REMOVED,&arg);
};
*/

