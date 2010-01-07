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
 * @file compressing_layer.cpp
 * Compressing layer 
 */

#include "common/common_stdafx.h"
#include "net/layers/compressing_layer.h"

void CCompressingLayer::write(const CData& data)
{
	m_outputBuffer.push_back(data);
};
bool CCompressingLayer::flush()
{
	std::vector<CData>::iterator i;
	/// 1. input packet must be less or equal 0xFFFF
	/// 2. output packet must be less or equal 0xFFFF
	for(i = m_outputBuffer.begin(); i != m_outputBuffer.end(); i++ )
			assert( (*i).length() <= (UNCOMPRESS_BUFFER_SIZE-0x2) ); 
	
	m_wrtPtr = &m_uncompressBuffer[0];		///replace pointer on begin u-buffer
	m_zwrtPtr = &m_compressBuffer[0];		///replace pointer on begin z-buffer
	
	bool dataResize = false;
	bool forceCompress = false;
	int zResult;
	/// first element from unprocessed element
	std::vector<CData>::iterator firstData = m_outputBuffer.begin();
   
    for(i = m_outputBuffer.begin(); i != m_outputBuffer.end(); i++ )
	{

		///if we can placed in u-buffer command and size of command
		///		
		///		placed in u-buffer size of command and command 
		///		replace pointer on write in u-buffer right on (size of command + command)
		///		
		///		if we have yet command and it maybe placed(+ size of course) in u buffer 
		///			continue 
		///pack u-buffer (  z-image = f(u-buffer) )
		///if  deflate return Z_BUF_ERROR (z-image > 0xffff) then ...... see down
		///push z-image in z-buffer, push z-buffer in CData zdata, write to underlying layer
		///replace pointer on begin z-buffer
		///end cycle
		if ( (*i).length() <= (&m_uncompressBuffer[0]+UNCOMPRESS_BUFFER_SIZE-m_wrtPtr-sizeof(ushort))
			&& !dataResize && !forceCompress)
		{
			size_t temp = (*i).length();
			memcpy(m_wrtPtr, (const ushort*)&temp,		sizeof(ushort));
			m_wrtPtr += sizeof(ushort);
			memcpy(m_wrtPtr, (const Bytef*)(*i).data(), (*i).length());
			m_wrtPtr += (*i).length();
			

			if (i != m_outputBuffer.end()-1  )
				if (((signed int)(( *(i+1) ).length())) <= (signed int)(&m_uncompressBuffer[0]+UNCOMPRESS_BUFFER_SIZE-m_wrtPtr-sizeof(ushort) ))
					continue;
		}
		uLongf len_ulong = COMPRESS_BUFFER_SIZE;
        if((zResult = compress2( (Bytef*)&m_compressBuffer[0],
								&len_ulong,
								(const Bytef*)(&m_uncompressBuffer[0]),
								(uLong)(m_wrtPtr-&m_uncompressBuffer[0]),
								Z_DEFAULT_COMPRESSION))
								!= Z_OK)
		{
			/// replace iterator on 1 element back and anew attempt pack,while z-image > 0xFFFF
			if (zResult == Z_BUF_ERROR && !dataResize)
			{
				if (i == firstData )		///if z-image first element too big
				{							///skip first element 				
					m_wrtPtr -= (*i).length()+sizeof(ushort);	
				/*	// for resizing data
					uint temp = (*i).length()>>1;				///decrease command
					memcpy(m_wrtPtr, (const ushort*)&temp,		sizeof(ushort));
					m_wrtPtr += sizeof(ushort);
					memcpy(m_wrtPtr, (const Bytef*)(*i).data(), (*i).length()>>1);
					m_wrtPtr += (*i).length()>>1;
					i--;*/
					//dataResize = true;
					CLog::instance()->log(CLog::msgFlagNetwork,CLog::msgLvlError,_("ZLib: Output command don't maybe compressed (z-image > 0xFFFF)"));
					continue;
				}
				else							///not first element
				{								///replace iterator on 2 element back
					m_wrtPtr-=(*i).length()+sizeof(ushort);
					i--;
					i--;
					forceCompress = true;		///skip check when an opportunity offers 
					continue;					///add some more data in z-buffer
				}
			}
			/// this another error
			CLog::instance()->log(CLog::msgFlagNetwork,CLog::msgLvlError,_("ZLib(Error): Compress2 return %d"),zResult);
			continue;
		};
		
		ushort len_ushort=static_cast<ushort>(len_ulong);
		CData zdata((const char*)&m_compressBuffer[0], len_ushort, true);
		m_nextLayer->write(zdata);
		/// next element it is unprocessed element
		firstData = i+1;
		m_zwrtPtr = &m_compressBuffer[0];			/// replace write pointer on begin z-buffer
		m_wrtPtr = &m_uncompressBuffer[0];			/// replace write pointer on begin u-buffer
		dataResize = false;							/// reset flags
		forceCompress = false;
	}
	m_outputBuffer.clear();
	/// process in underlying layer 
	if (!m_nextLayer->flush())
		return false;

	return true;
};

bool CCompressingLayer::read(std::vector<CData> &inputBuffer)
{
	inputBuffer.clear();
	/// Read input data
	std::vector<CData> input;
	if (!m_nextLayer->read(input))
		return false;
	
	std::vector<CData>::iterator i;
	int zResult;
	/// Uncompress data and wrap data
	for (i = input.begin();i!=input.end(); i++)
	{
		uLongf len_ulong = UNCOMPRESS_BUFFER_SIZE;
		if((zResult = uncompress((Bytef*)&m_in,
							&len_ulong,
							(const Bytef*)((*i).data()),
							(uLong)((*i).length()) )) != Z_OK)
		{
			CLog::instance()->log(CLog::msgFlagNetwork,CLog::msgLvlError,_("ZLib(Error): Uncompress return %d\n"),zResult);
			return false;
		};
		uint len=static_cast<uint>(len_ulong);
		/// Wrap data for sending it to output
		m_zreadPtr	= &m_in[0];
		m_zwrtPtr	= &m_in[0] + len;		
		m_zwaitFor	= 0;
		while (m_zreadPtr < (m_zwrtPtr - 2))
		{
			m_zwaitFor = *((ushort*)m_zreadPtr);
			m_zreadPtr += sizeof(ushort);
			if( !m_zwaitFor )
				continue;	/// strange, zero size command, skip this size
			if ( (signed int)m_zwaitFor > (signed int) (m_zwrtPtr - m_zreadPtr) )
				break;		/// request more data than we receive
			/// Wrap data for sending it to handler
			CData data((const char*)m_zreadPtr,m_zwaitFor,true);
			/// Process one block of data (command)
			inputBuffer.push_back(data);
			/// Move read pointer to data after current command
			m_zreadPtr += m_zwaitFor;
			m_zwaitFor = 0;
		}

	}	
	return true;

};



