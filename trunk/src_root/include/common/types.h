/**
 * @file types.h
 * Type defining
 */
 
/* Copyright (C) 2005 ireon.org developers council
 * $Id: types.h 861 2007-06-08 12:07:01Z mip $
 
 *  See LICENSE for details
 */
#ifndef _TYPES_H
#define _TYPES_H
 
#ifndef NO_OGRE_TYPES
 
#include "OgreString.h"
#include "OgreStringVector.h"
#include "OgreStringConverter.h"
#include "OgreMath.h"
#include "OgreVector3.h"
#include "OgreVector2.h"
#include "OgreMatrix3.h"
 
using namespace Ogre;
 
using Ogre::uint;
using Ogre::ulong;
using Ogre::ushort;
using Ogre::uchar;
using Ogre::String;
using Ogre::StringVector;
typedef unsigned char byte;
 
#else
 
#include <string>
#include <vector>
typedef std::string String;
typedef std::vector<String> StringVector;
 
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char byte;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef double Real;
 
#endif
 
#include "common/fixed.h"
typedef fixed Coord; 

#ifdef __WIN32__
typedef __int64 int64;
#else
typedef long long int64;
#endif
  
#endif
