/**
 * @file world.cpp
 * World manager
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: world.cpp 510 2006-02-26 21:09:40Z zak $

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
#include "stdafx.h"
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeRaySceneQuery.h"

#include "world/character.h"
#include "world/char_mob.h"
#include "world/mob_prototype.h"
#include "world/client_static.h"
#include "world/client_static_prototype.h"
#include "config.h"
#include "world/OgreOdeMeshInformer.h"
#include "resource/resource_manager.h"
#include "tinyxml/tinyxml.h"
#include "db/client_char_data.h"

bool operator> (std::pair<uint,uint>& a, std::pair<uint,uint>& b)
{
	return ((a.first > b.first) ? true : (a.second > b.second));
}

CWorld::CWorld():
m_sceneManager(NULL),
m_loaded(false),
m_query(NULL)
{
};

CWorld::~CWorld()
{
	if( m_query && m_sceneManager )
		m_sceneManager->destroyQuery(m_query);
};

bool CWorld::init()
{
	m_sceneManager = Ogre::Root::getSingleton().getSceneManager(Ogre::ST_EXTERIOR_REAL_FAR);

	if( !createScene() )
		return false;

	Ray ray;
	m_query = sceneManager()->createRayQuery(ray);

	if( !loadMobPrototypes() )
		return false;

	if( !loadStaticPrototypes() )
		return false;

	return true;
};

bool CWorld::loadMobPrototypes()
{
	CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlInfo,"Start loading mob prototypes\n");
	CConfig cf;
	cf.load(I_APP->getSetting("/config/world/ZonesDir") + "mobs.ini");
	StringVector vec = cf.getMultiSetting("Prototype");
	for( StringVector::iterator it = vec.begin(); it != vec.end(); ++it )
	{
		MobPrototypePtr prot(new CMobPrototype);
		if( prot->load(*it) )
		{
			if( m_mobPrototypes.find(prot->getId()) != m_mobPrototypes.end() )
			{
				CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlError,"Double mob prototype identifier %d!\n",prot->getId());
				continue;
			}
			m_mobPrototypes.insert(std::pair<uint,MobPrototypePtr>(prot->getId(),prot));
			CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlInfo,"Added mob prototype with id %d.\n",prot->getId());
		}
	}
	CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlInfo,"Mob prototypes loaded\n");
	return true;
};

bool CWorld::loadStaticPrototypes()
{
	CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlInfo,"Start loading static prototypes\n");
	CConfig cf;
	cf.load(I_APP->getSetting("/config/world/ZonesDir") + "static.ini");
	StringVector vec = cf.getMultiSetting("Prototype");
	for( StringVector::iterator it = vec.begin(); it != vec.end(); ++it )
	{
		StaticPrototypePtr prot(new CClientStaticPrototype);
		if( prot->load(*it) )
		{
			if( m_staticPrototypes.find(prot->getId()) != m_staticPrototypes.end() )
			{
				CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlError,"Double static prototype identifier %d!\n",prot->getId());
				continue;
			}
			m_staticPrototypes.insert(std::pair<uint,StaticPrototypePtr>(prot->getId(),prot));
			CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlInfo,"Added static prototype with id %d.\n",prot->getId());
		}
	}
	CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlInfo,"Static prototypes loaded\n");
	return true;
};


bool CWorld::createScene()
{
	return true;
};

void CWorld::pagePreloaded( const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox )
{
};

void CWorld::pageLoaded( const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox  )
{
	std::pair<uint,uint> page(pagex,pagez);
	for( CWorld::StaticVectorIterator it = m_statics.begin(); it != m_statics.end(); ++it )
		if( (*it)->getPage() == page )
		{
			(*it)->load();
		}
};

void CWorld::pageUnloaded( const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox  )
{
	std::pair<uint,uint> page(pagex,pagez);
	for( CWorld::StaticVectorIterator it = m_statics.begin(); it != m_statics.end(); ++it )
		if( (*it)->getPage() == page )
		{
			(*it)->unload();
		}
};

void CWorld::pagePostunloaded( const size_t pagex, const size_t pagez)
{
};

void CWorld::pageShow( const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox  )
{
};

void CWorld::pageHide( const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox  )
{
};

void CWorld::tileLoaded( const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox  )
{
};

void CWorld::tileUnloaded( const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox  )
{
};

void CWorld::tileDeformed( const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox  )
{
};

void CWorld::tileShow( const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox  )
{
};

void CWorld::tileHide( const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox  )
{
};

void CWorld::terrainReady( void )
{
};

CharMobPtr CWorld::addMob(ClientCharMobData* data)
{
	assert(data);
	if( findCharacter(data->m_id) )
		return CharMobPtr();
	MobPrototypePtr prot = getMobPrototype(data->m_prototype);
	if( !prot )
		return CharMobPtr();
	CharMobPtr mob(new CCharMob(data,prot));
	addCharacter(mob);
	return mob;
};

void CWorld::addCharacter(CharacterPtr ch)
{
	ch->setPosition(Vector2::ZERO);
	m_characters.insert(std::pair<uint,CharacterPtr>(ch->getId(),ch));
	CLog::instance()->log(CLog::msgLvlInfo,_("Character with id %d added to world.\n"),ch->getId());
	///Fire event
	CharacterEventArg arg;
	arg.m_ch = ch;
	CEventManager::instance()->execute(ON_CHAR_TO_WORLD,&arg);
};

void CWorld::clear()
{
	m_removeCharacters.clear();
	m_characters.clear();
};

void CWorld::removeCharacter(uint id)
{
	CharacterEventArg arg;
	std::map<uint, CharacterPtr>::iterator it = m_characters.find(id);
	if( it != m_characters.end() )
	{
		///Fire event
		arg.m_ch = (*it).second;
		CEventManager::instance()->execute(ON_CHAR_FROM_WORLD,&arg);
		(*it).second->stopFight();
		///Erase character
		if( !m_walkCharacters )
			m_characters.erase(it);
		else
			m_removeCharacters.insert((*it).second->getId());
		CLog::instance()->log(CLog::msgLvlInfo,_("Character with id %d removed from world.\n"),id);
	}
};

CharacterPtr CWorld::findCharacter(uint id)
{
	std::map<uint,CharacterPtr>::iterator it = m_characters.find(id);
	if( it != m_characters.end() )
		return (*it).second;
	return CharacterPtr();
};

CharacterPtr CWorld::findCharacter(SceneNode* node)
{
	for( std::map<uint,CharacterPtr>::iterator it = m_characters.begin(); it != m_characters.end(); ++it )
		if( (*it).second->getSceneNode() == node )
			return (*it).second;
	return CharacterPtr();
};

void CWorld::update(Real time)
{
	/// Updating characters
	std::map<uint, CharacterPtr>::iterator it;
	m_walkCharacters = true;
	for( it = m_characters.begin(); it != m_characters.end(); ++it )
		(*it).second->update(time);
	std::list<CharacterPtr>::iterator dead, nextDead;;
	for( dead = m_deadCharacters.begin(); dead != m_deadCharacters.end(); dead = nextDead )
	{
		nextDead = dead;
		++nextDead;
		(*dead)->update(time);
	}
	std::set<uint>::iterator remove;
	for( remove = m_removeCharacters.begin(); remove != m_removeCharacters.end(); ++remove )
		m_characters.erase(*remove);
	m_removeCharacters.clear();
	m_walkCharacters = false;
};

Vector2 CWorld::translateTo2d(const Vector3& coords) const
{
	Vector3 res = I_INTERFACE->getPlayerCam()->getViewMatrix() * coords;
	res = I_INTERFACE->getPlayerCam()->getProjectionMatrix() * res;
	if( I_INTERFACE->getPlayerCam()->getDirection().dotProduct(coords - I_INTERFACE->getPlayerCam()->getPosition()) < 0 )
		return Vector2(-1,-1);
	return Vector2(0.5f + res.x * 0.5f,0.5f - res.y * 0.5f);
};

bool CWorld::pointIsVisible(const Vector3 &point)
{
	Vector3 dir = (point - I_INTERFACE->getPlayerCam()->getPosition());
	Real distance2 = (point - I_INTERFACE->getPlayerCam()->getPosition()).squaredLength();
	//Real distance = sqrt(distance2);
	dir *= 1/(dir.length() * 4);
	m_query->setQueryMask(::RSQ_FirstTerrain);
	m_query->setRay(Ray(I_INTERFACE->getPlayerCam()->getPosition(),dir));
	RaySceneQueryResult res = m_query->execute();
	if( !res.empty() )
		if( res.begin()->worldFragment )
			if(	distance2 > (res.begin()->worldFragment->singleIntersection - I_INTERFACE->getPlayerCam()->getPosition()).squaredLength() )
				return false;
	m_query->setQueryMask(::RSQ_Entities);
	m_query->setSortByDistance(true);
	res = m_query->execute();
	for( RaySceneQueryResult::iterator it = res.begin(); it != res.end(); ++it )
		if( (*it).movable && (*it).distance > 0 && (*it).movable->getBoundingRadius() > CHAR_SIZE * 2 )
		{
				// *0.25f because query returns result in units of ray direction
				if( (*it).distance < 0 )
					int i = 0;
				else if( Math::Sqr((*it).distance*0.25f) < distance2 )
					return false;
		}
	Vector2 screen = translateTo2d(point);
	if( screen.x < 0 || screen.x > 1 || screen.y < 0 || screen.y > 1 )
		return false;
	return true;
};

MobPrototypePtr CWorld::getMobPrototype(uint id)
{
	std::map<uint, MobPrototypePtr>::iterator it = m_mobPrototypes.find(id);
	if( it != m_mobPrototypes.end() )
		return (*it).second;
	return MobPrototypePtr();
};

StaticPrototypePtr CWorld::getStaticPrototype(uint id)
{
	std::map<uint, StaticPrototypePtr>::iterator it = m_staticPrototypes.find(id);
	if( it != m_staticPrototypes.end() )
		return (*it).second;
	return StaticPrototypePtr();
};

StaticPrototypePtr CWorld::getStaticPrototype(const String& name)
{
	std::map<uint, StaticPrototypePtr>::iterator it;
	for( it = m_staticPrototypes.begin(); it != m_staticPrototypes.end(); ++it )
		if( name == (*it).second->getName() )
			return (*it).second;
	return StaticPrototypePtr();
};

void CWorld::addDead(const CharacterPtr& ch)
{
	m_deadCharacters.push_back(ch);
};

void CWorld::removeDead(uint id)
{
	std::list<CharacterPtr>::iterator it;
	for( it = m_deadCharacters.begin(); it != m_deadCharacters.end(); ++it )
		if( (*it)->getId() == id )
		{
			m_deadCharacters.erase(it);
			return;
		};
};