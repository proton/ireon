#ifndef _OGREODEPREREQS_H_
#define _OGREODEPREREQS_H_

#include "Ogre.h"
#include "OgreNoMemoryMacros.h"
#include "ode/ode.h"
#include "OgreMemoryMacros.h"
#include "OgrePagingLandScapeRaySceneQuery.h"
#include "OgrePagingLandScapeListenerManager.h"

using namespace Ogre;

#define OGREODE_TERRAINGEOMETRY
#define OGREODE_PLANARJOINT

namespace OgreOde
{
    #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    #   if defined( OGREODE_EXPORTS )
    #       define _OgreOdeExport __declspec( dllexport )
    #   else
    #       define _OgreOdeExport __declspec( dllimport )
    #   endif
    #else
    #   define _OgreOdeExport
    #endif

	class World;
	class Body;
	class Mass;
	class JointGroup;
	class Joint;
	class Space;
	class Geometry;
	class TriangleMeshGeometry;
	class BoxGeometry;
	class CapsuleGeometry;
	class CollisionListener;
	class Contact;
	class DebugObject;
	class Stepper;
	class MaintainedItem;
}

namespace OgreOde_Prefab
{
	class PrefabObject;
}

#endif

