/* Copyright (C) 2005 ireon.org developers council
 * portions (C) Radon Labs GmbH, www.nebuladevice.org
 * $Id: crc.cpp 681 2006-08-21 18:04:52Z zak $

 *  See LICENSE for details
 */

/**
 * @file crc.cpp
 * CRC realization
 */
#include "common/common_stdafx.h"
#include "common/file/crc.h"

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
