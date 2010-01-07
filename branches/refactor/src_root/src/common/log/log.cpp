/* Copyright (C) 2005 ireon.org developers council
 * $Id: log.cpp 316 2005-11-28 08:26:45Z zak $

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

#include "stdafx.h"

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

void CLog::log(byte msgLevel, const char* message, ... )
{
	va_list args;
	va_start(args, message);
	log(m_defFlags,msgLevel,message,args);
	va_end(args);
}

void CLog::log(uint flags, byte msgLevel, const char* message, ... )
{
	va_list args;
	va_start(args, message);
	log(flags,msgLevel,message,args);
	va_end(args);
}

void CLog::log(uint flags, byte msgLevel, const char* message, va_list& args)
{
	assert(message);
	assert(MAX_LOG_MESSAGE_LENGTH > 0);
	char buf[MAX_LOG_MESSAGE_LENGTH+1];
#ifdef __WIN32__
	_vsnprintf(buf,sizeof(buf)-1,message,args);
#else
	vsnprintf(buf,sizeof(buf)-1,message,args);
#endif
	std::list<CLogListener*>::iterator i;
	for (i = m_listeners.begin(); i != m_listeners.end(); i++ )
		(*i)->log(flags,msgLevel,buf);
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
