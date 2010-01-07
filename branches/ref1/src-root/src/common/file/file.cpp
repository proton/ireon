/* Copyright (C) 2005-2006 ireon.org developers council
 * portions (C) Radon Labs GmbH, www.nebuladevice.org
 * $Id: file.cpp 610 2006-04-20 11:04:59Z zak $

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

 * Based on Ogre::ColourValue, original copyright information follows
 * Copyright (c) 2000-2005 The OGRE Team
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 */


/**
 * @file file.cpp
 * CFile realization
 */
#include "common/common_stdafx.h"
#include "common/file/fs.h"
#include "common/file/file.h"

#if defined(__LINUX__) || defined(__MACOSX__) || defined(__UNIX__)
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

//#include "file/directory.h"
#include "file/fs.h"
#include "file/file.h"

CFile::CFile() :
	m_lineNumber(0),
	m_isOpen(false)
{
#ifdef __WIN32__
	this->m_handle = 0;
#else
	this->m_fp = 0;
#endif
}

CFile::~CFile()
{
	if(this->isOpen())
	{
		this->close();
	}
}


bool CFile::exists(const char* fileName) const
{
	assert(fileName != 0);

	String fullName = CFS::instance()->realPath(fileName);

#ifdef __WIN32__
	HANDLE fh = CreateFile(fullName.c_str(),     // filename
						   READ_CONTROL,         // access mode
						   FILE_SHARE_READ,      // share mode
						   0,                    // security flags
						   OPEN_EXISTING,        // what to do if file doesn't exist
						   FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS,  // flags'n'attributes
						   0);                   // template file
	if (fh != INVALID_HANDLE_VALUE)
	{
		CloseHandle(fh);
		return true;
	}
#else
	FILE* fp = fopen(fullName.c_str(), "r");
	if (fp != 0)
	{
		fclose(fp);
		return true;
	}
#endif
	return false;
}

bool CFile::open(const char* fileName, const char* accessMode)
{
	assert(!this->isOpen());

	assert(fileName);
	assert(accessMode);

	String realPath = CFS::instance()->realPath(fileName);
	this->m_lineNumber = 0;

#ifdef __WIN32__
	DWORD access = 0;
	DWORD disposition = 0;
	DWORD shareMode = 0;
	const char* ptr = accessMode;
	char c;
	while (c = *ptr++)
	{
		if ((c == 'r') || (c == 'R'))
		{
			access |= GENERIC_READ;
		}
		else if ((c == 'w') || (c == 'W'))
		{
			access |= GENERIC_WRITE;
		}
	}
	if (access & GENERIC_WRITE)
	{
		disposition = CREATE_ALWAYS;
	}
	else
	{
			disposition = OPEN_EXISTING;
			shareMode   = FILE_SHARE_READ;
	}
	this->m_handle = CreateFile(realPath.c_str(), // filename
							  access,           // access mode
							  shareMode,        // share mode
							  0,                // security flags
							  disposition,      // what to do if file doesn't exist
							  FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS,  // flags'n'attributes
							  0);               // template file
	if (this->m_handle == INVALID_HANDLE_VALUE)
	{
		this->m_handle = 0;
		return false;
	}
#else
	this->m_fp = fopen(realPath.c_str(), accessMode);
	if (!this->m_fp)
	{
		return false;
	}
#endif
	this->m_isOpen = true;
	return true;
}

void CFile::close()
{
#ifdef __WIN32__
	if (this->m_handle)
	{
		CloseHandle(this->m_handle);
		this->m_handle = 0;
	}
#else
	if (this->m_fp)
	{
		fclose(this->m_fp);
		this->m_fp = 0;
	}
#endif
	this->m_isOpen = false;
}

int CFile::write(const void* buffer, size_t numBytes)
{
	assert(this->isOpen());

	// statistics
	CFS::instance()->addBytesWritten(numBytes);

#ifdef __WIN32__
	DWORD written;
	WriteFile(this->m_handle, buffer, (DWORD) numBytes, &written, NULL);
	return written;
#else
	return fwrite(buffer, 1, numBytes, this->m_fp);
#endif
}

int CFile::read(void* buffer, size_t numBytes)
{
	assert(this->isOpen());

	// statistics
	CFS::instance()->addBytesRead(numBytes);

#ifdef __WIN32__
	DWORD read;
	ReadFile(this->m_handle, buffer, (DWORD) numBytes, &read, NULL);
	return read;
#else
	return fread(buffer, 1, numBytes, this->m_fp);
#endif
}


int CFile::tell() const
{
	assert(this->isOpen());
#ifdef __WIN32__
	return SetFilePointer(this->m_handle, 0, NULL, FILE_CURRENT);
#else
	return ftell(this->m_fp);
#endif
}

bool CFile::seek(int byteOffset, SeekType origin)
{
	assert(this->isOpen());

	CFS::instance()->addSeek();

#ifdef __WIN32__
	DWORD method = 0x00000000;
	switch (origin)
	{
		case CURRENT:
			method = FILE_CURRENT;
			break;
		case START:
			method = FILE_BEGIN;
			break;
		case END:
			method = FILE_END;
			break;
	}

	DWORD ret = SetFilePointer(this->m_handle, (LONG)byteOffset, NULL, method);
	return (ret != 0xffffffff);
#else
	int whence = SEEK_SET;
	switch (origin)
	{
		case CURRENT:
			whence = SEEK_CUR;
			break;
		case START:
			whence = SEEK_SET;
			break;
		case END:
			whence = SEEK_END;
			break;
	}
	return (0 == fseek(this->m_fp, byteOffset, whence)) ? true : false;
#endif
}

bool CFile::eof() const
{
	assert(this->isOpen());

#ifdef __WIN32__
	DWORD fpos = SetFilePointer(this->m_handle,0,NULL,FILE_CURRENT);
	DWORD size = GetFileSize(this->m_handle,NULL);

	return (fpos >= size)? true:false;
#else
	return (!feof(this->m_fp))? false:true;
#endif
}

int CFile::getSize() const
{
	assert(this->isOpen());

#ifdef __WIN32__
	return GetFileSize(this->m_handle, NULL);
#elif defined(__LINUX__) || defined(__MACOSX__) || defined(__UNIX__)
	struct stat s;
	fstat(fileno(this->m_fp), &s);
	return s.st_size;
#else
#error "CFile::GetSize(): NOT IMPLEMENTED!"
#endif
}

FileTime CFile::getLastWriteTime() const
{
	assert(this->isOpen());

#ifdef __WIN32__
	FileTime fileTime;
	GetFileTime(this->m_handle, NULL, NULL, &(fileTime.m_time));
	return fileTime;
#elif defined(__LINUX__) || defined(__MACOSX__) || defined(__UNIX__)
	FileTime fileTime;
	struct stat s;
	fstat(fileno(this->m_fp), &s);
	fileTime.m_time = s.st_mtime;
	return fileTime;
#else
#error "CFile::GetLastWriteTime(): NOT IMPLEMENTED!"
#endif
}

bool CFile::putS(const char* buffer)
{
	assert(this->isOpen());

	size_t len = strlen(buffer);
	size_t written = this->write(buffer, len);
	if (written != len)
	{
		return false;
	}
	else
	{
		this->m_lineNumber++;
		return true;
	}
}

String CFile::getS(int numChars)
{
	assert(this->isOpen());
	assert(numChars > 0);

	char* buf = new char[numChars];
	// store start filepointer position
	int seekPos = this->tell();

	// read 64 bytes at once, and scan for newlines
	const int chunkSize = 64;
	int readSize  = chunkSize;
	char* readPos = buf;

	int bytesRead = 0;
	int curIndex = 0;
	for (curIndex = 0; curIndex < (numChars - 1); curIndex++)
	{
		// read next chunk of data?
		if (0 == (curIndex % chunkSize))
		{
			readSize = chunkSize;
			if ((curIndex + readSize) >= numChars)
			{
				readSize = numChars - curIndex;
			}
			bytesRead = this->read(readPos, readSize);
			readPos[bytesRead] = 0;
			readPos += readSize;
		}

		// end of line reached?
		if (0 == bytesRead)
			break;

		// newline?
		if ((buf[curIndex] == '\n') || (buf[curIndex] == 0))
		{
			this->seek(seekPos + curIndex + 1, START);
			break;
		}
	}

	// terminate buffer
	buf[curIndex] = 0;
	this->m_lineNumber++;
	String s(buf);
	delete[] buf;
	return s;
}

int CFile::appendFile(CFile* other)
{
	assert(other);

	int numBytes = other->getSize();
	if (numBytes == 0)
	{
		// nothing to do
		return 0;
	}

	// allocate temp buffer and read bytes
	char* buffer =  new char[numBytes];
	assert(buffer);
	int numBytesRead = other->read(buffer, numBytes);
	assert(numBytesRead == numBytes);

	// write to this file
	int numBytesWritten = this->write(buffer, numBytes);
	assert(numBytesWritten == numBytes);

	// cleanup
	delete[] buffer;
	return numBytes;
}

void CFile::flush()
{
#ifdef __WIN32__
	FlushFileBuffers(m_handle);
#elif defined(__UNIX__)
	fflush(m_fp);
#endif
};

void CFile::operator<<(const char* str)
{
	if( !str )
	{
		wrt((byte)0);
		return;
	}
	size_t len = strlen(str);
	if( len < 0xFF )
		wrt((byte)len);
	else if (len < 0xFFFF)
	{
		wrt((byte)0xFF);
		wrt((ushort)len);
	}
	else
	{
		wrt((byte)0xFF);
		wrt((ushort)0xFFFF);
		wrt(len);
	}
	write(str,len);
}

void CFile::operator >>(String& s)
{
	int len;
	len=readStrLen();
	char* buf = new char[len+1];
	if( len > 0 )
		read(buf,len);
	buf[len] = '\0';
	s = buf;
	delete[] buf;
};

void CFile::operator >>(char* str)
{
	int len;
	len=readStrLen();
	if(len>0)
		read(str,len);
	str[len] = '\0';
};

int CFile::readStrLen()
{
 byte bLen;
 operator>>( bLen );
 if( bLen < 0xFF )
   return bLen;

 ushort sLen;
 operator>>( sLen );
 if( sLen < 0xFFFF )
   return sLen;

 int iLen;
 operator>>( iLen );
 return iLen;
}
