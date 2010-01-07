/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: world.cpp 522 2006-03-06 16:49:30Z zak $

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
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeRaySceneQuery.h"

#include "world/world.h"
#include "world/character.h"
#include "world/char_player.h"
#include "world/char_mob.h"
#include "world/mob_prototype.h"
#include "config.h"
#include "interface/interface.h"
#include "world/OgreOdeMeshInformer.h"

CWorld* CWorld::m_singleton = 0;

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
	m_query = CWorld::instance()->sceneManager()->createRayQuery(ray);

	if( !loadMobPrototypes() )
		return false;

	return true;
};

bool CWorld::loadMobPrototypes()
{
	CConfig cf;
	cf.load("../data/mobs.ini");
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
	return true;
};

bool CWorld::createScene()
{
    // Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	Camera* cam = m_sceneManager->createCamera("PlayerCam");
	// Position it at 500 in Z direction
	cam->setPosition(Ogre::Vector3(-50,300,500));
	
	cam->setNearClipDistance(1);

	m_sceneManager->setSkyBox(true,"Examples/MorningSkyBox");

	return true;
};

bool CWorld::load()
{
	CLog::instance()->log(CLog::msgLvlInfo,"Loading world...\n");
	// Set ambient light
	m_sceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

   // Create a light
    Light* l = m_sceneManager->createLight("MainLight");

    l->setPosition(0,800,-600);
	l->setSpecularColour(1,0.9,0);
	CLog::instance()->log(CLog::msgLvlInfo,"Created light.\n");

	l->setCastShadows(true);

	// Create a skydome
	m_sceneManager->setSkyBox(false,"");
    m_sceneManager->setSkyDome(true, "Examples/CloudySky", 5, 8);

	m_sceneManager->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
	m_sceneManager->setShadowColour(ColourValue(0.5,0.5,0.5));
	CLog::instance()->log(CLog::msgLvlInfo,"Scene initialization completed.\n");

	m_sceneManager->setWorldGeometry("../data/world.ini");
	PagingLandscapeListenerManager::getSingleton().addListener(this);

	CLog::instance()->log(CLog::msgLvlInfo,"Loading world complete.\n");

	return true;
}

void CWorld::pagePreloaded( const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox )
{
};
void CWorld::pageLoaded( const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox  )
{
};

void CWorld::pageUnloaded( const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox  )
{
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
	if( !m_loaded )
	{
		CLog::instance()->log(CLog::msgLvlInfo,"Terrain ready.\n");
		m_loaded = true;
		m_ownChar->setPosition(Vector2(m_ownChar->getPosition().x,m_ownChar->getPosition().z));
		for( std::map<uint,CharacterPtr>::iterator it = m_characters.begin(); it != m_characters.end(); ++it )
			(*it).second->setPosition(Vector2((*it).second->getPosition().x,(*it).second->getPosition().z));
	}
};

CharPlayerPtr CWorld::addPlayer(ClientCharPlayerData* data)
{
	assert(data);
	if( findCharacter(data->m_id) )
		return CharPlayerPtr();
	CharPlayerPtr ch(new CCharPlayer(data,"Ninja.mesh"));
	addCharacter(ch);
	return ch;
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
	CEventManager::instance()->execute(CEventManager::ON_CHAR_TO_WORLD,&arg);
};

void CWorld::clear()
{
	m_characters.clear();
	m_deadCharacters.clear();
	m_removeCharacters.clear();
	m_ownChar.reset();
};

OwnCharPlayerPtr CWorld::createOwnCharacter(ClientOwnCharData *data)
{
	m_ownChar.reset(new COwnCharPlayer(data,"Ninja.mesh"));
	/// Fire event
	CharacterEventArg arg;
	arg.m_ch = m_ownChar;
	CEventManager::instance()->execute(CEventManager::ON_CHAR_TO_WORLD,&arg);
	return m_ownChar;
};

void CWorld::removeCharacter(uint id)
{
	CharacterEventArg arg;
	std::map<uint, CharacterPtr>::iterator it = m_characters.find(id);
	if( it != m_characters.end() )
	{
		///Fire event
		arg.m_ch = (*it).second;
		CEventManager::instance()->execute(CEventManager::ON_CHAR_FROM_WORLD,&arg);
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
	if( m_ownChar && m_ownChar->getId() == id )
		return m_ownChar;
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
	std::set<uint>::iterator remove;
	for( remove = m_removeCharacters.begin(); remove != m_removeCharacters.end(); ++remove )
		m_characters.erase(*remove);
	m_removeCharacters.clear();
	m_walkCharacters = false;
	std::list<CharacterPtr>::iterator dead, nextDead;;
	for( dead = m_deadCharacters.begin(); dead != m_deadCharacters.end(); dead = nextDead )
	{
		nextDead = dead;
		++nextDead;
		(*dead)->update(time);
	}
	if( m_ownChar )
		m_ownChar->update(time);
};

Vector2 CWorld::translateTo2d(const Vector3& coords) const
{
	Vector3 res = CInterface::instance()->getPlayerCam()->getViewMatrix() * coords;
	res = CInterface::instance()->getPlayerCam()->getProjectionMatrix() * res;
	if( CInterface::instance()->getPlayerCam()->getDirection().dotProduct(coords - CInterface::instance()->getPlayerCam()->getPosition()) < 0 )
		return Vector2(-1,-1);
	return Vector2(0.5f + res.x * 0.5f,0.5f - res.y * 0.5f);
};

bool CWorld::pointIsVisible(const Vector3 &point)
{
	Vector3 dir = (point - CInterface::instance()->getPlayerCam()->getPosition());
	dir *= 1/(dir.length() * 4);
	m_query->setQueryMask(::RSQ_FirstTerrain);
	m_query->setRay(Ray(CInterface::instance()->getPlayerCam()->getPosition(),dir));
	RaySceneQueryResult res = m_query->execute();
	if( !res.empty() )
		if( res.begin()->worldFragment )
		{
			if(	(point - CInterface::instance()->getPlayerCam()->getPosition()).squaredLength()
					> (res.begin()->worldFragment->singleIntersection - CInterface::instance()->getPlayerCam()->getPosition()).squaredLength() )
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

void CWorld::updateCharactersInfo()
{
	for( std::map<uint,CharacterPtr>::iterator it = m_characters.begin(); it != m_characters.end(); ++it )
	{
		CharacterEventArg arg;
		arg.m_ch = (*it).second;
		CEventManager::instance()->execute(CEventManager::ON_CHAR_INFO_UPDATE,&arg);
	};
	CharacterEventArg arg;
	arg.m_ch = m_ownChar;
	CEventManager::instance()->execute(CEventManager::ON_CHAR_INFO_UPDATE,&arg);
};

bool characterLessByScreenCoordinates(CharacterPtr ch1, CharacterPtr ch2)
{
	return CWorld::instance()->translateTo2d(ch1->getPosition()).x < CWorld::instance()->translateTo2d(ch2->getPosition()).x;
};

void CWorld::makeVisibleList(std::vector<CharacterPtr> &list, bool sorted)
{
	list.clear();
	std::map<uint,CharacterPtr>::iterator it;
	for( it = m_characters.begin(); it != m_characters.end(); ++it )
		if( (*it).second->visible() )
			list.push_back((*it).second);
	if( sorted )
		std::sort(list.begin(),list.end(),characterLessByScreenCoordinates);
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