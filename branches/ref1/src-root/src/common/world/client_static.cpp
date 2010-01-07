/**
 * @file client_static.cpp
 * Static object
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id$

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
#include "world/client_static_prototype.h"
#include "world/client_static.h"
#include "world/OgreOdeMeshInformer.h"
#include "OgrePagingLandScapeData2DManager.h"


CClientStaticObject::CClientStaticObject(const StaticPrototypePtr& prot):
CObject(prot->getName()),
m_prot(prot),
m_loaded(false),
m_position(Vector3::ZERO),
m_rotation(Radian(0)),
m_page(std::pair<uint,uint>((uint)-1,(uint)-1)),
m_min(Vector2::ZERO),
m_max(Vector2::ZERO)
{
	m_type = CObject::STATIC;
	
	updateGeometry();
};

CClientStaticObject::~CClientStaticObject()
{
};

bool CClientStaticObject::load()
{
	if( m_loaded )
		return true;
	m_node = I_WORLD->sceneManager()->getRootSceneNode()->createChildSceneNode();

	m_node->setPosition(getPosition());

	m_entity = I_WORLD->sceneManager()->createEntity(getUniqueName(m_name.c_str()),m_prot->getMesh());

	m_entityNode = m_node->createChildSceneNode();
	m_entityNode->attachObject(m_entity);
	if( m_prot->getScale() != Vector3::ZERO )
	{
		m_entityNode->setScale(Vector3(m_prot->getScale().x,m_prot->getScale().y,m_prot->getScale().z));
	} else if( m_prot->getSize() != Vector3::ZERO )
	{
		Vector3 size = OgreOde::MeshInformer::getSize(m_entity->getMesh().get());
		Vector3 scale(m_prot->getSize().x / size.x, m_prot->getSize().y / size.y, m_prot->getSize().z / size.z);
		if( scale.x == 0 )
			scale.x = scale.y;
		if( scale.z == 0 )
			scale.z = scale.y;
		m_entityNode->setScale(Vector3(scale.x,scale.y,scale.z)); 
	}

	Quaternion q;
	q.FromAngleAxis(getRotation(),Vector3::UNIT_Y);
	setOrientation(q);
	m_loaded = true;

	for( std::vector<CSegment>::iterator it = m_segments.begin(); it != m_segments.end(); ++it )
	{
		Entity* ent = I_WORLD->sceneManager()->createEntity(getUniqueName(""),"cube.mesh");
		SceneNode *node = I_WORLD->sceneManager()->getRootSceneNode()->createChildSceneNode();
		node->attachObject(ent);
		Vector2 dir = (*it).p2()-(*it).p1();
		Real len = dir.length();
		dir.normalise();
		Vector2 per = dir.perpendicular();
		node->setPosition(((*it).p1() + dir*(len + 1)/2).x,getPosition().y+1,((*it).p1() + dir*(len+1)/2).y);
		Quaternion q;
		Vector3 x(dir.x,0,dir.y);
		x.normalise();
		Vector3 z(per.x,0,per.y);
		z.normalise();
		q.FromAxes(x,Vector3::UNIT_Y,z);
		node->setOrientation(q);
		node->setScale(len*0.01,0.01,0.01);
	}

	return true;
};

bool CClientStaticObject::unload()
{
	if( !m_loaded )
		return true;
	if( m_entityNode )
		m_entityNode->detachAllObjects();
	if( m_node )
	{
		m_node->detachAllObjects();
		m_node->removeAndDestroyAllChildren();
		I_WORLD->sceneManager()->destroySceneNode(m_node->getName());
	}
	if( m_entity )
		I_WORLD->sceneManager()->removeEntity(m_entity);
	m_loaded = false;
	return true;
};

void CClientStaticObject::setRotation(const Radian& r)
{
	m_rotation = r;
	if( m_loaded )
	{
		Quaternion q;
		q.FromAngleAxis(getRotation(),Vector3::UNIT_Y);
		CObject::setOrientation(q);
	}
	updateGeometry();
};

void CClientStaticObject::setPosition(const Vector3& pos)
{
	m_position = pos;
	uint x,z;
	PagingLandScapeData2DManager::getSingleton().getPageIndices(pos.x,pos.z,x,z,false);
	setPage(x,z);
	if( m_loaded )
		CObject::setPosition(pos);
};

void CClientStaticObject::updateGeometry()
{
	std::vector<CSegment>& seg = m_prot->getSegments();
	m_segments.clear();
	m_segments.assign(seg.begin(),seg.end());

	Real sin = Math::Sin(-m_rotation);
	Real cos = Math::Cos(-m_rotation);

	m_min = Vector2::ZERO;
	m_max = Vector2::ZERO;

	for( std::vector<CSegment>::iterator it = m_segments.begin(); it != m_segments.end(); ++it )
	{
		Vector2 p1((*it).p1().x * cos - (*it).p1().y * sin, (*it).p1().x * sin + (*it).p1().y * cos);
		Vector2 p2((*it).p2().x * cos - (*it).p2().y * sin, (*it).p2().x * sin + (*it).p2().y * cos);
		if( m_min.x > p1.x || m_min.x == 0 )
			m_min.x = p1.x;
		if( m_min.y > p1.y || m_min.y == 0 )
			m_min.y = p1.y;
		if( m_min.x > p2.x || m_min.x == 0 )
			m_min.x = p2.x;
		if( m_min.y > p2.y || m_min.y == 0 )
			m_min.y = p2.y;
		if( m_max.x < p1.x || m_max.x == 0 )
			m_max.x = p1.x;
		if( m_max.y < p1.y || m_max.y == 0 )
			m_max.y = p1.y;
		if( m_max.x < p2.x || m_max.x == 0 )
			m_max.x = p2.x;
		if( m_max.y < p2.y || m_max.y == 0 )
			m_max.y = p2.y;
		p1.x += m_position.x;
		p1.y += m_position.z;
		p2.x += m_position.x;
		p2.y += m_position.z;
		(*it).setP1(p1);
		(*it).setP2(p2);
	};
	m_min.x += m_position.x;
	m_min.y += m_position.z;
	m_max.x += m_position.x;
	m_max.y += m_position.z;
};

bool CClientStaticObject::canWalk(const Vector2& from, const Vector2& to, Vector2& norm)
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