/**
 * @file py_world.cpp
 * Export world to python
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: py_world.cpp 510 2006-02-26 21:09:40Z zak $

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
#include "world/object.h"
#include "world/character.h"

#define BOOST_PYTHON_STATIC_MODULE
#include <boost/python.hpp>
using namespace boost::python;

void (CObject::*setPosition1)(const Vector3& pos) = &CObject::setPosition;

BOOST_PYTHON_MODULE(World)
{
	enum_< CObject::Type >
		("ObjType")
		.value("NONE",CObject::Type::NONE)
		.value("PLAYER",CObject::Type::PLAYER)
		.value("OWN_PLAYER",CObject::Type::OWN_PLAYER)
		.value("MOB",CObject::Type::MOB);

	class_< CObject, ObjectPtr >
		("CObject", no_init)
		.add_property("type",&CObject::getType)
		.add_property("name",&CObject::getName)
		.add_property("position",&CObject::getPosition,setPosition1)
		.add_property("visible",&CObject::visible);

	class_< CCharacter, CharacterPtr, bases<CObject> >
		("Character", no_init )
		.add_property("info",&CCharacter::getInfo, &CCharacter::setInfo )
		.add_property("id",&CCharacter::getId )
		.add_property("enemy",&CCharacter::getEnemy )
		.add_property("percentHP", &CCharacter::getPercentHP, &CCharacter::setPercentHP )
		.add_property("dead", &CCharacter::isDead );
	
	class_< Vector2 >
		("Vector2",init<Real,Real>())
		.def_readwrite("x",&Vector2::x)
		.def_readwrite("y",&Vector2::y)
		.def(self + self)
		.def(self - self)
		.def(self * Real() )
		.def(self / Real() );

	class_< Vector3 >
		("Vector3", init<Real,Real,Real>())
		.def_readwrite("x",&Vector3::x)
		.def_readwrite("y",&Vector3::y)
		.def_readwrite("z",&Vector3::z)
		.def(self + self)
		.def(self - self)
		.def(self * Real() )
		.def(self / Real() );

	class_< CWorld >
		("CWorld",no_init)
		.def("translateTo2d",&CWorld::translateTo2d)
		.def("pointIsVisible",&CWorld::pointIsVisible);
}

void initWorldModule()
{
	initWorld();
};
