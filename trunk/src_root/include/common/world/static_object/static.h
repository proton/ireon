/**
  * @file client_static.h
  * Static object
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: client_static.h 829 2006-11-30 18:39:37Z mip $
 
  *  See LICENSE for details
  */
#ifndef _STATIC_H
#define _STATIC_H

//#include "boost/shared_ptr.hpp"
#include "common/world/generic_object/object.h"
#include "common/world/geometry.h"

class CStaticPrototype;

//typedef boost::shared_ptr<CStaticPrototype> StaticPrototypePtr;
//typedef boost::shared_ptr<CPrototype> PrototypePtr;

class CStaticObject : public CObject
{
public:
	CStaticObject(CStaticPrototype* proto);
	~CStaticObject();

	static CStaticObject* productCallback(CStaticPrototype* proto);
	
	bool load();
	bool unload();
	//const CPrototype* getPrototype() {return proto_;}

	// в менеджере прототипов хранить Указатели !
	//void setPrototype(const CPrototype* proto){ proto_ = proto; updateGeometry();};

	//--------------------- Game Action with Static Object -----------------
	///  
	void setRotation(const Radian &q); // TODO: rename to yaw
	/// 
	Radian getRotation() {return m_rotation;} //TODO: rename to getYaw
	///
	void pitch(const Radian &rot);
	///
	Radian getPitch() {return m_pitch;}
	///
	void roll(const Radian &rot);
	///
	Radian getRoll() {return m_roll;}
	///
	void yaw(const Radian &rot); // dont need ... use setRotation
	///
	void setPosition3D(const Vector3& pos);
	///
	Vector3 getPosition3D() const {return m_position;}
	///
	void setPosition2D(const Vector2& pos) {CObject::setPosition2D(pos);}

	//-------------------   ----------------------------
	///
	bool isLoaded() const {return m_loaded;}
	///
	std::pair<uint,uint>& getPage() {return m_page;}
	/// 	
	std::vector<CSegment>& getSegments() {return m_segments;}
	///
	bool canWalk(const Vector2& from, const Vector2& to, Vector2& norm);

protected:

	/// Update object's geometry and bounding square
	void updateGeometry();

	void setPage(uint x,uint z) {m_page.first = x; m_page.second = z;}

protected:
	///
	bool m_loaded;
	///
	//CPrototype* proto_;
 	/// Rotation of the object. It can be rotated only round the Y axis
	Radian m_rotation;
	/// TODO: mip work anology m_rotation 
	Radian m_pitch;
	Radian m_roll;
	Radian m_yaw; // == m_rotation
	/// Position of the object.
	Vector3 m_position;
 	/// Page in what we're situated
	std::pair<uint,uint> m_page;
 	/// Impassable segments
	std::vector<CSegment> m_segments;
 	/// Axis-aligned bounding square of this object
	Vector2 m_min;
	Vector2 m_max;

	CPrototype* proto_;
};

#endif

