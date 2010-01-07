/**
 * @file common/net/generic_state.h
 * Declares base class for states
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

#ifndef _GENERIC_STATE_H_
#define _GENERIC_STATE_H_

#include "common/common_stdafx.h"
#include "common/net/data.h"
#include "common/net/net_types.h"

class CGenericState;
typedef void (CGenericState::*CGenericSlot)(CData&);

class CGenericState
{
public:
	bool processSignal(const ireon::net::commandId requestId, ireon::net::netAddress fromAddress, CData& data);

protected:
	CGenericState();
	void setStateName(const String newName);
	bool registerSlot(const ireon::net::commandId requestId, const CGenericSlot slot, byte addrTypeMask = 255);

private:
	void onUnknownCommandResponse(CData& data);

	std::map<ireon::net::commandId, CGenericSlot>	m_slots;
	std::map<ireon::net::commandId, byte>	m_accessMasks;
	String	m_stateName;
};

#endif
