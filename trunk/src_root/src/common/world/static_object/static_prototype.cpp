/**
  * @file client_static_prototype.cpp
  * Prototype of static object
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: client_static_prototype.cpp 785 2006-10-25 15:48:43Z dox $
 
  *  See LICENSE for details
  */
 
#include "stdafx.h"
#include "common/world/static_object/static_prototype.h"
#include "common/world/static_object/static.h"
#include "common/resource/resource_manager.h"
#include "common/data.h"

CStaticPrototype::CStaticPrototype():
CPrototype()
{
};

CStaticPrototype::~CStaticPrototype()
{
};

void CStaticPrototype::setMeshFileFullPath(const String& meshFileFullPath)
{
	meshFileFullPath_ = meshFileFullPath;
};

String CStaticPrototype::getMeshFileFullPath()
{
	return meshFileFullPath_;
};

void CStaticPrototype::setMaterialFileFullPath(const String& materialFileFullPath)
{
	materialFileFullPath_ = materialFileFullPath;
};

String CStaticPrototype::getMaterialFileFullPath()
{
	return materialFileFullPath_;
};

