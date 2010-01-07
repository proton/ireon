/**
 * @file common/file/fs.cpp
 * CFS realization
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * portions (C) Radon Labs GmbH, www.nebuladevice.org
 * $Id: fs.cpp 565 2006-03-22 16:50:54Z zak $

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

#include "common/common_stdafx.h"
#include "common/file/fs.h"
#include "common/file/file.h"
#include "common/file/directory.h"
#include "common/misc/boost_string_algo_wrapper.h"


#include "kernel/crc.h"
#ifdef __WIN32__
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif
#if defined(__MACOSX__)
#include <Carbon/carbon.h>
#endif


CFS* CFS::m_singleton = 0;


CFS::CFS() :
	m_bytesRead(0),
	m_bytesWritten(0),
	m_numSeeks(0)
{
	assert(0 == m_singleton);
}

void CFS::init()
{
}

CFS::~CFS()
{
	assert(0 != m_singleton);
	m_singleton = 0;
}

bool CFS::addAlias(const char* pathName, const char* realPathName)
{
	assert(pathName);
	assert(realPathName);

    // make sure trailing slash exists
	String pathString = realPathName;
	if( !boost::algorithm::ends_with(pathString,"/") )
	 pathString += "/";

	m_mapPath.insert(std::pair<String,String>(pathName,pathString));
	return true;
}

String CFS::getAlias(const char* path) const
{
	assert(path);
	if (m_mapPath.end() != m_mapPath.find(path))
		return m_mapPath.find(path)->second;
	return "";
}


String CFS::realPath(const char* path) const
{
	if( !boost::algorithm::ends_with(path,"/") )
	{
		String res = path;
		boost::algorithm::erase_tail(res,(unsigned int) res.size()-1);
	}
	return path;
}

bool CFS::makePath(const char* path) const
{
	assert(path);

    DirPtr dir(new CDirectory());
    assert(dir);

    // build stack of non-existing dir components
    String spath = this->realPath(path);
	std::stack<String> pathStack;
    while ((!spath.empty()) && (!dir->open(spath.c_str())))
    {
        pathStack.push(spath);
		size_t p = spath.find_last_of('/');
		if( p != MAX_INT && !spath[p+1] )
		{
			spath[p] = 0;
			p = spath.find_last_of('/');
		}
		if( p == MAX_INT )
			break;
		spath.resize(p);
    }
    if (dir->isOpen())
    {
        dir->close();
    }

    // error?
    if (spath.empty())
    {
        return false;
    }

    // create missing directory components
	while ( pathStack.size() )
    {
        const String curPath = pathStack.top();
        #ifdef __WIN32__
            int err = _mkdir(curPath.c_str());
        #else
            int err = mkdir(curPath.c_str(), S_IRWXU|S_IRWXG);
        #endif
        if (-1 == err)
        {
            return false;
        }
		pathStack.pop();
    }
    return true;
}

FilePtr CFS::newFileObject() const
{
	FilePtr result(new CFile);
	assert(result.get() != 0);
	return result;
}


bool CFS::fileExists(const char* pathName) const
{
	assert(pathName != 0);
	bool result = false;
	FilePtr file = newFileObject();
	if (file->exists(realPath(pathName).c_str()))
	{
		result = true;
	}
	return result;
}

bool CFS::directoryExists(const char* pathName) const
{
	assert(pathName != 0);
	DirPtr dir(new CDirectory());
	if (dir->open(realPath(pathName).c_str()))
	{
		dir->close();
		return true;
	}
	return false;
}

bool CFS::copyFile(const char* from, const char* to)
{
	assert(from && to);

	#ifdef __WIN32__
	// Win32 specific method is more efficient

	String mangledFromPath = this->realPath(from);
	String mangledToPath   = this->realPath(to);

	// if the target file exists, remove the read/only file attribute
	if (this->fileExists(mangledToPath.c_str()))
	{
		DWORD fileAttrs = GetFileAttributes(mangledToPath.c_str());
		fileAttrs &= ~FILE_ATTRIBUTE_READONLY;
		SetFileAttributes(mangledToPath.c_str(), fileAttrs);
	}
	return ::CopyFile(mangledFromPath.c_str(), mangledToPath.c_str(), FALSE) ? true : false;

	#else

	FilePtr fromCFile = this->newFileObject();
	if (!fromCFile->open(from, "rb"))
	{
		printf("CFS::copyFile(): could not open source file '%s'\n", from);
		return false;
	}
	FilePtr toCFile = this->newFileObject();
	if (!toCFile->open(to, "wb"))
	{
		printf("CFS::copyFile(): could not open dest file '%s'\n", to);
		fromCFile->close();
		return false;
	}

	int size = fromCFile->getSize();
	assert(size > 0);
	char* buffer = new char[size];
	assert(buffer);
	int numRead = fromCFile->read(buffer, size);
	assert(numRead == size);
	int numWritten = toCFile->write(buffer, size);
	assert(numWritten == size);
	delete[] buffer;

	fromCFile->close();
	toCFile->close();
	return true;
	#endif
}

bool CFS::deleteFile(const char* filename)
{
	assert(filename);
	String realPath = this->realPath(filename);

	#ifdef __WIN32__
		return ::DeleteFile(realPath.c_str()) ? true : false;
	#elif defined(__LINUX__) || defined(__MACOSX__)  || defined(__UNIX__)
		return (0 == unlink(realPath.c_str())) ? true : false;
	#else
	#error "CFS::deleteFile() not implemented yet!"
	#endif
}

bool CFS::deleteDirectory(const char* dirName)
{
	assert(dirName);
	String realPath = this->realPath(dirName);

	#ifdef __WIN32__
		return ::RemoveDirectory(realPath.c_str()) ? true : false;
	#elif defined(__LINUX__) || defined(__MACOSX__)  || defined(__UNIX__)
		return (rmdir(realPath.c_str()) == 0) ? true : false;
	#else
		#error "CFS::deleteDirectory() not implemented yet!"
	#endif
}

bool CFS::checksum(const char* filename, uint& crc)
{
	assert(filename);
	crc = 0;
	bool success = false;
	FilePtr file = newFileObject();
	assert(file);
	if (file->open(filename, "rb"))
	{
		// read file into RAM buffer
		int numBytes = file->getSize();
		uchar* buf = new uchar[numBytes];
		assert(buf);

		int numRead = file->read(buf, numBytes);
		assert(numRead == numBytes);

		// compute CRC
		CRC crcSummer;
		crc = crcSummer.checksum(buf, numBytes);

		// free and close everything
		delete[] buf;
		file->close();
		success = true;
	}
	return success;
}

void CFS::setFileReadOnly(const char* filename, bool readOnly)
{
	assert(filename);
	String realPath = this->realPath(filename);
#ifdef __WIN32__
	DWORD fileAttrs = GetFileAttributes(realPath.c_str());
	if (readOnly)
	{
		fileAttrs |= FILE_ATTRIBUTE_READONLY;
	}
	else
	{
		fileAttrs &= ~FILE_ATTRIBUTE_READONLY;
	}
	SetFileAttributes(realPath.c_str(), fileAttrs);
#elif defined(__LINUX__) || defined(__MACOSX__)  || defined(__UNIX__)
	struct stat s;
	if (stat(realPath.c_str(), &s) != -1)
	{
		mode_t mode = s.st_mode;
		if (readOnly)
		{
			// Remove all write flags...
			mode &= ~S_IWUSR;
			mode &= ~S_IWGRP;
			mode &= ~S_IWOTH;
		}
		else
		{
			mode |= S_IWUSR;
		}
		chmod(realPath.c_str(), mode);
	}
#else
#error "CFS::setFileReadOnly() not implemented yet!"
#endif
}

bool CFS::isFileReadOnly(const char* filename)
{
	assert(filename);
	String realPath = this->realPath(filename);
#ifdef __WIN32__
	DWORD fileAttrs = GetFileAttributes(realPath.c_str());
	return (fileAttrs & FILE_ATTRIBUTE_READONLY);
#elif defined(__LINUX__) || defined(__MACOSX__)  || defined(__UNIX__)
	struct stat s;
	if (stat(realPath.c_str(), &s) == -1)
	{
		return false;
	}
	if (s.st_uid == geteuid() && (s.st_mode & S_IRWXU))
	{
		return true;
	}
	else if (s.st_gid == getegid() && (s.st_mode & S_IRWXG))
	{
		return true;
	}
	return false;
#else
#error "CFS::isFileReadOnly() not implemented yet!"
#endif
}
