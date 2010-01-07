/* Copyright (C) 2005 ireon.org developers council
 * portions (C) Radon Labs GmbH, www.nebuladevice.org
 * $Id: directory.h 309 2005-11-26 22:47:42Z llyeli $

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
 * @file directory.h
 * Clossplatform directory wrapper
 */
#ifndef _DIRECTORY_H
#define _DIRECTORY_H

#ifdef __XBxX__
#include "xbox/nxbwrapper.h"
#elif defined(__WIN32__)
#define WIN32_LEAN_AND_MEAN
#elif defined(__UNIX__)
#include <sys/types.h>
#include <dirent.h>
#endif

#include "resource/resource_location.h"

//------------------------------------------------------------------------------
class CDirectory : public CResourceLocation
{
public:
	enum EntryType
	{
		FILE,
		DIRECTORY,
		INVALID,
	};
	CDirectory(const String& path = BLANK_STRING);
	/// destructor
	virtual ~CDirectory();

	/// opens a directory
	virtual bool open(const String& dirName);
	virtual void open() {open(m_name);}
	/// closes the directory
	virtual void close();
	/// determines whether the directory is open
	virtual bool isOpen() const;
	/// get the full path name of the directory itself
	const char* getPathName() const;
	/// check if the directory is empty
	virtual bool isEmpty();
	/// set first entry as current
	virtual bool setToFirstEntry();
	/// set next entry as current
	virtual bool setToNextEntry();
	/// get full path name of current entry
	virtual const char* getEntryName();
	/// get type of current entry
	virtual EntryType getEntryType();

	DataPtr load(const String& name);
	void list(StringVector *res);
protected:

	bool m_empty;
	String m_path;
	String m_apath;

#ifdef __WIN32__
	/// win32 directory search handle
	HANDLE m_handle;
	/// win32 search result
	WIN32_FIND_DATA m_findData;
#elif defined(__UNIX__)
	DIR* m_dir;
	dirent* m_dp;
#endif
};

inline bool CDirectory::isOpen() const
{
	return ((this->m_path.empty()) ? false : true);
}

inline const char* CDirectory::getPathName() const
{
	assert(this->isOpen());
	return this->m_path.c_str();
}

#endif
