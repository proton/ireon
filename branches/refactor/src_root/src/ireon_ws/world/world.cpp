/* Copyright (C) 2005 ireon.org developers council
 * $Id: world.cpp 433 2005-12-20 20:19:15Z zak $

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
 * @file world.cpp
 * World manager
 */

#include "stdafx.h"
#include "config.h"
#include "world_app.h"
#include "world/world.h"
#include "world/world_char_player.h"
#include "world/world_char_mob.h"
#include "db/world_mob_prototype.h"
#include "resource/resource_manager.h"
#include "resource/image.h"

CWorld* CWorld::m_singleton = 0;

int64 CWorld::m_pulse = 0;

uint WorldPage::m_size = 1;
float WorldPage::m_scale = 1;

WorldPage::WorldPage(const WorldPage &p):
m_heightData(NULL)
{
	*this = p;
};

WorldPage::~WorldPage()
{
	if( m_heightData )
		delete[] m_heightData;
};

WorldPage& WorldPage::operator=(const WorldPage& p)
{
	if( m_heightData )
		delete[] m_heightData;
	m_x = p.m_x;
	m_z = p.m_z;
	if( p.m_heightData )
	{
		m_heightData = new float[m_size*m_size];
		memcpy(m_heightData,p.m_heightData,m_size*m_size*sizeof(float));
	};
	return *this;
};

float WorldPage::heightAt(const float& x, const float& z) const
{
	const float localXf = x / m_scale;
	const float localZf = z / m_scale;
	const int localX = static_cast<int>(localXf);
	const int localZ = static_cast<int>(localZf);

	assert( !(localX < 0 || localX >= m_size-1 || localZ < 0 || localZ >= m_size-1));

	// find the 4 heights around the point
	const float bottom_right_y = m_heightData[localX + localZ * m_size];
	const float bottom_left_y  = m_heightData[localX + 1 + localZ * m_size];
	const float top_right_y    = m_heightData[localX + (localZ + 1) * m_size];
	const float top_left_y     = m_heightData[localX + 1 + (localZ + 1) * m_size];

	const float z_pct = (localZf - localZ);
	float x_pct = (localXf - localX);
	 //   TL-----TR 1.0
	 //   |     / |
	 //   |    /  |  .
	 //   |   /   |  .
	 //   |  /    |  .   ^
	 //   | /     |      |
	 //   BL-----BR 0.0  z
	 //  1.0 ... 0.0
	 //
	 //      < - x

	if (x_pct > 1 - z_pct)
	{ 
		// This point is on the upper-left tri
		const float y1 = bottom_left_y * (1-z_pct) + top_left_y  * z_pct;
		const float y2 = bottom_left_y * (1-z_pct) + top_right_y * z_pct;
			if (z_pct > 0)
		x_pct = (x_pct - (1-z_pct)) / z_pct;

		return y1 * x_pct + y2 * (1-x_pct);
	} // if (x_pct > 1 - z_pct)
	else
	{ 
	    // This point is on the lower-right tri
		const float y1 = bottom_left_y  * (1-z_pct) + top_right_y * z_pct;
		const float y2 = bottom_right_y * (1-z_pct) + top_right_y * z_pct;

		if (z_pct < 1)
			x_pct = x_pct / (1-z_pct);

		return y1 * x_pct + y2 * (1-x_pct);
	}
};

void WorldPage::insertCharacter(const CharacterPtr& ch)
{
	removeCharacter(ch);
	m_players.push_back(ch);
};

void WorldPage::removeCharacter(const CharacterPtr& ch)
{
	std::list<CharacterPtr>::iterator it;
	for( it = m_players.begin(); it != m_players.end(); ++it)
		if( *it == ch )
		{
			m_players.erase(it);
			break;
		};
};

CWorld::CWorld()
{
};

CWorld::~CWorld()
{
};

bool CWorld::init()
{
	if( !loadTerrain( CWorldApp::instance()->getSetting("/config/WorldCfg")) )
		return false;

	if( !loadMobPrototypes() )
		return false;

	CharMobPtr mob(new CWorldCharMob);
	mob->m_id = 1 | MOB_ID_FLAG;
	mob->m_startPos = Vector2(-20,-60);
	mob->m_prototype = getMobPrototype(1);
	insertCharMob(mob);

	mob.reset(new CWorldCharMob);
	mob->m_id = 2 | MOB_ID_FLAG;
	mob->m_startPos = Vector2(-20,-120);
	mob->m_prototype = getMobPrototype(2);
	insertCharMob(mob);

	mob.reset(new CWorldCharMob);
	mob->m_id = 3 | MOB_ID_FLAG;
	mob->m_startPos = Vector2(50,50);
	mob->m_prototype = getMobPrototype(3);
	insertCharMob(mob);

	mob.reset(new CWorldCharMob);
	mob->m_id = 4 | MOB_ID_FLAG;
	mob->m_startPos = Vector2(20,-70);
	mob->m_prototype = getMobPrototype(4);
	insertCharMob(mob);

	mob.reset(new CWorldCharMob);
	mob->m_id = 5 | MOB_ID_FLAG;
	mob->m_startPos = Vector2(20,10);
	mob->m_prototype = getMobPrototype(5);
	insertCharMob(mob);

	return true;
};

bool CWorld::loadTerrain(const String& file)
{
	CConfig cf;
	cf.load(file);

	m_width = StringConverter::parseInt(cf.getSetting("Width"));
	m_height = StringConverter::parseInt(cf.getSetting("Height"));
	m_pageSize = StringConverter::parseInt(cf.getSetting("PageSize"));

	WorldPage::m_size = m_pageSize;

	m_pageSize--; /// pages overlap
	String name = cf.getSetting("LandScapeFileName");
	String ext = cf.getSetting("LandScapeExtension");
	float scaleX = StringConverter::parseReal(cf.getSetting("ScaleX"));
	float scaleY = StringConverter::parseReal(cf.getSetting("ScaleY"));
	float scaleZ = StringConverter::parseReal(cf.getSetting("ScaleZ"));

	if( scaleX != scaleZ )
	{
		CLog::instance()->log(CLog::msgLvlError,_("Landcape page isn't square.\n"));
		return false;
	};

	if( !m_width || !m_height || !scaleX || !scaleY || name == "" || ext == "" || !m_pageSize )
	{
		CLog::instance()->log(CLog::msgLvlError,_("Not enough options in terrain config. Must be:\n Width, Height, PageSize, ScaleX, ScaleY, ScaleZ, LandScapeFileName, LandScapeExtension.\n"));
		return false;
	};

	m_scale = (float)scaleZ / m_pageSize;
	
	WorldPage::m_scale = m_scale;

	m_invScale = (float)1/m_scale;
	m_scaledPage = m_scale * m_pageSize;
	
	m_maxUnscaledX = m_width * m_pageSize * 0.5f;
	m_maxScaledX = m_maxUnscaledX * m_scale;
	m_maxUnscaledZ = m_height * m_pageSize * 0.5f;
	m_maxScaledZ = m_maxUnscaledZ * m_scale;

	float scale = scaleY / 256; /// It must be 255, but in plsm it's 256

	CImage img;
	for( uint x = 0; x < m_width; ++x )
	 for( uint z = 0; z < m_height; ++z )
	 {
		 WorldPage page(x,z);
		 String filename = name + "." + StringConverter::toString(x)
			 + "." + StringConverter::toString(z) + "." + ext;
		 img.load(filename);
		 if( !img.getData() )
			 return false;
		 if( !img.getPixelSize() == 1 )
		 {
			 CLog::instance()->log(CLog::msgLvlError,_("Image format isn't 8 bit grayscale. File: '%s'.\n"),filename.c_str());
			 return false;
		 };
		 if( (img.getWidth() != (m_pageSize + 1)) || (img.getHeight() != (m_pageSize + 1)) )
		 {
			 CLog::instance()->log(CLog::msgLvlError,_("Terrain page must be %dx%d pixels. File: '%s'.\n"),m_pageSize + 1, m_pageSize + 1,filename.c_str());
			 return false;
		 };
		 page.m_heightData = new float[WorldPage::m_size * WorldPage::m_size];
		 for( uint i = 0; i < WorldPage::m_size; i++ )
			 for( uint j = 0; j < WorldPage::m_size; j++ )
				page.m_heightData[i + j * WorldPage::m_size] = (float)img.getData()[i + j * WorldPage::m_size] * scale;
		 m_pages.insert(page);
	 };
	return true;
};

bool CWorld::loadMobPrototypes()
{
	CConfig cf;
	cf.load("mobs.ini");
	StringVector vec = cf.getMultiSetting("Prototype");
	for( StringVector::iterator it = vec.begin(); it != vec.end(); ++it )
	{
		WorldMobProtPtr prot(new CWorldMobPrototype);
		if( prot->load(*it) )
		{
			if( m_mobPrototypes.find(prot->getId()) != m_mobPrototypes.end() )
			{
				CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlError,"Double mob prototype identifier %d!\n",prot->getId());
				continue;
			}
			m_mobPrototypes.insert(std::pair<uint,WorldMobProtPtr>(prot->getId(),prot));
			CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlInfo,"Added mob prototype with id %d.\n",prot->getId());
		}
	}
	return true;
};

WorldPage* CWorld::getPageByCoords(const float& x, const float& z)
{
	return getPage((uint)(x + m_maxScaledX) / m_scaledPage,(uint)(z + m_maxScaledZ) / m_scaledPage);
}

WorldPage* CWorld::getPage(const uint x, const uint z)
{
	static WorldPage finder(0,0);
	finder.m_x = x;
	finder.m_z = z;
	std::set<WorldPage>::iterator it = m_pages.find(finder);
	if( it != m_pages.end() )
		return const_cast<WorldPage*>(&(*it));
	return NULL;
};

float CWorld::heightAt(const float& x, const float& z) const
{
	static WorldPage finder(0,0);
	if( x + m_maxScaledX < 0 || z + m_maxScaledZ < 0 )
		return 0.0;
	finder.m_x = (uint)((x + m_maxScaledX) / m_scaledPage);
	finder.m_z = (uint)((z + m_maxScaledZ) / m_scaledPage);
	std::set<WorldPage>::const_iterator it = m_pages.find(finder);
	if( it != m_pages.end() )
		return (*it).heightAt(x + m_maxScaledX - (*it).m_x * m_scaledPage,z + m_maxScaledZ - (*it).m_z * m_scaledPage );
	return 0.0;
};

void CWorld::getPageIndices(const float& x, const float& z, uint& ix, uint& iz) const
{
	ix = (x + m_maxScaledX) / m_scaledPage;
	iz = (z + m_maxScaledZ) / m_scaledPage;
};

void CWorld::insertCharacter(const CharacterPtr &ch)
{
	ch->m_ptr = ch;
	ch->m_inWorld = true;
	ch->onEnterWorld();
};

void CWorld::insertCharPlayer(const CharPlayerPtr& ch)
{
	CLog::instance()->log(CLog::msgLvlInfo,"Insert player with id %d to world.\n",ch->getId());
	m_players.insert(std::pair<uint,CharPlayerPtr>(ch->getId(),ch));
	insertCharacter(ch);
};

void CWorld::insertCharMob(const CharMobPtr& mob)
{
	CLog::instance()->log(CLog::msgLvlInfo,"Insert mob with id %d to world.\n",mob->getId());
	m_mobs.insert(std::pair<uint,CharMobPtr>(mob->getId(),mob));
	insertCharacter(mob);
};

void CWorld::removeCharPlayer(const CharPlayerPtr& ch)
{
	removeCharPlayer(ch->getId());
};

void CWorld::removeCharPlayer(uint id)
{
	CLog::instance()->log(CLog::msgLvlInfo,"Removing character with id %d from world.\n",id);
	std::map<uint,CharPlayerPtr>::iterator it = m_players.find(id);
	if( it != m_players.end() )
	{
		(*it).second->onLeaveWorld();
		(*it).second->m_inWorld = false;
		(*it).second->m_ptr.reset();
		m_players.erase(it);
	}
};


void CWorld::removeCharMob(const CharMobPtr& mob)
{
	removeCharMob(mob->getId());
};

void CWorld::removeCharMob(uint id)
{
	CLog::instance()->log(CLog::msgLvlInfo,"Removing mob with id %d from world.\n",id);
	std::map<uint,CharMobPtr>::iterator it = m_mobs.find(id);
	if( it != m_mobs.end() )
	{
		(*it).second->onLeaveWorld();
		(*it).second->m_inWorld = false;
		(*it).second->m_ptr.reset();
		m_mobs.erase(it);
	}
};

CharacterPtr CWorld::findCharacter(uint id)
{
 if (id & MOB_ID_FLAG)
	 return findCharMob(id);
 else
	 return findCharPlayer(id);
}

CharPlayerPtr CWorld::findCharPlayer(uint id)
{
	std::map<uint,CharPlayerPtr>::iterator it = m_players.find(id);
	if( it != m_players.end() )
		return (*it).second;
	return CharPlayerPtr();
};

CharMobPtr CWorld::findCharMob(uint id)
{
	std::map<uint,CharMobPtr>::iterator it = m_mobs.find(id);
	if( it != m_mobs.end() )
		return (*it).second;
	return CharMobPtr();
};

void CWorld::update(int64 pulse)
{
	if( pulse % 5 == 0 )
	{
		std::map<uint,CharPlayerPtr>::iterator it;
		for( it = m_players.begin(); it != m_players.end(); ++it)
			(*it).second->update(PULSE_TIME * 5);
	};

	std::map<uint,CharMobPtr>::iterator mobIt;
	for( mobIt = m_mobs.begin(); mobIt != m_mobs.end(); ++mobIt )
		(*mobIt).second->update(PULSE_TIME);
};

WorldMobProtPtr CWorld::getMobPrototype(uint id)
{
	std::map<uint,WorldMobProtPtr>::iterator it = m_mobPrototypes.find(id);
	if( it != m_mobPrototypes.end() )
		return (*it).second;
	return WorldMobProtPtr();
};

