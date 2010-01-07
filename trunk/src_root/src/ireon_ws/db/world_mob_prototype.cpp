/**
 * @file world_mob_prototype.cpp
 * World mob's prototype
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: world_mob_prototype.cpp 752 2006-09-23 19:12:44Z zak $
 
 *  See LICENSE for details
 */

#include "ireon_ws/stdafx.h"
#include "ireon_ws/db/world_mob_prototype.h"
#include "common/resource/resource_manager.h"
#include "tinyxml/tinyxml.h"
#include "common/data.h"

CWorldMobPrototype::CWorldMobPrototype():
m_velocity(5)
{
};

CWorldMobPrototype::~CWorldMobPrototype()
{
};

bool CWorldMobPrototype::load(const String& type)
{
	m_type = type; 
	CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlInfo,__FUNCTION__,"Loading mob prototype from '%s'.\n",type.c_str());
	DataPtr dPtr = CResourceManager::instance()->load(type + ".mobprot");
	if( !dPtr )
	{
		CLog::instance()->log(CLog::msgLvlError,__FUNCTION__,"Error loading mob prototype: resource not found.\n",type.c_str());
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
		CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlError,__FUNCTION__, _("Error loading mob prototype: resource isn't mob prototype. %s\n"),root->Value());
		return false;
	};

	TiXmlNode* option;
	for( option = root->FirstChild(); option; option = option->NextSibling() )
		if( !processOption(option) )
		{
			CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlError,__FUNCTION__, _("Error loading mob prototype: error in file near '%s'.\n"),option->Value());
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
	else if( !strcmp(attr->Name(),"MaxHP") )
		m_maxHP = StringConverter::parseUnsignedInt(attr->Value());
	else if( !strcmp(attr->Name(),"MaxMana") )
		m_maxMana = StringConverter::parseUnsignedInt(attr->Value());
	else if( !strcmp(attr->Name(),"Str") )
		m_str = StringConverter::parseUnsignedInt(attr->Value());
	else if( !strcmp(attr->Name(),"Sta") )
		m_sta = StringConverter::parseUnsignedInt(attr->Value());
	else if( !strcmp(attr->Name(),"Int") )
		m_int = StringConverter::parseUnsignedInt(attr->Value());
	else if( !strcmp(attr->Name(),"Wis") )
		m_wis = StringConverter::parseUnsignedInt(attr->Value());
	else if( !strcmp(attr->Name(),"Con") )
		m_con = StringConverter::parseUnsignedInt(attr->Value());
	else if( !strcmp(attr->Name(),"Luck") )
		m_luck = StringConverter::parseUnsignedInt(attr->Value());
	else if( !strcmp(attr->Name(),"Velocity") )
		m_velocity = StringConverter::parseReal(attr->Value());
	else if( !strcmp(attr->Name(),"FightSpeed") )
		m_fightSpeed = StringConverter::parseUnsignedInt(attr->Value());
	return true;
};
