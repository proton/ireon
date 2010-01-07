/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id$

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
 * @file compressing_layer.h
 * Compressing layer 
 */

#ifndef _COMPRESSING_LAYER_H
#define _COMPRESSING_LAYER_H
#include "common/net/layers/net_layer_base.h"
#include <zlib.h> 
/// maximum size packet = 0xFFFF
/// therefore need on this packet yet 0x2 byte for storage size of packet
const ulong UNCOMPRESS_BUFFER_SIZE = 0xFFFF + 0x2;
const ulong COMPRESS_BUFFER_SIZE = 0xFFFF;

class CCompressingLayer: public CNetLayerBase
{
public:
	/// read uncompressed data from underlying layer
	virtual bool read(std::vector<CData> &inputBuffer);
	/// write uncompressed data to output buffer
	virtual void write(const CData& data);
	/// process all uncompressed data in output buffer to compress
	/// and write to underlying layer
	virtual bool flush();

private:
	std::vector<CData> m_outputBuffer;
	
	/// buffer for unpacked input data
	byte m_in[UNCOMPRESS_BUFFER_SIZE]; 
	/// u(ncompress)-buffer 
	byte m_uncompressBuffer[UNCOMPRESS_BUFFER_SIZE];  
	/// z(compress)-buffer
	byte m_compressBuffer[COMPRESS_BUFFER_SIZE];
	
	byte* m_zwrtPtr;	/// Write pointer to compress data
	byte* m_zreadPtr;	/// Read pointer to compress data
	uint m_zwaitFor;	/// Number of packed bytes to read
	
	byte* m_wrtPtr;		/// Write pointer on uncompress data
};

#endif