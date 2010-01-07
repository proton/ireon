/**
  * @file mob_prototype.cpp
  * Mob's prototype
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: mob_prototype.cpp 785 2006-10-25 15:48:43Z dox $
  
  *  See LICENSE for details
  */

#include "stdafx.h"
#include "common/world/dynamic_object/mob_prototype.h"
#include "common/resource/resource_manager.h"
 
CMobPrototype::CMobPrototype():
CPrototype() 
{
};

CMobPrototype::~CMobPrototype()
{
};

//bool CMobPrototype::load(const String& type)
//{
//	m_type = type; 
//	_log(_info,_loc,"Loading mob prototype from '%s'.\n",type.c_str());
//	DataPtr dPtr = CResourceManager::instance()->load(type + ".mobprot");
//	if( !dPtr )
//	{
//		_log(_error,_loc,"Error loading mob prototype: resource not found.\n",type.c_str());
//		return false;
//	}
//	TiXmlDocument doc;
//	std::stringstream buf;
//	//buf.write(dPtr->data(),dPtr->size());
//	//buf >> doc;
//
//	if (doc.Error())
//	{
//		_log(_error,_loc, _("Error loading mob prototype: XML parser returned an error: %s\n"), doc.ErrorDesc());
//		return false;
//	}
//
//	TiXmlNode* root =  doc.FirstChild();
//	while( root && root->Type() == TiXmlNode::DECLARATION )
//		root = root->NextSibling();
//	if( !root || root->Type() != TiXmlNode::ELEMENT || strcmp(root->ToElement()->Value(),"MobPrototype") )
//	{
//		_log(_error,_loc, _("Error loading mob prototype: resource isn't mob prototype. %s\n"),root->Value());
//		return false;
//	};
//
//	TiXmlNode* option;
//	for( option = root->FirstChild(); option; option = option->NextSibling() )
//		if( !processOption(option) )
//		{
//			_log(_error,_loc, _("Error loading mob prototype: error in file near '%s'.\n"),option->Value());
//			return false;
//		};
//	return true;
//};
//
//bool CMobPrototype::processOption(TiXmlNode* option)
//{
//	if( !option )
//		return false;
//
//	if( !option->Type() == TiXmlNode::ELEMENT )
//		return false;
//	TiXmlAttribute* attr = option->ToElement()->FirstAttribute();
//	if( !attr || !attr->Name() || !attr->Value() )
//		return false;
//	if( !strcmp(attr->Name(),"Mesh") )
//		m_mesh = attr->Value();
//	else if( !strcmp(attr->Name(),"Name") )
//		m_name = attr->Value();
//	else if( !strcmp(attr->Name(),"Scale") )
//	{
//		StringVector vec = StringConverter::parseStringVector(attr->Value());
//		if( vec.size() < 3 )
//			return false;
//		m_scale.x = StringConverter::parseReal(vec[0]);
//		m_scale.y = StringConverter::parseReal(vec[1]);
//		m_scale.z = StringConverter::parseReal(vec[2]);
//	};
//	return true;
//};
