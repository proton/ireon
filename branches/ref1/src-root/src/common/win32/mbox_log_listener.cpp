/**
 * @file common/win32/mbox_log_listener.cpp
 * Windows-specific popup message box log listener
 */

/* Copyright (C) 2005-2006 ireon.org developers council
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

#include "stdafx.h"
#include "common/win32/mbox_log_listener.h"

CMBoxLogListener::CMBoxLogListener(uint mask, CLog::msgLevels msgLevelThreshold):
m_mask(mask),
m_msgLevelThreshold(msgLevelThreshold)
{
}

//-----------------------------------------------------------------------

bool CMBoxLogListener::log(uint flags, CLog::msgLevels msgLevel, const char* message)
{
	if ( (m_mask & flags) && msgLevel <= m_msgLevelThreshold && works() )
	{
		MessageBox( NULL, message, "Error!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------

