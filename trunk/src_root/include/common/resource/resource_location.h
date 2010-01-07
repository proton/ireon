/* Copyright (C) 2005 ireon.org developers council
 * $Id: resource_location.h 672 2006-08-16 18:33:53Z zak $

 *  See LICENSE for details
 */

/**
 * @file resource_location.h
 * Abstract class, interface for working with resource
 * Resource location
 */
#ifndef _RESOURCE_LOCATION_H
#define _RESOURCE_LOCATION_H

#include "boost/shared_ptr.hpp"
class CData;
typedef boost::shared_ptr<CData> DataPtr;

class CResourceLocation
{
public:
	CResourceLocation(const String& name): m_name(name) {}
	virtual ~CResourceLocation() {}

	/// Open the location
	virtual void open() = 0;

	/// Close the location
	virtual void close() = 0;

	/// Get list of entities
	virtual void list(StringVector* res) = 0;

	/// Load resource
	virtual DataPtr load(const String& name) = 0;
protected:

	String m_name;
};

#endif
