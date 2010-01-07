/**
  * @file client_static_prototype.h
  * Prototype of static object
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: client_static_prototype.h 829 2006-11-30 18:39:37Z mip $
 
  *  See LICENSE for details
  */
#ifndef _STATIC_PROTOTYPE_H
#define _STATIC_PROTOTYPE_H
#include "common/world/generic_object/prototype.h"
//#include "boost/shared_ptr.hpp"
#include "common/world/geometry.h"

class CStaticObject;
//typedef boost::shared_ptr<CStaticObject> StaticPtr;
 
class CStaticPrototype: public CPrototype
{
public:
	CStaticPrototype();
	~CStaticPrototype();
	
	void setMeshFileFullPath(const String& meshFileFullPath);
	String getMeshFileFullPath();
	
	void setMaterialFileFullPath(const String& meshFileFullPath);
	String getMaterialFileFullPath();


protected:
	
	String meshFileFullPath_;
	String materialFileFullPath_;
};

#endif

