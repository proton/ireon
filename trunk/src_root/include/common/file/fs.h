/* Copyright (C) 2005 ireon.org developers council
 * portions (C) Radon Labs GmbH, www.nebuladevice.org
 * $Id: fs.h 672 2006-08-16 18:33:53Z zak $

 *  See LICENSE for details
 */

/**
 * @file fs.h
 * CFile System (or CFile Server :)
 */
#ifndef _CFS_H
#define _CFS_H

class CDirectory;
class IRawFile;
//class CFile;
#include "common/file/file.h"
#include <map>


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
	void addBytesRead(int b);
	/// add written bytes
	void addBytesWritten(int b);
	/// add seek operation to stats
	void addSeek();
	/// get bytes read since last reset
	int getBytesRead() const;
	/// get bytes written since last reset
	int getBytesWritten() const;
	/// get number of seeks
	int getNumSeeks() const;

private:

	static CFS* m_singleton;

	std::map<String, String> m_mapPath;

	int m_bytesRead;
	int m_bytesWritten;
	int m_numSeeks;
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

inline void CFS::addBytesRead(int b)
{
	this->m_bytesRead += b;
}

inline void CFS::addBytesWritten(int b)
{
	this->m_bytesWritten += b;
}

inline void CFS::addSeek()
{
	this->m_numSeeks++;
}

inline int CFS::getBytesRead() const
{
	return this->m_bytesRead;
}

inline int CFS::getBytesWritten() const
{
	return this->m_bytesWritten;
}

inline int CFS::getNumSeeks() const
{
	return this->m_numSeeks;
}

#endif
