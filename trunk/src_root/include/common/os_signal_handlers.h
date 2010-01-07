/**
  * @file signal_haldlers.h
  * defines some os signal handling things (for *nix and win32)
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: os_signal_handlers.h 694 2006-08-31 09:44:50Z zak $
 
  *  See LICENSE for details
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
 			APPLICATION_CLASS::instance()->shutdown();
 			return TRUE;
			fdwCtrlType;
 		} 
 	}
 }
 
 #endif // __WIN32__
 
 
 #endif
