/**
 * @file types.h
 * Type defining
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: types.h 397 2005-12-15 22:19:52Z llyeli $

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
#ifndef _TYPES_H
#define _TYPES_H

#ifdef _OGRE_TYPES

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

typedef std::string String;
typedef std::vector<String> StringVector;

typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char byte;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef double Real;

#endif


#ifdef __WIN32__
typedef __int64 int64;
#else
typedef long long int64;
#endif

/// For using assign operators
using namespace boost::assign;

#endif
