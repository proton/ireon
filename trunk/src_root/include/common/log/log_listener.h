/* Copyright (C) 2005 ireon.org developers council
 * $Id: log_listener.h 714 2006-09-07 16:32:40Z zak $

 * See LICENSE for details
 */

/**
 * @file log_listener.h
 * Log listener and subclasses.
 */

#ifndef _LOG_LISTENER_H
#define _LOG_LISTENER_H
const byte LOG_END_LINE = 0x0a; /// '\n' - endline . NOTE: unix file format use '\r\n' - '0xd 0xa'
const byte LOG_NONBREAK_LINE = 0x08; /// '\b' - bell ascii. used for nonbreak line
const byte LOG_SPACE = 0x20; /// ' ' - space

#include "log.h"

#include "common/file/file.h"
//#include "boost/shared_ptr.hpp"
//class CFile;
//typedef boost::shared_ptr<CFile> FilePtr;
#include <list>


class CLogListener
{
protected:
	///Abstract class
	CLogListener();

public:
	///Does it works?
	virtual bool works() const = 0;

	///Close log
	virtual void close() = 0;
	///get last symbol logged 
	virtual byte getLastSymbol() = 0;
	
public:
	///Process message
	virtual bool log(uint flags, byte msgLevel, const char* message) = 0;

	virtual bool dump(uint flags, byte msgLevel, const void* data, uint length) = 0;
};

class CFileLogListener: public CLogListener
{
private:
	///Default constructor is private
	CFileLogListener();
public:
	CFileLogListener(const char* filename, CLog::flags mask = CLog::msgFlagALL, CLog::msgLevels msgLevelThreshold = CLog::msgLvlVerbose);
	~CFileLogListener();

	bool log(uint flags, byte msgLevel, const char* message);

	bool dump(uint flags, byte msgLevel, const void* data, uint length);

	bool works() const;

	void close();

	byte getLastSymbol();
	

private:
	///Log file
	FilePtr m_file;
	uint m_mask;
	byte m_msgLevelThreshold;
	byte m_lastSymbol;

};

class CScreenLogListener: public CLogListener
{
public:
	/// @param prefix - short prefix (32 chars max) for output
	CScreenLogListener(const char* prefix = "", CLog::flags mask = CLog::msgFlagALL, CLog::msgLevels msgLevelThreshold = CLog::msgLvlError);
	~CScreenLogListener();

	/// log message
	bool log(uint flags, byte msgLevel, const char* message);

	/// log data
	bool dump(uint flags, byte msgLevel, const void* data, uint length);

	bool works() const {return true;}

	void close() {}
	
	byte getLastSymbol();
	
private:
	char m_prefix[33];
	uint m_mask;
	byte m_msgLevelThreshold;
	byte m_lastSymbol; 
};


#endif
