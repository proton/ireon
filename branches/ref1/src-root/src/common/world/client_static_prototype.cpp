/**
 * @file client_static_prototype.cpp
 * Prototype of static object
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id$

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "stdafx.h"
#include "resource/resource_manager.h"
#include "world/client_static_prototype.h"
#include "tinyxml/tinyxml.h"

CClientStaticPrototype::CClientStaticPrototype():
m_id(0),
m_scale(Vector3::ZERO),
m_size(Vector3::ZERO)
{
};

CClientStaticPrototype::~CClientStaticPrototype()
{
};

bool CClientStaticPrototype::load(const String& resourceName)
{
	CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlInfo,"Loading static prototype from '%s'.\n",resourceName.c_str());
	DataPtr dPtr = CResourceManager::instance()->load(resourceName);
	if( !dPtr )
	{
		CLog::instance()->log(CLog::msgLvlError,"Error loading static prototype: resource not found.\n",resourceName.c_str());
		return false;
	}

	TiXmlDocument doc;
	std::stringstream buf;
	buf.write(dPtr->data(),dPtr->size());
	buf >> doc;

	if (doc.Error())
	{
		CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlError, _("Error loading static prototype: XML parser returned an error: %s\n"), doc.ErrorDesc());
		return false;
	}

	TiXmlNode* root =  doc.FirstChild();
	while( root && root->Type() == TiXmlNode::DECLARATION )
		root = root->NextSibling();
	if( !root || root->Type() != TiXmlNode::ELEMENT || strcmp(root->ToElement()->Value(),"ClientStaticPrototype") )
	{
		CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlError, _("Error loading static prototype: resource isn't static prototype. %s\n"),root->Value());
		return false;
	};

	TiXmlNode* node;
	for( node = root->FirstChild(); node; node = node->NextSibling() )
	{
		if( node->ValueStr() == "Option" )
		{
			if( !processOption(node) )
			{
				CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlError, _("Error loading static prototype: error in file near '%s'.\n"),node->Value());
				return false;
			}
		} else if( node->ValueStr() == "Segment" )
		{
			if( !processSegment(node) )
			{
				CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlError, _("Error loading static prototype: error in file near '%s'.\n"),node->Value());
				return false;
			}
		}
	}

	if( m_id == 0 || m_mesh == BLANK_STRING)
		return false;

	return true;
};

bool CClientStaticPrototype::processOption(TiXmlNode* option)
{
	if( !option )
		return false;

	if( !option->Type() == TiXmlNode::ELEMENT )
		return false;
	TiXmlAttribute* attr = option->ToElement()->FirstAttribute();
	if( !attr || !attr->Name() || !attr->Value() )
		return false;
	if( !strcmp(attr->Name(),"Mesh") )
		m_mesh = attr->Value();
	else if( !strcmp(attr->Name(),"Name") )
		m_name = attr->Value();
	else if( !strcmp(attr->Name(),"Id") )
		m_id = StringConverter::parseLong(attr->Value());
	else if( !strcmp(attr->Name(),"Scale") )
	{
		StringVector vec = StringConverter::parseStringVector(attr->Value());
		if( vec.size() < 3 )
			return false;
		m_scale.x = StringConverter::parseReal(vec[0]);
		m_scale.y = StringConverter::parseReal(vec[1]);
		m_scale.z = StringConverter::parseReal(vec[2]);
	} else if( !strcmp(attr->Name(),"Size") )
	{
		StringVector vec = StringConverter::parseStringVector(attr->Value());
		if( vec.size() < 3 )
			return false;
		m_size.x = StringConverter::parseReal(vec[0]);
		m_size.y = StringConverter::parseReal(vec[1]);
		m_size.z = StringConverter::parseReal(vec[2]);
	}

	return true;
};

bool CClientStaticPrototype::processSegment(TiXmlNode* node)
{
	TiXmlAttribute* attr = node->ToElement()->FirstAttribute();
	CSegment seg(Vector2::ZERO,Vector2::ZERO);
	while( attr )
	{
		if( !strcmp(attr->Name(),"p1") )
		{
			StringVector vec = StringConverter::parseStringVector(attr->Value());
			if( vec.size() < 2 )
				return false;
			Vector2 p;
			p.x = StringConverter::parseReal(vec[0]);
			p.y = StringConverter::parseReal(vec[1]);
			seg.setP1(p);
		} else if( !strcmp(attr->Name(),"p2") )
		{
			StringVector vec = StringConverter::parseStringVector(attr->Value());
			if( vec.size() < 2 )
				return false;
			Vector2 p;
			p.x = StringConverter::parseReal(vec[0]);
			p.y = StringConverter::parseReal(vec[1]);
			seg.setP2(p);
		} else
			return false;

		attr = attr->Next();
	}
	m_segments.push_back(seg);
	return true;
};