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

#ifndef _RSPC_CODES_H_
#define _RSPC_CODES_H_

namespace ireon
{
namespace net
{
namespace rspcCodes
{

	/// incomming messages codes for root server player context
	enum rspcSignalCodes
	{
		// hello state
		scHelloLogin = 1,
		scHelloRegister = 2,

		// character selection state
		scCharSelList = 5,
		scCharSelSelect = 6,
		scCharSelCreate = 7,
		scCharSelRemove = 8,

		// wait remove ack state
		scCharRemAck = 15,			//< \see enum rspcCharRemoveAckFailedCodes
		scCheckVersion = 50
	};
	/// response codes for root server player context
	enum rspcResponseCodes
	{
		// hello state
		rcLoginOk = 1,
		rcLoginFailed = 2,			//< \see enum rspcLoginFailedCodes
		rcRegistrationOk = 5,
		rcRegistrationFailed = 6,	//< \see enum rspcRegistrationFailedCodes

		// character selection state
		rcCharListOk = 10,
		//rcCharListEmpty = 11,
		rcCharCreated = 12,
		rcCharCreationFailed = 13,
		rcCharRemoveOk = 20,
		rcCharRemoveWaitAck = 21,	//< server is waiting ack from cluster manager
		rcCharRemoveFailed = 22,	//< \see enum rspcCharRemoveFailedCodes
		rcVersionOk = 50,
		rcVersionOld = 51
	};

	/// login failures description codes
	enum rspcLoginFailedCodes
	{
		lfUnknown = 0,				//< reason unknown
		lfAccountNotFound = 1,		//< account was not found in db
		lfWrongPassword = 2,		//< account was found bu password doesn't match
		lfAlreadyLoggedIn = 3		//< account is already connected
	};

	/// registration failures description codes
	enum rspcRegistrationFailedCodes
	{
		rfUnknown = 0,				//< reason unknown
		rfAlreadyRegistered = 1,	//< account is already regitered
		rfMalformedData = 2			//< login or password too short?
	};

	/// char creation failures description codes
	enum rspcCharCreationFailedCodes
	{
		ccUnknown = 0,				//< reason unknown
		ccAlreadyRegistered = 1,	//< character with same name is already regitered
		ccDBError = 2				//< database returned error
	};

	/// char remove failures description codes
	enum rspcCharRemoveFailedCodes
	{
		crUnknown = 0,				//< reason unknown
		crIsInUse = 1,				//< char is being in use
		crClusterNotFound = 2,		//< char cluster not found (disconnect?)
		crCharNotFound = 3			//< char not found
	};

	/// char remove ack (from cm) failures description codes
	enum rspcCharRemoveAckFailedCodes
	{
		craOk = 0,				//< it is ok to remove character from root db
		craIsInUse = 1,			//< char is in use on cluster
		craCharUnknown = 2		//< char is unknown on cluster
	};
} // rspcCodes
} // net
} // ireon

#endif
