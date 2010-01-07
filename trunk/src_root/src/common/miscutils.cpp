/**
 * @file common/miscutils.cpp
 * miscellaneous functions
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: miscutils.cpp 778 2006-10-05 20:15:59Z zak $

 * See LICENSE for details
 */

#include "common/miscutils.h"
#include "common/sha256.h"

#ifdef __UNIX__
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

bool ireon::misc::printPIDToFile(const char *filename)
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
	// for other systems do nothing
	return true;
	filename;
}

//-----------------------------------------------------------------------

int64 ireon::misc::getTime()
{
	#ifdef __WIN32__
		return (int64) timeGetTime();
	#else
		struct timeval now;
		gettimeofday( &now, (struct timezone *) 0 );
		return now.tv_sec*1000 + now.tv_usec/1000;
	#endif
}

//-----------------------------------------------------------------------

String ireon::misc::getSHA256Digest(const String &plaintext)
{
	SHA256 digest;
	digest.write(reinterpret_cast<const byte *> (plaintext.c_str()), plaintext.size());
	digest.final();
	return base64_encode(digest.read(), 32);
}

