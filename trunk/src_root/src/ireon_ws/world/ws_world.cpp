/* Copyright (C) 2005 ireon.org developers council
 * $Id: ws_world.cpp 854 2007-04-09 12:45:28Z mip $

 *  See LICENSE for details
 */

/**
 * @file world.cpp
 * World manager
 */

#include "ireon_ws/stdafx.h"
#include "ireon_ws/world/ws_world.h"
/*
#include "common/config.h"
#include "ireon_ws/world_app.h"

#include "ireon_ws/world/world_char_player.h"
#include "ireon_ws/world/world_char_mob.h"
#include "ireon_ws/db/world_mob_prototype.h"
#include "common/resource/resource_manager.h"
#include "ireon_ws/resource/image.h"


#include "tinyxml/tinyxml.h"
*/

WSWorld* WSWorld::singleton_ = 0;

#if 0
//int64 WSWorld::m_pulse = 0;

/*
bool WSWorld::m_mobMovePulse;
bool WSWorld::m_playerMovePulse;
bool WSWorld::m_regenPulse;
bool WSWorld::m_processOutputPulse;
bool WSWorld::m_repopPulse;
*/
WorldPage WSWorld::m_finder(0,0);

uint WorldPage::m_size = 1;
float WorldPage::m_scale = 1;

WorldPage::WorldPage(uint x, uint z):
m_x(x),
m_z(z),
m_heightData(NULL),
m_normalData(NULL)
{
}

WorldPage::WorldPage(const WorldPage &p):
m_heightData(NULL),
m_normalData(NULL)
{
	*this = p;
};

WorldPage::~WorldPage()
{
	if( m_heightData )
		delete[] m_heightData;
	if( m_normalData )
		delete[] m_normalData;
};

WorldPage& WorldPage::operator=(const WorldPage& p)
{
	if( m_heightData )
		delete[] m_heightData;
	if( m_normalData )
		delete[] m_normalData;
	m_x = p.m_x;
	m_z = p.m_z;
	if( p.m_heightData )
	{
		m_heightData = new float[m_size*m_size];
		memcpy(m_heightData,p.m_heightData,m_size*m_size*sizeof(float));
	};
	if( p.m_normalData )
	{
		m_normalData = new Vector3[m_size*m_size*2];
		memcpy(m_normalData,p.m_normalData,m_size*m_size*sizeof(Vector3));
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

const Vector3& WorldPage::normalAt(const uint& x, const uint& z, bool upperTriangle) const
{
	assert( !(x >= m_size || z >= m_size) );
	return m_normalData[(x + z * m_size) * 2 + (upperTriangle ? 1 : 0)];
};

float WorldPage::vertexHeightAt(const uint& x, const uint& z) const
{
	assert( !(x > m_size || x > m_size));
	assert(m_heightData);
	return m_heightData[x + z * m_size];
};

void WorldPage::computeNormals()
{
	/// Computing of normals taken from PagingLandScapeSceneManager
	if( !m_normalData )
		m_normalData = new Vector3[m_size*m_size*2];

	const float divider = 1.0f / m_scale;
	//const float divider = static_cast <float> (m_size - 1) / WSWorld::instance()->getMaxHeight();
	uint x,z;
	/// Upper triangle
	for( x = 0; x < m_size; ++x )
		for( z = 0; z < m_size; ++z )
		{
			float h1,h2,h3;
			WorldPage* page;
			bool addX = false, addZ = false;
			if( x == m_size - 1 )
				addX = true;
			if( z == m_size - 1 )
				addZ = true;
			page = WSWorld::instance()->getPage(m_x + (addX ? 1 : 0), m_z + (addZ ? 1 : 0) );
			if( addZ )
			{
				h1 = page ? page->vertexHeightAt(x,0) : 0;
				if( addX )
					h3 = page ? page->vertexHeightAt(0,0) : 0;
				else
					h3 = page ? page->vertexHeightAt(x+1,0) : 0;
			}
			else
			{
				h1 = vertexHeightAt(x,z + 1);
				if( addX )
					h3 = page ? page->vertexHeightAt(0,z+1) : 0;
				else
					h3 = page ? page->vertexHeightAt(x+1,z+1) : 0;
			}
			if( addX )
				h2 = page ? page->vertexHeightAt(0,z) : 0;
			else
				h2 = vertexHeightAt(x+1,z);

			Vector3 result((h1 - h3) * divider,
							1.0f,
							(h2 - h3) * divider);

			result.normalise ();

			m_normalData[(x + z * m_size) * 2 + 1] = result;
		}
	/// Bottom triangle
	for( x = 0; x < m_size; ++x )
		for( z = 0; z < m_size; ++z )
		{
			float h1,h2,h3;
			WorldPage* page;
			bool addX = false, addZ = false;
			if( x == m_size - 1 )
				addX = true;
			if( z == m_size - 1 )
				addZ = true;
			page = WSWorld::instance()->getPage(m_x + (addX ? 1 : 0), m_z + (addZ ? 1 : 0) );
			if( addZ )
				h2 = page ? page->vertexHeightAt(x,0) : 0;
			else
				h2 = vertexHeightAt(x,z + 1);
			if( addX )
				h1 = page ? page->vertexHeightAt(0,z) : 0;
			else
				h1 = vertexHeightAt(x+1,z);
			h3 = vertexHeightAt(x,z);

			Vector3 result((h3 - h1) * divider,
							1.0f,
							(h3 - h2) * divider);

			result.normalise ();

			m_normalData[(x + z * m_size) * 2] = result;
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

// -----------------------------------------------------------
// -----------------------------------------------------------
// -----------------------------------------------------------

WSWorld::WSWorld():
m_charComposite(WSEntityPtr(new CWSEntityBase(0))),
m_worldPopulation(0)
{
};

WSWorld::~WSWorld()
{
};


uint WSWorld::getNewEntityID() 
{
	assert(m_singleton);
	return m_charComposite->getOverallSize()+1/* | MOB_ID_FLAG */;
}

WorldMobProtPtr WSWorld::getMobPrototype(String& type)
{
   std::map<String,WorldMobProtPtr>::iterator it = m_mobPrototypesCache.find(type);
	if( it != m_mobPrototypesCache.end() )
		return (*it).second;

	WorldMobProtPtr prot(new CWorldMobPrototype);
	if(prot->load(type))
	{
		m_mobPrototypesCache.insert(std::pair<String,WorldMobProtPtr>(type,prot));
		CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlInfo,__FUNCTION__,_("Mob prototype '%s' added to cache."), type.c_str());
		return prot;
	} else {
	 	return WorldMobProtPtr();
	}
};

bool WSWorld::init()
{
	m_worldPopulation = 0;
	if( !loadTerrain( WSApp::instance()->getSetting("/config/WorldCfg")) )
		return false;

	TiXmlDocument	doc;
	CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlInfo,__FUNCTION__, _("Loading mobs info..."));
	doc.LoadFile("../data/zones/teeran/mobs.xml");
	if (doc.Error())
	{
		CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlError,__FUNCTION__, _("XML parser returned an error: %s\n"), doc.ErrorDesc());
		return false;
	}


	TiXmlElement* mobsxml = doc.FirstChildElement("mobs");
	CWorldCharMob::setDefaults(mobsxml);

	TiXmlNode* childNode;
	for ( childNode = mobsxml->FirstChild(); childNode != 0; childNode = childNode->NextSibling() )
		if (childNode->Type() == TiXmlNode::ELEMENT)
			if (!strcmp(childNode->Value(), "mob"))
//			{
				insertEntity(WSEntityPtr(new CWorldCharMob(getNewEntityID(), childNode->ToElement())));
//			 	CharMobPtr mob(new CWorldCharMob(childNode->ToElement()));
//				insertCharMob(mob);
//			}
			else if (!strcmp(childNode->Value(), "flock"))
//			{
				insertEntity(WSEntityPtr(new CMobFlock(getNewEntityID(), childNode->ToElement())));
//				MobFlockPtr flock(new CMobFlock(childNode->ToElement()));
//				m_flocks.push_back(flock);
//			}

	CLog::instance()->log(CLog::msgFlagResources, CLog::msgLvlInfo,__FUNCTION__, _("WSWorld.init: Done."));
	return true;
};

bool WSWorld::loadTerrain(const String& file)
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
	m_maxHeight = StringConverter::parseReal(cf.getSetting("ScaleY"));
	float scaleZ = StringConverter::parseReal(cf.getSetting("ScaleZ"));

	if( scaleX != scaleZ )
	{
		CLog::instance()->log(CLog::msgLvlError,__FUNCTION__,_("Landcape page isn't square.\n"));
		return false;
	};

	if( !m_width || !m_height || !scaleX || !m_maxHeight || name == "" || ext == "" || !m_pageSize )
	{
		CLog::instance()->log(CLog::msgLvlError,__FUNCTION__,_("Not enough options in terrain config. Must be:\n Width, Height, PageSize, ScaleX, ScaleY, ScaleZ, LandScapeFileName, LandScapeExtension.\n"));
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

	float scale = m_maxHeight / 256; /// It must be 255, but in plsm it's 256

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
			 CLog::instance()->log(CLog::msgLvlError,__FUNCTION__,_("Image format isn't 8 bit grayscale. File: '%s'.\n"),filename.c_str());
			 return false;
		 };
		 if( (img.getWidth() != (m_pageSize + 1)) || (img.getHeight() != (m_pageSize + 1)) )
		 {
			 CLog::instance()->log(CLog::msgLvlError,__FUNCTION__,_("Terrain page must be %dx%d pixels. File: '%s'.\n"),m_pageSize + 1, m_pageSize + 1,filename.c_str());
			 return false;
		 };
		 page.m_heightData = new float[WorldPage::m_size * WorldPage::m_size];
		 for( uint i = 0; i < WorldPage::m_size; i++ )
			 for( uint j = 0; j < WorldPage::m_size; j++ )
				page.m_heightData[i + j * WorldPage::m_size] = (float)img.getData()[i + j * WorldPage::m_size] * scale;
		 page.computeNormals();
		 m_pages.insert(page);
	 };
// 	computeNormals();
	return true;
};

/*
void WSWorld::computeNormals()
{
	std::set<WorldPage>::iterator it;
	for( it = m_pages.begin(); it != m_pages.end(); ++it )
		(const_cast<WorldPage&>(*it)).computeNormals();
};
*/

WorldPage* WSWorld::getPageByCoords(const float& x, const float& z)
{
	return getPage((uint)(x + m_maxScaledX) / m_scaledPage,(uint)(z + m_maxScaledZ) / m_scaledPage);
}

WorldPage* WSWorld::getPage(const uint x, const uint z)
{
	m_finder.m_x = x;
	m_finder.m_z = z;
	std::set<WorldPage>::iterator it = m_pages.find(m_finder);
	if( it != m_pages.end() )
		return const_cast<WorldPage*>(&(*it));
	return NULL;
};

float WSWorld::heightAt(const float& x, const float& z) const
{
	WorldPage finder(0,0);
	if( x + m_maxScaledX < 0 || z + m_maxScaledZ < 0 )
		return 0.0;
	finder.m_x = (uint)((x + m_maxScaledX) / m_scaledPage);
	finder.m_z = (uint)((z + m_maxScaledZ) / m_scaledPage);
	std::set<WorldPage>::const_iterator it = m_pages.find(finder);
	if( it != m_pages.end() )
		return (*it).heightAt(x + m_maxScaledX - (*it).m_x * m_scaledPage,z + m_maxScaledZ - (*it).m_z * m_scaledPage );
	return 0.0;
};

const Vector3& WSWorld::normalAt(const float& x, const float& z) const
{
	WorldPage finder(0,0);
	if( x + m_maxScaledX < 0 || z + m_maxScaledZ < 0 )
		return Vector3::UNIT_Y;
	finder.m_x = (uint)((x + m_maxScaledX) / m_scaledPage);
	finder.m_z = (uint)((z + m_maxScaledZ) / m_scaledPage);
	std::set<WorldPage>::const_iterator it = m_pages.find(finder);
	if( it == m_pages.end() )
		return Vector3::UNIT_Y;

	float localX = x / m_scale;
	float localZ = z / m_scale;

	// adjust x and z to be local to page
	localX -= (float)finder.m_x*m_pageSize - m_maxUnscaledX;
	localZ -= (float)finder.m_z*m_pageSize - m_maxUnscaledZ;

	// make sure x and z do not go outside the world boundaries
	if (localX < 0)
		localX = 0;
	else if (localX >= m_pageSize)
		localX = m_pageSize - 1;

	if (localZ < 0)
		localZ = 0;
	else if (localZ >= m_pageSize)
		localZ = m_pageSize - 1;

	uint iX = static_cast<uint>(localX);
	uint iZ = static_cast<uint>(localZ);

	if( localX - iX > 1 - (localZ - iZ) )
		return (*it).normalAt(iX, iZ, true); /// Upper triangle
	else
		return (*it).normalAt(iX, iZ, false);/// Bottom triangle
};

void WSWorld::getPageIndices(const float& x, const float& z, uint& ix, uint& iz) const
{
	ix = (x + m_maxScaledX) / m_scaledPage;
	iz = (z + m_maxScaledZ) / m_scaledPage;
};


void WSWorld::insertEntity(const WSEntityPtr &ch)
{
	m_charComposite->addComponent(ch);
}

// -------------------------------------------------------

void WSWorld::removeEntity(const uint id)
{
	WSEntityPtr ent = findEntityById(id);
	if (ent.get())
	{
//		ent->onLeaveWorld();
		ent->markForRemoval();
	}
}

// -------------------------------------------------------

WSEntityPtr WSWorld::findEntityById(uint id)
{
	return m_charComposite->findComponentById(id);
}


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*
void WSWorld::insertCharacter(const CharacterPtr &ch)
{
	ch->m_ptr = ch;
	ch->m_inWorld = true;
	ch->onEnterWorld();
};
*/

/*
void WSWorld::insertCharPlayer(const CharPlayerPtr& ch)
{
	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,"Insert player with id %d to world.\n",ch->getId());
	m_players.insert(std::pair<uint,CharPlayerPtr>(ch->getId(),ch));
	insertCharacter(ch);
};
*/

/*
void WSWorld::insertCharMob(const CharMobPtr& mob)
{
	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,"Insert mob with id %d to world.\n",mob->getId());
	m_mobs.insert(std::pair<uint,CharMobPtr>(mob->getId(),mob));
	insertCharacter(mob);
};
*/
/*
void WSWorld::removeCharPlayer(const CharPlayerPtr& ch)
{
	removeCharPlayer(ch->getId());
};
*/
/*
void WSWorld::removeCharPlayer(uint id)
{
	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,"Removing character with id %d from world.\n",id);
	std::map<uint,CharPlayerPtr>::iterator it = m_players.find(id);
	if( it != m_players.end() )
	{
		(*it).second->onLeaveWorld();
		(*it).second->m_inWorld = false;
		(*it).second->m_ptr.reset();
		if( !m_walkingPlayers )
			m_players.erase(it);
		else
			m_removePlayers.insert(id);
	}
};
*/

/*
void WSWorld::removeCharMob(const CharMobPtr& mob)
{
	removeCharMob(mob->getId());
};
*/
/*
void WSWorld::removeCharMob(uint id)
{
	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,"Removing mob with id %d from world.\n",id);
	std::map<uint,CharMobPtr>::iterator it = m_mobs.find(id);
	if( it != m_mobs.end() )
	{
		(*it).second->onLeaveWorld();
		(*it).second->m_inWorld = false;
		(*it).second->m_ptr.reset();
		if( !m_walkingMobs )
			m_mobs.erase(it);
		else
			m_removeMobs.insert(id);
	}
};
*/

void WSWorld::addMobRepop(int64 pulse, const CharMobPtr &mob)
{
//	m_repop.insert(std::pair<int64,CharMobPtr>(pulse,mob));
};

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*
CharacterPtr WSWorld::findCharacter(uint id)
{
 if (id & MOB_ID_FLAG)
	 return findCharMob(id);
 else
	 return findCharPlayer(id);
}

CharPlayerPtr WSWorld::findCharPlayer(uint id)
{
	std::map<uint,CharPlayerPtr>::iterator it = m_players.find(id);
	if( it != m_players.end() )
		return (*it).second;
	return CharPlayerPtr();
};

CharMobPtr WSWorld::findCharMob(uint id)
{
	std::map<uint,CharMobPtr>::iterator it = m_mobs.find(id);
	if( it != m_mobs.end() )
		return (*it).second;
	return CharMobPtr();
};
*/
void WSWorld::update(uint time)
{
	m_charComposite->update(time);
/*
	if( m_pulse % 5 == 0 )
	{
		m_mobMovePulse = true;
		m_repopPulse = true;
	}
	else
	{
		m_repopPulse = false;
		m_mobMovePulse = false;
	}

	if( m_pulse % 5 == 1 )
		m_playerMovePulse = true;
	else
		m_playerMovePulse = false;

	if( m_pulse % 5 == 2 )
		m_processOutputPulse = true;
	else
		m_processOutputPulse = false;

	if( m_pulse % 50 == 0 )
		m_regenPulse = true;
	else
		m_regenPulse = false;

	m_walkingPlayers = true;
	std::map<uint,CharPlayerPtr>::iterator it;
	for( it = m_players.begin(); it != m_players.end(); ++it)
	{
		/// Update only if character is in world
		if( (*it).second->m_ptr )
			(*it).second->update(PULSE_TIME);
	}
	m_walkingPlayers = false;
	std::set<uint>::iterator setIt;
	for( setIt = m_removePlayers.begin(); setIt != m_removePlayers.end(); ++setIt )
		m_players.erase(*setIt);
	m_removePlayers.clear();


	m_walkingMobs = true;

	std::map<uint,CharMobPtr>::iterator mobIt;
	for( mobIt = m_mobs.begin(); mobIt != m_mobs.end(); ++mobIt )
		(*mobIt).second->update(PULSE_TIME);

	std::list<MobFlockPtr>::iterator flockIt;
	for( flockIt = m_flocks.begin(); flockIt != m_flocks.end(); ++flockIt )
		(*flockIt)->moveMobs(PULSE_TIME);

	m_walkingMobs = false;

	for( setIt = m_removeMobs.begin(); setIt != m_removeMobs.end(); ++setIt )
		m_mobs.erase(*setIt);
	m_removeMobs.clear();

	if( m_repopPulse )
	{
		std::multimap<int64,CharMobPtr>::iterator repop, next;
		for( repop = m_repop.begin(); repop != m_repop.end(); repop = next )
		{
			next = repop;
			++next;
			if( (*repop).first <= m_pulse )
			{
				(*repop).second->repop();
				insertCharMob((*repop).second);
				m_repop.erase(repop);
			}
		}
	}
	*/
};

// ------------------------------------------------------------
#endif

#include "ireon_ws/world_app.h"
#include "ireon_ws/xsd/config_xsd.h"
#include "ireon_ws/world/world_character.h"
#include "common/world/mesh.h"

WSWorldPage::WSWorldPage(Coord minX, Coord minY, Coord maxX, Coord maxY):
minX_(minX),
minY_(minY),
maxX_(maxX),
maxY_(maxY)
{
	charComposite_.reset(new CWSCharGroup());
};

void WSWorldPage::addEntity( WSEntityPtr ent )
{
	charComposite_->addComponent(ent);
}

// ------------------------------------------------------------

void WSWorldPage::removeEntity(WSEntityPtr ent)
{
	charComposite_->removeComponent(ent->getId());
}

//////////////////////////////////////////////////////////////////////////

void WSWorld::update(ushort time )
{
	for (std::vector< std::vector<WSWorldPage *> >::iterator it = pages_.begin(); it != pages_.end(); ++it)
		for (std::vector<WSWorldPage *>::iterator it1 = it->begin(); it1 != it->end(); ++ it1)
			(**it1).update(time);
}

// ------------------------------------------------------------

void WSWorld::addEntity( WSEntityPtr ent )
{
	Coord defx, defy;
	if (WSApp::instance()->getConfig()->playerDefaultPosition().present())
	{
		defx = WSApp::instance()->getConfig()->playerDefaultPosition().get().x();
		defy = WSApp::instance()->getConfig()->playerDefaultPosition().get().y();
	} else {
		defx = ireon::ws::CDefaultPosition::x::default_value();
		defy = ireon::ws::CDefaultPosition::y::default_value();
	}

	try
	{
		ent->setPosition(new Position(defx, defy, navigationMesh_->getTriangleForCoords(defx, defy)));
		getPageForCoords(defx, defy)->addEntity(ent);
	} catch (Mesh::ETriangleNotFound &) 
	{
		_log (_error, _loc, _("Could not add entity: triangle was not found in navigation mesh for entity default position."));
		throw EEntityInsertionError();
	}
}

// ------------------------------------------------------------

void WSWorld::removeEntity(WSEntityPtr ent)
{
	getPageForCoords(ent->getPosition()->x, ent->getPosition()->y)->removeEntity(ent);
}

// ------------------------------------------------------------

uint WSWorld::generateId()
{
	// FIXME check duplicates on ushort overflow
	static ushort id = 0;
	++id;
	return std::numeric_limits<ushort>::max() + id;
}

// ------------------------------------------------------------

const uint  PAGE_SIZE = 2;

void WSWorld::init()
{
	navigationMesh_ = new Mesh("../data/zones/teeran/navigation_mesh.xml");
	uint xCount = static_cast<uint> (ceil(abs(navigationMesh_->getMaxX() - navigationMesh_->getMinX()) / PAGE_SIZE));
	uint yCount = static_cast<uint> (ceil(abs(navigationMesh_->getMaxY() - navigationMesh_->getMinY()) / PAGE_SIZE));

	_log (_info, _loc, _("Creating world pages 2d array (size: %d x %d)."), xCount, yCount);

	// create 2d array of pages
	pages_.resize(xCount);
	for (uint i = 0; i < xCount; ++i)
	{
		Coord maxX;
		if (i != xCount-1)
			maxX = navigationMesh_->getMinX() + (i+1) * PAGE_SIZE;
		else
			maxX = navigationMesh_->getMaxX();

		pages_[i].reserve(yCount);
		for (uint j = 0; j < yCount; ++j)
		{
			Coord maxY;
			if (j != yCount-1)
				maxY = navigationMesh_->getMinY() + (j+1) * PAGE_SIZE;
			else
				maxY = navigationMesh_->getMaxY();

			pages_[i].push_back( new WSWorldPage( navigationMesh_->getMinX(), navigationMesh_->getMinY(), maxX, maxY) );
		}
	}

	// add neighbors to pages
	// TODO beautify this
	for (uint i = 0; i < xCount; ++i)
		for (uint j = 0; j < xCount; ++j)
		{
			if (j > 0)
			{
				if (i > 0)
					pages_[i][j]->addNeighbor(pages_[i-1][j-1]);

				pages_[i][j]->addNeighbor(pages_[i][j-1]);
	
				if (i < xCount-1)
					pages_[i][j]->addNeighbor(pages_[i+1][j-1]);
			}
			
			if (i > 0)
				pages_[i][j]->addNeighbor(pages_[i-1][j]);
			if (i < xCount-1)
				pages_[i][j]->addNeighbor(pages_[i+1][j]);

			if (j < yCount-1)
			{
				if (i > 0)
					pages_[i][j]->addNeighbor(pages_[i-1][j+1]);

				pages_[i][j]->addNeighbor(pages_[i][j+1]);

				if (i < xCount-1)
					pages_[i][j]->addNeighbor(pages_[i+1][j+1]);
			}
		}
}

// ------------------------------------------------------------

WSWorldPage * WSWorld::getPageForCoords(Coord x, Coord y)
{
	assert ( (navigationMesh_->getMinX() < x) && (navigationMesh_->getMaxX() > x) && 
			 (navigationMesh_->getMinY() < y) && (navigationMesh_->getMaxY() > y) &&
			 "WSWorld::getPageForCoords: coordinates are out of bounds!");
	uint i = static_cast<uint> (abs(x - navigationMesh_->getMinX()) / PAGE_SIZE);
	uint j = static_cast<uint> (abs(y - navigationMesh_->getMinY()) / PAGE_SIZE);
	return pages_[i][j];
}

// ------------------------------------------------------------




