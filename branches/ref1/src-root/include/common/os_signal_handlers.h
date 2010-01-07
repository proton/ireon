/**
 * @file signal_haldlers.h
 * defines some os signal handling things (for *nix and win32)
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: os_signal_handlers.h 561 2006-03-21 20:11:14Z zak $

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

#ifndef _SIGNAL_HANDLERS_H_
#define _SIGNAL_HANDLERS_H_

#ifndef APPLICATION_CLASS
#error APPLICATION_CLASS not defined
#endif

#include <iostream>
#ifdef __UNIX__

#include <signal.h>

namespace ireon {
	namespace misc {
		void (*setSignalHandler(int signo, void (*hndlr)(int)))(int)
		{
			struct sigaction act, oact;
			act.sa_handler = hndlr;
			sigemptyset(&act.sa_mask);
			act.sa_flags = 0;
			if(signo != SIGALRM)
				act.sa_flags |= SA_RESTART;
			if(sigaction(signo,&act,&oact) < 0)
				return (SIG_ERR);
			return oact.sa_handler;
		}

		//---------------------------------

		static inline void terminate(int)
		{
			std::cout << "\ngot SIGTERM, shutting down...\n";
			APPLICATION_CLASS::instance()->shutdown();
		}

	}
}

#define SET_OS_SIGNAL_HANDLERS ireon::misc::setSignalHandler(SIGTERM, ireon::misc::terminate)

#endif // __UNIX__

//---------------------------------
//---------------------------------
//---------------------------------

#ifdef __WIN32__

#define SET_OS_SIGNAL_HANDLERS SetConsoleCtrlHandler((PHANDLER_ROUTINE) ireon::misc::CtrlHandler, TRUE)

namespace ireon {
	namespace misc {
		BOOL CtrlHandler(DWORD fdwCtrlType) 
		{ 
			fdwCtrlType;
			APPLICATION_CLASS::instance()->shutdown();
			return TRUE;
		} 
	}
}

#endif // __WIN32__


#endif
