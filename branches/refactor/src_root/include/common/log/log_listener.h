/* Copyright (C) 2005 ireon.org developers council
 * $Id: log_listener.h 315 2005-11-28 06:16:29Z zak $

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

class CLogListener;
#include "log.h"

class CLogListener
{
protected:
	///Abstract class
	CLogListener();

public:
	///Does it works?
	virtual bool works() const = 0;

	///Close log
	virtual void close() = 0;
public:
	///Process message
	virtual bool log(uint flags, byte msgLevel, const char* message) = 0;

	virtual bool dump(uint flags, byte msgLevel, const void* data, uint length) = 0;
};

class CFileLogListener: public CLogListener
{
protected:
	///Default constructor is protected
	CFileLogListener();
public:
	CFileLogListener(const char* filename, uint mask = CLog::msgFlagALL, byte msgLevelThreshold = CLog::msgLvlVerbose);
	~CFileLogListener();

	bool log(uint flags, byte msgLevel, const char* message);

	bool dump(uint flags, byte msgLevel, const void* data, uint length);

	bool works() const;

	void close();
protected:
	///Log file
	FilePtr m_file;
	uint m_mask;
	byte m_msgLevelThreshold;
};

class CScreenLogListener: public CLogListener
{
public:
	/// @param prefix - short prefix (32 chars max) for output
	CScreenLogListener(const char* prefix = "", uint mask = CLog::msgFlagALL, byte msgLevelThreshold = CLog::msgLvlError);
	~CScreenLogListener();

	/// log message
	bool log(uint flags, byte msgLevel, const char* message);

	/// log data
	bool dump(uint flags, byte msgLevel, const void* data, uint length);

	bool works() const {return true;}

	void close() {}

protected:
	char m_prefix[33];
	uint m_mask;
	byte m_msgLevelThreshold;
};


#endif
