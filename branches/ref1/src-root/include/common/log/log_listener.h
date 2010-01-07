/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: log_listener.h 610 2006-04-20 11:04:59Z zak $

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
 * @file log_listener.h
 * Log listener and subclasses.
 */

#ifndef _LOG_LISTENER_H
#define _LOG_LISTENER_H
const LOG_END_LINE = 0x0a; /// '\n' - endline . NOTE: unix file format use '\r\n' - '0xd 0xa'
class CLogListener;
#include "common/log/log.h"

class CLogListener
{
protected:
	CLogListener();

public:
	virtual bool works() const = 0;

	///Close log
	virtual void close() = 0;
	///last symbol logged was 'end line'?
	virtual bool endLine() {return true;};
	
public:
	///Process message
	virtual bool log(uint flags, CLog::msgLevels msgLevel, const char* message) = 0;

	virtual bool dump(uint flags, CLog::msgLevels msgLevel, const void* data, size_t length) = 0;
};

class CFileLogListener: public CLogListener
{
protected:
	///Default constructor is protected
	CFileLogListener();
public:
	CFileLogListener(const char* filename, uint mask = CLog::msgFlagALL, CLog::msgLevels msgLevelThreshold = CLog::msgLvlVerbose);
	~CFileLogListener();

	bool log(uint flags, CLog::msgLevels msgLevel, const char* message);

	bool dump(uint flags, CLog::msgLevels msgLevel, const void* data, size_t length);

	bool works() const;

	void close();

	bool endLine();
	

protected:
	///Log file
	FilePtr m_file;
	uint m_mask;
	CLog::msgLevels m_msgLevelThreshold;
	bool m_lastSymbolBeEndLine; 
};

class CScreenLogListener: public CLogListener
{
public:
	/// @param prefix - short prefix (32 chars max) for output
	CScreenLogListener(const char* prefix = "", uint mask = CLog::msgFlagALL, CLog::msgLevels msgLevelThreshold = CLog::msgLvlError);
	~CScreenLogListener();

	/// log message
	bool log(uint flags, CLog::msgLevels msgLevel, const char* message);

	/// log data
	bool dump(uint flags, CLog::msgLevels msgLevel, const void* data, size_t length);

	bool works() const {return true;}

	void close() {}

	bool endLine();
	
protected:
	char m_prefix[33];
	uint m_mask;
	CLog::msgLevels m_msgLevelThreshold;
	bool m_lastSymbolBeEndLine; 
};


#endif
