/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: ws_char_stats.cpp 670 2006-07-31 18:41:05Z zak $

 * See LICENSE for details
 */

/**
 * @file ireon_ws/world/ws_char_stats.cpp
 * Chracter stats object
 */

#include "ireon_ws/world/ws_char_stats.h"

void CWSCharStats::reset_stats()
{
// 	m_baseMaxHP = 100;
// 	m_curHP = m_maxHP = m_baseMaxHP;
 	m_curHP = m_maxHP = 100;
	m_regHPModifier = 1;
/*
 	m_baseMaxMana = 100;
 	m_curMana = m_maxMana = m_baseMaxMana;
 	m_baseStr = 10;
 	m_baseSta = 10;
 	m_baseInt = 10;
 	m_baseWis = 10;
 	m_baseCon = 10;
 	m_baseLuck = 10;
*/
};