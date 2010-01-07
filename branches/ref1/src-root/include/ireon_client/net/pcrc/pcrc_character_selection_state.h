/**
 * @file ireon_client/net/pcrc/pcrc_character_selection_state.h
 * client root server context character manipulations state
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

#ifndef _PCRC_CHARACTER_SELECTION_STATE_H_
#define _PCRC_CHARACTER_SELECTION_STATE_H_

#include "common/net/generic_state.h"
#include "ireon_client/net/pc_rs_connection.h"

class CPCRCCharacterSelectionState : public CGenericState
{
public:
	CPCRCCharacterSelectionState(CPCRSConnection *ownerConnnection);

private:
	CPCRSConnection		*m_ownerConnection;
/*
	void onListCharsRequest (CData& data);
	void onSelectCharRequest (CData& data);
	void onCreateCharRequest (CData& data);
	void onRemoveCharRequest (CData& data);
*/
};

#endif
