/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: log.cpp 610 2006-04-20 11:04:59Z zak $

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
 * @file log.cpp
 * Log manager.
 */

#include "common/common_stdafx.h"
#include "common/log/log.h"

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

void CLog::log(const char* message, ... )
{
	va_list args;
	va_start(args, message);
	log(m_defFlags,m_defMsgLevel,message,args);
	va_end(args);
}

void CLog::log(CLog::msgLevels msgLevel, const char* message, ... )
{
	va_list args;
	va_start(args, message);
	log(m_defFlags,msgLevel,message,args);
	va_end(args);
}

void CLog::log(uint flags, CLog::msgLevels msgLevel, const char* message, ... )
{
	va_list args;
	va_start(args, message);
	log(flags,msgLevel,message,args);
	va_end(args);
}

void CLog::log(uint flags, CLog::msgLevels msgLevel, const char* message, va_list& args)
{
	assert(message);
	assert(MAX_LOG_MESSAGE_LENGTH > 0);
	char buf[MAX_LOG_MESSAGE_LENGTH+1];
	char* ptrText = buf;
	time_t t = time(NULL);
	tm* timeVal = gmtime(&t);
	ptrText += strftime(ptrText,MAX_LOG_MESSAGE_LENGTH,"[%H:%M:%S] ",timeVal);
	
#ifdef __WIN32__
	_vsnprintf(ptrText,buf + sizeof(buf) - ptrText - 1,message,args);
#else
	vsnprintf(ptrText,buf + sizeof(buf) - ptrText - 1,message,args);
#endif
	char* ptrMessage;
	std::list<CLogListener*>::iterator i;
	for (i = m_listeners.begin(); i != m_listeners.end(); i++ )
	{
		if ( (*i)->endLine() ) // if last symbol write in listener of the end of line
			ptrMessage = buf; // timestamp + message
		else
			ptrMessage = ptrText; // message
		(*i)->log(flags,msgLevel,ptrMessage);
	}
		 
}

void CLog::close()
{
	std::list<CLogListener*>::iterator i;
	for (i = m_listeners.begin(); i != m_listeners.end(); i++ )
		(*i)->close();
};

void CLog::dump(uint flags, CLog::msgLevels msgLevel, const void *data, size_t length)
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
		(*i)->log((uint) CLog::msgFlagALL,CLog::msgLvlInfo,buf);
};
