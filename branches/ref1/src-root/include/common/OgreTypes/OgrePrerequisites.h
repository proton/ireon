/*-------------------------------------------------------------------------
This source file is a part of OGRE
(Object-oriented Graphics Rendering Engine)

For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This library is free software; you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License (LGPL) as 
published by the Free Software Foundation; either version 2.1 of the 
License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public 
License for more details.

You should have received a copy of the GNU Lesser General Public License 
along with this library; if not, write to the Free Software Foundation, 
Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA or go to
http://www.gnu.org/copyleft/lesser.txt
-------------------------------------------------------------------------*/
#ifndef __Prerequisites_H__
#define __Prerequisites_H__

// undefine this to not require new angular units where applicable
#define OGRE_FORCE_ANGLE_TYPES

// Platform-specific stuff
#include "OgrePlatform.h"

#if OGRE_COMPILER == OGRE_COMPILER_MSVC
// Turn off warnings generated by long std templates
// This warns about truncation to 255 characters in debug/browse info
#   pragma warning (disable : 4786)

// Turn off warnings generated by long std templates
// This warns about truncation to 255 characters in debug/browse info
#   pragma warning (disable : 4503)

// disable: "conversion from 'double' to 'float', possible loss of data
#   pragma warning (disable : 4244)

// disable: "truncation from 'double' to 'float'
#   pragma warning (disable : 4305)

// disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#   pragma warning (disable : 4251)

// disable: "non dll-interface class used as base for dll-interface class"
// Happens when deriving from Singleton because bug in compiler ignores
// template export
#   pragma warning (disable : 4275)

// disable: "C++ Exception Specification ignored"
// This is because MSVC 6 did not implement all the C++ exception 
// specifications in the ANSI C++ draft.
#   pragma warning( disable : 4290 )

// disable: "no suitable definition provided for explicit template 
// instantiation request" Occurs in VC7 for no justifiable reason on all 
// #includes of Singleton
#   pragma warning( disable: 4661)

// disable: deprecation warnings when using CRT calls in VC8 
// These show up on all C runtime lib code in VC8, disable since they clutter
// the warnings with things we may not be able to do anything about (e.g. 
// generated code from nvparse etc). I doubt very much that these calls
// will ever be actually removed from VC anyway, it would break too much code.
#	pragma warning( disable: 4996)
#endif

#if OGRE_COMPILER == OGRE_COMPILER_MSVC
#   undef _DEFINE_DEPRECATED_HASH_CLASSES
#   if OGRE_COMP_VER > 1300
#       define _DEFINE_DEPRECATED_HASH_CLASSES 0
#   else
#      define _DEFINE_DEPRECATED_HASH_CLASSES 1
#   endif
#endif

/* Include all the standard header *after* all the configuration
   settings have been made.
*/
#include "OgreStdHeaders.h"


// define the real number values to be used
// default to use 'float' unless precompiler option set
namespace Ogre {
    // Define ogre version
    #define OGRE_VERSION_MAJOR 1
    #define OGRE_VERSION_MINOR 0
    #define OGRE_VERSION_PATCH 5
    #define OGRE_VERSION_NAME "Azathoth"

    #if OGRE_DOUBLE_PRECISION == 1
		/** Software floating point type.
		@note Not valid as a pointer to GPU buffers / parameters
		*/
        typedef double Real;
    #else
		/** Software floating point type.
		@note Not valid as a pointer to GPU buffers / parameters
		*/
        typedef float Real;
    #endif

    // define the Char type as either char or wchar_t
    #if OGRE_WCHAR_T_STRINGS == 1
    #   define OgreChar wchar_t
    #	define _TO_CHAR( x ) L##x
    #else
    #   define OgreChar char
    #	define _TO_CHAR( x ) x
    #endif

    #if OGRE_COMPILER == OGRE_COMPILER_GNUC && OGRE_COMP_VER >= 310
    #   define HashMap ::__gnu_cxx::hash_map
    #else
    #   if OGRE_COMPILER == OGRE_COMPILER_MSVC
    #       if OGRE_COMP_VER > 1300 && !defined(_STLP_MSVC)
    #           define HashMap ::stdext::hash_map
    #       else
    #           define HashMap ::std::hash_map
    #       endif
    #   else
    #       define HashMap ::std::hash_map
    #   endif
    #endif

    /** In order to avoid finger-aches :)
    */
    typedef unsigned char uchar;
    typedef unsigned short ushort;
    typedef unsigned int uint;  
	typedef unsigned long ulong;  

    /// Useful macros
    #define OGRE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
    #define OGRE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }

	#if OGRE_WCHAR_T_STRINGS
		typedef std::wstring _StringBase;
	#else
		typedef std::string _StringBase;
	#endif
	
	typedef _StringBase String;

	// Useful threading defines
	#define OGRE_AUTO_MUTEX_NAME mutex
	#if OGRE_THREAD_SUPPORT
		#define OGRE_AUTO_MUTEX mutable boost::recursive_mutex OGRE_AUTO_MUTEX_NAME;
		#define OGRE_LOCK_AUTO_MUTEX boost::recursive_mutex::scoped_lock ogreAutoMutexLock(OGRE_AUTO_MUTEX_NAME);
		#define OGRE_MUTEX(name) mutable boost::recursive_mutex name;
		#define OGRE_LOCK_MUTEX(name) boost::recursive_mutex::scoped_lock ogrenameLock(name);
		// like OGRE_AUTO_MUTEX but mutex held by pointer
		#define OGRE_AUTO_SHARED_MUTEX mutable boost::recursive_mutex *OGRE_AUTO_MUTEX_NAME;
		#define OGRE_LOCK_AUTO_SHARED_MUTEX boost::recursive_mutex::scoped_lock ogreAutoMutexLock(*OGRE_AUTO_MUTEX_NAME);
		#define OGRE_NEW_AUTO_SHARED_MUTEX OGRE_AUTO_MUTEX_NAME = new boost::recursive_mutex();
		#define OGRE_DELETE_AUTO_SHARED_MUTEX delete OGRE_AUTO_MUTEX_NAME;
		#define OGRE_COPY_AUTO_SHARED_MUTEX(from) OGRE_AUTO_MUTEX_NAME = from;
	#else
		#define OGRE_AUTO_MUTEX 
		#define OGRE_LOCK_AUTO_MUTEX 
		#define OGRE_MUTEX(name)
		#define OGRE_LOCK_MUTEX(name)
		#define OGRE_AUTO_SHARED_MUTEX 
		#define OGRE_LOCK_AUTO_SHARED_MUTEX
		#define OGRE_NEW_AUTO_SHARED_MUTEX 
		#define OGRE_DELETE_AUTO_SHARED_MUTEX 
		#define OGRE_COPY_AUTO_SHARED_MUTEX(from) 
	#endif

    class Angle;
	class ColourValue;
	class Degree;
    class Math;
    class Matrix3;
	class Matrix4;
	class Plane;
    class Quaternion;
	class Radian;
    class Vector2;
    class Vector3;
    class Vector4;

}

#endif // __OgrePrerequisites_H__


