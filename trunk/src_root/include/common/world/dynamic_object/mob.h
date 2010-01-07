/**
  * @file char_mob.h
  * Player's character
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: char_mob.h 829 2006-11-30 18:39:37Z mip $
 
  *  See LICENSE for details
  */
#ifndef _MOB_H
#define _MOB_H

#include "common/world/dynamic_object/dynamic.h" 

class CMobPrototype;

class CMob : public CDynamicObject
{
public:
	CMob(CMobPrototype* proto);
	~CMob();

	static CMob* productCallback(CMobPrototype* proto);

};

#endif

