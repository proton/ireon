/* Copyright (C) 2005 ireon.org developers council
  * $Id: object.h 829 2006-11-30 18:39:37Z mip $
 
  *  See LICENSE for details
  */
 
 /**
  * @file object.h
  * Object class, base for all world object
  */
#ifndef _OBJECT_H
#define _OBJECT_H
#pragma warning(push,1)
#include "Ogre/main/Ogre.h"
#pragma warning(pop)
#include "common/world/mesh.h"
class CPrototype;
class CObject: protected CLogMixin
{
public:
	CObject(const String& name);
	virtual ~CObject();
	/// Type object : TODO: refactor this
	enum Type
	{
		NONE,
		PLAYER,
		OWN_PLAYER,
		MOB,
		STATIC
	};
 
public:
	ireon::nav::Point getPointOnMesh();
	//--------------------- Geometry Action with Object -----------------	
	/// get 3d position (in World)
	Vector3 getPosition3D();
	/// set 3d position (in World)
	virtual void setPosition3D(const Vector3& pos);
	/// Get 2d position (at terrain)
	Vector2 getPosition2D();
	/// Set 2d position (at terrain)
	virtual void setPosition2D(const Vector2& pos);
	/// get quaternion 
	Quaternion getOrientation();
	/// set quaternion
	void setOrientation(const Quaternion& q);
	
	/// Rotate around on local X axis
	virtual void pitch(const Radian& angle);
	/// Rotate around local Y axis
	virtual void yaw(const Radian& angle);
	/// Rotate around local Z axis
	virtual void roll(const Radian& angle);
	
	/// Rotate around vector 'axis' on angle 'angle'
	void rotateAA(const Vector3& axis, const Radian& angle);
	/// Rotate around current quaternion on quaternion 'q'
	void rotateQ(const Quaternion& q);

	//------------------- Information of Object -----------------
	/// create unique name entity within scene
	static String getUniqueName(const char* base);
	/// Get name of object
	const char* getName() {return name_.c_str();}
	/// Set name of object
	void setName(const char* name){name_ = name;};
	/// Get type of object ( TODO: refactor this ) 
	Type getType() {return type_;}
	/// Get node of object
	SceneNode* getSceneNode() {return node_;}
	/// Get entity of object
	Entity* getEntity() {return entity_;}

	// ------------------ Program Action with Object -------------
	/// Update, 'time' = time from last call
	virtual void update(Real) {}
	/// check - object is visible?
	virtual bool visible();
	/// save data, state
	//virtual void serialize(CData& data);

protected:
	/// name of object
	Ogre::String name_;
	/// node 
	SceneNode* node_;
	/// entity node
	SceneNode* entityNode_;
	/// entity
	Entity* entity_;
	/// type 
	Type type_;
	/// prototype - template, keeping info about concrete data object
	//CPrototype* proto_;

	ireon::nav::Point posOnMesh_;
};
 
 
 #endif
