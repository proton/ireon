/**
* @file xmlconfig.cpp
* Configuration options storage class
*/

/* Copyright (C) 2005 ireon.org developers council
* $Id: xmlconfig.cpp 781 2006-10-07 19:52:08Z zak $

*  See LICENSE for details
*/


#include "common/common_stdafx.h"
#include "common/xmlconfig.h"
#include "common/misc/boost_string_algo_wrapper.h"
#include <sstream>

CXMLConfig::CXMLConfig()
{
}

//-----------------------------------------------------------------------

CXMLConfig::CXMLConfig(const char* input, bool useAsDefault)
{
	load(String(input), useAsDefault, "");
}

//-----------------------------------------------------------------------

CXMLConfig::CXMLConfig(const String& input, bool useAsDefault)
{
	load(input, useAsDefault, "");
}

//-----------------------------------------------------------------------

void CXMLConfig::rmTrailingSlash(String& input)
{
	if (input.size())
	if (input[input.size()-1] == '/')
		input.erase(input.length()-1,1);
}

//-----------------------------------------------------------------------

String CXMLConfig::getFullKey(const String& key, bool defaultKey)
{
	if (defaultKey)
		return key + "_default";
	else
		return key;
}

//-----------------------------------------------------------------------

bool CXMLConfig::load(const String& input, bool useAsDefault, const String& root)
{
	String	inputString(input);
	boost::algorithm::trim(inputString);

	if (inputString.size() == 0)
		return false;

	std::stringstream stringStream;
	stringStream << input;

	TiXmlDocument	doc;
	stringStream >> doc;
	if (doc.Error())
	{
//		CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlError,__FUNCTION__, gtc_("XML parser returned an error: %s\n"), doc.ErrorDesc());
		return false;
	}
	processTinyXMLOutput(&doc, root, useAsDefault);

	for ( std::map<String, std::list<String> >::iterator it = m_children.begin(); it != m_children.end(); ++it )
		it->second.unique();
//		std::unique(it->second.begin(), it->second.end());

	return true;
}

//-----------------------------------------------------------------------

void CXMLConfig::processTinyXMLOutput(TiXmlNode* node, String path, bool useAsDefault)
{
	if ( !node ) return;

	switch ( node->Type() )
	{
	case TiXmlNode::DOCUMENT:
		rmTrailingSlash(path);
		break;

	case TiXmlNode::TEXT:
		rmTrailingSlash(path);
		appendValue(path, node->ToText()->Value(), useAsDefault);
		break;

	default:
		break;
	}

	TiXmlNode* childNode;
	String curPath = path + String(node->Value());
	for ( childNode = node->FirstChild(); childNode != 0; childNode = childNode->NextSibling() )
	{
		processTinyXMLOutput(childNode, curPath + '/', useAsDefault);
		m_children[curPath].push_back(String(childNode->Value()));
	}
}

//-----------------------------------------------------------------------

void CXMLConfig::appendValue(const String& key, const String& value, bool useAsDefault)
{
	m_settingsStorage.insert(std::multimap<String, String>::value_type(getFullKey(key, useAsDefault), value));
}

//-----------------------------------------------------------------------

void CXMLConfig::setFirstValue(const String& key, const String& value, bool useAsDefault)
{
	if( m_settingsStorage.find(getFullKey(key, useAsDefault)) != m_settingsStorage.end())
		m_settingsStorage.find(getFullKey(key, useAsDefault))->second = value;
	else
		appendValue(key, value, useAsDefault);
}

//-----------------------------------------------------------------------

String CXMLConfig::getFirstValue(const String& key)
{
	if (m_settingsStorage.find(key) == m_settingsStorage.end())
		return getFirstDefaultValue(key);
	else
		return m_settingsStorage.find(key)->second;
}

//-----------------------------------------------------------------------

String CXMLConfig::getFirstDefaultValue(const String& key)
{
	if (m_settingsStorage.find(getFullKey(key, true)) == m_settingsStorage.end())
		return "";
	else
		return m_settingsStorage.find(getFullKey(key, true))->second;
}

//-----------------------------------------------------------------------

StringVector CXMLConfig::getAllValuesForKey(const String& key)
{
	StringVector result;

	std::multimap<String, String>::const_iterator i;

	i = m_settingsStorage.find(key);
	if (i != m_settingsStorage.end())
	{
		while (i != m_settingsStorage.end() && i->first == key)
		{
			result.push_back(i->second);
			++i;
		}
		return result;
	}
	else
		return getAllDefaultValuesForKey(key);
}

//-----------------------------------------------------------------------

StringVector CXMLConfig::getAllDefaultValuesForKey(const String& key)
{
	StringVector result;

	std::multimap<String, String>::const_iterator i;

	i = m_settingsStorage.find(getFullKey(key, true));
	while (i != m_settingsStorage.end() && i->first == getFullKey(key, true))
	{
		result.push_back(i->second);
		++i;
	}
	return result;
}

//-----------------------------------------------------------------------

bool CXMLConfig::checkRequiredOptions(StringVector& requiredOptions)
{
	for(StringVector::iterator i = requiredOptions.begin(); i != requiredOptions.end(); i++ )
		if( getFirstValue(*i) == "" ) {
//			CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlCritical,__FUNCTION__, gtc_("Can't find required option '%s' in xml config.\n"),(*i).c_str());
			return false;
		}
	return true;
}

//-----------------------------------------------------------------------

std::list<String> CXMLConfig::getChildrenForKey( const String& key )
{
	return m_children[key];
}

//-----------------------------------------------------------------------

