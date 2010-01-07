/**
  * @file ireon_client/main.cpp
  * Main project .cpp file.
  * Contains "main" function that does some initialization actions
  * and CtrlHandler function that handles Ctrl key presses (for win32).
  */
 
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: main.cpp 693 2006-08-31 08:02:35Z zak $
 
  *  See LICENSE for details
  */
 
 
 
 #include "stdafx.h"
 #include "version.h"
 #include "editor_app.h"
 
 #define APPLICATION_CLASS CEditorApp
 #include "common/os_signal_handlers.h"
 #undef APPLICATION_CLASS
 
 /**
  * Main client procedure.
  * It creates application instance, initializes and runs it.
  * Also, for windows version it sets up handler for Ctrl key presses.
  * Most functionality is contained in application class.
  * @see CEditorApp
  */
 #ifdef __WIN32__
 INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
 #else
 int main(int argc, char* argv[])
 #endif
 {
 	CEditorApp app;
 	try
 	{
 		if( !app.init() )
 		{
 			std::cerr << "Application initialization failed!" << std::endl;
 			return 1;
 		}
 		SET_OS_SIGNAL_HANDLERS;
 		app.go();
 	}
 	catch( Ogre::Exception& e )
 	{
 		#ifdef __WIN32__
 		MessageBox( NULL, e.getFullDescription().c_str(), "An OGRE exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
 		#else
 		std::cerr << "An OGRE exception has occured: " <<
 		    e.getFullDescription().c_str() << std::endl;
 		#endif
 	}
 	return 0;
 }
