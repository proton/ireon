/**
 * @file ireon_rs/db/character.h
 * root server character info class
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: character.h 775 2006-09-29 20:11:59Z zak $

 * See LICENSE for details
 */

#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "ireon_rs/stdafx.h"
#include "boost/shared_ptr.hpp"
#include <map>

class ECharacterError {};
class ECharacterNotFound : public ECharacterError {};
class EOwnerNotSet : public ECharacterError {};

class CRSCharacterInfo
{
public:
	CRSCharacterInfo();
	CRSCharacterInfo(String &name, bool loadFromDB = true);
	CRSCharacterInfo(ushort id, bool loadFromDB = true);

	void setOwner(uint accoutId) { m_owner = accoutId; }

	ushort getId() { return m_id; }
	byte getWS() { return m_ws; }
	void setWS(byte wsid) { m_ws = wsid; }
	String getName() { return m_name; }

	void saveToDB();

	static byte getCharactersCount (uint accountId);
	static void getCharacters (uint accountId, std::map<ushort, boost::shared_ptr<CRSCharacterInfo>> &charlist);

private:
	ushort	m_id;
	uint	m_owner;
	byte	m_ws;
	String	m_name;
};

#endif
