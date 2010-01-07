/**
 * @file client_world.h
 * Client world manager
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: world.h 510 2006-02-26 21:09:40Z zak $

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

#ifndef _CLIENT_WORLD_H
#define _CLIENT_WORLD_H

#include "world/world.h"

class CClientWorld : public CWorld
{
private:
	CClientWorld();

protected:

	void terrainReady();

public:
	~CClientWorld();

	static CClientWorld* instance() {if( !m_singleton ) m_singleton = new CClientWorld; return m_singleton;}

	bool init();

	bool createScene();

	/// Load world when logged to world server
	bool load();

	bool loadZones();

	OwnCharPlayerPtr getOwnChar() const {return m_ownChar;}

	CharPlayerPtr addPlayer(ClientCharPlayerData* data);
	OwnCharPlayerPtr createOwnCharacter(ClientOwnCharData* data);

	void updateCharactersInfo();

	void makeVisibleList(std::vector<CharacterPtr> &list, bool sorted = false);

	CharacterPtr findCharacter(uint id);
	CharacterPtr findCharacter(SceneNode* node) {return CWorld::findCharacter(node);}
public:

	/// Update world
	void update(Real time);

	/// Clear world
	void clear();
private:

	static CClientWorld* m_singleton;

	OwnCharPlayerPtr m_ownChar;
};

#endif