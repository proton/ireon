/**
 * @file log_listener.cpp
 * Concrete log listeners.
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: log_listener.cpp 314 2005-11-27 19:54:19Z zak $

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

#include "stdafx.h"
#include "file/file.h"

CLogListener::CLogListener()
{

};

//-----------------------------------------------------------------------

CFileLogListener::CFileLogListener(const char* filename, uint mask, byte msgLevelThreshold):
m_mask(mask),
m_msgLevelThreshold(msgLevelThreshold)
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

bool CFileLogListener::log(uint flags, byte msgLevel, const char* message)
{
	if ( (m_mask & flags) && msgLevel <= m_msgLevelThreshold && works() )
	{
		m_file->write(message,strlen(message));
		m_file->flush();
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------

bool CFileLogListener::dump(uint flags, byte msgLevel, const void* data, uint length)
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

// CScreenLogListener

//-----------------------------------------------------------------------

CScreenLogListener::CScreenLogListener(const char* prefix, uint mask, byte msgLevelThreshold):
m_mask(mask),
m_msgLevelThreshold(msgLevelThreshold)
{
	strcpy(m_prefix,"");
	if (strlen(prefix)>0) 
		strncat(m_prefix, prefix, 32);
}

//-----------------------------------------------------------------------

bool CScreenLogListener::log(uint flags, byte msgLevel, const char* message)
{
	if ( (m_mask & flags) && msgLevel <= m_msgLevelThreshold && works() )
	{
		if (strlen(m_prefix)>0) 
			printf("[%s] ", m_prefix);
		printf(message);
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------

bool CScreenLogListener::dump(uint flags, byte msgLevel, const void* data, uint length)
{
	if ( (m_mask & flags) && msgLevel <= m_msgLevelThreshold && works() )
	{
		printf("Can't printf dump :(\n");
		return true;
	}
	return false;
}
