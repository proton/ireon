#include "stdafx.h"

namespace Ogre
{
 class Root;
 class RenderWindow;
};

bool configure(Ogre::Root* m_ogreRoot)
{
 	if( !m_ogreRoot )
 		return false;
 	if(m_ogreRoot->showConfigDialog())
 	{
 		return true;
 	}
 	else
 	{
 		return false;
 	}
 	return true;
}

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	Ogre::Root* m_ogreRoot;
	#ifdef _DEBUG
		const char *plugins = "../config/plugins_debug.cfg";
	#elif defined NDEBUG
		const char *plugins = "../config/plugins_release.cfg";
	#else 
		#error _DEBUG or NDEBUG must be defined.
	#endif

 	m_ogreRoot = new Ogre::Root(plugins,"../config/ogre.cfg","../logs/ogre.log");

	if( !configure(m_ogreRoot) )
 		return 1;
	return 0;
} 
 