/**
 * @file xmlconfig.h
 * Configuration options storage class header.
 * Parses data in xml format and stores key-valuse pairs.
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: xmlconfig.h 433 2005-12-20 20:19:15Z zak $

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

#ifndef _XMLCONFIG_H
#define _XMLCONFIG_H

#include "stdafx.h"
#include "tinyxml/tinyxml.h"

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

	bool checkRequiredOptions(StringVector& requiredOptions);

private:
	std::multimap<String, String> settingsStorage;

	void rmTrailingSlash(String& input);
	void processTinyXMLOutput(TiXmlNode* node, String path, bool useAsDefault);
	String getFullKey(const String& key, bool defaultKey);
};

#endif
