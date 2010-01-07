/**
  * @file system.h
  * General system-dependent headers
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: system.h 690 2006-08-29 18:43:35Z zak $
 
  *  See LICENSE for details
  */
 
#ifndef _SYSTEM_H
#define _SYSTEM_H
 
 
#ifdef UNIX
#define __UNIX__
#endif
 
 
// Visual Studio defines WIN32 macro by default,
// use it to auto detect platform
#ifdef WIN32
#ifndef __WIN32__
	#define __WIN32__
#endif
#endif
 
#ifdef __WIN32__
	#define NOMINMAX
	#include <windows.h>
#endif
 
#endif
