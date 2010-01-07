/**
  * @file world.cpp
  * World manager
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: world.cpp 855 2007-04-27 09:33:36Z mip $
 
  *  See LICENSE for details
  */
#include "stdafx.h"

#include "common/world/managers/dynamic_collection.h"
#include "common/world/managers/static_collection.h"
#include "common/world/managers/prototype_manager.h"
#include "common/world/managers/object_factory.h"
#include "common/world/managers/command.h"
#include "common/world/managers/dispatcher.h"

#include "common/world/world.h"
#include "common/interface/event_manager.h"
#include "ireon_client/interface/client_interface.h"
#include "common/world/dynamic_object/character.h"
#include "common/world/dynamic_object/mob.h"
#include "common/world/static_object/static.h"

#include "common/world/OgreOdeMeshInformer.h"
#include "common/resource/resource_manager.h"
//#include "tinyxml/tinyxml.h"
#include "Ogre/octree_scene_manager/OgreOctreeSceneManager.h"
#pragma warning(push, 1)
#include "boost/lambda/lambda.hpp"
#include "boost/lambda/bind.hpp"
#pragma warning(pop)
bool operator> (std::pair<uint,uint>& a, std::pair<uint,uint>& b)
{
	return ((a.first > b.first) ? true : (a.second > b.second));
}

CWorld::CWorld():
sceneManager_(NULL),
m_loaded(false),
m_query(NULL)
{
};
 
 CWorld::~CWorld()
{
	if( m_query && sceneManager_ )
		sceneManager_->destroyQuery(m_query);
};
 
bool CWorld::init()
{
	initSceneManager();

 	Ray ray;
 	m_query = sceneManager()->createRayQuery(ray);

	CStaticCollection::getSingletonPtr()->init();
	CDynamicCollection::getSingletonPtr()->init();
	CPrototypeManager::getSingletonPtr()->init();
	CObjectFactory::getSingletonPtr()->registerProduct("ownPlayer",(productCallbackPtr)COwnPlayer::productCallback);
	CObjectFactory::getSingletonPtr()->registerProduct("static",(productCallbackPtr)CStaticObject::productCallback);

	CDispatcher::getSingletonPtr()->executeNow(CommandPtr(new CLoadOwnPlayerPrototypeCommand));
	//CDispatcher::getSingletonPtr()->executeNow(CommandPtr(new CLoadStaticPrototypesCommand));
 	
	if( !createScene() )
		return false;

	return true;
};
//----------------------- Init scene manager - PLSM2 -------------------------------------------
void CWorld::initSceneManager()
{
	_log(_info, _loc, _("Initializing scene manager - Octree Scene Manager") );
	bool found = false;
	// Get the SceneManager, in this case the Octree Scene Manager specialization
	SceneManagerEnumerator::MetaDataIterator
		it = Ogre::Root::getSingletonPtr()->getSceneManagerMetaDataIterator();

	while (it.hasMoreElements ())
	{
		const SceneManagerMetaData* metaData = it.getNext ();
		 /// A mask describing which sorts of scenes this manager can handle
		if (
			// this is not work.. ST_GENERIC = 1, but sceneTypeMask = 65535 (uint(-1)) 
			//metaData->sceneTypeMask == ST_GENERIC && 
			//metaData->worldGeometrySupported == true &&
			metaData->typeName == "OctreeSceneManager")
		{
			found = true;
			break;
		}
	}
	if (!found)
	{
		 OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Could not find OctreeSceneManager plugin. Check if it's in plugins.cfg.",
			"initSceneManager");
	}

	sceneManager_ =  Ogre::Root::getSingletonPtr()->createSceneManager("OctreeSceneManager", "IreonSceneManager" ) ;
}
/*
 bool CWorld::loadMobPrototypes()
 {
 	CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlInfo,__FUNCTION__,"Start loading mob prototypes\n");
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
 				CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlError,__FUNCTION__,"Double mob prototype identifier %d!\n",prot->getId());
 				continue;
 			}
 			m_mobPrototypes.insert(std::pair<uint,MobPrototypePtr>(prot->getId(),prot));
 			CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlInfo,__FUNCTION__,"Added mob prototype with id %d.\n",prot->getId());
 		}
 	}
 	CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlInfo,__FUNCTION__,"Mob prototypes loaded\n");
 	return true;
 };
*/
/*
 bool CWorld::loadStaticPrototypes()
 {
 	CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlInfo,__FUNCTION__,"Start loading static prototypes\n");
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
 				CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlError,__FUNCTION__,"Double static prototype identifier %d!\n",prot->getId());
 				continue;
 			}
 			m_staticPrototypes.insert(std::pair<uint,StaticPrototypePtr>(prot->getId(),prot));
 			CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlInfo,__FUNCTION__,"Added static prototype with id %d.\n",prot->getId());
 		}
 	}
 	CLog::instance()->log(CLog::msgFlagResources,CLog::msgLvlInfo,__FUNCTION__,"Static prototypes loaded\n");
 	return true;
 };
 
 */
 bool CWorld::createScene()
 {
 	return true;
 };
 
 void CWorld::pagePreloaded( const size_t , const size_t , const Real* , const AxisAlignedBox& )
 {
 };
 
 void CWorld::pageLoaded( const size_t pagex, const size_t pagez, const Real* , const AxisAlignedBox& )
 {
 	std::pair<uint,uint> page(pagex,pagez);
	using namespace boost::lambda;
	//boost::lambda::placeholder1_type _1;
	
	boost::function<bool(CStaticObject)> f = ( (bind(&CStaticObject::getPage, _1)) == page );
	

	CDispatcher::getSingletonPtr()->addCommand(CommandPtr( new CLoadAllStaticObjectCommand( f )));
 /*   for( it = m_statics.begin(); it != m_statics.end(); ++it )
 		if( (*it)->getPage() == page )
 		{
 			(*it)->load();
 		}
 */
 };
 
 void CWorld::pageUnloaded( const size_t pagex, const size_t pagez, const Real* , const AxisAlignedBox&  )
 {
 	std::pair<uint,uint> page(pagex,pagez);

	using namespace boost::lambda;
	//boost::lambda::placeholder1_type _1;

	boost::function<bool(CStaticObject)> f = ( (bind(&CStaticObject::getPage, _1)) == page );


	CDispatcher::getSingletonPtr()->addCommand(CommandPtr( new CUnloadAllStaticObjectCommand( f )));
/* 	for( CWorld::StaticVectorIterator it = m_statics.begin(); it != m_statics.end(); ++it )
 		if( (*it)->getPage() == page )
 		{
 			(*it)->unload();
 		}
 */
 };
 
 void CWorld::pagePostunloaded( const size_t , const size_t )
 {
 };
 
 void CWorld::pageShow( const size_t , const size_t , const Real*, const AxisAlignedBox&  )
 {
 };
 
 void CWorld::pageHide( const size_t , const size_t , const Real* , const AxisAlignedBox&  )
 {
 };
 
 void CWorld::tileLoaded( const size_t , const size_t , const size_t, const size_t , const AxisAlignedBox&  )
 {
 };
 
 void CWorld::tileUnloaded( const size_t , const size_t , const size_t, const size_t , const AxisAlignedBox&  )
 {
 };
 
 void CWorld::tileDeformed( const size_t , const size_t , const size_t, const size_t , const AxisAlignedBox& )
 {
 };
 
 void CWorld::tileShow( const size_t , const size_t , const size_t, const size_t , const AxisAlignedBox&  )
 {
 };
 
 void CWorld::tileHide( const size_t , const size_t , const size_t, const size_t , const AxisAlignedBox&  )
 {
 };
 
 void CWorld::terrainReady( void )
 {
 };
 
 CharMobPtr CWorld::addMob(ClientCharMobData* )
 {
 	/*assert(data);
 	if( findCharacter(data->m_id) )
 		return CharMobPtr();
 	MobPrototypePtr prot = getMobPrototype(data->m_prototype);
 	if( !prot )
 		return CharMobPtr();
 	CharMobPtr mob(new CCharMob(data,prot));
 	addCharacter(mob);
 	return mob;*/
	return CharMobPtr();
 };
 
 void CWorld::addCharacter(CDynamicObject* ch)
 {
 	//ch->setPosition2D(Vector2::ZERO);
 	//m_characters.insert(std::pair<uint,CharacterPtr>(ch->getId(),ch));
 	_log(_info,_loc,_("Character with id %d added to world.\n"),ch->getId());
 	///Fire event
 	//CharacterEventArg arg;
 	//arg.m_ch = ch;
 	//CEventManager::instance()->execute(ON_CHAR_TO_WORLD,&arg);
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
 		_log(_info,_loc,_("Character with id %d removed from world.\n"),id);
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
 	//m_query->setQueryMask(::RSQ_FirstTerrain);
 	m_query->setRay(Ray(I_INTERFACE->getPlayerCam()->getPosition(),dir));
 	RaySceneQueryResult res = m_query->execute();
 	if( !res.empty() )
 		if( res.begin()->worldFragment )
 			if(	distance2 > (res.begin()->worldFragment->singleIntersection - I_INTERFACE->getPlayerCam()->getPosition()).squaredLength() )
 				return false;
 	//m_query->setQueryMask(::RSQ_Entities);
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
 
 /*MobPrototypePtr CWorld::getMobPrototype(String& type)
 {
    std::map<String,MobPrototypePtr>::iterator it = m_mobPrototypesCache.find(type);
 	if( it != m_mobPrototypesCache.end() )
 		return (*it).second;

	MobPrototypePtr prot(new CMobPrototype);
	if(prot->load(type))
	{
		m_mobPrototypesCache.insert(std::pair<String,MobPrototypePtr>(type,prot));
		_log(_info,_loc,_("Mob prototype '%s' added to cache."), type.c_str());
		return prot;
	} else {
	 	return MobPrototypePtr();
	}
 };*/
 
/* StaticPrototypePtr CWorld::getStaticPrototype(uint id)
 {
 	std::map<uint, StaticPrototypePtr>::iterator it = m_staticPrototypes.find(id);
 	if( it != m_staticPrototypes.end() )
 		return (*it).second;
 	return StaticPrototypePtr();
 };*/
 
 /*StaticPrototypePtr CWorld::getStaticPrototype(const String& name)
 {
 	std::map<uint, StaticPrototypePtr>::iterator it;
 	for( it = m_staticPrototypes.begin(); it != m_staticPrototypes.end(); ++it )
 		if( name == (*it).second->getName() )
 			return (*it).second;
 	return StaticPrototypePtr();
 };*/
 
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