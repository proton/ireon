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

#include "Ogre.h"
#include "paging_landscape/OgrePagingLandScapeOptions.h"
#include "paging_landscape/OgrePagingLandScapeSceneManager.h"

#include "stdafx.h"
#include "world/world.h"
#include "world/character.h"
#include "world/char_player.h"
#include "world/char_mob.h"
#include "world/mob_prototype.h"
#include "config.h"
#include "interface/interface.h"

CWorld* CWorld::m_singleton = 0;

CWorld::CWorld():
m_sceneManager(NULL),
m_terrain(NULL),
m_physWorld(NULL),
m_stepper(NULL),
m_loaded(false),
m_query(NULL)
{
};

CWorld::~CWorld()
{
	if( m_terrain )
		delete m_terrain;
	if( m_physWorld )
		delete m_physWorld;
	if( m_stepper )
		delete m_stepper;
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

bool CWorld::load(CWorldConectionState* context)
{
	CLog::instance()->log(CLog::msgLvlInfo,"Loading world...\n");
	if (!context)
		CLog::instance()->log(CLog::msgLvlWarn,"Context was not set!\n");
	m_worldContext = context;
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

	m_physWorld = new OgreOde::World(m_sceneManager);
	m_physWorld->setGravity(Vector3(0,-9.81,0));
	m_physWorld->setCFM(10e-5);
	m_physWorld->setERP(0.95);
	m_physWorld->setAutoSleep(true);
	m_physWorld->setContactCorrectionVelocity(1.0);
	m_physWorld->setShowDebugObjects(false);
	m_physWorld->setCollisionListener(this);

	m_stepper = new OgreOde::ForwardFixedQuickStepper(0.01);
	m_stepper->setAutomatic(OgreOde::Stepper::AutoMode_PostFrame,&Root::getSingleton());
	m_stepper->setStepListener(this);
	CLog::instance()->log(CLog::msgLvlInfo,"OgreOde world initialization completed.\n");

	CConfig conf;
	conf.load(CClientApp::instance()->getSetting("/config/world/config").c_str());
	if( conf.getSetting("ScaleX") == "" )
	{
		CLog::instance()->log(CLog::msgLvlError,_("Error: Landscape config file hasn't option 'ScaleX'.\n"));
		return false;
	}
	Real sideLen = atof(conf.getSetting("ScaleX").c_str());
	if( conf.getSetting("ScaleZ") != conf.getSetting("ScaleX"))
	{
		CLog::instance()->log(CLog::msgLvlError,_("Error: Landscape config has different scales fo X and Z.\n"));
		return false;
	}
	uint perSide = atoi(conf.getSetting("PageSize").c_str());
	perSide--;
	Real maxHeight = atof(conf.getSetting("ScaleY").c_str());

	m_terrain = new OgreOde::TerrainGeometry(perSide,sideLen,maxHeight,m_physWorld->getDefaultSpace());
	m_terrain->setHeightListener(this);
	CLog::instance()->log(CLog::msgLvlInfo,"Created terrain.\n");

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

bool CWorld::collision(OgreOde::Contact *contact)
{
	CObject::Type type1 = CObject::NONE;
	CObject::Type type2 = CObject::NONE;
	if( contact->getFirstGeometry()->getObject() )
		type1 = contact->getFirstGeometry()->getObject()->getType();
	if( contact->getSecondGeometry()->getObject() )
		type2 = contact->getSecondGeometry()->getObject()->getType();
	if( type1 != CObject::NONE )
		return contact->getFirstGeometry()->getObject()->processCollision(contact);
	if( type2 != CObject::NONE )
		return contact->getSecondGeometry()->getObject()->processCollision(contact);
	return false;
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

OwnCharPlayerPtr CWorld::createOwnCharacter(ClientOwnCharData *data)
{
	OwnCharPlayerPtr ch(new COwnCharPlayer(data,"Ninja.mesh"));
	m_ownChar = ch;
	/// Fire event
	CharacterEventArg arg;
	arg.m_ch = ch;
	CEventManager::instance()->execute(CEventManager::ON_CHAR_TO_WORLD,&arg);
	return ch;
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
		///Erase character
		m_characters.erase(it);
		CLog::instance()->log(CLog::msgLvlInfo,_("Character with id %d removed from world.\n"),id);
	}
};

bool CWorld::preStep(Real time)
{
	return true;
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

void CWorld::update(Real time)
{
	/// Updating characters
	std::map<uint, CharacterPtr>::iterator it;
	for( it = m_characters.begin(); it != m_characters.end(); ++it )
		(*it).second->update(time);
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
	m_query->setQueryMask(RSQ_FirstTerrain);
	m_query->setRay(Ray(CInterface::instance()->getPlayerCam()->getPosition(),dir));
	RaySceneQueryResult res = m_query->execute();
	if( !res.empty() )
		if( res.begin()->worldFragment )
		{
			if(	(point - CInterface::instance()->getPlayerCam()->getPosition()).squaredLength()
					> (res.begin()->worldFragment->singleIntersection - CInterface::instance()->getPlayerCam()->getPosition()).squaredLength() )
				return false;
		}
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
};