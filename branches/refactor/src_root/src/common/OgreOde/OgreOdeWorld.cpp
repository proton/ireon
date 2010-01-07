#include "OgreOdeWorld.h"
#include "OgreOdeBody.h"
#include "OgreOdeJoint.h"
#include "OgreOdeGeometry.h"
#include "OgreOdeSpace.h"
#include "OgreOdeDebugObject.h"

using namespace OgreOde;

template<> World* Singleton<World>::ms_Singleton = 0;
CollisionListener* World::_collision_listener = 0;

World* World::getSingletonPtr()
{
    return ms_Singleton;
}

void World::setCollisionListener(CollisionListener* collision_listener)
{
	_collision_listener = collision_listener;
}

CollisionListener* World::getCollisionListener()
{
	return _collision_listener;
}

World& World::getSingleton()
{  
    assert(ms_Singleton);  
	return(*ms_Singleton);  
}

World::World(SceneManager* manager)
{
	_world = dWorldCreate();
	_contacts = dJointGroupCreate(0);
	_collision_listener = 0;
	_show_debug = false;
	_manager = manager;

	_default_space = new HashTableSpace();
	_default_space->setAutoCleanup(false);
	_default_space->setInternalCollisions(true);

	setDamping(0.0,0.0);
}

void World::setGravity(const Vector3& gravity)
{
	dWorldSetGravity(_world,(dReal)gravity.x,(dReal)gravity.y,(dReal)gravity.z);
}

const Vector3& World::getGravity()
{
	dVector3 g;
	dWorldGetGravity(_world,g);
	_gravity.x = (Real)g[0];
	_gravity.y = (Real)g[1];
	_gravity.z = (Real)g[2];
	return _gravity;
}

void World::setERP(Real erp)
{
	dWorldSetERP(_world,(dReal)erp);
}

Real World::getERP()
{
	return (Real)dWorldGetERP(_world);
}

void World::setCFM(Real cfm)
{
	dWorldSetCFM(_world,(dReal)cfm);
}

Real World::getCFM()
{
	return (Real)dWorldGetCFM(_world);
}

void World::setAutoSleep(bool auto_sleep)
{
	dWorldSetAutoDisableFlag(_world,(auto_sleep)?1:0);
}

bool World::getAutoSleep()
{
	return (dWorldGetAutoDisableFlag(_world))?true:false;
}

void World::setAutoSleepLinearThreshold(Real linear_threshold)
{
	dWorldSetAutoDisableLinearThreshold(_world,(dReal)linear_threshold);
}

Real World::getAutoSleepLinearThreshold()
{
	return (Real)dWorldGetAutoDisableLinearThreshold(_world);
}

void World::setAutoSleepAngularThreshold(Real angular_threshold)
{
	dWorldSetAutoDisableAngularThreshold(_world,(dReal)angular_threshold);
}

Real World::getAutoSleepAngularThreshold()
{
	return (Real)dWorldGetAutoDisableAngularThreshold(_world);
}

void World::setAutoSleepSteps(int steps)
{
	dWorldSetAutoDisableSteps(_world,steps);
}

int World::getAutoSleepSteps()
{
	return dWorldGetAutoDisableSteps(_world);
}

void World::setAutoSleepTime(Real time)
{
	dWorldSetAutoDisableTime(_world,(dReal)time);
}

Real World::getAutoSleepTime()
{
	return (Real)dWorldGetAutoDisableTime(_world);
}

void World::step(Real stepsize)
{
	dWorldStep(_world,(dReal)stepsize); 
}

void World::quickStep(Real stepsize)
{
	dWorldQuickStep(_world,(dReal)stepsize);
}

void World::setContactCorrectionVelocity(Real velocity)
{
	dWorldSetContactMaxCorrectingVel(_world,(dReal)velocity);
}

void World::setContactSurfaceLayer(Real layer)
{
	dWorldSetContactSurfaceLayer(_world,(dReal)layer);
}

dWorldID World::getWorldID()
{
	return _world;
}

dJointGroupID World::getContactGroupID()
{
	return _contacts;
}

void World::collisionCallback(void *data,dGeomID geom_a,dGeomID geom_b)
{
	bool a_space = (dGeomIsSpace(geom_a))?true:false;
	bool b_space = (dGeomIsSpace(geom_b))?true:false;
	
	void* ptr_a = dGeomGetData(geom_a);
	void* ptr_b = dGeomGetData(geom_b);

	if(a_space  || b_space )
	{
		// Collide a space with a space
		if(a_space && b_space) 
			static_cast<Space*>(ptr_a)->collide(static_cast<Space*>(ptr_b),data);
		else if(a_space) 
			static_cast<Space*>(ptr_a)->collide(static_cast<Geometry*>(ptr_b),data);
		else 
			static_cast<Space*>(ptr_b)->collide(static_cast<Geometry*>(ptr_a),data);

		// Collide geoms internal to the spaces
		if(a_space) static_cast<Space*>(ptr_a)->collide(data);
		if(b_space) static_cast<Space*>(ptr_b)->collide(data);
	}
    else
	{
		// Collide a geom with a geom, i.e. generate contacts
		static_cast<Geometry*>(ptr_a)->collide(static_cast<Geometry*>(ptr_b),_collision_listener);
	}
}

Body* World::findBody(SceneNode* node)
{
	Body* body = 0;
	for(int i = 0;i < node->numAttachedObjects();i++)
	{
		MovableObject* obj = node->getAttachedObject(i);
		if(obj)
		{
			if(obj->getMovableType() == String("OgreOde::Body"))
			{
				body = static_cast<Body*>(obj);
				break;
			}
		}
	}
	return body;
}

Body* World::findBody(const String& name)
{
	for(std::map<unsigned long,MaintainedItem*>::iterator i = _body_list._map.begin();i != _body_list._map.end();++i)
	{
		if(((Body*)i->second)->getName() == name) return (Body*)i->second;
	}
	return 0;
}

void World::setDefaultSpace(Space* space)
{
	delete _default_space;
	_default_space = space;
}

void World::clearContacts()
{
	dJointGroupEmpty(_contacts);
}

void World::setQuickStepIterations(int iterations)
{
	dWorldSetQuickStepNumIterations(_world,iterations);
}

int World::getQuickStepIterations()
{
	return dWorldGetQuickStepNumIterations(_world);
}

void World::setShowDebugObjects(bool show)
{
	_body_list.setDebug(show);
	/*
	std::map<dBodyID,Body*>::iterator bi = _body_map.begin();
	std::map<dBodyID,Body*>::iterator bend = _body_map.end();
	for(;bi != bend;++bi)
	{
		bi->second->destroyDebugNode();
	}
	*/

	_geometry_list.setDebug(show);
	/*
	std::map<dGeomID,Geometry*>::iterator gi = _geometry_map.begin();
	std::map<dGeomID,Geometry*>::iterator gend = _geometry_map.end();
	for(;gi != gend;++gi)
	{
		gi->second->destroyDebugObject();
		if(show) gi->second->createDebugObject();
	}
	*/
	_show_debug = show;
}

void World::notifyGeometry(Body* body)
{
	_geometry_list.notify(body);
	/*
	std::map<dGeomID,Geometry*>::iterator i = _geometry_map.begin();
	std::map<dGeomID,Geometry*>::iterator end = _geometry_map.end();
	for(;i != end;++i)
	{
		if(i->second->getBody() == body)
		{
			i->second->destroyDebugObject();
		}
	}
	*/
}

void World::synchronise()
{
	/*
	std::map<dBodyID,Body*>::iterator i = _body_map.begin();
	std::map<dBodyID,Body*>::iterator end = _body_map.end();
	for(;i != end;++i)
	{
		i->second->applyDamping();
		i->second->updateParentNode();
	}
	*/
	_body_list.sync();
}

void World::setDamping(Real linear_damping,Real angular_damping)
{
	_linear_damping = -(dReal)linear_damping;
	_angular_damping = -(dReal)angular_damping;
}

Real World::getLinearDamping()
{
	return -(Real)_linear_damping;
}

Real World::getAngularDamping()
{
	return -(Real)_angular_damping;
}

World::~World()
{
	delete _default_space;
	dJointGroupDestroy(_contacts);
	dWorldDestroy(_world);
	dCloseODE();
}
