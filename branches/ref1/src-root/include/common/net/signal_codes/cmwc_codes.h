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

#ifndef _CMWC_CODES_H_
#define _CMWC_CODES_H_

namespace ireon
{
namespace net
{
namespace cmwcCodes
{

	/// incomming messages codes for cluster manager world context
	enum cmwcSignalCodes
	{
		// hello state
		scHelloLogin =			1,	//< request for login from WS		
		// main state
		scMainGetAccount =		2,	//< get account requested checking login client in WS   
		scMainSendPlayerChar =	3,	//< ?? 
		scMainNewChar =			4,  //< answer from WS for request on creating new char in WS
	    scMainRemoveChar =		5	//< answer from WS for request on remove char in WS	
	};
	
	enum cmwcInternalSignalCodes
	{
		// main state
		iscMainSendAccount	= 60,	//< send account in WS
		iscMainSendNewChar	= 61,	//< send new char in WS
		iscMainRemoveChar	= 62	//< remove char in WS
	};

	/// response codes for root server player context
	enum cmwcResponseCodes
	{
		// hello state
		cmLogin = 1				//< see enum : cmwcLoginDesriptionCodes
		
	};

	/// login description codes
	enum cmwcLoginDesriptionCodes
	{
		ldLoggedFailed = 0,		/// world is already connected or failed login
		ldLoggedOk = 1			///	login ok 
	};

	
} // cmwcCodes
} // net
} // ireon

#endif
