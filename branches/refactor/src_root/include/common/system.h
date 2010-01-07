/**
 * @file system.h
 * General system-dependent headers
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: system.h 284 2005-11-26 09:12:05Z zak $

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

#ifndef _SYSTEM_H
#define _SYSTEM_H


#ifdef UNIX
#define __UNIX__
#endif


// Visual Studio defines WIN32 macro by default,
// use it to autodetect platform
#ifdef WIN32
#define __WIN32__
#endif

#ifdef __WIN32__
#define NOMINMAX
#include <windows.h>
#endif

#endif
