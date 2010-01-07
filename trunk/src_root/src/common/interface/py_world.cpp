/**
  * @file py_world.cpp
  * Export world to python
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: py_world.cpp 844 2007-01-03 13:28:38Z mip $
 
  *  See LICENSE for details
  */
#ifdef _ENABLE_EXPORT_TO_PYTHON
#include "stdafx.h"
#include "common/world/object.h" 
#include "common/world/world.h"
#include "common/world/character.h"
#include "common/world/static_object/client_static.h"
#include "common/world/static_object/client_static_prototype.h"
typedef boost::shared_ptr<CObject> ObjectPtr;
 
#define BOOST_PYTHON_STATIC_MODULE
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/copy_const_reference.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/python/reference_existing_object.hpp>
#include <boost/python/manage_new_object.hpp>
using namespace boost::python;
 
void (CObject::*setPosition1)(const Vector3& pos) = &CObject::setPosition;
void (CObject::*setPosition2)(const Vector2& pos) = &CObject::setPosition;
void (CObject::*rotate1)(const Vector3 &axis, const Radian &angle) = &CObject::rotate;
void (CObject::*rotate2)(const Quaternion& q) = &CObject::rotate;

void (CStaticObject::*setPosition3)(const Vector3& pos) = &CStaticObject::setPosition;
void (CStaticObject::*setPosition4)(const Vector2& pos) = &CStaticObject::setPosition;

//void (CObject::*rotate1)(const Quaternion& q) = &CObject::rotate;
//void (CObject::*rotate2)(const Vector3& axis, const Radian& angle) = &CObject::rotate;

 BOOST_PYTHON_MODULE(World)
 {
 	enum_< CObject::Type >
 		("ObjType")
 		.value("NONE",CObject::Type::NONE)
 		.value("PLAYER",CObject::Type::PLAYER)
 		.value("OWN_PLAYER",CObject::Type::OWN_PLAYER)
 		.value("MOB",CObject::Type::MOB);
 
 	class_< CObject, ObjectPtr >
 		("CObject", no_init)
 		.add_property("type",&CObject::getType)
 		.add_property("name",&CObject::getName)
 		.add_property("position",&CObject::getPosition,setPosition1)
 		.add_property("position2",&CObject::getPosition2,setPosition2)
		.add_property("orientation", &CObject::getOrientation, &CObject::setOrientation)
 		.def("rotateV3R", rotate1)
		.def("rotateQ", rotate2)
		.add_property("visible",&CObject::visible);
 
 	class_< CDynamicObject, CharacterPtr, bases<CObject> >
 		("Character", no_init )
 		.add_property("info",&CDynamicObject::getInfo, &CDynamicObject::setInfo )
 		.add_property("id",&CDynamicObject::getId )
 		.add_property("enemy",&CDynamicObject::getEnemy )
 		.add_property("percentHP", &CDynamicObject::getPercentHP, &CDynamicObject::setPercentHP )
 		.add_property("dead", &CDynamicObject::isDead )
 		.add_property("rotation", &CDynamicObject::getRotation);

	
	class_< CStaticObject, StaticPtr, bases<CObject> >
 		("Static", no_init )
 		.def("pitch", &CStaticObject::pitch)
		.def("roll", &CStaticObject::roll)
		.def("yaw", &CStaticObject::yaw)
		.add_property("position", &CStaticObject::getPosition,setPosition3)
 		.add_property("position2", &CStaticObject::getPosition2,setPosition4)
		.add_property("rotation", &CStaticObject::getRotation, &CStaticObject::setRotation)
 		.add_property("prototype", &CStaticObject::getPrototype);
	
	class_< CStaticPrototype, StaticPrototypePtr  >
 		("StaticPrototype", no_init)
		.add_property("id",&CStaticPrototype::getId)
 		.add_property("name",&CStaticPrototype::getName);

	//register_ptr_to_python< boost::shared_ptr<CClientStaticObject> >();
	
	
	
//register_ptr_to_python<boost::shared_ptr<CClientStaticPrototype> >();
		
	class_< Vector2 >
 		("Vector2",init<Real,Real>())
 		.def_readwrite("x",&Vector2::x)
 		.def_readwrite("y",&Vector2::y)
 		.def(self + self)
 		.def(self - self)
 		.def(self * Real() )
 		.def(self / Real() );
 
 	class_< Vector3 >
 		("Vector3", init<Real,Real,Real>())
 		.def_readwrite("x",&Vector3::x)
 		.def_readwrite("y",&Vector3::y)
 		.def_readwrite("z",&Vector3::z)
 		.def(self + self)
 		.def(self - self)
 		.def(self * Real() )
 		.def(self / Real() );
 
 	class_< Radian >
 		("Radian", init<Real>())
 		.def(self + self)
 		.def(self - self)
 		.def(self * Real() )
 		.def(self / Real() )
 		.add_property("value",&Radian::valueRadians);
 
	class_< Quaternion >
 		("Quaternion", init<Radian, Vector3>())
		.def(init<Real, Real, Real, Real>())
		.def_readwrite("w",&Quaternion::w)
		.def_readwrite("x",&Quaternion::x)
 		.def_readwrite("y",&Quaternion::y)
 		.def_readwrite("z",&Quaternion::z)
 		.def(self + self)
 		.def(self - self)
 		.def(self * Real() );

	

 	class_< CWorld >
 		("CWorld",no_init)
// 		.add_property("statics", &CWorld::getStatics )
		.def("translateTo2d",&CWorld::translateTo2d)
        .def("pointIsVisible",&CWorld::pointIsVisible);
 
	
    
 }
 
 void initWorldModule()
 {
 	initWorld();
 };
#endif