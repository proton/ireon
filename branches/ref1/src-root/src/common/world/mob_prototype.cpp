/**
 * @file mob_prototype.cpp
 * Mob's prototype
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: mob_prototype.cpp 510 2006-02-26 21:09:40Z zak $
 
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
#include "world/mob_prototype.h"
#include "resource/resource_manager.h"
#include "tinyxml/tinyxml.h"

CMobPrototype::CMobPrototype():
m_id(0),
m_scale(Vector3::UNIT_SCALE)
{
};

CMobPrototype::~CMobPrototype()
{
};

bool CMobPrototype::load(const String& resourceName)
{
	CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlInfo,"Loading mob prototype from '%s'.\n",resourceName.c_str());
	DataPtr dPtr = CResourceManager::instance()->load(resourceName);
	if( !dPtr )
	{
		CLog::instance()->log(CLog::msgLvlError,"Error loading mob prototype: resource not found.\n",resourceName.c_str());
		return false;
	}

	TiXmlDocument doc;
	std::stringstream buf;
	buf.write(dPtr->data(),dPtr->size());
	buf >> doc;

	if (doc.Error())
	{
		CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlError, _("Error loading mob prototype: XML parser returned an error: %s\n"), doc.ErrorDesc());
		return false;
	}

	TiXmlNode* root =  doc.FirstChild();
	while( root && root->Type() == TiXmlNode::DECLARATION )
		root = root->NextSibling();
	if( !root || root->Type() != TiXmlNode::ELEMENT || strcmp(root->ToElement()->Value(),"MobPrototype") )
	{
		CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlError, _("Error loading mob prototype: resource isn't mob prototype. %s\n"),root->Value());
		return false;
	};

	TiXmlNode* option;
	for( option = root->FirstChild(); option; option = option->NextSibling() )
		if( !processOption(option) )
		{
			CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlError, _("Error loading mob prototype: error in file near '%s'.\n"),option->Value());
			return false;
		};
	return true;
};

bool CMobPrototype::processOption(TiXmlNode* option)
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
	};
	if( m_id == 0 )
		return false;
	return true;
};
