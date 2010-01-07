/**
* @file client_static.cpp
* Static object
*/

/* Copyright (C) 2005 ireon.org developers council
* $Id: client_static.cpp 829 2006-11-30 18:39:37Z mip $

*  See LICENSE for details
*/
#include "stdafx.h"
#include "common/world/generic_object/prototype.h"

CPrototype::CPrototype():
scale_(Vector3::ZERO),
size_(Vector3::UNIT_SCALE),
name_(""),
meshName_("")
//m_scale(Vector3::UNIT_SCALE)
{
};

CPrototype::~CPrototype()
{
};

bool CPrototype::load(const String& )
{
	//_log(_info,_loc,"Loading static prototype from '%s'.\n",resourceName.c_str());
	//DataPtr dPtr = CResourceManager::instance()->load(resourceName);
	//if( !dPtr )
	//{
	//	_log(_error,_loc,"Error loading static prototype: resource not found.\n",resourceName.c_str());
	//	return false;
	//}

	//TiXmlDocument doc;
	//std::stringstream buf;
	//buf.write(dPtr->data(),dPtr->size());
	//buf >> doc;

	//if (doc.Error())
	//{
	//	_log(_error,_loc, _("Error loading static prototype: XML parser returned an error: %s\n"), doc.ErrorDesc());
	//	return false;
	//}

	//TiXmlNode* root =  doc.FirstChild();
	//while( root && root->Type() == TiXmlNode::DECLARATION )
	//	root = root->NextSibling();
	//if( !root || root->Type() != TiXmlNode::ELEMENT || strcmp(root->ToElement()->Value(),"ClientStaticPrototype") )
	//{
	//	_log(_error,_loc, _("Error loading static prototype: resource isn't static prototype. %s\n"),root->Value());
	//	return false;
	//};

	//TiXmlNode* node;
	//for( node = root->FirstChild(); node; node = node->NextSibling() )
	//{
	//	if( node->ValueStr() == "Option" )
	//	{
	//		if( !processOption(node) )
	//		{
	//			_log(_error,_loc, _("Error loading static prototype: error in file near '%s'.\n"),node->Value());
	//			return false;
	//		}
	//	} else if( node->ValueStr() == "Segment" )
	//	{
	//		if( !processSegment(node) )
	//		{
	//			_log(_error,_loc, _("Error loading static prototype: error in file near '%s'.\n"),node->Value());
	//			return false;
	//		}
	//	}
	//}

	//if( m_name == BLANK_STRING || m_mesh == BLANK_STRING)
	//	return false;

	return true;
};
//
//bool CPrototype::processOption(TiXmlNode* option)
//{
//	//if( !option )
//	//	return false;
//
//	//if( !option->Type() == TiXmlNode::ELEMENT )
//	//	return false;
//	//TiXmlAttribute* attr = option->ToElement()->FirstAttribute();
//	//if( !attr || !attr->Name() || !attr->Value() )
//	//	return false;
//	//if( !strcmp(attr->Name(),"Mesh") )
//	//	m_mesh = attr->Value();
//	//else if( !strcmp(attr->Name(),"Name") )
//	//	m_name = attr->Value();
//	///*	 else if( !strcmp(attr->Name(),"Id") )
//	//m_id = StringConverter::parseLong(attr->Value());  //don't need... not used manual id*/
//	//else if( !strcmp(attr->Name(),"Scale") )
//	//{
//	//	StringVector vec = StringConverter::parseStringVector(attr->Value());
//	//	if( vec.size() < 3 )
//	//		return false;
//	//	m_scale.x = StringConverter::parseReal(vec[0]);
//	//	m_scale.y = StringConverter::parseReal(vec[1]);
//	//	m_scale.z = StringConverter::parseReal(vec[2]);
//	//} else if( !strcmp(attr->Name(),"Size") )
//	//{
//	//	StringVector vec = StringConverter::parseStringVector(attr->Value());
//	//	if( vec.size() < 3 )
//	//		return false;
//	//	m_size.x = StringConverter::parseReal(vec[0]);
//	//	m_size.y = StringConverter::parseReal(vec[1]);
//	//	m_size.z = StringConverter::parseReal(vec[2]);
//	//}
//
//	return true;
//};
//
//bool CPrototype::processSegment(TiXmlNode* node)
//{
//	//TiXmlAttribute* attr = node->ToElement()->FirstAttribute();
//	//CSegment seg(Vector2::ZERO,Vector2::ZERO);
//	//while( attr )
//	//{
//	//	if( !strcmp(attr->Name(),"p1") )
//	//	{
//	//		StringVector vec = StringConverter::parseStringVector(attr->Value());
//	//		if( vec.size() < 2 )
//	//			return false;
//	//		Vector2 p;
//	//		p.x = StringConverter::parseReal(vec[0]);
//	//		p.y = StringConverter::parseReal(vec[1]);
//	//		seg.setP1(p);
//	//	} else if( !strcmp(attr->Name(),"p2") )
//	//	{
//	//		StringVector vec = StringConverter::parseStringVector(attr->Value());
//	//		if( vec.size() < 2 )
//	//			return false;
//	//		Vector2 p;
//	//		p.x = StringConverter::parseReal(vec[0]);
//	//		p.y = StringConverter::parseReal(vec[1]);
//	//		seg.setP2(p);
//	//	} else
//	//		return false;
//
//	//	attr = attr->Next();
//	//}
//	//m_segments.push_back(seg);
//	return true;
//};