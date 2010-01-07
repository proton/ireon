/**
  * @file char_mob.cpp
  * Mob
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: char_mob.cpp 715 2006-09-10 20:12:50Z mip $
 
  *  See LICENSE for details
  */

#include "stdafx.h"
#include "common/world/dynamic_object/mob.h"
#include "common/world/dynamic_object/mob_prototype.h"
#include "common/world/managers/dynamic_collection.h"
CMob::CMob(CMobPrototype* proto):
CDynamicObject(proto)
{
	type_ = CObject::MOB;
	Quaternion q;
 	q.FromAxes(Vector3::UNIT_X * proto_->getScale().x,Vector3::UNIT_Y * proto_->getScale().y,Vector3::UNIT_Z * proto_->getScale().z);
 	entityNode_->setOrientation(q);
};

CMob::~CMob()
{
};

CMob* CMob::productCallback(CMobPrototype* proto)
{
	assert(proto);
	CMob* mobObject = new CMob(proto);
	assert(mobObject);
	CDynamicCollection::getSingletonPtr()->addObject(mobObject);
	return mobObject;
}