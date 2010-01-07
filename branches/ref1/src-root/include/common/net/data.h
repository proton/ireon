/**
 * @file data.h
 * Data container header
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: data.h 579 2006-03-26 12:58:49Z zak $

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

#ifndef _DATA_H
#define _DATA_H

/**
 * Wrapper of array of char
 * Used internally for CData
 */ 
class CRawData
{
public:
	CRawData(size_t size);
	~CRawData();

	operator char*() const { return m_data; }

	void Swap(CRawData& data);

	size_t size() const { return m_size; }

	/// Do not destroy data in destructor
	void freeBuf() {m_freeBuf = true;}

private:
	char* m_data;
	size_t m_size;

	/// Buffer is used by somebody, no need to delete
	bool m_freeBuf;

	CRawData& operator=(const CRawData&);
	CRawData(CRawData &data);
};


/* --------------------------------------- */

/**
 * Generic data container.
 * With auto sizing.
 * One can only write or only read to CData.
 */

class CData
{
public:
	/** Initialize container
	 * @param store false -- data is read only
	 */
	CData();

	/** Initialize container
	 * @param allowWrite false -- data is read only
	 */
	CData(const char* data, size_t length, bool allowWrite = true);

	CData(const CData&);

	/** Reads data from file
	 * \deprecated to be deleted
	 */
	CData(CFile* f);
	CData(const String& filename);
	/// Reads data from file
	void fromFile(CFile* f);

	/// Return pointer to data
	const char* data() const {	return m_data; }
	/// returns number of read or written bytes
	size_t length() const {return (m_ptr - m_data);}
	/// returns buffer size, when in read mode it equals to data size
	size_t size() const {return m_data.size();}
	/// returns true if end of data is reached
	bool end() const {return length() >= size();}

	/** Appends new data to data in container
	 * isAllowedToWrite() must be true
	 */
	size_t append(const void* data, size_t length) { return write(data, length); }
	/// \deprecated use append(const void* data, uint length)
	size_t write(const void* data, size_t length);

	/** reads data into string
	 * Reads string of data to new line or numChars which is smaller/
	 * Strips new lines (and double new lines) at the end of string
	 * \param numChars maximum output string length
	 * \param bulkBytes how much bytes to read at once and analyze, you don't need it :)
	 */
	void getS(uint numChars, String& s, uint bulkBytes=64);

	/// whether writing to container is allowed
	bool isAllowedToWrite() {return m_store;}

	/// \deprecated use isAllowedToWrite()
	bool store() {return isAllowedToWrite();}

	/** Resets the data to another mode
	 *  mostly used for reading from 
	 *  previously writed data
	 */
	void reset(bool allowWrite);

private:

	/// Clears data
	void clear();

	/// returns true if there was an attempt to read data beyond the end
	bool readEmpty() const {return m_readEmpty;}


	/// Return pointer to data
	char* wdata() const { return m_data; }

	/** Reads length bytes to data
	 * isAllowedToWrite() must be false
	 */
	size_t read(void* data, size_t length);

	///Read and write functions that don't touch m_ptr
	size_t swrite(const void* data, size_t length, size_t& indent);
	size_t sread(void* data, size_t length, size_t& indent);
	/// if called data within container will not be deleted on container destruction
	void freeBuf() {m_data.freeBuf();}

public:
	//Operators
	CData& operator = (const CData&);

	// bool
	void wrt( bool b ) { write((char*)&b,sizeof(b)); }
	CData& operator<<( bool& b ) { write((char*)&b,sizeof(b)); return *this;}
	CData& operator>>( bool& b ) { read((char*)&b,sizeof(b)); return *this;}

	// byte
	void wrt( byte b ) { write((char*)&b,sizeof(b)); }
	CData& operator<<( byte& b ) { write((char*)&b,sizeof(b)); return *this;}
	CData& operator>>( byte& b ) { read((char*)&b,sizeof(b)); return *this;}

	// char
	void wrt( char c ) { write((char*)&c,sizeof(c)); }
	CData& operator<<( char& c ) { write((char*)&c,sizeof(c)); return *this;}
	CData& operator>>( char& c ) { read((char*)&c,sizeof(c)); return *this;}

	// ushort
	void wrt( ushort i ) { write((char*)&i,sizeof(i)); }
	CData& operator<<( ushort& i ) { write((char*)&i,sizeof(i)); return *this;}
	CData& operator>>( ushort& i ) { read((char*)&i,sizeof(i)); return *this;}

	// short
	void wrt( short i ) { write((char*)&i,sizeof(i)); }
	CData& operator<<( short& i ) { write((char*)&i,sizeof(i)); return *this;}
	CData& operator>>( short& i ) { read((char*)&i,sizeof(i)); return *this;}

	// uint
	void wrt( uint i ) { write((char*)&i,sizeof(i)); }
	CData& operator<<( uint& i ) { write((char*)&i,sizeof(i)); return *this;}
	CData& operator>>( uint& i ) { read((char*)&i,sizeof(i)); return *this;}

	// uint
	void wrt( unsigned long i ) { write((char*)&i,sizeof(i)); }
	CData& operator<<( unsigned long& i ) { write((char*)&i,sizeof(i)); return *this;}
	CData& operator>>( unsigned long& i ) { read((char*)&i,sizeof(i)); return *this;}

	// int
	void wrt( int i ) { write((char*)&i,sizeof(i)); }
	CData& operator<<( int& i ) { write((char*)&i,sizeof(i)); return *this;}
	CData& operator>>( int& i ) { read((char*)&i,sizeof(i)); return *this;}

	// float
	void wrt( float i ) { write( (char*)&i, sizeof(i) ); }
	CData& operator<<( float& i ) { write((char*)&i, sizeof(i) ); return *this;}
	CData& operator>>( float& i ) { read((char*)&i,sizeof(i)); return *this;}

	// double
	void wrt( double i ) { write( (char*)&i, sizeof(i) ); }
	CData& operator<<( double& i ) { write((char*)&i, sizeof(i) ); return *this;}
	CData& operator>>( double& i ) { read((char*)&i,sizeof(i)); return *this;}

	// int64
	void wrt( int64 i ) { write((char*)&i,sizeof(i)); }
	CData& operator<<( int64& i ) { write((char*)&i, sizeof(i) ); return *this;}
	CData& operator>>( int64& i ) { read((char*)&i, sizeof(i)); return *this;}

#ifdef _CDATA_MATH

	void wrt( Vector2 v ) { write((char*)&v.val,sizeof(v.val)); }
	CData& operator<<( const Vector2& v ) {write((char*)&v.val,sizeof(v.val)); return *this;}
	CData& operator>>( Vector2& v ) { read((char*)&v.val,sizeof(v.val)); return *this;}

	void wrt( Vector3 v ) { write((char*)&v.val,sizeof(v.val));}
	CData& operator<<( const Vector3& v ) {write((char*)&v.val,sizeof(v.val)); return *this;}
	CData& operator>>( Vector3& v ) { read((char*)&v.val,sizeof(v.val)); return *this;}

#endif


	// Strings
	CData& operator<<( const char* str );
	CData& operator>>( char* str );
	CData& operator<<( const String& str) { return operator<<( str.c_str() ); }
	CData& operator>>( String& str );
	int readStrLen(size_t& indent);

private:
	/// Data storage
	CRawData m_data;

	/// Points to the end of actual data + 1
	char* m_ptr;

	/// Data in container is read only if false
	bool m_store;
	
	/// Increases size of container (can not decrease it)
	void resize(size_t size);

	/// Somebody tryed to read us when we reached end of data
	bool m_readEmpty;
	
};

#endif

