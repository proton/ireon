/* Copyright (C) 2005 ireon.org developers council
  * portions (C) Radon Labs GmbH, www.nebuladevice.org
 
  *  See LICENSE for details
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
