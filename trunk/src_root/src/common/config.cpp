/**
* @file config.cpp
* Configuration options storage class
*/

/* Copyright (C) 2005 ireon.org developers council
* $Id: config.cpp 785 2006-10-25 15:48:43Z dox $

*  See LICENSE for details
*/


#include "common/common_stdafx.h"
#include "common/config.h"
#include "common/file/file.h"
#include "common/data.h"
#include "common/misc/boost_string_algo_wrapper.h"

CConfig::CConfig()
{
	// we add sections in setSetting, so we shouldn't need this
	// mSettings[BLANK_STRING] = new SettingsMultiMap();
}

//-----------------------------------------------------------------------

CConfig::~CConfig()
{
   SettingsBySection::iterator seci, secend;
   secend = mSettings.end();
   for (seci = mSettings.begin(); seci != secend; ++seci)
   {
       delete seci->second;
   }
}

//-----------------------------------------------------------------------

void CConfig::clear(void)
{
   for (SettingsBySection::iterator seci = mSettings.begin(); 
       seci != mSettings.end(); ++seci)
   {
       delete seci->second;
   }
   mSettings.clear();
}

//-----------------------------------------------------------------------

void CConfig::load(const String& filename, const String& separators)
{
	/* Open the configuration file */
	FilePtr f(new CFile);
	if( !f )
		return;
	f->open(filename.c_str(),"r");
	if( !f->isOpen() )
		return;
	CData d(f.get());
	load(d,separators);
}

//-----------------------------------------------------------------------
void CConfig::load(CData& d, const String& separators)
{
	assert( separators.length() > 0 );
	String currentSection = BLANK_STRING;
	SettingsMultiMap* currentSettings = addSection(currentSection);
/*
	if( mSettings.find(currentSection) == mSettings.end() )
	{
		currentSettings = new SettingsMultiMap();
		mSettings[currentSection] = currentSettings;
	} else
		currentSettings = mSettings[currentSection];
*/


	/* Process the file line for line */
	String line, optName, optVal;
	while (!d.end())
	{
		d.getS(2048,line);
		boost::algorithm::trim(line);
		/* Ignore comments & blanks */
		if (line.length() > 0 && line.at(0) != '#' && line.at(0) != '@')
		{
			if (line.at(0) == '[' && line.at(line.length()-1) == ']')
			{
				// Section
				currentSection = line.substr(1, line.length() - 2);
				boost::algorithm::trim(currentSection);
				currentSettings = addSection(currentSection);
				SettingsBySection::const_iterator seci = mSettings.find(currentSection);
				if (seci == mSettings.end())
				{
					currentSettings = new SettingsMultiMap();
					mSettings[currentSection] = currentSettings;
				}
				else
				{
					currentSettings = seci->second;
				} 

			}
			else
			{
	            /* Find the first seperator character and split the string there */
				uint i = 0;
				uint separator_pos = 0;
				do {
					separator_pos = line.find(separators.at(i), 0);
					++i;
				} while ((i < separators.length()) && (separator_pos == std::string::npos));

				if (separator_pos != std::string::npos)
				{
					optName = line.substr(0, separator_pos);
					/* Find the first non-seperator character following the name */
					// int nonseparator_pos = line.find_first_not_of(separators, separator_pos);
					/* ... and extract the value */
					/* Make sure we don't crash on an empty setting (it might be a valid value) */
					// optVal = (nonseparator_pos == std::string::npos) ? "" : line.substr(nonseparator_pos);
					optVal = line.substr(separator_pos+1);
					boost::algorithm::trim(optVal);
					boost::algorithm::trim(optName);
					//setSetting(optName, optVal, currentSection);
					currentSettings->insert(std::multimap<String, String>::value_type(optName, optVal));
				}
			}
		}
	}
}

//-----------------------------------------------------------------------

CConfig::SettingsMultiMap* CConfig::addSection(const String& sectionName)
{
	SettingsMultiMap* currentSettings;

	SettingsBySection::const_iterator seci = mSettings.find(sectionName);
	if (seci == mSettings.end())
	{
		currentSettings = new SettingsMultiMap();
		mSettings[sectionName] = currentSettings;
	}
	else
	{
		currentSettings = seci->second;
	} 
	return currentSettings;
}

//-----------------------------------------------------------------------

void CConfig::setSetting(const String& key, const String& value, const String& sect)
{
	addSection(sect);
	if( mSettings.find(sect) == mSettings.end() ) {
		assert(1); // hey, we added section, this can't be true:)
		return;
	}
	if( mSettings[sect]->find(key) != mSettings[sect]->end())
		(*mSettings[sect]->find(key)).second = value;
	else
		mSettings[sect]->insert(std::multimap<String, String>::value_type(key, value));
};

//-----------------------------------------------------------------------

String CConfig::getSetting(const String& key, const String& section) const
{
       
	SettingsBySection::const_iterator seci = mSettings.find(section);
	if (seci == mSettings.end())
	{
		return BLANK_STRING;
	}
	else
	{
		SettingsMultiMap::const_iterator i = seci->second->find(key);
		if (i == seci->second->end())
		{
			return BLANK_STRING;
		}
		else
		{
			return i->second;
		}
	}
}

//-----------------------------------------------------------------------

StringVector CConfig::getMultiSetting(const String& key, const String& section) const
{
	StringVector ret;

	SettingsBySection::const_iterator seci = mSettings.find(section);
	if (seci != mSettings.end())
	{
		SettingsMultiMap::const_iterator i;

		i = seci->second->find(key);
		// Iterate over matches
		while (i != seci->second->end() && i->first == key)
		{
			ret.push_back(i->second);
			++i;
		}
	}
	return ret;
}

//-----------------------------------------------------------------------

bool CConfig::checkRequiredOptions(std::vector<const char*>* requiredOptions)
{
	for( std::vector<const char*>::iterator i = requiredOptions->begin(); i != requiredOptions->end(); i++ )
		if( getSetting(*i) == "" ) {
			_log(_fatal,_loc, gtc_("Can't find required option '%s' in config file.\n"),*i);
			return false;
		}
	return true;
}
