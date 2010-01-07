/* Copyright (C) 2005 ireon.org developers council
  * $Id: world_object.h 801 2006-11-07 15:44:53Z zak $
 
  *  See LICENSE for details
  */
 
 /**
  * @file world_object.h
  * Object
  */
 #ifndef _WORLD_OBJECT_H
 #define _WORLD_OBJECT_H
 
 class CWorldObject
 {
 public:
 	CWorldObject();
 	virtual ~CWorldObject();
 
 	virtual void setPos(const Vector2& pos) {m_pos = pos;}
 	Vector2 getPos() const {return m_pos;}
 
 	virtual void setRotation(byte rot) {m_rotation = rot;}
 	byte getRotation() const {return m_rotation;}
 
 	/// time im miliseconds
 	virtual void update(ushort time);
 
 	bool inWorld() {return m_inWorld;}
 protected:
 
 	/** This function is called when character enters world
 	*/
 	virtual void onEnterWorld() {}
 
 	/** This function is called when character leaves world
 	*/
 	virtual void onLeaveWorld() {}
 protected:
 
 	Vector2 m_pos;
 	///Rotation
 	byte m_rotation;
 
 	/// Object is in world
 	bool m_inWorld;
 };
 
 #endif
