/**
 * @file stdafx.h
 * Precompiled headers
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: stdafx.h 510 2006-02-26 21:09:40Z zak $

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

#include "version.h"
#include "common_stdafx.h"

//CEGUI class declarations
#include <OgreNoMemoryMacros.h>
#include "CEGUI/CEGUIForwardRefs.h"
#include "CEGUI/CEGUIString.h"
#include <OgreMemoryMacros.h>

class CObject;
class CCharacter;
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
typedef boost::shared_ptr<CCharMob> CharMobPtr;
typedef boost::shared_ptr<CClientStaticObject> StaticPtr;
typedef boost::shared_ptr<CMobPrototype> MobPrototypePtr;
typedef boost::shared_ptr<CClientStaticPrototype> StaticPrototypePtr;
typedef boost::shared_ptr<CWindow> WndPtr;
const WndPtr NULL_WINDOW = WndPtr();

#include "interface/editor_event_manager.h"
#include "world/editor_world.h"
#include "interface/editor_interface.h"
#include "editor_app.h"

#include "Ogre.h"

#define I_WORLD (CEditorWorld::instance())
#define I_INTERFACE (CEditorInterface::instance())
#define I_APP (CEditorApp::instance())

#endif
