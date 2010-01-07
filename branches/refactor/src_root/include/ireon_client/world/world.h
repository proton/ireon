/* Copyright (C) 2005 ireon.org developers council
 * $Id: world.h 433 2005-12-20 20:19:15Z zak $

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
 * @file world.h
 * World manager
 */

#ifndef _WORLD_H
#define _WORLD_H

namespace Ogre{
	class SceneManager;
};
#include "OgrePagingLandScapeListenerManager.h"
#include "OgrePagingLandScapeData2DManager.h"
#include "OgreOde_Core.h"
#include "ireon_client/app_states/world_connection_state.h"

class CWorld :
	public Ogre::PagingLandscapeListener,
	public OgreOde::CollisionListener,
	public OgreOde::TerrainGeometryHeightListener,
	public OgreOde::StepListener
{
private:
	CWorld();

	/// Paging landscape listener
	void pagePreloaded( const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox );
	void pageLoaded( const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox  );
	void pageUnloaded( const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox  );
	void pagePostunloaded( const size_t pagex, const size_t pagez);
	void pageShow( const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox  );
	void pageHide( const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox  )  ;
	void tileLoaded( const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox  );
	void tileUnloaded( const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox  );
	void tileDeformed( const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox  );
	void tileShow( const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox  );
	void tileHide( const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox  );
	void terrainReady( void );
private:
	/// OgreOde
	bool collision(OgreOde::Contact *);

	bool preStep(Real time);

public:
	~CWorld();

	static CWorld* instance() {if( !m_singleton ) m_singleton = new CWorld; return m_singleton;}

	Real heightAt(const Ogre::Vector3 &pos)
	{
		return PagingLandScapeData2DManager::getSingleton().getRealWorldHeight(pos.x,pos.z);
		//assert(m_terrain);
		//return m_terrain->getHeightAt(pos);
	}

	bool init();

	bool createScene();

	bool loadMobPrototypes();

	/// Load world when logged to world server
	bool load(CWorldConectionState* context = NULL);

	Ogre::SceneManager* sceneManager() const {return m_sceneManager;}

	OwnCharPlayerPtr getOwnChar() const {return m_ownChar;}

	//void removeObject(const String& name);
	//void removeObject(ObjectPtr obj);

	CharPlayerPtr addPlayer(ClientCharPlayerData* data);
	CharMobPtr addMob(ClientCharMobData* data);
	OwnCharPlayerPtr createOwnCharacter(ClientOwnCharData* data);
	void removeCharacter(uint id);

	CharacterPtr findCharacter(uint id);

	bool loaded() const {return m_loaded;}

	Vector2 translateTo2d(const Vector3& coords) const;

	bool pointIsVisible(const Vector3& coords);

	MobPrototypePtr getMobPrototype(uint id);

	void updateCharactersInfo();
public:

	/// Update world
	void update(Real time);

	CWorldConectionState* getWorldContext() { return m_worldContext; }

private:

	/// Add character to world, common for mob and player
	void addCharacter(CharacterPtr ch);

private:

	//std::map<String, ObjectPtr> m_objects;
	std::map<uint,CharacterPtr> m_characters;
	OwnCharPlayerPtr m_ownChar;

	Ogre::SceneManager* m_sceneManager;

	OgreOde::World* m_physWorld;

	OgreOde::TerrainGeometry* m_terrain;

	OgreOde::Stepper* m_stepper;

	RaySceneQuery* m_query;

	/// World is loaded?
	bool m_loaded;
	
	/// pointer to main client app state, to send signals
	/// maybe should be moved to another place
	CWorldConectionState* m_worldContext;
	
	std::map<uint,MobPrototypePtr> m_mobPrototypes;

	static CWorld* m_singleton;
};

#endif