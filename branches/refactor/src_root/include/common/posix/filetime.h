/* Copyright (C) 2005 ireon.org developers council
 * portions (C) Radon Labs GmbH, www.nebuladevice.org

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
 * @file posix/filetime.h
 * FileTime structure for posix systems.
 */
#ifndef _FILETIMEPOSIX_H
#define _FILETIMEPOSIX_H

#if !defined(__LINUX__) && !defined(__MACOSX__) && !defined(__UNIX__)
#error "FileTimePosix: trying to compile POSIX class on hostile platform"
#endif

#include <time.h>

//------------------------------------------------------------------------------
class FileTimePosix
{
public:
	/// constructor
	FileTimePosix();
	/// operator ==
	friend bool operator ==(const FileTimePosix& a, const FileTimePosix& b);
	/// operator >=
	friend bool operator >(const FileTimePosix& a, const FileTimePosix& b);
	/// operator <=
	friend bool operator <(const FileTimePosix& a, const FileTimePosix& b);

private:
	friend class CFile;
	time_t m_time;
};

//------------------------------------------------------------------------------
/**
*/
inline
FileTimePosix::FileTimePosix()
{
	m_time = 0;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
operator ==(const FileTimePosix& a, const FileTimePosix& b)
{
	return (a.m_time == b.m_time);
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
operator >(const FileTimePosix& a, const FileTimePosix& b)
{
	return (a.m_time > b.m_time);
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
operator <(const FileTimePosix& a, const FileTimePosix& b)
{
	return (a.m_time < b.m_time);
}

//------------------------------------------------------------------------------
#endif

