/* Copyright (C) 2005 ireon.org developers council
 * $Id: player_client.h 433 2005-12-20 20:19:15Z zak $

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

/**
 * @file player_client.h
 * Player's network client
 */

#ifndef _PLAYER_CLIENT_H
#define _PLAYER_CLIENT_H

#include "net/net_client.h"
#include "ireon_client/app_states/client_app_state.h"

class CPlayerClient;

typedef void (CPlayerClient::*PCSig)(CData& data);

class CPlayerClient : public CNetClient
{
public:
	bool init() { initSignals();	return true; }

protected:
	virtual void initSignals() {};
	bool processCommand(CData& data, byte id);
	void connectionFailed(uint error);

	std::vector<PCSig> m_signals;
};

#endif