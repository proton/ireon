/**
* @file client_static.h
* Static object
*/

/* Copyright (C) 2005 ireon.org developers council
* $Id: client_static.h 829 2006-11-30 18:39:37Z mip $

*  See LICENSE for details
*/
#ifndef _CHARACTER_PROTOTYPE_H
#define _CHARACTER_PROTOTYPE_H

#include "common/world/generic_object/prototype.h"

class CCharacterPrototype: public CPrototype
{
public:
	CCharacterPrototype();
	virtual ~CCharacterPrototype();

	virtual bool load(const String& resourceName);

};

class COwnPlayerPrototype: public CCharacterPrototype
{
public:
	COwnPlayerPrototype();
	virtual ~COwnPlayerPrototype();

	virtual bool load(const String& resourceName);

};
#endif
