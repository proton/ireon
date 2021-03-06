/**
 * @file config.h
 * Configuration options storage class header.
 * Parses and stores key-valuse pairs.
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: config.h 785 2006-10-25 15:48:43Z dox $

 *  See LICENSE for details
 */


#ifndef _CONFIG_H
#define _CONFIG_H

class CData;
#include <map>
#include "stdafx.h"
/**
 * Configuration options storage class.
 * Parses and stores key-valuse pairs.
 */
class CConfig: protected CLogMixin
{
public:
	CConfig();
	virtual ~CConfig();


	typedef std::multimap<String, String> SettingsMultiMap;
	/** Gets an iterator for stepping through all the keys / values in the file. */
	typedef std::map<String, SettingsMultiMap*> SettingsBySection;
	typedef std::map<String, SettingsMultiMap*>::iterator SectionIterator;


	/// use this routine to load data info config
	void load(CData& data, const String& separators = "=:\t");

	/// load from a filename (not using resource group locations)
	/// \deprecated to be removed
	void load(const String& filename, const String& separators = "=:\t");

	/** Gets the first setting from the file with the named key. 
		@param key The name of the setting
		@param section The name of the section it must be in (if any)
	*/
	String getSetting(const String& key, const String& section = BLANK_STRING) const;
	/** Gets all settings from the file with the named key. */
	/// \note not covered by unit tests
	StringVector getMultiSetting(const String& key, const String& section = BLANK_STRING) const;

	/// Creates key=value pair in sect section.
	/// No transformations performed on parameters.
	void setSetting(const String& key, const String& value, const String& sect = BLANK_STRING);
        
	/// \note not covered by unit tests
	SectionIterator* getFirstSection() {m_it = mSettings.begin(); if( m_it == mSettings.end() ) return NULL; return &m_it;}
	/// \note not covered by unit tests
	SectionIterator* getNextSection() {m_it++; if(m_it == mSettings.end() ) return NULL; return &m_it;}
	
	/// check that all required options are present
	bool checkRequiredOptions(std::vector<const char*> *requiredOptions);

	/** Clear the settings */
	void clear(void);

protected:
	SettingsBySection mSettings;

	SectionIterator m_it;

private:
	/// Creates new section and returns pointer or just returns pointer if section exists.
	/// No transformations performed on sectionName.
	SettingsMultiMap* addSection(const String& sectionName);

};

#endif
