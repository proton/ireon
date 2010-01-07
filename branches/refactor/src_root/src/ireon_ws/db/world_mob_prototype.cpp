/**
 * @file world_mob_prototype.cpp
 * World mob's prototype
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: world_mob_prototype.cpp 433 2005-12-20 20:19:15Z zak $
 
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
#include "db/world_mob_prototype.h"
#include "resource/resource_manager.h"
#include "tinyxml/tinyxml.h"

CWorldMobPrototype::CWorldMobPrototype():
m_id(0),
m_velocity(5)
{
};

CWorldMobPrototype::~CWorldMobPrototype()
{
};

bool CWorldMobPrototype::load(const String& resourceName)
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

bool CWorldMobPrototype::processOption(TiXmlNode* option)
{
	if( !option )
		return false;

	if( !option->Type() == TiXmlNode::ELEMENT )
		return false;
	TiXmlAttribute* attr = option->ToElement()->FirstAttribute();
	if( !attr || !attr->Name() || !attr->Value() )
		return false;
	else if( !strcmp(attr->Name(),"Name") )
		m_name = attr->Value();
	else if( !strcmp(attr->Name(),"Id") )
		m_id = StringConverter::parseLong(attr->Value());
	else if( !strcmp(attr->Name(),"Velocity") )
		m_velocity = StringConverter::parseReal(attr->Value());
	else if( !strcmp(attr->Name(),"FightSpeed") )
		m_fightSpeed = StringConverter::parseUnsignedInt(attr->Value());

	if( m_id == 0 )
		return false;
	return true;
};
