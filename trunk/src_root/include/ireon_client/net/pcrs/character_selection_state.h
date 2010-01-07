/**
 * @file ireon_client/net/pcrs/character_selection_state.h
 * client root server context character manipulations state
 */

/* Copyright (C) 2005-2006 ireon.org developers council
* $Id$

* See LICENSE for details
*/

#ifndef _PCRS_CHARACTER_SELECTION_STATE_H_
#define _PCRS_CHARACTER_SELECTION_STATE_H_

#include "common/net/generic_state.h"
#include "ireon_client/net/pcrs_connection.h"

class CPCRSCharacterSelectionState : public CGenericState
{
public:
	CPCRSCharacterSelectionState(CNetConnection *ownerConnnection);

private:
	void initiateCreateChar		(String &PacketData);
	void onCreateCharResponse	(String &PacketData);
	
	void initiateListChar		(String &PacketData);
	void onListCharsResponse	(String &PacketData);

	void initiateSelectChar		(String &PacketData);
	void onSelectCharAck		(String &PacketData);
	void onSelectCharResponse	(String &PacketData);

};

#endif
