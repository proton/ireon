/* Copyright (C) 2005 ireon.org developers council
* $Id: log.cpp 752 2006-09-23 19:12:44Z zak $

*  See LICENSE for details
*/

/**
* @file log.cpp
* Log manager.
*/

#include "common/common_stdafx.h"
#include "common/log/log.h"
#include "common/log/log_listener.h"
#include <time.h>

#ifdef __UNIX__
#include <stdarg.h>
#endif

CLog* CLog::m_singleton = NULL;

CLog::CLog():
m_defMsgLevel(CLog::msgLvlInfo),
m_defFlags(CLog::msgFlagNone)
{
	assert(m_singleton == NULL);
}

CLog::~CLog()
{
	assert(m_singleton);
	std::list<CLogListener*>::iterator i;
	for( i = m_listeners.begin(); i != m_listeners.end(); i++ )
		delete *i;
	m_singleton = 0;
}

void CLog::log(const char* functionName, const char* message, ... )
{
	va_list args;
	va_start(args, message);
	log(m_defFlags,m_defMsgLevel,functionName,message,args);
	va_end(args);
}

void CLog::log(byte msgLevel, const char* functionName, const char* message, ... )
{
	va_list args;
	va_start(args, message);
	log(m_defFlags,msgLevel,functionName,message,args);
	va_end(args);
}

void CLog::log(uint flags, byte msgLevel, const char* functionName, const char* message, ... )
{
	va_list args;
	va_start(args, message);
	log(flags,msgLevel,functionName,message,args);
	va_end(args);
}

void CLog::log(uint flags, byte msgLevel, const char* functionName, const char* message, va_list& args)
{
	assert(functionName);
	assert(message);
	assert(MAX_LOG_MESSAGE_LENGTH > 0);
	char buf[MAX_LOG_MESSAGE_LENGTH+1];
	char* ptrText = buf;
	time_t t = time(NULL);
	tm* timeVal = gmtime(&t);
	ptrText += strftime(ptrText,MAX_LOG_MESSAGE_LENGTH,"[%H:%M:%S] ",timeVal);

	// reserve at least 50 characters for log message
	if ( (buf + sizeof(buf) - ptrText - strlen(functionName)) > 53 ) 
	{
		strcat(ptrText, "(");
		strcat(ptrText, functionName);
		strcat(ptrText, ") ");
		ptrText += strlen(ptrText);
	}
	
#ifdef __WIN32__
	_vsnprintf(ptrText,buf + sizeof(buf) - ptrText - 1,message,args);
#else
	vsnprintf(ptrText,buf + sizeof(buf) - ptrText - 1,message,args);
#endif
	
	char* ptrMessage;
	std::list<CLogListener*>::iterator i;
	for (i = m_listeners.begin(); i != m_listeners.end(); i++ )
	{
		switch ((*i)->getLastSymbol()) // last symbol write in listener
		{
      case LOG_END_LINE:
			ptrMessage = buf; // timestamp + message
			break;
		case LOG_NONBREAK_LINE:
			ptrMessage = ptrText; // message
			break;
		default:
			ptrMessage = buf; // timestamp + message
			break;
		};
		
		(*i)->log(flags,msgLevel,ptrMessage);
	}
		 
}

void CLog::close()
{
	std::list<CLogListener*>::iterator i;
	for (i = m_listeners.begin(); i != m_listeners.end(); i++ )
		(*i)->close();
};

void CLog::dump(uint flags, byte msgLevel, const void *data, uint length)
{
	assert(data);
	std::list<CLogListener*>::iterator i;
	for (i = m_listeners.begin(); i != m_listeners.end(); i++ )
		(*i)->dump(flags,msgLevel,data,length);
};

void CLog::timeStamp()
{
	char buf[MAX_LOG_MESSAGE_LENGTH+1];
	time_t t = time(NULL);
	tm* timeVal = gmtime(&t);
	strftime(buf,MAX_LOG_MESSAGE_LENGTH,"[[%d.%m.%Y %H:%M:%S]]\n",timeVal);
	std::list<CLogListener*>::iterator i;
	for (i = m_listeners.begin(); i != m_listeners.end(); i++ )
		(*i)->log((uint) CLog::msgFlagALL,0,buf);
};
