/* Copyright (C) 2005 ireon.org developers council
 * $Id: world_object.cpp 687 2006-08-22 17:30:48Z zak $

 *  See LICENSE for details
 */

/**
 * @file world_object.cpp
 * Object
 */
#include "ireon_ws/stdafx.h"
#include "ireon_ws/world/world_object.h"

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
