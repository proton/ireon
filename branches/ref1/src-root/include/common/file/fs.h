/* Copyright (C) 2005-2006 ireon.org developers council
 * portions (C) Radon Labs GmbH, www.nebuladevice.org
 * $Id: fs.h 565 2006-03-22 16:50:54Z zak $

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
 * @file fs.h
 * CFile System (or CFile Server :)
 */
#ifndef _CFS_H
#define _CFS_H

class CDirectory;
class IRawFile;
class CFile;

class CFS
{
protected:
	friend class CClientApp;
	/// constructor
	CFS();

	// init
	static void init();
public:
	/// destructor
	virtual ~CFS();

	/// get instance pointer
	static CFS* instance();

	/// sets a path alias
	bool addAlias(const char* assignName, const char* pathName);
	
	String getAlias(const char* path) const;

	String realPath(const char* path) const;
	/// makes a path
	bool makePath(const char* path) const;
	/// copy a file
	bool copyFile(const char* from, const char* to);
	/// delete a file
	bool deleteFile(const char* filename);
	/// delete an empty directory
	bool deleteDirectory(const char* dirName);
	/// compute the Crc checksum for a file
	bool checksum(const char* filename, uint& crc);
	/// set read only status of a file
	void setFileReadOnly(const char* filename, bool readOnly);
	/// get read only status of a file
	bool isFileReadOnly(const char* filename);

	/// creates a new nCFile object
	FilePtr newFileObject() const;
	/// check if file exists
	bool fileExists(const char* pathName) const;
	/// check if directory exists
	bool directoryExists(const char* pathName) const;

	/// reset statistics
	void resetStatistics();
	/// add read bytes 
	void addBytesRead(size_t b);
	/// add written bytes
	void addBytesWritten(size_t b);
	/// add seek operation to stats
	void addSeek();
	/// get bytes read since last reset
	size_t getBytesRead() const;
	/// get bytes written since last reset
	size_t getBytesWritten() const;
	/// get number of seeks
	size_t getNumSeeks() const;

private:

	static CFS* m_singleton;

	std::map<String, String> m_mapPath;

	size_t m_bytesRead;
	size_t m_bytesWritten;
	size_t m_numSeeks;
};

inline CFS* CFS::instance()
{
	if( !m_singleton )
		m_singleton = new CFS;
	return m_singleton;
}

inline void CFS::resetStatistics()
{
	this->m_bytesRead = 0;
	this->m_bytesWritten = 0;
	this->m_numSeeks = 0;
}

inline void CFS::addBytesRead(size_t b)
{
	this->m_bytesRead += b;
}

inline void CFS::addBytesWritten(size_t b)
{
	this->m_bytesWritten += b;
}

inline void CFS::addSeek()
{
	this->m_numSeeks++;
}

inline size_t CFS::getBytesRead() const
{
	return this->m_bytesRead;
}

inline size_t CFS::getBytesWritten() const
{
	return this->m_bytesWritten;
}

inline size_t CFS::getNumSeeks() const
{
	return this->m_numSeeks;
}

#endif
