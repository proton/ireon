/**
 * @file ireon_client/main.cpp
 * Main project .cpp file.
 * Contains "main" function that does some initialization actions
 * and CtrlHandler function that handles Ctrl key presses (for win32).
 */


/* Copyright (C) 2005 ireon.org developers council
 * $Id: main.cpp 510 2006-02-26 21:09:40Z zak $

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
