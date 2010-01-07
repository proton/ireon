/**
 * @file log_listener.cpp
 * Concrete log listeners.
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: log_listener.cpp 610 2006-04-20 11:04:59Z zak $

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

#include "common/common_stdafx.h"
#include "file/file.h"

CLogListener::CLogListener()
{

};

//-----------------------------------------------------------------------

CFileLogListener::CFileLogListener(const char* filename, uint mask, CLog::msgLevels msgLevelThreshold):
m_mask(mask),
m_msgLevelThreshold(msgLevelThreshold),
m_lastSymbolBeEndLine(true) 
{
	m_file.reset(new CFile);
	assert(m_file);
	m_file->open(filename,"w+");
	if( !m_file->isOpen() )
		CLog::instance()->log(_("Error creating log file '%s'.\n"),filename);
}

//-----------------------------------------------------------------------

CFileLogListener::~CFileLogListener()
{
	if (works())
		close();
}

//-----------------------------------------------------------------------

bool CFileLogListener::works() const
{
	return m_file && m_file->isOpen();
}

//-----------------------------------------------------------------------

bool CFileLogListener::log(uint flags, CLog::msgLevels msgLevel, const char* message)
{
	if ( (m_mask & flags) && msgLevel <= m_msgLevelThreshold && works() )
	{
		m_file->write(message,strlen(message));
		m_file->flush();
		if (message[strlen(message)-1] == LOG_END_LINE) // check last symbol
			m_lastSymbolBeEndLine = true;
		else 
			m_lastSymbolBeEndLine = false;
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------

bool CFileLogListener::dump(uint flags, CLog::msgLevels msgLevel, const void* data, size_t length)
{
	if ( (m_mask & flags) && msgLevel <= m_msgLevelThreshold && works() )
	{
		m_file->write(data,length);
		m_file->write("\n",strlen("\n"));
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------

void CFileLogListener::close()
{
	if (m_file->isOpen())
		m_file->close();
}

//-----------------------------------------------------------------------

bool CFileLogListener::endLine()
{
	return m_lastSymbolBeEndLine;
}
//-----------------------------------------------------------------------

// CScreenLogListener

//-----------------------------------------------------------------------

CScreenLogListener::CScreenLogListener(const char* prefix, uint mask, CLog::msgLevels msgLevelThreshold):
m_mask(mask),
m_msgLevelThreshold(msgLevelThreshold),
m_lastSymbolBeEndLine(true) 
{
	strcpy(m_prefix,"");
	if (strlen(prefix)>0) 
		strncat(m_prefix, prefix, 32);
}

//-----------------------------------------------------------------------

bool CScreenLogListener::log(uint flags, CLog::msgLevels msgLevel, const char* message)
{
	if ( (m_mask & flags) && msgLevel <= m_msgLevelThreshold && works() )
	{
		if (strlen(m_prefix)>0) 
			printf("[%s] ", m_prefix);
		printf(message);
		
		if (message[strlen(message)-1] == LOG_END_LINE)
			m_lastSymbolBeEndLine = true;
		else 
			m_lastSymbolBeEndLine = false;
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------

bool CScreenLogListener::dump(uint flags, CLog::msgLevels msgLevel, const void* data, size_t length)
{
	if ( (m_mask & flags) && msgLevel <= m_msgLevelThreshold && works() )
	{
		printf("Can't printf dump :(\n");
		return true;
	}
	return false;
	length;
	data;
}

//-----------------------------------------------------------------------

bool CScreenLogListener::endLine()
{
	return m_lastSymbolBeEndLine;
}
