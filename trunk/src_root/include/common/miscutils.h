/**
 * @file common/miscutils.h
 * miscellaneous functions
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: miscutils.h 778 2006-10-05 20:15:59Z zak $

 * See LICENSE for details
 */

#ifndef _MISCUTILS_H_
#define _MISCUTILS_H_

#include "common/common_stdafx.h"

namespace ireon {
	namespace misc {
		bool printPIDToFile(const char *filename);
		int64 getTime();
		String getSHA256Digest(const String &plaintext);
	}
}

#endif
