/**
 * @file common/net/state_machine.h
 * generic state machine
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: rs_player_context.h 519 2006-03-03 10:09:33Z zak $

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


#ifndef _STATE_MACHINE_H
#define _STATE_MACHINE_H

#include "common/net/generic_state.h"
#include "common/common_stdafx.h"

typedef byte stateId;

class CStateCreatorBase
{
public:
	virtual CGenericState* create() = 0;
protected:
	CStateCreatorBase() {}
};

template <class State, class OwnerClass>
class CStateCreator : public CStateCreatorBase
{
public:
	CStateCreator(OwnerClass* oc) {m_owner = oc;}
	CGenericState* create() {return new State(m_owner);}
private:
	OwnerClass*		m_owner;
};

/*
template <class State>
CGenericState* createState() {return new State;}
typedef CGenericState* (*stateCreator)();
*/

using namespace ireon::net;

class CStateMachine
{
public:
	bool setNextState(stateId newState);

protected:
	CStateMachine():m_statePointer(NULL), m_currentState(0xFF) {}
	~CStateMachine();
	bool registerState(stateId si, CStateCreatorBase *sc);
	bool switchStates();
	bool processSignal(ireon::net::commandId id, netAddress fromAddress, CData& data);

private:
	CGenericState	*m_statePointer;
	stateId			m_nextState, m_currentState;
	std::map<stateId, CStateCreatorBase*>	m_stateCreators;
};

#endif

