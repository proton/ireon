/**
  * @file client_world.h
  * Client world manager
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: client_world.h 864 2007-08-20 07:28:11Z mip $
 
  *  See LICENSE for details
  */
 
#ifndef _CLIENT_WORLD_H
#define _CLIENT_WORLD_H
 
#include "common/world/world.h"
#include "common/world/dynamic_object/character.h"
//#include "common/db/client_char_data.h"
#define I_WORLD (CClientWorld::instance())

class CClientZone;

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

	//OwnCharPlayerPtr getOwnChar() const {return m_ownChar;}

	//CharPlayerPtr addPlayer(ClientCharPlayerData* data);
	//OwnCharPlayerPtr createOwnCharacter(ClientOwnCharData* data);

	void updateCharactersInfo();

	void makeVisibleList(std::vector<CharacterPtr> &list, bool sorted = false);

/*	CharacterPtr findCharacter(uint id);
	CharacterPtr findCharacter(SceneNode* node) {return CWorld::findCharacter(node);}*/

	/*void showNavigationMesh();*/
	void showSimpleNavigationMesh();

	CClientZone* getZone(){return zone_;};

public:

	/// Update world
	void update(Real time);

	/// Clear world
	void clear();
private:
 	static CClientWorld* m_singleton;
	
	CClientZone* zone_;
	//OwnCharPlayerPtr m_ownChar;
};
 
 #endif
