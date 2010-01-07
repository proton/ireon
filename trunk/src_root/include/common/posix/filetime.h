/* Copyright (C) 2005 ireon.org developers council
  * portions (C) Radon Labs GmbH, www.nebuladevice.org
 
  *  See LICENSE for details
  */
 
 /**
  * @file posix/filetime.h
  * FileTime structure for posix systems.
  */
 #ifndef _FILETIMEPOSIX_H
 #define _FILETIMEPOSIX_H
 
 #if !defined(__LINUX__) && !defined(__MACOSX__) && !defined(__UNIX__)
 #error "FileTimePosix: trying to compile POSIX class on hostile platform"
 #endif
 
 #include <time.h>
 
 //------------------------------------------------------------------------------
 class FileTimePosix
 {
 public:
 	/// constructor
 	FileTimePosix();
 	/// operator ==
 	friend bool operator ==(const FileTimePosix& a, const FileTimePosix& b);
 	/// operator >=
 	friend bool operator >(const FileTimePosix& a, const FileTimePosix& b);
 	/// operator <=
 	friend bool operator <(const FileTimePosix& a, const FileTimePosix& b);
 
 private:
 	friend class CFile;
 	time_t m_time;
 };
 
 //------------------------------------------------------------------------------
 /**
 */
 inline
 FileTimePosix::FileTimePosix()
 {
 	m_time = 0;
 }
 
 //------------------------------------------------------------------------------
 /**
 */
 inline
 bool
 operator ==(const FileTimePosix& a, const FileTimePosix& b)
 {
 	return (a.m_time == b.m_time);
 }
 
 //------------------------------------------------------------------------------
 /**
 */
 inline
 bool
 operator >(const FileTimePosix& a, const FileTimePosix& b)
 {
 	return (a.m_time > b.m_time);
 }
 
 //------------------------------------------------------------------------------
 /**
 */
 inline
 bool
 operator <(const FileTimePosix& a, const FileTimePosix& b)
 {
 	return (a.m_time < b.m_time);
 }
 
 //------------------------------------------------------------------------------
 #endif
 
