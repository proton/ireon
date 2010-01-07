/* Copyright (C) 2005-2006 ireon.org developers council
 * portions (C) Radon Labs GmbH, www.nebuladevice.org
 * $Id: crc.cpp 522 2006-03-06 16:49:30Z zak $

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

/**
 * @file crc.cpp
 * CRC realization
 */
#include "common/common_stdafx.h"
#include "kernel/crc.h"

const uint CRC::m_key = 0x04c11db7;
uint CRC::m_table[NUM_BYTE_VALUES] = { 0 };
bool CRC::m_tableInitialized = false;

CRC::CRC()
{
	// initialize byte table
	if (!m_tableInitialized)
	{
		uint i;
		for (i = 0; i < NUM_BYTE_VALUES; ++i)
		{
			uint reg = i << 24;
			int j;
			for (j = 0; j < 8; ++j)
			{
				bool topBit = (reg & 0x80000000) != 0;
				reg <<= 1;
				if (topBit)
				{
					reg ^= m_key;
				}
			}
			m_table[i] = reg;
		}
		m_tableInitialized = true;
	}
}

uint CRC::checksum(uchar* ptr, uint numBytes)
{
	uint reg = 0;
	uint i;
	for (i = 0; i < numBytes; i++)
	{
		uint top = reg >> 24;
		top ^= ptr[i];
		reg = (reg << 8) ^ m_table[top];
	}
	return reg;
}
