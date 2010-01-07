/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: ws_char_stats.h 670 2006-07-31 18:41:05Z zak $

 * See LICENSE for details
 */

/**
 * @file ireon_ws/world/ws_char_stats.h
 * Chracter stats object
 */

#ifndef _WS_CHAR_STATS_H
#define _WS_CHAR_STATS_H

#include "common/common_stdafx.h"

class CWSCharStats
{
public:
	CWSCharStats() {reset_stats();}
	~CWSCharStats() {}

	void reset_stats();
 
 	uint getMaxHP() {return m_maxHP;}
 	void setMaxHP(uint hp) {m_maxHP = hp;}
 	uint getHP() {return m_curHP;}
 	void setHP(uint hp) {m_curHP = hp;}
 	byte getPercentHP() {return m_curHP * 100 / m_maxHP;}

/*
 	uint getMaxMana() {return m_maxMana;}
 	uint getMana() {return m_curMana;}
 	void setMana(uint mana) {m_curMana = mana;}
 
 	uint getStr() {return m_str;}
 	uint getSta() {return m_sta;}
 	uint getInt() {return m_int;}
 	uint getWis() {return m_wis;}
 	uint getCon() {return m_con;}
 	uint getLuck() {return m_luck;}

 	uint getAc() {return m_ac;}
 	uint getHitroll() {return m_hitroll;}
 	uint getDamroll() {return m_damroll;}
 	uint getArmor() {return m_armor;}
*/

private:
// 	uint m_baseMaxHP;
 	uint m_maxHP;
 	uint m_curHP;
 	byte m_regHPModifier;

/*
	uint m_baseMaxMana;
 	uint m_maxMana;
 	uint m_curMana;
 	byte m_regManaModifier;
 
 	uint m_baseStr;
 	uint m_str;
 	uint m_baseSta;
 	uint m_sta;
 	uint m_baseInt;
 	uint m_int;
 	uint m_baseWis;
 	uint m_wis;
 	uint m_baseCon;
 	uint m_con;
 	uint m_baseLuck;
 	uint m_luck;
 
 	uint m_ac;
 	uint m_armor;
 	uint m_hitroll;
 	uint m_damroll;
*/

};

#endif
