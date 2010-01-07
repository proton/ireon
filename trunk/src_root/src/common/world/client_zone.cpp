/**
  * @file client_zone.cpp
  * Client-side zone class
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: client_zone.cpp 857 2007-05-18 12:56:16Z mip $
 
  *  See LICENSE for details
  */
#include "stdafx.h"
#include "common/world/client_zone.h"
#include "ireon_client/world/client_world.h"
#include "common/world/static_object/static.h"
#include "common/world/static_object/static_prototype.h"
#include "common/world/managers/static_collection.h"

#include "common/world/managers/command.h"
#include "common/world/managers/dispatcher.h"
#include "common/resource/resource_manager.h"
#include <Ogre/main/OgreNoMemoryMacros.h>
#include "ireon_client/xsd/zone_xsd.h"
#include <Ogre/main/OgreMemoryMacros.h>

#include "common/world/managers/object_factory.h"
#include "common/world/managers/prototype_manager.h"

#include "common/world/mesh.h"

bool processNode(ireon::client::CfgNodes::node::iterator itNode, const String& parentNodeName);
String getEntityTypeFromNodeType(const String& nodeType);

CClientZone::CClientZone(const String& resourceName)
{
	_log(_info,_loc,_("Loading zone from '../data/proto/zones/%s'.\n"),resourceName.c_str());

	// read configuration from scene file from path 'resourceName'
	// http://www.yake.org/wiki/doku.php?id=octopus:octopus&s=octopus

	String pathToConfig = "../data/proto/zones/";
	try 
	{
		_log(_info,_loc,_("Process info about nodes from scene file")); 

		std::auto_ptr<ireon::client::CfgScene> config;
		config = ireon::client::scene(pathToConfig.append(resourceName));

		for ( ireon::client::CfgNodes::node::iterator
			it = config->nodes().node().begin();
			it != config->nodes().node().end();
		++it )
		{
			processNode(it, CClientWorld::instance()->sceneManager()->getRootSceneNode()->getName());
		}
	}
	catch (const xml_schema::parsing& exc)
	{
		_log(_error, _loc, _("%s"), exc.what());
		for (xml_schema::errors::const_iterator it = exc.errors().begin(); it != exc.errors().end(); it++)
			_log(_error, _loc, _("[%u:%u] %s "),  it->line(), it->column(), it->message().c_str());
		//TODO: add xml_schema::expected_element and etc 
		_log(_error, _loc, _("Scene file mailformed\n"), exc.what());
	}

	try 
	{
		_log(_info,_loc,_("Process info about navigation mesh from '../data/proto/zones/navigation_mesh.xml'")); 

		navigationMesh_.reset(new ireon::nav::Mesh("../data/proto/zones/navigation_mesh.xml"));
	
	}
	catch (const xml_schema::parsing& exc)
	{
		_log(_error, _loc, _("%s"), exc.what());
		for (xml_schema::errors::const_iterator it = exc.errors().begin(); it != exc.errors().end(); it++)
			_log(_error, _loc, _("[%u:%u] %s "),  it->line(), it->column(), it->message().c_str());
		//TODO: add xml_schema::expected_element and etc 
		_log(_error, _loc, _("Navigation mesh file mailformed"), exc.what());
	}
	
}

CClientZone::~CClientZone()
{
};



bool processNode(ireon::client::CfgNodes::node::iterator itNode, const String& parentNodeName)
{
	
	//uint nodeId   = itNode->id();
	
	// TODO: rename attribute to 'type'
	//_log(_debug,_loc,_("read type = "));
	String nodeType = itNode->name();
	//_log(_debug,_loc,_("'%s'.\n"),nodeType.c_str());
	
	// read info about node position
	//_log(_debug,_loc,_("read position = "));
	long double posX = itNode->position().x();
	long double posY = itNode->position().y();
	long double posZ = itNode->position().z();
	
	// read info about node rotation
	long double rotQX = itNode->rotation().qx();
	long double rotQY = itNode->rotation().qy();
	long double rotQZ = itNode->rotation().qz();
	long double rotQW = itNode->rotation().qw();

	// read info about node scale
	long double scaleX = itNode->scale().x();
	long double scaleY = itNode->scale().y();
	long double scaleZ = itNode->scale().z();
	

	// node linked with entity-object
	if (itNode->entity().present())
	{
 	    // create info about linked with this entity material and mesh
		String pathToData = "..\\data\\";
		String entityType = getEntityTypeFromNodeType(nodeType);
		String entityName         = nodeType;
		String entityMesh = entityType;
		entityMesh.append(".mesh");
		String entityMeshFile     = pathToData;
		entityMeshFile.append("models\\").append(entityType).append(".mesh");
		String entityMaterialFile = pathToData;
		entityMaterialFile.append("materials\\scripts\\").append(entityType).append(".material");;
		
		// create static object prototype, etc entity prototype - for multiply creating from this prototype static objects
		CStaticPrototype* staticProto = new CStaticPrototype();
		staticProto->setMeshFileFullPath(entityMeshFile);
		staticProto->setMaterialFileFullPath(entityMaterialFile);
		staticProto->setName(entityType.c_str());
		staticProto->setMeshName(entityMesh.c_str());
		Vector3 scale3;
		scale3.x = scaleX;
		scale3.y = scaleY;
		scale3.z = scaleZ;
		staticProto->setScale(scale3);
		// all proto must be accessed from proto manager
		CPrototypeManager::getSingletonPtr()->addObject(staticProto);
		// create object and add him in static collection
		CStaticObject* staticObject = (CStaticObject*)CObjectFactory::getSingletonPtr()->createProduct("static", staticProto );
		
		// apply info about node on static object
		// apply name
		staticObject->setName(entityName.c_str());

		// apply rotation
		Quaternion rotQ;
		rotQ.x = static_cast<Real> (rotQX);
		rotQ.y = static_cast<Real> (rotQY);
		rotQ.z = static_cast<Real> (rotQZ);
		rotQ.w = static_cast<Real> (rotQW);

		CDispatcher::getSingletonPtr()->executeNow(
			CommandPtr(	new CRotateAbsoluteStaticObjectCommand(	entityName, rotQ)));
		
		// apply position
		Vector3 pos;
		pos.x = posX;
		pos.y = posY;
		pos.z = posZ;
		CDispatcher::getSingletonPtr()->executeNow(
			CommandPtr(	new CMoveAbsoluteStaticObjectCommand(	entityName, pos)));

	}
	
	// recursive call for node linked this node 
	for ( ireon::client::CfgNode::node::iterator
		it = itNode->node().begin();
		it != itNode->node().end();
		++it )
	{
		if (!processNode(it, nodeType))
			return false;				
	}
	return true;
}

String getEntityTypeFromNodeType(const String& nodeType)
{
	StringVector splitNodeType = StringUtil::split(nodeType,"-",1);
	return splitNodeType.empty()?nodeType:splitNodeType[0];
}

 
