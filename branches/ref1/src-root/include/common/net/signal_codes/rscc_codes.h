/**
 * @file common/net/rscc_codes.h
 * defines signal codes for root server cm contexts
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

#ifndef _RSCC_CODES_H_
#define _RSCC_CODES_H_

namespace ireon
{
namespace net
{
namespace rsccCodes
{
	/// incomming messages codes for root server cm context
	enum rsccSignalCodes
	{
		// hello state
		scHelloLogin = 1,

		// main state
		scCheckRemoveCharRequest = 5, //< check whether it is ok to remove char from rs (\see rcCheckCharRemove)
		scCharRemoveAck = 6
	};

	/// outgoing messages codes for root server cm context
	enum rsccResponseCodes
	{
		// hello state
//		rcLoginOk = 1,
//		rcLoginFailed = 2,			//< \see enum rspcLoginFailedCodes
		//TODO: mip request check this value , maybe 6 ?
		rcCheckCharRemove = 5		//< check whether character is ok to delete from rs
	};

	enum rsccLoginFailedCodes
	{
		lfHostNotAllowed = 0,		//< host was not found in config list of allowed cm addrs
		lfAlreadyLoggedIn = 1
	};

	enum rsccCharRemoveAckCodes
	{
		craRemoveOk = 0,			//< char is cleared for deletion
		craInUse = 1,				//< char is in use on cluster
		craCharUnknown = 2			//< char is unknown on cluster
	};

} // rsccCodes
} // net
} // ireon

#endif
