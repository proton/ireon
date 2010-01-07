/* Copyright (C) 2005 ireon.org developers council
 * $Id: define.h 752 2006-09-23 19:12:44Z zak $
 
 *  See LICENSE for details
 */
 
/**
 * @file define.h
 * Defines and constants
 */
#ifndef _DEFINE_H
#define _DEFINE_H
 
//#pragma warning(disable:4267) ///Disable warning 'possible loss of data'
 
#ifndef DISABLE_GETTEXT
	#include <libintl.h>
	#define gtc_(string) dgettext ("common", string)
	#ifndef GETTEXT_USE_COMMON_DOMAIN_ONLY
		#define _(string) gettext (string)
	#endif
#else
	#define _(txt) (txt)
	#define gtc_(txt) (txt)
#endif

//#define g_log(txt) CLog::instance()->log(txt);
 
#endif
