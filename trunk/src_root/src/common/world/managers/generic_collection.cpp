/**
* @file object_manager.cpp
* Editor world manager
*/

/* Copyright (C) 2005 ireon.org developers council
* $Id: client_static_collection.cpp 715 2006-09-10 20:12:50Z mip $

*  See LICENSE for details
*/
#include "stdafx.h"
#include "common/world/managers/generic_collection.h"
const uint MAX_TRY_RENAME_OBJECT = 10;

template<class ObjectCollectionType, class ObjectType>
ObjectCollectionType* 
CGenericCollection<ObjectCollectionType, ObjectType>::singleton_ = NULL;

template<class ObjectCollectionType, class ObjectType>
ObjectCollectionType*
CGenericCollection<ObjectCollectionType, ObjectType>::getSingletonPtr()
{
	if( !singleton_ )
		singleton_ = new ObjectCollectionType();
	return singleton_;
};

template<class ObjectCollectionType, class ObjectType>
typename CGenericCollection<ObjectCollectionType, ObjectType>::iterator
CGenericCollection<ObjectCollectionType, ObjectType>::begin()
{
	return (!objectMap_.empty())? 
		iterator( (*objectMap_.begin()).first ,(*objectMap_.begin()).second) :
	iterator(0, NULL);
}

template<class ObjectCollectionType, class ObjectType>
typename  CGenericCollection<ObjectCollectionType, ObjectType>::iterator
CGenericCollection<ObjectCollectionType, ObjectType>::end()
{
	return iterator( 0 , NULL) ;
}

template<class ObjectCollectionType, class ObjectType>
bool CGenericCollection<ObjectCollectionType, ObjectType>::empty()
{
	return objectMap_.empty();
}

template<class ObjectCollectionType, class ObjectType>
typename CGenericCollection<ObjectCollectionType, ObjectType>::iterator
CGenericCollection<ObjectCollectionType, ObjectType>::find(uint id)
{
	std::map<uint,ObjectType*>::iterator it = objectMap_.find(id);

	if (it == objectMap_.end())
	{
		_log(_debug,_loc,gtc_("Don't found object with id = '%u'"),id);
		return iterator(0, NULL);
	}
	return iterator(id, (*it).second);
}

template<class ObjectCollectionType, class ObjectType>
typename CGenericCollection<ObjectCollectionType, ObjectType>::iterator
CGenericCollection<ObjectCollectionType, ObjectType>::find(Ogre::String& name)
{
	// TODO: refactor this
	uint id = 0;
	bool finded = false;
	try
	{
		std::map<uint,ObjectType*>::iterator it;
		std::map<uint,ObjectType*>::iterator itBegin = objectMap_.begin();
		std::map<uint,ObjectType*>::iterator itEnd = objectMap_.end();
		for (it = itBegin; it != itEnd; it++)
		{
			if ( Ogre::String((*it).second->getName()).compare(name) == 0)
			{
				id = (*it).first;
				finded = true;
				break;
			}
		}
		if (!finded)
			throw ENotFoundIdGenericCollectionError();
	}
	catch ( ENotFoundIdGenericCollectionError& )
	{
		_log(_debug, _loc, gtc_("Don't found object with name = '%s'"), name.c_str() );
		return end();
	}
	return find(id);
}



//---------------------------------------------------------
template<class ObjectCollectionType, class ObjectType>
bool CGenericCollection<ObjectCollectionType, ObjectType>::init()
{
	// first free id - this 0
	freeId_.push_back(1);
	return true;
};

template<class ObjectCollectionType, class ObjectType>
uint CGenericCollection<ObjectCollectionType, ObjectType>::getFirstFreeObjectId()
{
	//assert(!freeId_.empty());
	uint firstFreeId = (uint)freeId_.back();
	freeId_.pop_back();
	// push next id
	if (freeId_.empty())
		freeId_.push_back(++firstFreeId);

	return firstFreeId;
};

template<class ObjectCollectionType, class ObjectType>
uint CGenericCollection<ObjectCollectionType, ObjectType>::addObject(ObjectType* object)
{
	// TODO: check and repair

	//assert(object);
	
	//skip check on duplicate, return id if object in memory
	iterator it = find( Ogre::String(object->getName()));
	if (it != end())
		return it.id();



	uint numberIteration = 0;

	String oldName = object->getName();
	//TODO: try rename if exist object , 
	// check on duplicate PTR.  
	
	while  ( find( Ogre::String(object->getName()) ) != end() )
	{
		object->setName( Ogre::String(object->getName()).append("Ex").c_str() );
		numberIteration++;
		if (numberIteration >= MAX_TRY_RENAME_OBJECT)
		{
			_log(_error,_loc,gtc_("Try change name for duplicate object '%s' >= 10:"),oldName.c_str());
			_log(_error,_loc,gtc_("Check for trying add duplicate PTR on object"));
			object->setName(oldName.c_str());
			return 0;
		}
	}

	uint id = getFirstFreeObjectId();
	_log(_debug,_loc,gtc_("Add object: id = '%u' name = '%s'\n"), id, object->getName() );
	objectMap_.insert(std::map< uint, ObjectType* >::value_type( id, object));
	return id;
};

template<class ObjectCollectionType, class ObjectType>
bool CGenericCollection<ObjectCollectionType, ObjectType>::deleteObject(uint id)
{
	std::map< uint, ObjectType* >::iterator it = objectMap_.find( id );
	if (it == objectMap_.end())
		return false;
	_log(_debug,_loc,gtc_("Delete object: id = '%u' name = '%s'\n"),(*it).first, (*it).second->getName());

	// freed id for next objects
	freeId_.push_back((*it).first);
	// we ownershiped ptr ? 
	delete((*it).second);
	objectMap_.erase(it);
	return true;
};

template<class ObjectCollectionType, class ObjectType>
bool CGenericCollection<ObjectCollectionType, ObjectType>::deleteObject(Ogre::String name)
{
	uint id;
	try
	{
		id = find(name).id();
	}
	catch ( ENotFoundIdGenericCollectionError& )
	{
		_log(_debug,_loc,gtc_("Don't found object with name = '%s'"), name.c_str() );
		return false;
	}
	return deleteObject(id);
};

template<class ObjectCollectionType, class ObjectType>
void CGenericCollection<ObjectCollectionType, ObjectType>::deleteAllObject()
{
	std::map< uint, ObjectType* >::iterator itBegin = objectMap_.begin();
	std::map< uint, ObjectType* >::iterator itEnd = objectMap_.end();
	std::map< uint, ObjectType* >::iterator it;

	for (it = itBegin; it != itEnd; it++)
	{
		_log(_debug,_loc,gtc_("Delete object: id = '%u' name = '%s'\n"),(*it).first, (*it).second->getName());
		// freed id for next objects
		freeId_.push_back((*it).first);
		// we ownershiped ptr ? 
		delete((*it).second);
	}
	objectMap_.clear();
};

template<class ObjectCollectionType, class ObjectType>
StringVector CGenericCollection<ObjectCollectionType, ObjectType>::getInfoAllObject() const
{
	std::map< uint, ObjectType* >::iterator itBegin = objectMap_.begin();
	std::map< uint, ObjectType* >::iterator itEnd = objectMap_.end();
	std::map< uint, ObjectType* >::iterator it;
	//TODO: mb return map<uint, ObjectType*> ? but don't hide realisation from user
	StringVector infoAllObject;
	for (it = itBegin; it != itEnd; it++)
	{
		infoAllObject.push_back((*it).first);
		infoAllObject.push_back(Ogre::String( (*it).second->getName() ));

	}
}