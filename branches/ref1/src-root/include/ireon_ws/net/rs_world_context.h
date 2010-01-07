/* Copyright (C) 2005 ireon.org developers council

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
 * @file ws_player_context.h
 * Player connection context on world server
 */

#ifndef _RS_WORLD_CONTEXT_H
#define _RS_WORLD_CONTEXT_H

class CRSWorldContext : public CClientContext
{
public:
	CRSWorldContext(CNetServer* server, PlayerID id);

	bool emit(byte id, CData& data);

public:
	///Commands

	///Send account data, 1
	void sendAcc(CAccount* acc);

	///Ask for char data, 2
	void getChar(uint id, int64 sync);

	///Ask for world server for char, 3
	void getWorld(uint id);

	///Send new character, 4
	void sendChar(PlayerOwnCharData* d, uint acc);
public:

	///Command handlers

	///Cluster manager logging in, 0
	void onLogin(CData& data);

	///Cluster manager asks for account, 1
	void onGetAccount(CData& data);

	///Cluster manager returns char, 2
	void onSendChar(CData& data);

	///Cluster manager returns world server for char, 3
	void onSendWorld(CData& data);

	///Cluster manager returns answer to new character
	void onNewCharacter(CData& data);
public:

	uint getClusterId() {return m_clusterId;}
public:

	///Cluster manager's id
	uint m_clusterId;
};


#endif