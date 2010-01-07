/**
 * @file common/net/generic_state.cpp
 * Base class for states
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

#include "common/common_stdafx.h"
#include "common/net/generic_state.h"

CGenericState::CGenericState():
m_stateName("Undefined State")
{
	registerSlot(ireon::net::rcCommandNotFound, (CGenericSlot) &CGenericState::onUnknownCommandResponse);
}

// ------------------------------------------------------------------

void CGenericState::setStateName(const String newName)
{
	m_stateName = newName;
}

// ------------------------------------------------------------------

bool CGenericState::processSignal(const ireon::net::commandId requestId, ireon::net::netAddress fromAddress, CData& data)
{
	if (m_slots.find(requestId) != m_slots.end())
	{
		if (fromAddress.m_type && m_accessMasks[requestId])
		{
			(this->*m_slots[requestId])(data);	
			return true;
		}
		else
		{
			CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError, _("State '%s': access to slot %u denied.\n"), m_stateName.c_str(), requestId);
			return false;
		}
	} 
	else 
	{
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError, _("State '%s': could not find signal with id %u.\n"), m_stateName.c_str(), requestId);
		return false;
	}
}

// ------------------------------------------------------------------

bool CGenericState::registerSlot(const ireon::net::commandId requestId, const CGenericSlot slot, byte addrTypeMask)
{
	assert(slot);
	if (m_slots.find(requestId) != m_slots.end())
		CLog::instance()->log(CLog::msgLvlWarn, _("Signal handler re-registration -- signal %u.\n"), requestId);
	m_slots[requestId] = slot;
	m_accessMasks[requestId] = addrTypeMask;
	return true;
}

// ------------------------------------------------------------------

void CGenericState::onUnknownCommandResponse(CData& data)
{
	ireon::net::commandId id;
	data >> id;
	CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlError, _("Got <<unknown command>> response for command %d.\n"), id);
}

