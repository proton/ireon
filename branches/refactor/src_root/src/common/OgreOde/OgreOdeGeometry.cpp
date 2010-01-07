#include "OgreOdeGeometry.h"
#include "OgreOdeWorld.h"
#include "OgreOdeSpace.h"
#include "OgreOdeBody.h"
#include "OgreOdeCollision.h"
#include "OgreOdeDebugObject.h"

using namespace OgreOde;

int Geometry::_geometry_count = 0;

Geometry::Geometry(Space* space)
{
	_contact_high_water_mark = 0;
	_max_contacts = 32;
	_contacts = new dContact[_max_contacts];
	
	_encapsulator = 0;
	_debug_obj = 0;
	_debug_node = 0;

	_user_data = 0;
	_object = 0;
}

dGeomID Geometry::getGeometryID() const
{
	return _geom;
}

void Geometry::setEncapsulator(Geometry* encapsulator)
{
	_encapsulator = encapsulator;

	if(World::getSingleton().getShowDebugObjects())
	{
		reparentDebugObject(encapsulator->_debug_node);
	}
}

void Geometry::reparentDebugObject(Node* node)
{
	if((node)&&(_debug_node)&&(_debug_node->getParent() != node))
	{
		SceneNode* parent = static_cast<SceneNode*>(_debug_node->getParent());
		parent->removeChild(_debug_node->getName());
		node->addChild(_debug_node);

		_debug_node->setPosition(getPosition());
		_debug_node->setOrientation(getOrientation());
	}
}

void Geometry::createDebugObject()
{
	if(!_debug_node)
	{
		SceneNode* parent = World::getSingletonPtr()->_manager->getRootSceneNode();
		_debug_node = static_cast<Node*>(parent->createChildSceneNode(String("OgreOde::Geometry_") + StringConverter::toString(_geometry_count) + String("_Debug"))); 

		if(_debug_obj)
		{
			static_cast<SceneNode*>(_debug_node)->attachObject(_debug_obj);
		}

		if(getBody())
		{
			static_cast<SceneNode*>(_debug_node->getParent())->removeChild(_debug_node->getName());
			getBody()->addDebugNode(_debug_node);
			if(_debug_obj) _debug_obj->setMode(DebugObject::Mode_Enabled);
		}
		else if(_encapsulator)
		{
			reparentDebugObject(_encapsulator->_debug_node);
		}
		else
		{
			if(_debug_obj) _debug_obj->setMode(DebugObject::Mode_Static);

			_debug_node->setPosition(getPosition());
			_debug_node->setOrientation(getOrientation());
		}
		_geometry_count++;
	}
}

void Geometry::destroyDebugObject()
{
	if(_debug_node)
	{
		SceneNode* sn = static_cast<SceneNode*>(_debug_node->getParent());
		sn->removeAndDestroyChild(_debug_node->getName());		
		_debug_node = 0;
	}

	if(_debug_obj)
	{
		delete _debug_obj;
		_debug_obj = 0;
	}
}

void Geometry::registerGeometry()
{
	dGeomSetData(_geom,(void*)this);
	World::getSingleton().getGeometryList().registerItem(this);
}

dSpaceID Geometry::getSpaceID(Space* space) const
{
	if(!space) return 0;
	return space->getSpaceID();
}

void Geometry::setBody(Body* body)
{
	destroyDebugObject();
	dGeomSetBody(_geom,(body)?body->getBodyID():0); 
	if(World::getSingleton().getShowDebugObjects()) createDebugObject();
}

Body* Geometry::getBody()
{
	const dBodyID body = dGeomGetBody(_geom);
	if(!body) return 0;
	else return (Body*)dBodyGetData(body); 
}

void Geometry::notify(Body* body)
{
	if(getBody() == body)
	{
		destroyDebugObject();
	}
}

void Geometry::setDebug(bool debug)
{
	destroyDebugObject();
	if(debug) createDebugObject();
}

void Geometry::setPosition(const Vector3& position)
{
	dGeomSetPosition(_geom,(dReal)position.x,(dReal)position.y,(dReal)position.z);

	if((_debug_node)&& ((!getBody()) || (_encapsulator))) _debug_node->setPosition(position);
}

void Geometry::setOrientation(const Quaternion& orientation)
{
	dQuaternion q;
	q[0] = (dReal)orientation.w;
	q[1] = (dReal)orientation.x;
	q[2] = (dReal)orientation.y;
	q[3] = (dReal)orientation.z;
	dGeomSetQuaternion(_geom,q); 
	if((_debug_node)&& ((!getBody()) || (_encapsulator))) _debug_node->setOrientation(orientation);
}

const Vector3& Geometry::getPosition()
{
	const dReal* position = dGeomGetPosition(_geom);
	_position.x = (Real)position[0];
	_position.y = (Real)position[1];
	_position.z = (Real)position[2];
	return _position;
}

const Quaternion& Geometry::getOrientation()
{
	dQuaternion orientation;
	dGeomGetQuaternion(_geom,orientation); 
	_orientation.w = (Real)orientation[0];
	_orientation.x = (Real)orientation[1];
	_orientation.y = (Real)orientation[2];
	_orientation.z = (Real)orientation[3];
	return _orientation;
}

const AxisAlignedBox& Geometry::getAxisAlignedBox()
{
	dReal aabb[6];
	dGeomGetAABB(_geom,aabb);
	_bounding_box.setExtents((Real)aabb[0],(Real)aabb[2],(Real)aabb[4],(Real)aabb[1],(Real)aabb[3],(Real)aabb[5]);
	return _bounding_box;
}

Space* Geometry::getSpace()
{
	return (Space*)World::getSingleton().getSpaceList().findItem((unsigned int)dGeomGetSpace(_geom));
}


void Geometry::enable()
{
	dGeomEnable(_geom);
}

void Geometry::disable()
{
	dGeomDisable(_geom); 
}

bool Geometry::isEnabled()
{
	return ((dGeomIsEnabled(_geom))?true:false); 
}

Geometry::Class Geometry::getClass()
{
	return (Geometry::Class)dGeomGetClass(_geom);
}

void Geometry::setCategoryBitfield(unsigned long bits)
{
	dGeomSetCategoryBits(_geom,bits); 
}

void Geometry::setCollisionBitfield(unsigned long bits)
{
	dGeomSetCollideBits(_geom,bits); 
}

unsigned long Geometry::getCategoryBitfield()
{
	return dGeomGetCategoryBits(_geom); 
}

unsigned long Geometry::getCollisionBitfield()
{
	return dGeomGetCollideBits(_geom); 
}

int Geometry::collide(Geometry* geometry,CollisionListener* listener)
{
	const int num_contacts = dCollide(_geom,geometry->getGeometryID(),_max_contacts,&(_contacts[0].geom),sizeof(dContact));
	if(num_contacts)
	{
		_contact_high_water_mark = std::max(_contact_high_water_mark,num_contacts);

		Contact contact;
		
		const dWorldID wid = World::getSingleton().getWorldID();
		const dJointGroupID cid = World::getSingleton().getContactGroupID();
		const dBodyID b1 = dGeomGetBody(_geom);
		const dBodyID b2 = dGeomGetBody(geometry->getGeometryID());
		if(listener)
		{
			for(int i = 0;i < num_contacts;i++)
			{
				contact = &(_contacts[i]);
				if(listener->collision(&contact))
				{
					dJointID c = dJointCreateContact(wid,cid,&_contacts[i]);
					dJointAttach(c,b1,b2);
				}
			}
		}
		else
		{
			for(int i = 0;i < num_contacts;i++)
			{
				contact = &(_contacts[i]);
   
				dJointID c = dJointCreateContact(wid,cid,&_contacts[i]);
				dJointAttach(c,b1,b2);
			}
		} 
	}
	return num_contacts;
}

int Geometry::getMaxContacts()
{
	return _max_contacts;
}

void Geometry::setMaxContacts(int max_contacts)
{
	delete[] _contacts;
	_max_contacts = max_contacts;
	_contacts = new dContact[_max_contacts];
}

int Geometry::getContactHighWaterMark()
{
	return _contact_high_water_mark;
}

unsigned long Geometry::getID()
{
	return (unsigned long)_geom;
}

Geometry::~Geometry()
{
	destroyDebugObject();

	delete[] _contacts;
	World::getSingleton().getGeometryList().unregisterItem((unsigned long)_geom);
	dGeomDestroy(_geom); 
}

SphereGeometry::SphereGeometry(Real radius,Space* space):Geometry(space)
{
	_geom = dCreateSphere(getSpaceID(space),(dReal)radius); 
	registerGeometry();
}

void SphereGeometry::setRadius(Real radius)
{
	dGeomSphereSetRadius(_geom,(dReal)radius);
}

Real SphereGeometry::getRadius()
{
	return (Real)dGeomSphereGetRadius(_geom); 
}

Real SphereGeometry::getPointDepth(const Vector3& point)
{
	return (Real)dGeomSpherePointDepth(_geom,(dReal)point.x,(dReal)point.y,(dReal)point.z); 
}

void SphereGeometry::createDebugObject()
{
	_debug_obj = new SphereDebugObject(getRadius());
	Geometry::createDebugObject();
}


SphereGeometry::~SphereGeometry()
{
}

BoxGeometry::BoxGeometry(const Vector3& size,Space* space):Geometry(space)
{
	_geom = dCreateBox(getSpaceID(space),(dReal)size.x,(dReal)size.y,(dReal)size.z); 
	registerGeometry();
}

void BoxGeometry::setSize(const Vector3& size)
{
	dGeomBoxSetLengths(_geom,(dReal)size.x,(dReal)size.y,(dReal)size.z); 
}

const Vector3& BoxGeometry::getSize()
{
	dVector3 result;
	dGeomBoxGetLengths(_geom,result); 
	_size.x = (Real)result[0];
	_size.y = (Real)result[1];
	_size.z = (Real)result[2];
	return _size;
}

Real BoxGeometry::getPointDepth(const Vector3& point)
{
	return (Real)dGeomBoxPointDepth(_geom,(dReal)point.x,(dReal)point.y,(dReal)point.z); 
}

void BoxGeometry::createDebugObject()
{
	_debug_obj = new BoxDebugObject(getSize());
	Geometry::createDebugObject();	
}


BoxGeometry::~BoxGeometry()
{
}

InfinitePlaneGeometry::InfinitePlaneGeometry(const Plane& plane,Space* space):Geometry(space)
{
	_geom = dCreatePlane(getSpaceID(space),(dReal)plane.normal.x,(dReal)plane.normal.y,(dReal)plane.normal.z,(dReal)-plane.d); 
	registerGeometry();
}

void InfinitePlaneGeometry::setDefinition(const Plane& plane)
{
	dGeomPlaneSetParams(_geom,(dReal)plane.normal.x,(dReal)plane.normal.y,(dReal)plane.normal.z,(dReal)plane.d); 
}

const Plane& InfinitePlaneGeometry::getDefinition()
{
	dVector4 result;
	dGeomPlaneGetParams(_geom,result); 
	_plane.normal.x = result[0];
	_plane.normal.y = result[1];
	_plane.normal.z = result[2];
	_plane.d = result[3];
	return _plane;
}

Real InfinitePlaneGeometry::getPointDepth(const Vector3& point)
{
	return (Real)dGeomPlanePointDepth(_geom,(dReal)point.x,(dReal)point.y,(dReal)point.z); 
}

void InfinitePlaneGeometry::setPosition(const Vector3& position)
{
}

void InfinitePlaneGeometry::setOrientation(const Quaternion& orientation)
{
}

const Vector3& InfinitePlaneGeometry::getPosition()
{
	return Vector3::ZERO;
}

const Quaternion& InfinitePlaneGeometry::getOrientation()
{
	return Quaternion::ZERO;
}

const AxisAlignedBox& InfinitePlaneGeometry::getAxisAlignedBox()
{
	return _bounding_box;
}

InfinitePlaneGeometry::~InfinitePlaneGeometry()
{
}

CapsuleGeometry::CapsuleGeometry(Real radius,Real length,Space* space):Geometry(space)
{
	_geom = dCreateCCylinder(getSpaceID(space),(dReal)radius,(dReal)length);
	registerGeometry();
}

void CapsuleGeometry::setDefinition(Real radius,Real length)
{
	dGeomCCylinderSetParams(_geom,(dReal)radius,(dReal)length);
}

Real CapsuleGeometry::getRadius()
{
	dReal radius,length;
	dGeomCCylinderGetParams(_geom,&radius,&length); 
	return radius;
}

Real CapsuleGeometry::getLength()
{
	dReal radius,length;
	dGeomCCylinderGetParams(_geom,&radius,&length); 
	return length;
}

Real CapsuleGeometry::getPointDepth(const Vector3& point)
{
	return (Real)dGeomCCylinderPointDepth(_geom,(dReal)point.x,(dReal)point.y,(dReal)point.z); 
}

void CapsuleGeometry::createDebugObject()
{
	_debug_obj = new CapsuleDebugObject(getRadius(),getLength());
	Geometry::createDebugObject();
}


CapsuleGeometry::~CapsuleGeometry()
{
}

RayGeometry::RayGeometry(Real length,Space* space)
{
	_geom = dCreateRay(getSpaceID(space),(dReal)length);
	registerGeometry();
}


void RayGeometry::createDebugObject()
{
	_debug_obj = new RayDebugObject(getStart(),getDirection(),getLength());
	Geometry::createDebugObject();
}


void RayGeometry::setLength(Real length)
{
	dGeomRaySetLength(_geom,(dReal)length); 
}

Real RayGeometry::getLength()
{
	return (Real)dGeomRayGetLength(_geom); 
}

void RayGeometry::setDefinition(const Vector3& start,const Vector3& direction)
{
	dGeomRaySet(_geom,(dReal)start.x,(dReal)start.y,(dReal)start.z,(dReal)direction.x,(dReal)direction.y,(dReal)direction.z);
}

const Vector3& RayGeometry::getStart()
{
	dVector3 start,direction;
	dGeomRayGet(_geom,start,direction); 
	_start.x = (Real)start[0];
	_start.y = (Real)start[1];
	_start.z = (Real)start[2];
	return _start;
}

const Vector3& RayGeometry::getDirection()
{
	dVector3 start,direction;
	dGeomRayGet(_geom,start,direction); 
	_direction.x = (Real)direction[0];
	_direction.y = (Real)direction[1];
	_direction.z = (Real)direction[2];
	return _direction;
}

RayGeometry::~RayGeometry()
{
}

TransformGeometry::TransformGeometry(Space* space)
{
	_geom = dCreateGeomTransform(getSpaceID(space)); 
	dGeomTransformSetCleanup(_geom,0);
	dGeomTransformSetInfo(_geom,1);
	registerGeometry();
}

void TransformGeometry::setEncapsulatedGeometry(Geometry* geometry)
{
	dGeomTransformSetGeom(_geom,geometry->getGeometryID()); 
	destroyDebugObject();
	if(World::getSingleton().getShowDebugObjects()) createDebugObject();	

	geometry->setEncapsulator(this);
}


void TransformGeometry::createDebugObject()
{
	Geometry::createDebugObject();
	if(getEncapsulatedGeometry())
	{
		getEncapsulatedGeometry()->destroyDebugObject();
		getEncapsulatedGeometry()->createDebugObject();
	}
}

void TransformGeometry::destroyDebugObject()
{
	if(getEncapsulatedGeometry()) getEncapsulatedGeometry()->destroyDebugObject();
	Geometry::destroyDebugObject();
}

Geometry* TransformGeometry::getEncapsulatedGeometry() const
{
	dGeomID id = dGeomTransformGetGeom(_geom);
	if(id == 0) return 0;
	else return (Geometry*)dGeomGetData(id); 
}

TransformGeometry::~TransformGeometry()
{
}

TriangleMeshGeometry::TriangleMeshGeometry(const Vector3* vertices,int vertex_count,const int* indices,int index_count,Space* space)
{
	_vertex_count = vertex_count;
	_index_count = index_count;
	_vertices = new dVector3[vertex_count];
	_indices = new int[index_count];

	for(int i = 0;i < vertex_count;i++)
	{
		_vertices[i][0] = (dReal)vertices[i].x;
		_vertices[i][1] = (dReal)vertices[i].y;
		_vertices[i][2] = (dReal)vertices[i].z;
	}

	memcpy(_indices,indices,sizeof(int) * index_count);

	_data = dGeomTriMeshDataCreate(); 
	dGeomTriMeshDataBuildSimple(_data,(const dReal*)_vertices,vertex_count,_indices,index_count); 
	_geom = dCreateTriMesh(getSpaceID(space),_data,0,0,0);
	registerGeometry();

	_collision_listener = 0;
}

Vector3 TriangleMeshGeometry::getPoint(int index,const Vector3& uv)
{
	dVector3 out;
	dGeomTriMeshGetPoint(_geom,index,(dReal)uv.x,(dReal)uv.y,out); 
	return Vector3((Real)out[0],(Real)out[1],(Real)out[2]);
}

TriangleMeshTriangle TriangleMeshGeometry::getTriangle(int index)
{
	dVector3 v0,v1,v2;
	dGeomTriMeshGetTriangle(_geom,index,&v0,&v1,&v2); 
	
	TriangleMeshTriangle tri;

	tri.v0.x = v0[0]; tri.v0.y = v0[1]; tri.v0.z = v0[2];
	tri.v1.x = v1[0]; tri.v1.y = v1[1]; tri.v1.z = v1[2];
	tri.v2.x = v2[0]; tri.v2.y = v2[1]; tri.v2.z = v2[2];
	
	return tri;
}

void TriangleMeshGeometry::clearTemporalCoherenceCache()
{
	dGeomTriMeshClearTCCache(_geom); 
}

void TriangleMeshGeometry::enableTemporalCoherence(Geometry::Class geometry_class,bool enable)
{
	assert((geometry_class == Geometry::Class_Sphere)||(geometry_class == Geometry::Class_Box));
	dGeomTriMeshEnableTC(_geom,(int)geometry_class,(enable)?1:0); 
}

bool TriangleMeshGeometry::isTemporalCoherenceEnabled(Geometry::Class geometry_class)
{
	return ((dGeomTriMeshIsTCEnabled(_geom,(int)geometry_class))?true:false);
}

int TriangleMeshGeometry::_collisionCallback(dGeomID mesh,dGeomID object,int triangle)
{
	TriangleMeshGeometry* trimesh = (TriangleMeshGeometry*)dGeomGetData(mesh);
	if(trimesh->_collision_listener)
	{
		Geometry* geometry = (object)?((Geometry*)dGeomGetData(object)):0;
		return ((trimesh->_collision_listener->collide(trimesh,geometry,triangle))?1:0);
	}
	return 1;
}

void TriangleMeshGeometry::setCollisionListener(TriangleMeshCollisionListener* collision_listener)
{
	_collision_listener = collision_listener;
	dGeomTriMeshSetCallback(_geom,(_collision_listener)?TriangleMeshGeometry::_collisionCallback:0); 
}

void TriangleMeshGeometry::_intersectionCallback(dGeomID mesh,dGeomID object,const int* triangles,int triangle_count)
{
	TriangleMeshGeometry* trimesh = (TriangleMeshGeometry*)dGeomGetData(mesh);
	if(trimesh->_intersection_listener)
	{
		Geometry* geometry = (object)?((Geometry*)dGeomGetData(object)):0;
		trimesh->_intersection_listener->intersect(trimesh,geometry,triangles,triangle_count);
	}
}

void TriangleMeshGeometry::setIntersectionListener(TriangleMeshIntersectionListener* intersection_listener)
{
	_intersection_listener = intersection_listener;
	dGeomTriMeshSetArrayCallback(_geom,(_intersection_listener)?TriangleMeshGeometry::_intersectionCallback:0); 
}

int TriangleMeshGeometry::_rayCallback(dGeomID mesh,dGeomID ray,int triangle,dReal u,dReal v)
{
	TriangleMeshGeometry* trimesh = (TriangleMeshGeometry*)dGeomGetData(mesh);
	if(trimesh->_ray_listener)
	{
		RayGeometry* ray_geometry = (ray)?((RayGeometry*)dGeomGetData(ray)):0;
		return ((trimesh->_ray_listener->collide(trimesh,ray_geometry,triangle,Vector3((Real)u,(Real)v,0.0)))?1:0);
	}
	return 1;
}

void TriangleMeshGeometry::setRayListener(TriangleMeshRayListener* ray_listener)
{
	_ray_listener = ray_listener;
	dGeomTriMeshSetRayCallback(_geom,(_ray_listener)?TriangleMeshGeometry::_rayCallback:0); 
}


void TriangleMeshGeometry::createDebugObject()
{
	_debug_obj = new TriangleMeshDebugObject((_index_count / 3) * 6);
	TriangleMeshDebugObject* obj = static_cast<TriangleMeshDebugObject*>(_debug_obj);

	obj->beginDefinition();
	for(int i = 0,j = 0;i < _index_count;i+=3,j+=6)
	{
		obj->setVertex(j,Vector3((Real)_vertices[_indices[i]][0],(Real)_vertices[_indices[i]][1],(Real)_vertices[_indices[i]][2]));
		obj->setVertex(j+1,Vector3((Real)_vertices[_indices[i+1]][0],(Real)_vertices[_indices[i+1]][1],(Real)_vertices[_indices[i+1]][2]));

		obj->setVertex(j+2,Vector3((Real)_vertices[_indices[i+1]][0],(Real)_vertices[_indices[i+1]][1],(Real)_vertices[_indices[i+1]][2]));
		obj->setVertex(j+3,Vector3((Real)_vertices[_indices[i+2]][0],(Real)_vertices[_indices[i+2]][1],(Real)_vertices[_indices[i+2]][2]));

		obj->setVertex(j+4,Vector3((Real)_vertices[_indices[i+2]][0],(Real)_vertices[_indices[i+2]][1],(Real)_vertices[_indices[i+2]][2]));
		obj->setVertex(j+5,Vector3((Real)_vertices[_indices[i]][0],(Real)_vertices[_indices[i]][1],(Real)_vertices[_indices[i]][2]));
	}
	obj->endDefinition();
	Geometry::createDebugObject();
}


TriangleMeshGeometry::~TriangleMeshGeometry()
{
	dGeomTriMeshDataDestroy(_data); 
	delete[] _vertices;
	delete[] _indices;
}

#ifdef OGREODE_TERRAINGEOMETRY

TerrainGeometry::TerrainGeometry(uint nodes_per_side, Real side_length, Real max_height, Space* space)
{
	_geom = dCreateTerrainCallback(getSpaceID(space),0,(dReal)max_height,(dReal)side_length,nodes_per_side,0,0);
	
	_listener = 0;
	_ray = Ray(Vector3::ZERO,Vector3::NEGATIVE_UNIT_Y);
	_ray_query = World::getSingleton().getSceneManager()->createRayQuery(_ray);
	_max_height = max_height;

	registerGeometry();
}

Real TerrainGeometry::getPointDepth(const Vector3& point)
{
	return (Real)dGeomTerrainCallbackPointDepth(_geom,(dReal)point.x,(dReal)point.y,(dReal)point.z); 
}

void TerrainGeometry::setHeightListener(TerrainGeometryHeightListener* listener)
{
	_listener = listener;
	dGeomTerrainCallbackSetHeightCallback(_geom,(_listener)?TerrainGeometry::_heightCallback:0); 
}

dReal TerrainGeometry::_heightCallback(void* data,dReal x,dReal z)
{
	TerrainGeometry* terrain = (TerrainGeometry*)data;
	if(terrain->_listener)
	{
		return (dReal)terrain->_listener->heightAt(Vector3((Real)x,terrain->_max_height,(Real)z));
	}
	return (dReal)(0.0);
}

Real TerrainGeometry::getHeightAt(const Vector3& position)
{

	return 0.0;
}

const Vector3& TerrainGeometry::getPosition()
{
	return Vector3::ZERO;
}

const Quaternion& TerrainGeometry::getOrientation()
{
	return Quaternion::ZERO;
}

TerrainGeometry::~TerrainGeometry()
{
	World::getSingleton().getSceneManager()->destroyQuery(_ray_query);
}

#endif

