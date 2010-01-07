/* Copyright (C) 2005 ireon.org developers council
 * portions (C) Radon Labs GmbH, www.nebuladevice.org

 *  See LICENSE for details
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


