/**
* @file object_factory.h
* Factory creating object
* 
*/

/* Copyright (C) 2005 ireon.org developers council
* $Id$

*  See LICENSE for details
*/
#ifndef _OBJECT_FACTORY_H
#define _OBJECT_FACTORY_H
class CObject;
class CPrototype;
// Принимает в качестве параметра прототип обьекта
// Вызывает функцию clone у прототипа
class EObjectFactoryError {};
class EProductIdNotFound : public EObjectFactoryError {};
class EProductCallbackError : public EObjectFactoryError {};
struct CmpStringLess
{
	bool operator() (const String& arg1, const String& arg2)
	{
		if ( strcmp(arg1.c_str(), arg2.c_str()) < 0 )
			return true;
		else 
			return false;
	}
};

typedef CObject* (*productCallbackPtr)(CPrototype*);

class CObjectFactory: protected CLogMixin
{
public:
	/// get pointer on singleton
	static CObjectFactory* getSingletonPtr();

	/// for register callback from  products
	/// return 'True' if create is succeed
	///
	bool registerProduct(const String& productId, productCallbackPtr productCallback );

	bool unregisterProduct(const String& productId);

	CObject* createProduct(const String& productId, CPrototype* proto);

private:
	CObjectFactory();
	~CObjectFactory(){};
	CObjectFactory(const CObjectFactory&){};

	/// instance of singleton
	static CObjectFactory* singleton_;

	/// List keep pointer on callback,
	/// where is writed about create CONCRETE object
	std::map<String, productCallbackPtr, CmpStringLess> productCallbackList_;

};

#endif
