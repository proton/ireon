#include "OgreOdeSpace.h"
#include "OgreOdeGeometry.h"
#include "OgreOdeWorld.h"

using namespace OgreOde;

Space::Space(const Space* space)
{
	_internal_collisions = true;
}

dSpaceID Space::getSpaceID() const
{
	return _space;
}

dSpaceID Space::getSpaceID(const Space* space) const
{
	if(space) return space->getSpaceID();
	return 0;
}

void Space::setAutoCleanup(bool on)
{
	dSpaceSetCleanup(_space,(on)?1:0);
}

bool Space::getAutoCleanup()
{
	return ((dSpaceGetCleanup(_space))?true:false); 
}

void Space::addGeometry(const Geometry& geometry)
{
	dSpaceAdd(_space,geometry.getGeometryID()); 
}

void Space::removeGeometry(const Geometry& geometry)
{
	dSpaceRemove(_space,geometry.getGeometryID()); 
}

bool Space::containsGeometry(const Geometry& geometry)
{
	return ((dSpaceQuery(_space,geometry.getGeometryID()))?true:false); 
}

int Space::getGeometryCount()
{
	return dSpaceGetNumGeoms(_space); 
}

Geometry* Space::getGeometry(int index)
{
	return (Geometry*)World::getSingleton().getGeometryList().findItem((unsigned int)dSpaceGetGeom(_space,index)); 
}

void Space::registerSpace()
{
	World::getSingleton().getSpaceList().registerItem(this);
	dGeomSetData((dGeomID)_space,(void*)this);
}

void Space::collide(void* data)
{
	if(_internal_collisions)
	{
		dSpaceCollide(_space,data,World::collisionCallback);
	}
}

void Space::collide(Space* space,void* data)
{
	dSpaceCollide2((dGeomID)_space,(dGeomID)(space->getSpaceID()),data,World::collisionCallback);
}

void Space::collide(Geometry* geometry,void* data)
{
	dSpaceCollide2((dGeomID)_space,geometry->getGeometryID(),data,World::collisionCallback);
}

void Space::setInternalCollisions(bool collide)
{
	_internal_collisions = collide;
}

bool Space::getInternalCollisions()
{
	return _internal_collisions;
}

const AxisAlignedBox& Space::getAxisAlignedBox()
{
	dReal aabb[6];
	dGeomGetAABB((dGeomID)_space,aabb);
	_bounding_box.setExtents((Real)aabb[0],(Real)aabb[2],(Real)aabb[4],(Real)aabb[1],(Real)aabb[3],(Real)aabb[5]);
	return _bounding_box;
}

unsigned long Space::getID()
{
	return (unsigned long)_space;
}

Space::~Space()
{
	World::getSingleton().getSpaceList().unregisterItem((unsigned long)_space);
	dSpaceDestroy(_space); 
}

SimpleSpace::SimpleSpace(const Space* space)
{
	_space = dSimpleSpaceCreate(getSpaceID(space)); 
	registerSpace();
}

SimpleSpace::~SimpleSpace()
{
}

HashTableSpace::HashTableSpace(const Space* space)
{
	_space = dHashSpaceCreate(getSpaceID(space)); 
	registerSpace();
}

void HashTableSpace::setLevels(int min_level,int max_level)
{
	dHashSpaceSetLevels(_space,min_level,max_level); 
}

int HashTableSpace::getMinimumLevel()
{
	int min_level,max_level;
	dHashSpaceGetLevels(_space,&min_level,&max_level);
	return min_level;
}

int HashTableSpace::getMaximumLevel()
{
	int min_level,max_level;
	dHashSpaceGetLevels(_space,&min_level,&max_level);
	return max_level;
}

HashTableSpace::~HashTableSpace()
{
}

QuadTreeSpace::QuadTreeSpace(const Vector3& center,const Vector3& extents,int depth,const Space* space)
{
	dVector3 c,e;
	
	c[0] = (dReal)center.x;
	c[1] = (dReal)center.y;
	c[2] = (dReal)center.z;

	e[0] = (dReal)extents.x;
	e[1] = (dReal)extents.y;
	e[2] = (dReal)extents.z;

	_space = dQuadTreeSpaceCreate(getSpaceID(space),c,e,depth); 
	registerSpace();
}

QuadTreeSpace::~QuadTreeSpace()
{
}
