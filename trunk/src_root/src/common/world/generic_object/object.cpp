/* Copyright (C) 2005 ireon.org developers council
  * $Id: object.cpp 715 2006-09-10 20:12:50Z mip $
 
  *  See LICENSE for details
  */
 
 /**
  * @file object.cpp
  * Object class
  */

#include "stdafx.h"
#include "common/world/generic_object/object.h"

 CObject::CObject(const String& name):
 type_(NONE),
 name_(name),
 entity_(NULL),
 node_(NULL),
 entityNode_(NULL)
 {
 
 };
 
 CObject::~CObject()
 {
 	if( entityNode_ )
 		entityNode_->detachAllObjects();
 	if( node_ )
 	{
 		node_->detachAllObjects();
 		node_->removeAndDestroyAllChildren();
 		//I_WORLD->sceneManager()->destroySceneNode(m_node->getName());
 	}
 //	if( m_entity )
 //		I_WORLD->sceneManager()->removeEntity(m_entity);
 };
 
 String CObject::getUniqueName(const char* base)
 {
 	String entityName = base;
 	//SceneManager::EntityIterator iter = I_WORLD->sceneManager()->getEntityIterator();
 	//bool found = true;
 /*	while( found )
 	{
 		entityName = base + StringConverter::toString(rand());
 		found = false;
 		while( iter.hasMoreElements() )
 		{
 			if( iter.peekNextKey() == entityName )
 			{
 				found = true;
 				break;
 			}
 			iter.moveNext();
 		}
 	}*/
 	return entityName;
 }
 
 void CObject::setPosition3D(const Vector3 &pos)
 {
 	if( node_ )
 		node_->setPosition(pos);
	posOnMesh_ = ireon::nav::Point(pos);
 };
 
 void CObject::setPosition2D(const Vector2& pos)
 {
 	Vector3 pos3(pos.x,0,pos.y);
 	/*if( I_WORLD->loaded() )
 	{
 		pos3.y = I_WORLD->heightAt(pos3);
 		Real y = I_WORLD->heightAt(pos3);
 		y = 5;
 	}*/
 	setPosition3D(pos3);
 };
 
 void CObject::setOrientation(const Quaternion &q)
 {
 	if( node_ )
 		node_->setOrientation(q);
 };
 
 Vector3 CObject::getPosition3D()
 {
 	if( node_ )
 		return node_->getPosition();
 	return Vector3::ZERO;
 };
 
 ireon::nav::Point CObject::getPointOnMesh()
 {
	 return posOnMesh_;
 };
 
 Vector2 CObject::getPosition2D()
 {
 	if( node_ )
 		return Vector2(node_->getPosition().x, node_->getPosition().z);
 	return Vector2::ZERO;
 };
 
 Quaternion CObject::getOrientation()
 {
 	if( node_ )
 		return node_->getOrientation();
 	return Quaternion::IDENTITY;
 };
 
 void CObject::pitch(const Radian& angle)
 {
 	Vector3 xAxis = getOrientation() * Vector3::UNIT_X;
 	rotateAA(xAxis, angle);
 };
 
 void CObject::yaw(const Radian& angle)
 {
 	Vector3 yAxis = getOrientation() * Vector3::UNIT_Y;
 	rotateAA(yAxis, angle);
 };
 
 void CObject::roll(const Radian& angle)
 {
 	Vector3 zAxis = getOrientation() * Vector3::UNIT_Z;
 	rotateAA(zAxis, angle);
 };
 
 void CObject::rotateAA(const Vector3 &axis, const Radian &angle)
 {
 	Quaternion q;
 	q.FromAngleAxis(angle,axis);
 	rotateQ(q);
 };
 
 void CObject::rotateQ(const Quaternion& q)
 {
 	setOrientation(getOrientation() * q);
 };
 
 bool CObject::visible()
 {
 	return true; //I_WORLD->pointIsVisible(getPosition());
 };

 //void CObject::serialize(CData& data)
 //{
	//if( data.store() )
 //	{
	//	
	//	data << m_name;
	//	if( m_node )
	//	{
	//		data << m_node->getPosition();
 //			data << m_node->getOrientation();
	//	}	
 //		else
	//	{
	//		data << Vector3::ZERO;
	//		data << Quaternion::IDENTITY;
	//	};
 //	} else
 //	{
 //		Ogre::Vector3 position;
	//	Ogre::Quaternion orientation;
	//		
	//	data >> m_name;
	//	data >> position;
	//	data >> orientation;
	//	
	//	m_node->setOrientation(orientation);
	//	m_node->setPosition(position);
 //	};
 //}
