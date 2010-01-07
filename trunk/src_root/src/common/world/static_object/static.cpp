/**
  * @file client_static.cpp
  * Static object
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: client_static.cpp 829 2006-11-30 18:39:37Z mip $
 
  *  See LICENSE for details
  */
 
#include "stdafx.h"
#include "common/world/static_object/static.h"
#include "common/world/managers/static_collection.h"
#include "common/world/static_object/static_prototype.h"

#include "common/world/OgreOdeMeshInformer.h"
#include "ireon_client/world/client_world.h"

//#include "Ogre/paging_landscape/OgrePagingLandScapeData2DManager.h"

CStaticObject::CStaticObject(CStaticPrototype* proto):
CObject(proto->getName()),
proto_(proto),
m_loaded(false),
m_position(Vector3::ZERO),
m_rotation(Radian(0)),
m_pitch(Radian(0)),
m_roll(Radian(0)),
m_yaw(Radian(0)),
m_page(std::pair<uint,uint>((uint)-1,(uint)-1)),
m_min(Vector2::ZERO),
m_max(Vector2::ZERO)
{
	type_ = CObject::STATIC;
	
	updateGeometry();
};

CStaticObject::~CStaticObject()
{
};

CStaticObject* CStaticObject::productCallback(CStaticPrototype* proto)
{
	assert(proto);
	CStaticObject* staticObject = new CStaticObject(proto);
	
	assert(staticObject);
	CStaticCollection::getSingletonPtr()->addObject(staticObject);
	staticObject->load();
	return staticObject;
}

bool CStaticObject::load()
{
	if( m_loaded )
		return true;
	node_ = I_WORLD->sceneManager()->getRootSceneNode()->createChildSceneNode();
 
	node_->setPosition(getPosition3D());
 
	entity_ = I_WORLD->sceneManager()->createEntity(getUniqueName(name_.c_str()),proto_->getMeshName());

	entityNode_ = node_->createChildSceneNode();
	entityNode_->attachObject(entity_);
	if( proto_->getScale() != Vector3::ZERO )
	{
		entityNode_->setScale(Vector3(proto_->getScale().x,proto_->getScale().y,proto_->getScale().z));
	} else if( proto_->getSize() != Vector3::ZERO )
	{
		Vector3 size = OgreOde::MeshInformer::getSize(entity_->getMesh().get());
		Vector3 scale(proto_->getSize().x / size.x, proto_->getSize().y / size.y, proto_->getSize().z / size.z);
		if( scale.x == 0 )
			scale.x = scale.y;
		if( scale.z == 0 )
			scale.z = scale.y;
		entityNode_->setScale(Vector3(scale.x,scale.y,scale.z)); 
	}
	/*Caution: If you scale your scene node only to find out that the object(s) attached to it suddenly have
	increased (or decreased) significantly in intensity when rendered, then you probably did not realize that
	everything attached to the node is scaled . . . including vertex normals. Since local lighting calculations
	assume that the normals they are using are normalized (unit-length vectors), those calculations will happily
	(and unquestioningly) use the scaled normals when shading your polygons. The fix: after you scale a scene
	node, call setNormaliseNormals() on the affected entities to reset their normals to a “normal” state.
	Understand that this operation does have a performance cost.*/
	entity_->setNormaliseNormals(true);

	_log(_info,_loc,"'%s' Size = '%s' Scale = '%s' ", name_.c_str(),StringConverter::toString(proto_->getSize()).c_str(), StringConverter::toString(proto_->getScale()).c_str());
	Quaternion q;
	q.FromAngleAxis(getRotation(),Vector3::UNIT_Y);
	setOrientation(q);
	// TODO fixme ... maybe replace after m_loaded and use CClientStaticObject::pitch --> CObject::pitch ?
	Quaternion p;
	p.FromAngleAxis(getPitch(),Vector3::UNIT_X);
	setOrientation(getOrientation() * p);
	
	Quaternion r;
	r.FromAngleAxis(getRoll(),Vector3::UNIT_Z);
	setOrientation(getOrientation() * r);
	
	m_loaded = true;
	//
	//for( std::vector<CSegment>::iterator it = m_segments.begin(); it != m_segments.end(); ++it )
	//{
	//	Entity* ent = I_WORLD->sceneManager()->createEntity(getUniqueName(""),"cube.mesh");
	//	SceneNode *node = I_WORLD->sceneManager()->getRootSceneNode()->createChildSceneNode();
	//	node->attachObject(ent);
	//	Vector2 dir = (*it).p2()-(*it).p1();
	//	Real len = dir.length();
	//	dir.normalise();
	//	Vector2 per = dir.perpendicular();
	//	node->setPosition(((*it).p1() + dir*(len + 1)/2).x,getPosition().y+1,((*it).p1() + dir*(len+1)/2).y);
	//	Quaternion q;
	//	Vector3 x(dir.x,0,dir.y);
	//	x.normalise();
	//	Vector3 z(per.x,0,per.y);
	//	z.normalise();
	//	q.FromAxes(x,Vector3::UNIT_Y,z);
	//	node->setOrientation(q);
	//	node->setScale((Real)len*0.01,(Real)0.01,(Real)0.01);
	//}
 
	return true;
};

bool CStaticObject::unload()
{
	if( !m_loaded )
		return true;
	if( entityNode_ )
		entityNode_->detachAllObjects();
	if( node_ )
	{
		node_->detachAllObjects();
		node_->removeAndDestroyAllChildren();
		I_WORLD->sceneManager()->destroySceneNode(node_->getName());
	}
	/*
	if( m_entity )
		I_WORLD->sceneManager()->removeEntity(m_entity);
	*/
	m_loaded = false;
	return true;
};

void CStaticObject::setRotation(const Radian& r)
{
	m_rotation = r;
	//if( m_loaded )
	//{
		Quaternion q;
		q.FromAngleAxis(getRotation(),Vector3::UNIT_Y);
		CObject::setOrientation(q);
	//}
	updateGeometry();
};

void CStaticObject::setPosition3D(const Vector3& pos)
{
	m_position = pos;
	uint x = 0,z = 0;
	//I_WORLD->getPageIndices(pos.x,pos.z,x,z,false);
	setPage(x,z);
	//if( m_loaded )
		CObject::setPosition3D(pos);
};
void CStaticObject::pitch(const Radian& rot)
{
	m_pitch = rot;
	//if( m_loaded )
		CObject::pitch(rot);
};
void CStaticObject::roll(const Radian& rot)
{
	m_roll = rot;
	//if( m_loaded )
		CObject::roll(rot);
};
void CStaticObject::yaw(const Radian& rot)
{
	m_yaw = rot;
	//if( m_loaded )
		CObject::yaw(rot);
};

void CStaticObject::updateGeometry()
{
	//std::vector<CSegment>& seg = proto_->getSegments();
	//m_segments.clear();
	//m_segments.assign(seg.begin(),seg.end());

	//Real sin = Math::Sin(-m_rotation);
	//Real cos = Math::Cos(-m_rotation);

	//m_min = Vector2::ZERO;
	//m_max = Vector2::ZERO;

	//for( std::vector<CSegment>::iterator it = m_segments.begin(); it != m_segments.end(); ++it )
	//{
	//	Vector2 p1((*it).p1().x * cos - (*it).p1().y * sin, (*it).p1().x * sin + (*it).p1().y * cos);
	//	Vector2 p2((*it).p2().x * cos - (*it).p2().y * sin, (*it).p2().x * sin + (*it).p2().y * cos);
	//	if( m_min.x > p1.x || m_min.x == 0 )
	//		m_min.x = p1.x;
	//	if( m_min.y > p1.y || m_min.y == 0 )
	//		m_min.y = p1.y;
	//	if( m_min.x > p2.x || m_min.x == 0 )
	//		m_min.x = p2.x;
	//	if( m_min.y > p2.y || m_min.y == 0 )
	//		m_min.y = p2.y;
	//	if( m_max.x < p1.x || m_max.x == 0 )
	//		m_max.x = p1.x;
	//	if( m_max.y < p1.y || m_max.y == 0 )
	//		m_max.y = p1.y;
	//	if( m_max.x < p2.x || m_max.x == 0 )
	//		m_max.x = p2.x;
	//	if( m_max.y < p2.y || m_max.y == 0 )
	//		m_max.y = p2.y;
	//	p1.x += m_position.x;
	//	p1.y += m_position.z;
	//	p2.x += m_position.x;
	//	p2.y += m_position.z;
	//	(*it).setP1(p1);
	//	(*it).setP2(p2);
	//};
	//m_min.x += m_position.x;
	//m_min.y += m_position.z;
	//m_max.x += m_position.x;
	//m_max.y += m_position.z;
};

bool CStaticObject::canWalk(const Vector2& from, const Vector2& to, Vector2& norm)
{
	if( to.x > m_max.x && from.x > m_max.x )
		return true;
	if( to.x < m_min.x && from.x < m_min.x )
		return true;
	if( to.y > m_max.y && from.y > m_max.y )
		return true;
	if( to.y < m_min.y && from.y < m_min.y )
		return true;
	Vector2 dir = to - from;
	std::vector<CSegment>::iterator it;
	Real t;
	for( it = m_segments.begin(); it != m_segments.end(); ++it )
	{
		if( (*it).intersects(CSegment(from,to)) )
			break;
		if( (*it).squareDist(to,&t) < (CHAR_SIZE * CHAR_SIZE / 10) && t < 1.0f && t > 0.0f )
			break;
	}
	if( it != m_segments.end() )
	{
		if( dir.dotProduct((*it).getNormal()) > 0 )
			norm = (*it).getNormal();
		else
			norm = -(*it).getNormal();
		return false;
	}
	return true;
};

