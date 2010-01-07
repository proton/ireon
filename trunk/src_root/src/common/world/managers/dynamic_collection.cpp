/**
* @file dynamic_collection.cpp
* Dynamic object collection
*/

/* Copyright (C) 2005 ireon.org developers council
* $Id$

*  See LICENSE for details
*/

#include "stdafx.h"
#include "common/world/managers/dynamic_collection.h"

CDynamicCollection::CDynamicCollection():
CGenericCollection<CDynamicCollection, CDynamicObject>("DynamicCollection")
{

}