/* Copyright (C) 2005 ireon.org developers council
 * portions (C) Radon Labs GmbH, www.nebuladevice.org

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
 * @file crc.h
 * Class for computing checksum.
 */
#ifndef _CRC_H
#define _CRC_H

class CRC
{
public:
	/// default constructor
	CRC();
	/// compute a CRC checksum for a chunk of memory
	uint checksum(uchar* ptr, uint numBytes);

private:
	enum
	{
		NUM_BYTE_VALUES = 256,      // MUST BE 256 (for each possible byte value 1 entry)
	};
	static const uint m_key;
	static uint m_table[NUM_BYTE_VALUES];
	static bool m_tableInitialized;
};
#endif
