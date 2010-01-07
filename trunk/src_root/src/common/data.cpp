/**
* @file data.cpp
* Data container class
*/

/* Copyright (C) 2005-2006 ireon.org developers council
* $Id: data.cpp 690 2006-08-29 18:43:35Z zak $

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
#include "common/data.h"
#include "common/file/file.h"


CRawData::CRawData(uint size):
m_data(new char[size]),
m_size(size),
m_freeBuf(false)
{
};

CRawData::~CRawData()
{
	if( !m_freeBuf )
		delete[] m_data;
};

void CRawData::Swap(CRawData& data)
{
 char* tmp = data.m_data;
 data.m_data = m_data;
 m_data = tmp;
 size_t size = data.m_size;
 data.m_size = m_size;
 m_size = size;
}


/* ---------------------------------------- */

CData::CData():
m_store(true),
m_readEmpty(false),
m_data(32)
{
	m_ptr = m_data;
};


CData::CData(const CData& data):
m_data(data.size())
{
	memcpy((char*)m_data,(char*)data.m_data,size());
	m_store = data.m_store;
	m_ptr = m_data + (data.m_ptr - data.m_data);
	m_readEmpty = data.m_readEmpty;
};

CData::CData(const char* data, size_t length, bool allowWrite):
m_data(length)
{
	assert(data);
	memcpy((char*)m_data,data,length);
	m_store = allowWrite;
	if( m_store )
		m_ptr = m_data + length;
	else
		m_ptr = m_data;
	m_readEmpty = false;
};

CData::CData(const String& name):
m_store(false),
m_readEmpty(false),
m_data(1)
{
	FilePtr f(new CFile());
	f->open(name.c_str(),"rb");
	if( !f->isOpen() )
		*m_data = 0;
	else
		fromFile(f.get());
};

CData::CData(CFile* f):
m_store(false),
m_readEmpty(false),
m_data(1)
{
	if( !f || !f->isOpen() )
		*m_data = 0;
	else
		fromFile(f);
};

void CData::fromFile(CFile* f)
{
	if( !f->isOpen() )
		*m_data = 0;
	else
	{
		CRawData data(f->getSize());
		f->read(data,data.size());
		m_data.Swap(data);
		if( store() )
			m_ptr = m_data + size();
		else
			m_ptr = m_data;
	}
};

void CData::getS(uint numChars, String& s, uint bulkBytes)
{
	assert(m_store == 0);
	assert(numChars > 0);
	assert(bulkBytes > 15);

	// store start filepointer position
	char* seekPos = m_ptr;

	// read bulkBytes at once, and scan for newlines
	const int chunkSize = bulkBytes;
	uint readSize  = chunkSize;

	CRawData buf(numChars+1);
	char* readPos = buf;

	size_t bytesRead = 0;
	uint curIndex = 0;
	for (curIndex = 0; curIndex < numChars; ++curIndex)
	{
		// read next chunk of data?
		if (0 == (curIndex % chunkSize))
		{
			readSize = chunkSize;
			if ((curIndex + readSize) >= numChars)
			{
				readSize = numChars - curIndex;
			}
			bytesRead = read(readPos, readSize);
			readPos[bytesRead] = 0;
			readPos += readSize;
		}

		// end of line reached?
		if (0 == bytesRead)
			break;

		// newline?
		if ((buf[curIndex] == '\n')  || (buf[curIndex] == '\r') || (buf[curIndex] == 0) || (0 == bytesRead) )
		{
			m_ptr = seekPos + curIndex + 1;
			if( buf[curIndex] = '\r' && buf[curIndex+1] == '\n')
				m_ptr++;
			if( 0 == bytesRead )
				curIndex++;
			break;
		}
	}

	// terminate buffer
	buf[curIndex] = 0;
	s = buf;
}

CData& CData::operator =(const CData& data)
{
	CRawData d(data.size());
	memcpy(d,(const char*)data.m_data,data.size());
	m_data.Swap(d);
	m_store = data.m_store;
	m_ptr = m_data + (data.m_ptr - data.m_data);
	m_readEmpty = data.m_readEmpty;
	return *this;
}

void CData::resize(size_t nsize)
{
	assert(m_data);
	CRawData d(nsize);
	memcpy((char*)d,(const char*)m_data,size() > nsize ? nsize : size());
	m_ptr = (char*)d + (m_ptr - (char*)m_data);
	assert((m_ptr - (char*)d) >= 0);
	if((size_t) (m_ptr - (char*)d) > nsize )
		m_ptr = (char*)d + nsize;
	m_data.Swap(d);
};

void CData::reset(bool store)
{
	if( m_store && !store )
	{
		resize(length());
		m_ptr = m_data;
	}
	else if( !m_store && store )
		m_ptr = m_data + size();
	m_store = store;
	m_readEmpty = false;
};

size_t CData::read(void *data, size_t length)
{
	size_t indent = 0;
	length = sread(data,length,indent);
	m_ptr += length;
	return length;
};

size_t CData::sread(void *data, size_t length, size_t& indent)
{
	assert(data);
	assert(!m_store);
	if(m_ptr + length + indent > m_data + size())
		length = (uint)((m_ptr + indent >= m_data + size()) ? 0 : (m_data + size() - m_ptr - indent));
	if( !length )
	{
		*((char*)data) = 0;
		m_readEmpty = true;
	}
	else
	{
	 memcpy(data,m_ptr + indent,length);
	 indent+=length;
	}
	return length;
};

size_t CData::write(const void *data, size_t length)
{
	uint indent = 0;
	length = swrite(data,length,indent);
	m_ptr += length;
	return length;
}

size_t CData::swrite(const void *data, size_t length, size_t& indent)
{
	assert(data);
	assert(m_store);
	while(m_ptr + length + indent > m_data + size())
		resize(size()*2);
	memcpy(m_ptr + indent,data,length);
	indent+=length;
	return length;
}

CData& CData::operator<<(const char* str)
{
	static const byte maxbyte = 0xFF;
	static const ushort maxshort = 0xFFFF;
	uint indent = 0;
	if( !str )
	{
		wrt((byte)0);
		return *this;
	}
	size_t len = strlen(str);
	if( len < 0xFF )
		swrite(&(byte&)len,sizeof(byte),indent);
	else if (len < 0xFFFF)
	{
		swrite(&(maxbyte),sizeof(byte),indent);
		swrite(&(ushort&)len,sizeof(ushort),indent);
	}
	else
	{
		swrite(&maxbyte,sizeof(byte),indent);
		swrite(&maxshort,sizeof(ushort),indent);
		swrite(&len,sizeof(len),indent);
	}
	swrite(str,len,indent);
	m_ptr+=indent;
	return *this;
}

CData& CData::operator >>(String& s)
{
	int len;
	uint indent = 0;
	len=readStrLen(indent);
	CRawData buf(len+1);
	if( len > 0 )
		sread((char*)buf,len,indent);
	buf[len] = '\0';
	s = buf;
	m_ptr+=indent;
	return *this;
};

CData& CData::operator >>(char* str)
{
	int len;
	uint indent = 0;
	len=readStrLen(indent);
	if(len>0)
		sread(str,len,indent);
	str[len] = '\0'; 
	m_ptr += indent;
	return *this;
};

int CData::readStrLen(size_t& indent)
{
byte bLen;
sread( &bLen, sizeof(byte), indent );
if( bLen < 0xFF )
  return bLen;

ushort sLen;
sread( &sLen, sizeof(ushort), indent );
if( sLen < 0xFFFF )
  return sLen;

int iLen;
sread( &iLen, sizeof(int), indent );
return iLen;
}

void CData::clear()
{
	CRawData data(32);
	m_data.Swap(data);
	m_store = true;
	m_readEmpty = false;
	m_ptr = m_data;
};
