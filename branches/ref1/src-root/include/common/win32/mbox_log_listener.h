/**
 * @file common/win32/mbox_log_listener.h
 * Windows-specific popup message box log listener
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id$

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

#ifndef _MBOX_LOG_LISTENER_H
#define _MBOX_LOG_LISTENER_H

#include "common/log/log_listener.h"

class CMBoxLogListener: public CLogListener
{
public:
	CMBoxLogListener(uint mask = CLog::msgFlagALL, CLog::msgLevels msgLevelThreshold = CLog::msgLvlCritical);
	~CMBoxLogListener();

	/// log message
	bool log(uint flags, CLog::msgLevels msgLevel, const char* message);

	/// does not work
	bool dump(uint flags, CLog::msgLevels msgLevel, const void* data, uint length) { return true; }

	bool works() const {return true;}

	void close() {}

protected:
	uint m_mask;
	CLog::msgLevels m_msgLevelThreshold;
};

#endif

