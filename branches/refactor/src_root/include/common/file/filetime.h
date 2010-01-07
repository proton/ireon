/* Copyright (C) 2005 ireon.org developers council
 * portions (C) Radon Labs GmbH, www.nebuladevice.org
 * $Id: filetime.h 236 2005-11-22 19:35:40Z llyeli $

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
 * @file filetime.h
 * Incapsulate a file time structure.
 */
#ifndef _FILETIME_H
#define _FILETIME_H

#ifdef __WIN32__
#include "win32/filetime.h"

typedef FileTimeWin32 FileTime;

#elif defined(__LINUX__) || defined(__MACOSX__) || defined(__UNIX__)
#include "posix/filetime.h"

typedef FileTimePosix FileTime;

#else
#error "FileTime not implemented for this platform"
#endif

//------------------------------------------------------------------------------
#endif
