/**
* @file generic_collection.h
* Generic Collection
* 
*/

/* Copyright (C) 2005 ireon.org developers council
* $Id$

*  See LICENSE for details
*/

#ifndef _GENERIC_COLLECTION_H
#define _GENERIC_COLLECTION_H

//#include "boost/lambda/lambda.hpp"
//#include "boost/shared_ptr.hpp"


class EGenericCollectionError {};
class ENotFoundIdGenericCollectionError: public EGenericCollectionError {};

class CGenericObject;

template<class ObjectCollectionType, class ObjectType>
class GenericObjectIterator
{
	template<class ObjectCollectionType, class ObjectType>
	friend class CGenericCollection;
public:
	/*StaticPtr& operator* ()
	{
	return *object_;
	};*/

	ObjectType* operator* ()
	{
		return object_;
	};

	uint id()
	{
		return index_;
	};

	GenericObjectIterator& operator++()
	{
		if ( internal_iterator_ == CGenericCollection<ObjectCollectionType, ObjectType>::getSingletonPtr()->objectMap_.end() )
		{
			index_ = 0;
			object_ = NULL;
			return *this;
		}
		internal_iterator_++;
		if ( internal_iterator_ == CGenericCollection<ObjectCollectionType, ObjectType>::getSingletonPtr()->objectMap_.end() )
		{
			index_ = 0;
			object_ = NULL;
			return *this;
		}
		index_ = (*internal_iterator_).first;
		object_ = (*internal_iterator_).second;
		return *this; 
	};

	GenericObjectIterator operator++(int)
	{
		GenericObjectIterator tmp = *this;
		++*this;
		return (tmp); 
	};

	bool operator==( const GenericObjectIterator& rv)
	{
		return (rv.index_ == index_);
	}
	bool operator!=( const GenericObjectIterator& rv)
	{
		return  !( *this == rv );
	}
	GenericObjectIterator& operator=(const GenericObjectIterator& other)
	{
		index_ = other.index_;
		object_ = other.object_;
		internal_iterator_ = other.internal_iterator_;
		return *this;
	};

	GenericObjectIterator():index_(0), object_(NULL)
	{
		internal_iterator_ = CGenericCollection<ObjectCollectionType, ObjectType>::getSingletonPtr()->objectMap_.end();
	};

	GenericObjectIterator(const GenericObjectIterator& rv)
	{
		index_ = rv.index_;
		object_ = rv.object_;
		internal_iterator_ = rv.internal_iterator_;
	}
private:
	GenericObjectIterator(uint index, ObjectType* object):index_(index), object_(object)
	{
		internal_iterator_ = CGenericCollection<ObjectCollectionType, ObjectType>::getSingletonPtr()->objectMap_.find(index);
	};
	// need for emulating iterator
	typename std::map<uint,ObjectType*>::iterator internal_iterator_;
	uint index_;
	ObjectType* object_;

};


template<class ObjectCollectionType, class ObjectType>
class CGenericCollection: public CLogMixin 
{
	template<class ObjectCollectionType, class ObjectType>
	friend class GenericObjectIterator;

public:

	typedef GenericObjectIterator<ObjectCollectionType,ObjectType> iterator;

	static ObjectCollectionType* getSingletonPtr();

	bool init();

	uint addObject(ObjectType* object);

	bool deleteObject(uint id);

	bool deleteObject(Ogre::String name);

	void deleteAllObject();
	iterator begin();

	iterator end();

	bool empty();

	iterator find(uint id);

	iterator find(Ogre::String& name);

	/// get info about objects keeped colection in format |id|name|
	StringVector getInfoAllObject() const;
protected:

	uint getFirstFreeObjectId();

	/// private constructor, therefore use getSingletonPtr() or getSingleton()
	CGenericCollection(const char* loggerName):CLogMixin(loggerName){};
	virtual ~CGenericCollection(void){};
protected:	
	/// instance of singleton
	static ObjectCollectionType* singleton_;

	std::map<uint,ObjectType*> objectMap_;
	std::vector<uint> freeId_;
	//std::map<String, uint> name2idMap_;
	uint startIndex_;
	uint endIndex_;
};
#pragma warning(push)
#pragma warning(disable: 4239)
#include "../src/common/world/managers/generic_collection.cpp"
#pragma warning(pop)
#endif