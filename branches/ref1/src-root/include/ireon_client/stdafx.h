/**
 * @file stdafx.h
 * Precompiled headers
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: stdafx.h 635 2006-06-02 15:55:11Z zak $

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

#ifndef _IRN_STDAFX_H
#define _IRN_STDAFX_H

#define _OGRE_TYPES
#define _CDATA_MATH

#include "common_stdafx.h"

//CEGUI class declarations
#include <OgreNoMemoryMacros.h>
#include "CEGUI/CEGUIForwardRefs.h"
#include "CEGUI/CEGUIString.h"

class CObject;
class CCharacter;
class CCharPlayer;
class COwnCharPlayer;
class CCharMob;
class CClientStaticObject;
class CMobPrototype;
class CClientStaticPrototype;
struct ClientCharPlayerData;
struct ClientOwnCharData;
struct ClientCharMobData;

class CWindow;

typedef boost::shared_ptr<CObject> ObjectPtr;
typedef boost::shared_ptr<CCharacter> CharacterPtr;
typedef boost::shared_ptr<CCharPlayer> CharPlayerPtr;
typedef boost::shared_ptr<COwnCharPlayer> OwnCharPlayerPtr;
typedef boost::shared_ptr<CCharMob> CharMobPtr;
typedef boost::shared_ptr<CClientStaticObject> StaticPtr;
typedef boost::shared_ptr<CMobPrototype> MobPrototypePtr;
typedef boost::shared_ptr<CClientStaticPrototype> StaticPrototypePtr;
typedef boost::shared_ptr<CWindow> WndPtr;
const WndPtr NULL_WINDOW = WndPtr();

#include <OgreMemoryMacros.h>
#include "interface/client_event_manager.h"
#include "world/client_world.h"
#include "interface/client_interface.h"
#include "client_app.h"

#include "Ogre.h"

#define I_WORLD (CClientWorld::instance())
#define I_INTERFACE (CClientInterface::instance())
#define I_APP (CClientApp::instance())

#endif
