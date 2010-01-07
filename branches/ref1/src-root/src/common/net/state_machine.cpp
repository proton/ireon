/**
 * @file common/net/state_machine.cpp
 * Generic state machine
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

#include "common/net/state_machine.h"

bool CStateMachine::registerState(stateId si, CStateCreatorBase *sc) 
{ 
	assert(sc);
	m_stateCreators[si] = sc;
	return true; 
}

// -----------------------------------------------------------------------------------

CStateMachine::~CStateMachine() 
{ 
	std::map<stateId, CStateCreatorBase*>::iterator	i;
	i = m_stateCreators.begin();
	while ((i != m_stateCreators.end()) && (i->second))
	{
		delete i->second;
		++i;
	}
}

// -----------------------------------------------------------------------------------

/// Sets next state to be set at the end of packet pocessing loop (see
bool CStateMachine::setNextState(stateId newState)
{
	m_nextState = newState;
	return true;
}

// -----------------------------------------------------------------------------------
/// Actually changes state (opposing to setNextState())
bool CStateMachine::switchStates()
{
	if (m_currentState == m_nextState)
		return false;

	if (m_stateCreators.find(m_nextState) == m_stateCreators.end())
		return false;

	m_currentState = m_nextState;
	if (m_statePointer)
		delete m_statePointer;
	m_statePointer = m_stateCreators[m_nextState]->create();
	return true;
}

// -----------------------------------------------------------------------------------

bool CStateMachine::processSignal(ireon::net::commandId id, ireon::net::netAddress fromAddress, CData& data)
{
	if (!m_statePointer)
		CLog::instance()->log(CLog::msgFlagNetwork, CLog::msgLvlCritical, _("State machine could not find state!."));
	bool result = m_statePointer->processSignal(id, fromAddress, data);
	// switch context state if needed
	switchStates();
	return result;
};
