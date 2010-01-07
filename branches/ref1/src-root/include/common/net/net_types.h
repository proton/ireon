/**
 * @file common/net/net_types.h
 * Misc network types
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: net_types.h 609 2006-04-20 04:14:35Z zak $

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

#ifndef _NET_TYPES_H
#define _NET_TYPES_H


namespace ireon
{
namespace net
{
	typedef byte	commandId;

	enum generalResponseCodes
	{
		rcCommandNotFound = 0	
	};

	/// used to describe various types of addresses, e.g. net packet from address
	struct netAddress
	{
		netAddress(byte type): m_type(type) {};
		enum addrTypeFlags
		{
			atUndefinedAddress = 1,
			atExternalAddress = 2,	//< packet came from network
			atInternalAddress = 4	//< packet was sent from iside of program
		};
		byte m_type;
	};
}
}

#include "net/data.h"

#endif

