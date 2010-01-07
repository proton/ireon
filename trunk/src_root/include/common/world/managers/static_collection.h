/**
  * @file object_manager.h
  * Object manager
  * 
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: client_static_collection.h 752 2006-09-23 19:12:44Z zak $
 
  *  See LICENSE for details
  */

#ifndef _OBJECT_MANAGER_H
#define _OBJECT_MANAGER_H
#include "common/world/managers/generic_collection.h"
//#include "boost/lambda/lambda.hpp"
//#include "boost/shared_ptr.hpp"

class CStaticObject;
//typedef boost::shared_ptr<CStaticObject> StaticPtr;

class CStaticCollection: public CGenericCollection<CStaticCollection, CStaticObject>
{

public:
	
	CStaticCollection();
	virtual ~CStaticCollection(){};
};



#endif