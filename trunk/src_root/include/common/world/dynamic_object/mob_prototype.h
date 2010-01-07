/**
  * @file mob_prototype.h
  * Mob prototype
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: mob_prototype.h 785 2006-10-25 15:48:43Z dox $
  
  *  See LICENSE for details
  */
 
#ifndef _MOB_PROTOTYPE_H
#define _MOB_PROTOTYPE_H
#include "common/world/generic_object/prototype.h" 
//#include "boost/shared_ptr.hpp"

//class CMobPrototype;
//typedef boost::shared_ptr<CMobPrototype> MobPrototypePtr;

class CMobPrototype: public CPrototype
{
friend class CWorld;
protected:
	CMobPrototype();
public:
	~CMobPrototype();

	String getType() const {return m_type;}

protected:
 
 	// Identifier
 	String m_type;
};

#endif