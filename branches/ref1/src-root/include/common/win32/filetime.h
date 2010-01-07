/* Copyright (C) 2005-2006 ireon.org developers council
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
 * @file win32/filetime.h
 * FileTime structure for win32.
 */
#ifndef _FILETIMEWIN32_H
#define _FILETIMEWIN32_H

#define WIN32_LEAN_AND_MEAN
#ifndef __WIN32__
#error "FileTimeWin32: trying to compile Win32 class on hostile platform"
#endif

//------------------------------------------------------------------------------
class FileTimeWin32
{
public:
	/// constructor
	FileTimeWin32();
	/// operator ==
	friend bool operator ==(const FileTimeWin32& a, const FileTimeWin32& b);
	/// operator >=
	friend bool operator >(const FileTimeWin32& a, const FileTimeWin32& b);
	/// operator <=
	friend bool operator <(const FileTimeWin32& a, const FileTimeWin32& b);

private:
	friend class CFile;
	FILETIME m_time;
};

inline FileTimeWin32::FileTimeWin32()
{
	m_time.dwLowDateTime = 0;
	m_time.dwHighDateTime = 0;
}

inline bool operator ==(const FileTimeWin32& a, const FileTimeWin32& b)
{
	return (0 == CompareFileTime(&(a.m_time), &(b.m_time)));
}

inline bool operator >(const FileTimeWin32& a, const FileTimeWin32& b)
{
	return (1 == CompareFileTime(&(a.m_time), &(b.m_time)));
}

inline bool operator <(const FileTimeWin32& a, const FileTimeWin32& b)
{
	return (-1 == CompareFileTime(&(a.m_time), &(b.m_time)));
}

//------------------------------------------------------------------------------
#endif


