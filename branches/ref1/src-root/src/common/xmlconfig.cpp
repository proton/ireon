/**
 * @file common/xmlconfig.cpp
 * Configuration options storage class
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: xmlconfig.cpp 635 2006-06-02 15:55:11Z zak $

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


#include "common/common_stdafx.h"
#include "common/misc/boost_string_algo_wrapper.h"
#include "common/xmlconfig.h"
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
		CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlError, _("XML parser returned an error: %s\n"), doc.ErrorDesc());
		return false;
	}
	processTinyXMLOutput(&doc, root, useAsDefault);

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
	for ( childNode = node->FirstChild(); childNode != 0; childNode = childNode->NextSibling() )
		processTinyXMLOutput(childNode, path + String(node->Value()) + '/', useAsDefault);
}

//-----------------------------------------------------------------------

void CXMLConfig::appendValue(const String& key, const String& value, bool useAsDefault)
{
	settingsStorage.insert(std::multimap<String, String>::value_type(getFullKey(key, useAsDefault), value));
}

//-----------------------------------------------------------------------

void CXMLConfig::setFirstValue(const String& key, const String& value, bool useAsDefault)
{
	if( settingsStorage.find(getFullKey(key, useAsDefault)) != settingsStorage.end())
		settingsStorage.find(getFullKey(key, useAsDefault))->second = value;
	else
		appendValue(key, value, useAsDefault);
}

//-----------------------------------------------------------------------

String CXMLConfig::getFirstValue(const String& key)
{
	if (settingsStorage.find(key) == settingsStorage.end())
		return getFirstDefaultValue(key);
	else
		return settingsStorage.find(key)->second;
}

//-----------------------------------------------------------------------

String CXMLConfig::getFirstDefaultValue(const String& key)
{
	if (settingsStorage.find(getFullKey(key, true)) == settingsStorage.end())
		return "";
	else
		return settingsStorage.find(getFullKey(key, true))->second;
}

//-----------------------------------------------------------------------

StringVector CXMLConfig::getAllValuesForKey(const String& key)
{
	StringVector result;

	std::multimap<String, String>::const_iterator i;

	i = settingsStorage.find(key);
	if (i != settingsStorage.end())
	{
		while (i != settingsStorage.end() && i->first == key)
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

	i = settingsStorage.find(getFullKey(key, true));
	while (i != settingsStorage.end() && i->first == getFullKey(key, true))
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
			CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlCritical, _("Can't find required option '%s' in xml config.\n"),(*i).c_str());
			return false;
		}
	return true;
}
