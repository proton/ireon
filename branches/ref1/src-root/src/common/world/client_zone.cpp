/**
 * @file client_zone.cpp
 * Client-side zone class
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

#include "world/client_zone.h"
#include "world/client_static.h"
#include "resource/resource_manager.h"

CClientZone::CClientZone()
{
};

CClientZone::~CClientZone()
{
};

bool CClientZone::load(const String& resourceName)
{
	CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlInfo,"Loading zone from '%s'.\n",resourceName.c_str());
	DataPtr dPtr = CResourceManager::instance()->load(resourceName);
	if( !dPtr )
	{
		CLog::instance()->log(CLog::msgLvlError,"Error loading zone: resource not found.\n",resourceName.c_str());
		return false;
	}

	TiXmlDocument doc;
	std::stringstream buf;
	buf.write(dPtr->data(),dPtr->size());
	buf >> doc;

	if (doc.Error())
	{
		CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlError, _("Error loading zone: XML parser returned an error: %s\n"), doc.ErrorDesc());
		return false;
	}

	TiXmlNode* root =  doc.FirstChild();
	while( root && root->Type() == TiXmlNode::DECLARATION )
		root = root->NextSibling();
	if( !root || root->Type() != TiXmlNode::ELEMENT || strcmp(root->ToElement()->Value(),"Zone") )
	{
		CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlError, _("Error loading zone: resource isn't zone. %s\n"),root->Value());
		return false;
	};

	TiXmlNode* option;
	for( option = root->FirstChild(); option; option = option->NextSibling() )
		if( !processOption(option) )
		{
			CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlError, _("Error loading zone: error in file near '%s'.\n"),option->Value());
			return false;
		};
	return true;
};

bool CClientZone::processOption(TiXmlNode* option)
{
	if( !option )
		return false;

	if( !option->Type() == TiXmlNode::ELEMENT )
		return true;
	const char* name = option->Value();
	if( !strcmp(name,"Static") )
	{
		if( !processStatic(option) )
			return false;
	}
	else
	{
		return false;
	}
	return true;
};

bool CClientZone::processStatic(TiXmlNode *node)
{
	TiXmlAttribute* attr = node->ToElement()->FirstAttribute();
	StaticPtr st;
	while( attr )
	{
		if( !attr || !attr->Name() || !attr->Value() )
			return false;
		if( !st )
		{
			if( !strcmp(attr->Name(),"Name") )
			{
				StaticPrototypePtr prot = I_WORLD->getStaticPrototype(attr->Value());
				if( prot )
					st.reset( new CClientStaticObject(prot) );
			} else if( !strcmp(attr->Name(),"Id") )
			{
				StaticPrototypePtr prot = I_WORLD->getStaticPrototype(StringConverter::parseLong(attr->Value()));
				if( prot )
					st.reset( new CClientStaticObject(prot) );
			}
		} else
		{
			if( !strcmp(attr->Name(),"Position") )
			{
				StringVector vec = StringConverter::parseStringVector(attr->Value());
				if( vec.size() < 3 )
					return false;
				Vector3 pos;
				pos.x = StringConverter::parseReal(vec[0]);
				pos.y = StringConverter::parseReal(vec[1]);
				pos.z = StringConverter::parseReal(vec[2]);
				st->setPosition(pos);
			} else if( !strcmp(attr->Name(),"Rotation") )
			{
				Radian rot;
				rot = Radian(StringConverter::parseReal(attr->Value()));
				st->setRotation(rot);
			}
			
		}
		attr = attr->Next();
	}
	if( st) 
	{
		m_statics.push_back(st);
		I_WORLD->addStatic(st);
	}
	return true;
};

void CClientZone::unload()
{
	for( std::vector<StaticPtr>::iterator it = m_statics.begin(); it != m_statics.end(); ++it )
		I_WORLD->removeStatic(*it);
};