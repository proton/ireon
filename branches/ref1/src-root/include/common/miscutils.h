/**
 * @file miscutils.h
 * miscellaneous functions
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: miscutils.h 561 2006-03-21 20:11:14Z zak $

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

#ifndef _MISCUTILS_H_
#define _MISCUTILS_H_

#ifdef __UNIX__
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

namespace ireon {
	namespace misc {
		bool printPIDToFile(const char *filename)
		{
			#ifdef __UNIX__
			FILE *fp;
			if ((fp = fopen(filename, "w"))==NULL)
				return false;
			if (fprintf(fp, "%i", getpid()) < 1) {
				fclose(fp);
				return false;
			}
			fclose(fp);
			#endif
			filename;
			// for other systems do nothing
			return true;
		}

	}
}

#endif
