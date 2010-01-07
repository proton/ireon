/**
* @file object_factory.cpp
* Factory creating object
*/

/* Copyright (C) 2005 ireon.org developers council
* $Id$

*  See LICENSE for details
*/
#include "stdafx.h"
#include "common/world/managers/object_factory.h"

CObjectFactory* CObjectFactory::singleton_ = NULL;

CObjectFactory::CObjectFactory():
CLogMixin("ObjectFactory")
{
}

CObjectFactory* CObjectFactory::getSingletonPtr()
{
	if (!singleton_)
		singleton_ = new CObjectFactory;
	return singleton_;
}

bool CObjectFactory::registerProduct(const String& productId, productCallbackPtr productCallback )
{
	_log(_info,_loc, gtc_("Registering object type '%s'..."), productId.c_str() );
	return productCallbackList_.insert(
		std::map<String, productCallbackPtr, CmpStringLess>::value_type(productId, productCallback)).second;  
};

bool CObjectFactory::unregisterProduct(const String& productId)
{
	_log(_info,_loc, gtc_("UnRegistering object type '%s'..."), productId.c_str() );
	return productCallbackList_.erase(productId) == 1;
};

CObject* CObjectFactory::createProduct(const String& productId, CPrototype* proto)
{
	std::map<String, productCallbackPtr, CmpStringLess>::const_iterator it = productCallbackList_.find(productId);
	if ( it == productCallbackList_.end() )
	{
		_log(_error, _loc, gtc_("Attempt create unknown object type '%s' !!!"), productId.c_str() );
		throw EProductIdNotFound();
	};
	_log(_info,_loc, gtc_("Create product '%s'..."), productId.c_str() );
	try
	{
		return (it->second)(proto);
	}
	catch (EProductCallbackError)
	{
		_log(_error,_loc, gtc_("For product '%s' accured callback error ..."), productId.c_str() );
		throw EProductCallbackError();
	}
};


