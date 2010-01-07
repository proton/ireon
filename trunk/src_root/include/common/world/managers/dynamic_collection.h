/**
* @file client_static.h
* Static object
*/

/* Copyright (C) 2005 ireon.org developers council
* $Id: client_static.h 829 2006-11-30 18:39:37Z mip $

*  See LICENSE for details
*/
#ifndef _DYNAMIC_COLLECTION_H
#define _DYNAMIC_COLLECTION_H

#include "common/world/managers/generic_collection.h"
#include "common/world/dynamic_object/character.h"

class CDynamicObject;

class CDynamicCollection: public CGenericCollection<CDynamicCollection, CDynamicObject>
{
public:

	CDynamicCollection();
 
	virtual ~CDynamicCollection(){};

	COwnPlayer* getOwnPlayer(){assert(ownPlayer_);return ownPlayer_;};
	void setOwnPlayer(COwnPlayer* player){assert(player); ownPlayer_ = player;};
	

private:
	COwnPlayer* ownPlayer_;
};

#endif
