/**
 * @file world.h
 * World manager
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

#ifndef _WORLD_H
#define _WORLD_H

namespace Ogre{
	class SceneManager;
};
#include "OgrePagingLandScapeListenerManager.h"
#include "OgrePagingLandScapeData2DManager.h"

bool operator> (std::pair<uint,uint>& a, std::pair<uint,uint>& b);

class TiXmlNode;

class CWorld :
	public Ogre::PagingLandscapeListener
{
protected:
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
public:
	~CWorld();

	//static CWorld* instance() {if( !m_singleton ) m_singleton = new CWorld; return m_singleton;}

	Real heightAt(const Ogre::Vector3 &pos)
	{
		return PagingLandScapeData2DManager::getSingleton().getRealWorldHeight(pos.x,pos.z);
	}

public:

	virtual bool init();

	virtual bool createScene();

	/// Update world
	virtual void update(Real time);

	/// Clear world
	virtual void clear();

	bool loadMobPrototypes();
	bool loadStaticPrototypes();

	Ogre::SceneManager* sceneManager() const {return m_sceneManager;}

	CharMobPtr addMob(ClientCharMobData* data);
	void addDead(const CharacterPtr& ch);

	void removeDead(uint id);
	void removeCharacter(uint id);

	void addStatic(StaticPtr st) {m_statics.push_back(st);}
	void removeStatic(StaticPtr st) {
		StaticVectorIterator it;
		for( it = m_statics.begin(); it != m_statics.end(); ++it )
			if( *it == st )
			{
				m_statics.erase(it);
				break;
			}
	}

	virtual CharacterPtr findCharacter(uint id);
	virtual CharacterPtr findCharacter(SceneNode* node);

	bool loaded() const {return m_loaded;}

	Vector2 translateTo2d(const Vector3& coords) const;

	bool pointIsVisible(const Vector3& coords);

	MobPrototypePtr getMobPrototype(uint id);
	StaticPrototypePtr getStaticPrototype(uint id);
	StaticPrototypePtr getStaticPrototype(const String& name);
protected:

	/// Add character to world, common for mob and player
	void addCharacter(CharacterPtr ch);

public: /// Iterators

	typedef	std::map<uint,CharacterPtr> CharMap;
	typedef CharMap::iterator CharMapIterator;

	CharMapIterator& getFirstChar() {m_charactersIter = m_characters.begin(); return m_charactersIter;}
	CharMapIterator& getNextChar() {if( m_charactersIter != m_characters.end() ) ++m_charactersIter; return m_charactersIter;}
	bool lastChar(CharMapIterator& it) {return (it == m_characters.end()); }

	typedef std::vector<StaticPtr> StaticVector;
	typedef StaticVector::iterator StaticVectorIterator;

	StaticVectorIterator& getFirstStatic() {m_staticIter = m_statics.begin(); return m_staticIter;}
	StaticVectorIterator& getNextStatic() {if( m_staticIter != m_statics.end() ) ++m_staticIter; return m_staticIter;}
	bool lastStatic(StaticVectorIterator& it) {return (it == m_statics.end()); }

protected:

	/// World is loaded?
	bool m_loaded;
	std::map<uint,CharacterPtr> m_characters;

	/* Vector of static objects
	 * use functions addStatic and removeStatic to change
	 * this vector
	 * use functions getFirstStatic, getNextStatic, lastStatic
	 * to walk through this vector
	 */
	StaticVector m_statics;
private:

	CharMapIterator m_charactersIter;
	bool m_walkCharacters;
	std::set<uint> m_removeCharacters;

	StaticVectorIterator m_staticIter;

	SceneManager* m_sceneManager;

	RaySceneQuery* m_query;

	std::map<uint,MobPrototypePtr> m_mobPrototypes;
	std::map<uint,StaticPrototypePtr> m_staticPrototypes;

	std::list<CharacterPtr> m_deadCharacters;
	//static CWorld* m_singleton;
};

#endif