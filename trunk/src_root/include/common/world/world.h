/**
  * @file world.h
  * World manager
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: world.h 844 2007-01-03 13:28:38Z mip $
 
  *  See LICENSE for details
  */
 
#ifndef _WORLD_H
#define _WORLD_H
#pragma warning(push, 1)
#include "Ogre/octree_scene_manager/OgreOctreeSceneManager.h"

#include "boost/shared_ptr.hpp"
#pragma warning(pop)

class CStaticObject;
typedef boost::shared_ptr<CStaticObject> StaticPtr;
class CStaticPrototype;
typedef boost::shared_ptr<CStaticPrototype> StaticPrototypePtr;
class COwnCharPlayer;
typedef boost::shared_ptr<COwnCharPlayer> OwnCharPlayerPtr;
class CCharacter;
typedef boost::shared_ptr<CCharacter> CharPlayerPtr;
class CMob;
typedef boost::shared_ptr<CMob> CharMobPtr;
class CDynamicObject;
typedef boost::shared_ptr<CDynamicObject> CharacterPtr;
class CMobPrototype;
typedef boost::shared_ptr<CMobPrototype> MobPrototypePtr;
struct ClientCharMobData;

namespace Ogre
{
 	class SceneManager;
};

 
 bool operator> (std::pair<uint,uint>& a, std::pair<uint,uint>& b);
 
 class TiXmlNode;
 
 class CWorld :
	 //public Ogre::OctreeSceneManager,
	protected CLogMixin
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
 
 	Real heightAt(const Ogre::Vector3&) const
 	{
		//return PagingLandScapeData2DManager::getSingleton().getRealWorldHeight(pos.x,pos.z);
 		//return sceneManager_->getHeightAt(pos.x,pos.z);
		return 0;
	}
	Vector3 getNormalAt(const Real&, const Real&) const
	{
	/*	return 
			sceneManager_->getData2DManager()->getNormalAt(
				sceneManager_->getPageManager()->getCurrentCameraPageX(),
				sceneManager_->getPageManager()->getCurrentCameraPageX(),
				x,
				z
				);*/
		return Vector3(1, 1, 1);
	}

	void getPageIndices(Real&, Real&, uint, uint, bool)
	{
		//sceneManager_->getPageManager()->getPageIndices(posx, posz, x, z, alwaysAnswer);
		
	}
 public:
 
 	virtual bool init();
 
 	virtual bool createScene();
 
 	/// Update world
 	virtual void update(Real time);
 
 	/// Clear world
 	virtual void clear();
	
	/// Load mob protypes from xml ../data/zones/mobs/*.mobprot
	/// in m_mobPrototypes
 	//bool loadMobPrototypes();
	/// Load static protypes from xml ../data/zones/static/*.static
	/// in m_staticPrototypes
 	//bool loadStaticPrototypes();
 
 	Ogre::SceneManager* sceneManager() const {return sceneManager_;}
 
 	CharMobPtr addMob(ClientCharMobData* data);
 	void addDead(const CharacterPtr& ch);
 
 	void removeDead(uint id);
 	void removeCharacter(uint id);
 
 	//void addStatic(StaticPtr st) {m_statics.push_back(st);}
 	/*void removeStatic(StaticPtr st) {
 		StaticVectorIterator it;
 		for( it = m_statics.begin(); it != m_statics.end(); ++it )
 			if( *it == st )
 			{
 				m_statics.erase(it);
 				break;
 			}
 	}*/
 
 	virtual CharacterPtr findCharacter(uint id);
 	virtual CharacterPtr findCharacter(SceneNode* node);
 
 	bool loaded() const {return m_loaded;}
 
 	Vector2 translateTo2d(const Vector3& coords) const;
 
 	bool pointIsVisible(const Vector3& coords);
 
 	//MobPrototypePtr getMobPrototype(String& type);
 	//StaticPrototypePtr getStaticPrototype(uint id);
 	//StaticPrototypePtr getStaticPrototype(const String& name);
 protected:
 
 	/// Add character to world, common for mob and player
 	void addCharacter(CDynamicObject* ch);
 
	void initSceneManager();
 public: /// Iterators
 
 	typedef	std::map<uint,CharacterPtr> CharMap;
 	typedef CharMap::iterator CharMapIterator;
 
 	CharMapIterator& getFirstChar() {m_charactersIter = m_characters.begin(); return m_charactersIter;}
 	CharMapIterator& getNextChar() {if( m_charactersIter != m_characters.end() ) ++m_charactersIter; return m_charactersIter;}
 	bool lastChar(CharMapIterator& it) {return (it == m_characters.end()); }
 
 	typedef std::vector<StaticPtr> StaticVector;
 	typedef StaticVector::iterator StaticVectorIterator;
 
	//StaticVector getStatics() { return m_statics;}
 	//StaticVectorIterator& getFirstStatic() {m_staticIter = m_statics.begin(); return m_staticIter;}
 	//StaticVectorIterator& getNextStatic() {if( m_staticIter != m_statics.end() ) ++m_staticIter; return m_staticIter;}
 	//bool lastStatic(StaticVectorIterator& it) {return (it == m_statics.end()); }
 
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
 	//StaticVector m_statics;
 private:
 
 	CharMapIterator m_charactersIter;
 	bool m_walkCharacters;
 	std::set<uint> m_removeCharacters;
 
 	StaticVectorIterator m_staticIter;
 
 	SceneManager* sceneManager_;
 
 	RaySceneQuery* m_query;
 
	std::map<String,MobPrototypePtr> m_mobPrototypesCache;

 	std::map<uint,StaticPrototypePtr> m_staticPrototypes;
 
 	std::list<CharacterPtr> m_deadCharacters;
 	//static CWorld* m_singleton;
 };
 
 #endif
