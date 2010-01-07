/* Copyright (C) 2005 ireon.org developers council
 * portions (C) Radon Labs GmbH, www.nebuladevice.org
 * $Id: filetime.h 687 2006-08-22 17:30:48Z zak $

 *  See LICENSE for details
 */

/**
 * @file filetime.h
 * Incapsulate a file time structure.
 */
#ifndef _FILETIME_H
#define _FILETIME_H

#ifdef __WIN32__
#include "common/win32/filetime.h"

typedef FileTimeWin32 FileTime;

#elif defined(__LINUX__) || defined(__MACOSX__) || defined(__UNIX__)
#include "common/posix/filetime.h"

typedef FileTimePosix FileTime;

#else
#error "FileTime not implemented for this platform"
#endif

//------------------------------------------------------------------------------
#endif
