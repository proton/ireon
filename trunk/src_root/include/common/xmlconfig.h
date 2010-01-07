/**
 * @file xmlconfig.h
 * Configuration options storage class header.
 * Parses data in xml format and stores key-valuse pairs.
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: xmlconfig.h 778 2006-10-05 20:15:59Z zak $

 *  See LICENSE for details
 */

#ifndef _XMLCONFIG_H
#define _XMLCONFIG_H

#include "common/common_stdafx.h"
#include "tinyxml/tinyxml.h"
#include <map>
#include <list>

/**
 * Configuration options storage class.
 * Parses data in xml format and stores key-valuse pairs.
 */
class CXMLConfig
{
public:
	CXMLConfig();
	CXMLConfig(const char* input, bool useAsDefault = false);
	CXMLConfig(const String& input, bool useAsDefault = false);

	bool load(const String& input, bool useAsDefault = false, const String& root = "");

	void setFirstValue(const String& key, const String& value, bool useAsDefault = false);
	String getFirstValue(const String& key);
	String getFirstDefaultValue(const String& key);

	void appendValue(const String& key, const String& value, bool useAsDefault = false);
	StringVector getAllValuesForKey(const String& key);
	StringVector getAllDefaultValuesForKey(const String& key);

	// returns list of direct children names (without path, duplicates are removed)
	std::list<String> getChildrenForKey(const String& key);

	bool checkRequiredOptions(StringVector& requiredOptions);

private:
	std::multimap<String, String> m_settingsStorage;
	std::map<String, std::list<String> > m_children;

	void rmTrailingSlash(String& input);
	void processTinyXMLOutput(TiXmlNode* node, String path, bool useAsDefault);
	String getFullKey(const String& key, bool defaultKey);
};

#endif
