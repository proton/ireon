/**
* @file ireon_ws/world/ws_world.h
* World manager
*/

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: ws_world.h 854 2007-04-09 12:45:28Z mip $

 * See LICENSE for details
 */

#pragma once

#if 0
#include "boost/shared_ptr.hpp"
class CWSCharacter;
typedef boost::shared_ptr<CWSCharacter> CharacterPtr;
class CWSEntityBase;
typedef boost::shared_ptr<CWSEntityBase> WSEntityPtr;
class CWorldCharMob;
typedef boost::shared_ptr<CWorldCharMob> CharMobPtr;
class CWorldMobPrototype;
typedef boost::shared_ptr<CWorldMobPrototype> WorldMobProtPtr;
struct ClientCharData;
typedef boost::shared_ptr<ClientCharData> ClientCharDataPtr;

#include <list>
#include <map>


struct WorldPage
{
	WorldPage(uint x = 0, uint z = 0);

	WorldPage(const WorldPage& p);
	WorldPage& operator=(const WorldPage& p);

	~WorldPage();

	bool operator == ( const WorldPage& p ) const {return (p.m_x == m_x && p.m_z == m_z );}
	bool operator != ( const WorldPage& p ) const {return (p.m_x != m_x || p.m_z != m_z);}
	bool operator > ( const WorldPage& p ) const {return  (m_x == p.m_x ? m_z > p.m_z : m_x > p.m_z);}
	bool operator < ( const WorldPage& p ) const {return (m_x == p.m_x ? m_z < p.m_z : m_x < p.m_x);}

	float heightAt(const float& x, const float& z) const;

	float vertexHeightAt(const uint& x, const uint& z) const;

	const Vector3& normalAt(const uint& x, const uint& z, bool upperTriangle) const;

	void removeCharacter(const CharacterPtr& ch);
	void insertCharacter(const CharacterPtr& ch);

	void computeNormals();

public:
	float* m_heightData;
	Vector3* m_normalData;

	uint m_x;
	uint m_z;

	static uint m_size;
	static float m_scale;

	///Characters in this page
	std::list<CharacterPtr> m_players;
};


class CMobFlock;
class TiXmlElement;
typedef boost::shared_ptr<CMobFlock> MobFlockPtr;
#include "ireon_ws/db/player_characters_manager.h"

class WSWorld
{
public:

	static WSWorld* instance() {if (!m_singleton) m_singleton = new WSWorld(); return m_singleton;}

	bool init();

	bool loadTerrain(const String& file);

// 	void computeNormals();

	void update(ushort time);

	float heightAt(const float& x, const float& z) const;
	const Vector3& normalAt(const float& x, const float& z) const;

//	CharacterPtr findCharacter(uint id);
//	CharPlayerPtr findCharPlayer(uint id);
//	CharMobPtr findCharMob(uint id);


	void insertEntity(const WSEntityPtr& ch);
	void removeEntity(const uint id);
	WSEntityPtr findEntityById(uint id);

//	void insertCharPlayer(const CharPlayerPtr& ch);
//	void removeCharPlayer(uint id);
//	void removeCharPlayer(const CharPlayerPtr& ch);
//	void removeCharMob(uint id);
	void addMobRepop(int64 pulse, const CharMobPtr& mob);
/*
	void insertCharMob(const CharMobPtr& mob);

	void removeCharMob(const CharMobPtr& mob);
*/

	WorldPage* getPage(const uint x, const uint z);
	WorldPage* getPageByCoords(const float& x, const float& z);

	void getPageIndices(const float& x, const float& z, uint& ix, uint& iz) const;

	float getMaxX() {return m_maxScaledX;}
	float getMaxZ() {return m_maxScaledZ;}

	float getMaxHeight() {return m_maxHeight;}

	WorldMobProtPtr getMobPrototype(String& type);
	uint getNewEntityID();

	PlayerCharactersManager& getPlayerCharactersManager();
//	static int64 m_pulse; /// Game pulse

private:
	WSWorld();
	~WSWorld();

//	void insertCharacter(const CharacterPtr& ch);

	PlayerCharactersManager	m_playerCharactersManager;

/*
	static bool m_mobMovePulse;
	static bool m_playerMovePulse;
	static bool m_regenPulse;
	static bool m_processOutputPulse;
	static bool m_repopPulse;
*/

	std::set<WorldPage> m_pages;


	/// holds all characters and character groups
	WSEntityPtr			m_charComposite;

	/// used to get new ids for chars (new id = m_worldPopulation+1)
	/// \see getNewEntityID()
	uint		m_worldPopulation;

//	std::map<uint,CharPlayerPtr> m_players;
//	std::map<uint,CharMobPtr> m_mobs;
	/** We can't remove players and mobs when we walking map
	 *  so we put their ids into set to remove later
	*/
//	bool m_walkingPlayers;
//	std::set<uint> m_removePlayers;
//	bool m_walkingMobs;
//	std::set<uint> m_removeMobs;

	/// Mobs that are waiting for repopulation
//	std::multimap<int64,CharMobPtr> m_repop;


	uint m_width;		/// Width in pages
	uint m_height;		/// Height in pages
	uint m_pageSize;	/// Size of page in verticles
	uint m_maxUnscaledX;/// Max unscaled X coordinate
	uint m_maxUnscaledZ;/// Max unscaled Z coordinate
	float m_scale;       /// Scale of coordinates
	float m_invScale;    /// Inverted scale
	float m_scaledPage;  /// Scaled page size
	float m_maxScaledX;  /// Max scaled X coordinate
	float m_maxScaledZ;  /// Max scaled Z coordinate
	float m_maxHeight;   /// Max landscape height (scaleY)

//	std::list<MobFlockPtr>			 m_flocks;

	/// for WSWorld::getPage optimization only
	static WorldPage m_finder;


	std::map<String,WorldMobProtPtr> m_mobPrototypesCache;


	static WSWorld* m_singleton;
};
#endif

#include "boost/shared_ptr.hpp"
class CWSEntityBase;
typedef boost::shared_ptr<CWSEntityBase> WSEntityPtr;
class CWSCharGroup;
typedef boost::shared_ptr<CWSCharGroup> WSGroupPtr;

#include "common/world/mesh.h"
#include "common/log_mixin.h"

class WSWorldPage
{
public:
	WSWorldPage(Coord minX, Coord minY, Coord maxX, Coord maxY);

	// default destructor is ok
	
	void addNeighbor(WSWorldPage * n) { neighbors_.push_back(n); }

	/// ads player or mob to this page
	void addEntity(WSEntityPtr ent);
	void removeEntity(WSEntityPtr ent);

	void update(ushort) {};

private:
	// no copy or assignment
	WSWorldPage(const WSWorldPage & rhs);
	WSWorldPage & operator = (const WSWorldPage & rhs);

	Coord minX_, minY_, maxX_, maxY_;

	std::list<WSWorldPage *> neighbors_;

	/// holds all characters and character groups in this page
	WSGroupPtr			charComposite_;
};

//////////////////////////////////////////////////////////////////////////

class EWSWorldError : public EWSError {};
class EEntityInsertionError : public EWSWorldError {};

class WSWorld : private CLogMixin
{
public:
	static WSWorld* instance() {if (!singleton_) singleton_ = new WSWorld();  return singleton_;}

	void init();
	/// sends update signal to all world pages
	void update(ushort time);

	/// ads entity (player, mob, group) to default location in world
	void addEntity(WSEntityPtr ent);
	void removeEntity(WSEntityPtr ent);

	/// generates unique id for entity
	uint generateId();

private:
	WSWorld(){};
	
	// no copy or assignment
	WSWorld(const WSWorld & rhs);
	WSWorld & operator = (const WSWorld & rhs);

//	CPosition getNewPosition(CPosition startPos, Vector2 speed, short time);
	WSWorldPage * getPageForCoords(Coord x, Coord y);
	std::vector< std::vector<WSWorldPage *> > pages_;

	static WSWorld* singleton_;
	Mesh *			navigationMesh_;
};

