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

/**
 * @file world.h
 * World manager
 */

#ifndef _WORLD_H
#define _WORLD_H


class CTerrain;

struct WorldPage
{
	WorldPage(uint x, uint z);

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


class CWorld
{
public:

	static CWorld* instance() {if (!m_singleton) m_singleton = new CWorld(); return m_singleton;}

	bool init();

	bool loadTerrain(const String& file);

	void computeNormals();

	bool loadMobPrototypes();

	void update(int64 pulse);

	float heightAt(const float& x, const float& z) const;
	const Vector3& normalAt(const float& x, const float& z) const;

	CharacterPtr findCharacter(uint id);
	CharPlayerPtr findCharPlayer(uint id);
	CharMobPtr findCharMob(uint id);

	WorldMobProtPtr getMobPrototype(uint id);

	void insertCharPlayer(const CharPlayerPtr& ch);
	void insertCharMob(const CharMobPtr& mob);
	void addMobRepop(int64 pulse, const CharMobPtr& mob);

	void removeCharPlayer(uint id);
	void removeCharPlayer(const CharPlayerPtr& ch);
	void removeCharMob(uint id);
	void removeCharMob(const CharMobPtr& mob);

	WorldPage* getPage(const uint x, const uint z);
	WorldPage* getPageByCoords(const float& x, const float& z);

	void getPageIndices(const float& x, const float& z, uint& ix, uint& iz) const;

	float getMaxX() {return m_maxScaledX;}
	float getMaxZ() {return m_maxScaledZ;}

	float getMaxHeight() {return m_maxHeight;}
protected:
	CWorld();
	~CWorld();

	void insertCharacter(const CharacterPtr& ch);
public:

		static int64 m_pulse; /// Game pulse

		static bool m_mobMovePulse;
		static bool m_playerMovePulse;
		static bool m_regenPulse;
		static bool m_processOutputPulse;
		static bool m_repopPulse;
protected:

	std::set<WorldPage> m_pages;

	std::map<uint,CharPlayerPtr> m_players;
	std::map<uint,CharMobPtr> m_mobs;
	/** We can't remove players and mobs when we walking map
	 *  so we put their ids into set to remove later
	*/
	bool m_walkingPlayers;
	std::set<uint> m_removePlayers;
	bool m_walkingMobs;
	std::set<uint> m_removeMobs;
	
	/// Mobs that are waiting for repopulation
	std::multimap<int64,CharMobPtr> m_repop;


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

	std::map<uint,WorldMobProtPtr> m_mobPrototypes;

	static CWorld* m_singleton;
};

#endif
