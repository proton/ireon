/**
* @file log_listener.cpp
* Concrete log listeners.
*/

/* Copyright (C) 2005 ireon.org developers council
* $Id: log_listener.cpp 781 2006-10-07 19:52:08Z zak $

*  See LICENSE for details
*/

#include "common/common_stdafx.h"
#include "common/log/log_listener.h"
#include "common/file/file.h"

CLogListener::CLogListener()
{

};

//-----------------------------------------------------------------------

CFileLogListener::CFileLogListener(const char* filename, CLog::flags mask, CLog::msgLevels msgLevelThreshold):
m_mask(mask),
m_msgLevelThreshold(static_cast<byte> (msgLevelThreshold)),
m_lastSymbol(LOG_END_LINE) 
{
	m_file.reset(new CFile);
	assert(m_file);
	m_file->open(filename,"w+");
//	if( !m_file->isOpen() )
//		CLog::instance()->log(__FUNCTION__, gtc_("Error creating log file '%s'.\n"),filename);
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
				
		char buf[MAX_LOG_MESSAGE_LENGTH + 3]; /// 1 byte x00 + 1-2 byte '\n'
		strcpy(buf, message);
		char* ptrLastSymbol = &buf[strlen(message)-1]; /// pointer on last symbol
		m_lastSymbol = *ptrLastSymbol; /// last symbol
		
		switch (m_lastSymbol) ///check last symbol 
		{
		case LOG_END_LINE: /// all okey; don't touch message
			break; 
		case LOG_NONBREAK_LINE: /// replace last symbol '\b' with 'space'
			strcpy(ptrLastSymbol," ");
			break;
		default: /// add in end message in buffer 'end line' - '\n' 
			strcat(buf, "\n");
			break;
		};
	

		m_file->write(buf,strlen(buf));
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

byte CFileLogListener::getLastSymbol()
{
	return m_lastSymbol;
}
//-----------------------------------------------------------------------

// CScreenLogListener

//-----------------------------------------------------------------------

CScreenLogListener::CScreenLogListener(const char* prefix, CLog::flags mask, CLog::msgLevels msgLevelThreshold):
m_mask(mask),
m_msgLevelThreshold(static_cast<byte> (msgLevelThreshold)),
m_lastSymbol(LOG_END_LINE) 
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
		
		char buf[MAX_LOG_MESSAGE_LENGTH + 3]; /// 1 byte x00 + 1-2 byte '\n'
		strcpy(buf, message);
		char* ptrLastSymbol = &buf[strlen(message)-1]; /// pointer on last symbol
		m_lastSymbol = *ptrLastSymbol; /// last symbol

		
		switch (m_lastSymbol) ///check last symbol 
		{
		case LOG_END_LINE: /// all okey; don't touch message
			break; 
		case LOG_NONBREAK_LINE: /// replace last symbol '\b' with 'space'
			strcpy(ptrLastSymbol," ");
			break;
		default: /// add in end message in buffer 'end line' - '\n' 
			strcat(buf, "\n");
			break;
		};
		printf(buf);
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
	data; length;
}

//-----------------------------------------------------------------------

byte CScreenLogListener::getLastSymbol()
{
	return m_lastSymbol;
}
