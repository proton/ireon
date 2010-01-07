/* Copyright (C) 2005 ireon.org developers council
 * $Id: world_object.cpp 391 2005-12-13 15:49:32Z llyeli $

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

/**
 * @file world_object.cpp
 * Object
 */
#include "stdafx.h"
#include "world/world_object.h"

CWorldObject::CWorldObject():
m_inWorld(false),
m_rotation(0)
{
};

CWorldObject::~CWorldObject()
{
};

void CWorldObject::update(uint time)
{
};
