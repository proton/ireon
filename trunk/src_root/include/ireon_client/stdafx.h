/**
  * @file stdafx.h
  * Precompiled headers
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: stdafx.h 845 2007-01-04 21:48:12Z mip $
 
  *  See LICENSE for details
  */
 
#ifndef _IRN_STDAFX_H
#define _IRN_STDAFX_H
 
#define _OGRE_TYPES
#define _CDATA_MATH
#define TIXML_USE_STL

#if 0
// [sample]: warning C4251: 'Ogre::AnimableObject::msAnimableDictionary' :
// class 'std::map<_Kty,_Ty>' needs to have dll-interface to be used
// by clients of class 'Ogre::AnimableObject' 
// [MSDN 2005]: C4251 can be ignored in Microsoft Visual C++ 2005
// if you are deriving from a type in the Standard C++ Library,
// compiling a debug release (/MTd) and where the compiler error message
// refers to _Container_base.
#pragma warning( disable : 4251)
//  warning C4244: 'operator' : conversion from 'type1' to 'type2',
//	possible loss of data
#pragma warning( disable : 4244)
//warning C4275: non dll-interface struct 'class1'
//used as base for dll-interface class 'class2'
#pragma warning( disable : 4275)
#endif

//warning C4512: assignment operator could not be generated
#pragma warning (disable : 4512)

#include "ireon_client/version.h"
#include "common/common_stdafx.h"



#include "common/log/log.h"


#endif
