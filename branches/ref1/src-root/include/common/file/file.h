/* Copyright (C) 2005-2006 ireon.org developers council
 * portions (C) Radon Labs GmbH, www.nebuladevice.org
 * $Id: file.h 610 2006-04-20 11:04:59Z zak $

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
 * @file file.h
 * Crossplatform file wrapper.
 */
#ifndef _FILE_H
#define _FILE_H

#include "file/filetime.h"

#ifdef __XBxX__
#include "xbox/nxbwrapper.h"
#elif defined(__WIN32__)
#define WIN32_LEAN_AND_MEAN
#include <winbase.h>
#else
#include <stdio.h>
#endif

class CFS;

//------------------------------------------------------------------------------
class CFile
{
public:
	CFile();
	/// destructor
	virtual ~CFile();

public:
	/// start point for seeking in file
	enum SeekType
	{
		CURRENT,
		START,
		END,
	};

	/// does the file physically exist on disk?
	virtual bool exists(const char* fileName) const;
	/// opens a file
	virtual bool open(const char* fileName, const char* accessMode);
	/// closes the file
	virtual void close();
	/// writes some bytes to the file
	virtual int write(const void* buffer, size_t numBytes);
	/// reads some bytes from the file
	virtual int read(void* buffer, size_t numBytes);
	/// gets actual position in file
	virtual int tell() const;
	/// sets new position in file
	virtual bool seek(int byteOffset, SeekType origin);
	/// is the file at the end
	virtual bool eof() const;
	/// get size of file in bytes
	virtual int getSize() const;
	/// writes a String to the file
	bool putS(const char* buffer);
	/// reads a String from the file
	String getS(int numChars);
	/// get current line number (incremented by PutS() and GetS())
	int getLineNumber() const;
	/// determines whether the file is opened
	bool isOpen() const;
	/// append one file to another file
	virtual int appendFile(CFile* other);
	/// get the last write time
	virtual FileTime getLastWriteTime() const;
	/// write a 32bit int to the file
	void putInt(int &val);
	/// write a 16bit int to the file
	void putShort(short &val);
	/// write a 8bit int to the file
	void putChar(char &val);
	/// write a float to the file
	void putFloat(float &val);
	/// write a double to the file
	void putDouble(double &val);
	/// read a 32 bit int from the file
	void getInt(int &val);
	/// read a signed 16 bit int from the file
	void getShort(short &val);
	/// read an unsigned 16 bit int from the file
	void getUShort(ushort &val);
	/// read a 8 bit int from the file
	void getChar(char &val);
	/// read a float from the file
	void getFloat(float &val);
	/// read a double from the file
	void getDouble(double &val);
	/// flush file
	void flush();
public: 
	///Operators
	// bool
	void wrt( bool b ) { putChar((char&)b); }
	void operator<<( bool& b ) { putChar((char&)b); }
	void operator>>( bool& b ) { getChar((char&)b); }

	// byte
	void wrt( byte b ) { putChar((char&)b); }
	void operator<<( byte& b ) { putChar((char&)b); }
	void operator>>( byte& b ) { getChar((char&)b); }

	// char
	void wrt( char c ) { putChar(c); }
	void operator<<( char& c ) { putChar(c); }
	void operator>>( char& c ) { getChar(c); }

	// ushort
	void wrt( ushort i ) { putShort((short&)i); }
	void operator<<( ushort& i ) { putShort((short&)i); }
	void operator>>( ushort& i ) { getUShort(i); }

	// short
	void wrt( short i ) { putShort(i); }
	void operator<<( short& i ) { putShort(i); }
	void operator>>( short& i ) { getShort(i); }

	// size_t
	void wrt( size_t i ) { write((char*)&i,sizeof(i)); }
	void operator<<( size_t& i ) { write((char*)&i, sizeof(i) ); }
	void operator>>( size_t& i ) { read((char*)&i, sizeof(i)); }

	// uint
//	void wrt( uint i ) { putInt((int&)i); }
//	void operator<<( uint& i ) { putInt((int&)i); }
//	void operator>>( uint& i ) { getInt((int&)i); }

	// uint
	void wrt( unsigned long i ) { putInt((int&)i); }
	void operator<<( unsigned long& i ) { putInt((int&)i); }
	void operator>>( unsigned long& i ) { getInt((int&)i); }

	// int
	void wrt( int i ) { putInt(i); }
	void operator<<( int& i ) { putInt(i); }
	void operator>>( int& i ) { getInt(i); }

	// double
	void wrt( double i ) { write( (char*)&i, sizeof(i) ); }
	void operator<<( double& i ) { write((char*)&i, sizeof(i) ); }
	void operator>>( double& i ) { read((char*)&i,sizeof(i)); }

	// int64
	void wrt( int64 i ) { write((char*)&i,sizeof(i)); }
	void operator<<( int64& i ) { write((char*)&i, sizeof(i) ); }
	void operator>>( int64& i ) { read((char*)&i, sizeof(i)); }

	// Строка
	void operator<<( const char* str );
	void operator>>( char* str );
	void operator<<( const String& str) { operator<<( str.c_str() ); }
	void operator>>( String& str );
	int readStrLen();

protected:
	ushort m_lineNumber;
	bool m_isOpen;

	#ifdef __WIN32__
		// win32 file handle
		HANDLE m_handle;
	#else
		// ansi c file pointer
		FILE* m_fp;
	#endif
};

inline bool CFile::isOpen() const
{
	return this->m_isOpen;
}

inline int CFile::getLineNumber() const
{
	return this->m_lineNumber;
}

inline void CFile::putInt(int &val)
{
	write(&val, sizeof(val));
}

inline void CFile::putShort(short &val)
{
	write(&val, sizeof(val));
}

inline void CFile::putChar(char &val)
{
	write(&val, sizeof(val));
}

inline void CFile::putFloat(float &val)
{
	write(&val, sizeof(val));
}

inline void CFile::putDouble(double &val)
{
	write(&val, sizeof(val));
}

inline void CFile::getInt(int &val)
{
	this->read(&val, sizeof(val));
}

inline void CFile::getShort(short &val)
{
	this->read(&val, sizeof(val));
}

inline void CFile::getUShort(ushort &val)
{
	this->read(&val, sizeof(val));
}

inline void CFile::getChar(char &val)
{
	this->read(&val, sizeof(val));
}

inline void CFile::getFloat(float &val)
{
	this->read(&val, sizeof(val));
}

inline void CFile::getDouble(double &val)
{
	this->read(&val, sizeof(val));
}

#endif
