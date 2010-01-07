/**
 * @file log.h
 * Log manager.
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: log.h 579 2006-03-26 12:58:49Z zak $

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

#ifndef _LOG_H
#define _LOG_H

/// max number of chars in log message
const uint MAX_LOG_MESSAGE_LENGTH = 200;

/// Central facility for logging. Redirects messages to 
/// its listeners to do actual work.
class CLog
{
	//Constructor protected
	CLog();
public:

	/** message levels */
	enum msgLevels {
		msgLvlOff = 0,			///< should not be used for messages, just to set threshold in log listeners
		msgLvlCritical = 40,	///< critical messages, program execution can not be continued
		msgLvlError = 80,		///< non critical errors
		msgLvlWarn = 120,		///< warnings of all kinds
		msgLvlInfo = 160,		///< some informational messages, like "staring server..."
		msgLvlVerbose = 200,	///< verbose info output
		msgLvlDebug = 240,		///< debugging messages
		msgLvlTrace = 250		///< for tracing execution flow
	};

	/** message flags (for filtering) */
	enum flags
	{
		msgFlagNone = 0x01,			///< default flag
		msgFlagPython = 0x02,		///< python connectivity issues
		msgFlagNetwork = 0x04,		///< network issues
		msgFlagResources = 0x08,	///< resources loading and usage
		msgFlagMysql = 0x10,		///< db connectivity
		msgFlagAccChar = 0x20,		///< account and character management issues
		msgFlagALL = 0xFFFFFFFF		///<
	};

	///Instance
	static CLog* instance();
	///Destructor
	virtual ~CLog();
	///Register listener
	void regListener(CLogListener *listener) { assert(listener); m_listeners.insert(m_listeners.begin(),listener); }
	///Remove listener
	inline void remListener(CLogListener *listener) {m_listeners.remove(listener);}
	///Logging functions
	void log(uint flags, CLog::msgLevels msgLevel, const char* message, ... );
	void log(uint flags, CLog::msgLevels msgLevel, const char* message, va_list& args);
	void log(CLog::msgLevels msgLevel, const char* message, ... );
	void log(const char* message, ...);
	void dump(uint flags, CLog::msgLevels msgLevel, const void* data,size_t length);
	void dump(CLog::msgLevels msgLevel, const void* data, size_t length ) {dump(m_defFlags,msgLevel,data,length); }
	void dump(const void* data, size_t length) {dump(m_defFlags,m_defMsgLevel,data,length);}
	/// Write time to log (with newline) in format (day.month.year hour:minutes:seconds)
	void timeStamp();

	///Access to variables
	void setDefFlags(uint flags) {m_defFlags = flags;}
	uint getDefFlags() {return m_defFlags;}
	void setDefMsgLevel(CLog::msgLevels msgLevel) {m_defMsgLevel = msgLevel;}
	CLog::msgLevels getDefMsgLevel() {return m_defMsgLevel;}

	///Close all logs
	void close();

protected:
	///Singleton
	static CLog* m_singleton;

	///Default flags
	uint m_defFlags;
	///Default message level
	CLog::msgLevels m_defMsgLevel;
	///List of log listeners
	std::list<CLogListener*> m_listeners;
};

inline CLog* CLog::instance()
{
	if( !m_singleton )
		m_singleton = new CLog();
	return m_singleton;
};

#endif
