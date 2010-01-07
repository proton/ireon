/**
 * @file common/net/rspc_codes.h
 * defines signal codes for root server player contexts
 */

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

#ifndef _WSCC_CODES_H_
#define _WSCC_CODES_H_

namespace ireon
{
namespace net
{
namespace wsccCodes
{

	/// incomming messages codes for world server cluster manager context
	enum wsccSignalCodes
	{
		// hello login
		scHelloLogin		= 1,
		// main state
		scMainSendAccount	= 2, //< \see enum  wsccResponseCodes
		//scMainGetChar		= 3,
		scMainSendNewChar	= 4,  
		scMainRemoveChar	= 5

	};
	
	enum wsccResponseCodes
	{
		wcSendNewAccount = 0,
	};

	enum wsccSendNewAccountDescriptionCodes
	{
		wcUpdateAccountData		= 0,
		wcNotUpdateAccountData	= 1
	};

} // wsccCodes
} // net
} // ireon

#endif
