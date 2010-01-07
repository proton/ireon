/**
* @file object_manager.cpp
* Editor world manager
*/

/* Copyright (C) 2005 ireon.org developers council
* $Id: command.cpp 825 2006-11-29 14:39:37Z mip $

*  See LICENSE for details
*/
#include "stdafx.h"
#include "common/world/managers/command.h"
#include "common/world/managers/object_factory.h"
#include "common/world/managers/static_collection.h"
#include "common/world/managers/dynamic_collection.h"
#include "common/world/managers/prototype_manager.h"
#include "common/world/dynamic_object/character.h"
#include "common/world/static_object/static_prototype.h"
#include "common/world/static_object/static.h"
#include "ireon_client/interface/client_interface.h"
#include "ireon_client/world/client_world.h"
#include "ireon_client/client_app.h"
#include "common/world/mesh.h"
#include "common/interface/debug_overlay.h"
#include "common/world/client_zone.h"
void CCreateOwnPlayerCommand::execute()
{
	CPrototypeManager::iterator protoIt = CPrototypeManager::getSingletonPtr()->find(protypeName_);
	COwnPlayer* ownPlayer = (COwnPlayer*)CObjectFactory::getSingletonPtr()->createProduct("ownPlayer", *protoIt );
	CDynamicCollection::getSingletonPtr()->setOwnPlayer(ownPlayer);
	
}
String CCreateOwnPlayerCommand::description() const 
{
	String desc;
	desc += "Create own player object  from prototype ";
	desc += protypeName_;
	return desc;
};
//-------------------------------------------------------------
void CCreateStaticObjectCommand::execute()
{
	CPrototypeManager::iterator protoIt = CPrototypeManager::getSingletonPtr()->find(protypeName_);
	// create object and add him in static collection
	CStaticObject* staticObject = (CStaticObject*)CObjectFactory::getSingletonPtr()->createProduct("static", *protoIt );
	staticObject->setName(objectName_.c_str());
	// don't need .. see earlier
	//CStaticCollection::getSingletonPtr()->addObject(staticObject);
}
String CCreateStaticObjectCommand::description() const 
{
	String desc;
	desc += "Create static object '";
	desc += objectName_;
	desc += "' from prototype ";
	desc += protypeName_;
	return desc;
};

///---------------------- 
void CLoadOwnPlayerPrototypeCommand::execute()
{
	CPrototypeManager::getSingletonPtr()->loadOwnPlayerPrototypes();

};
String CLoadOwnPlayerPrototypeCommand::description() const
{
	String desc;
	desc += "Load Own Player Prototype...";
	return desc;
};
//----------------------------------
void CLoadStaticPrototypesCommand::execute()
{
	CPrototypeManager::getSingletonPtr()->loadStaticPrototypes();

};
String CLoadStaticPrototypesCommand::description() const
{
	String desc;
	desc += "Load Static Prototypes...";
	return desc;
};

///-----------------------------------

///-----------------------------------

void CMoveAbsoluteStaticObjectCommand::execute()
{
	CStaticCollection::iterator it = CStaticCollection::getSingletonPtr()->find(m_objectName);
	(*it)->setPosition3D(m_pos);
};
String CMoveAbsoluteStaticObjectCommand::description() const
{
	String desc;
	desc += "Move static object ";
	desc += m_objectName;
	desc += " to position ";
	desc += StringConverter::toString(m_pos);
	return desc;
};
///-----------------------------------

void CRotateAbsoluteStaticObjectCommand::execute()
{
	CStaticCollection::iterator it = CStaticCollection::getSingletonPtr()->find(objectName_);
	(*it)->setOrientation(q_);
};
String CRotateAbsoluteStaticObjectCommand::description() const
{
	String desc;
	desc += "Rotate static object ";
	desc += objectName_;
	desc += " ; set orientation (quaternion) =  ";
	desc += StringConverter::toString(q_);
	return desc;
};
///------------------------------

void CLoadStaticObjectCommand::execute()
{
	CStaticCollection::iterator it = CStaticCollection::getSingletonPtr()->find(m_objectName);
	(*it)->load();
};
String CLoadStaticObjectCommand::description() const
{
	String desc;
	desc += "Load static object ";
	desc += m_objectName;
	return desc;
};

///-----------------------------------


void CLoadAllStaticObjectCommand::execute()
{
	CStaticCollection::iterator it;
	CStaticCollection* collPtr = CStaticCollection::getSingletonPtr();
	it = collPtr->begin();
	CStaticCollection::iterator endit = collPtr->end();
	for( ; it != endit; ++it)
	{
		_log(_info,_loc,_("Loading '%s'... \b"), (*it)->getName());
		if ( m_statement(*(*it)) )
		{
			_log(_info,_loc,_("Yes"));
			(*it)->load();
		}
		else
			_log(_info,_loc,_("No"));
	};
};
String CLoadAllStaticObjectCommand::description() const
{
	String desc;
	desc += "Load All static object ";

	return desc;
};

///-----------------------------------

void CUnloadAllStaticObjectCommand::execute()
{
	CStaticCollection::iterator it;
	CStaticCollection* collPtr = CStaticCollection::getSingletonPtr();
	for( it = collPtr->begin(); it != collPtr->end(); ++it)
		if ( m_statement(*(*it)) )
			(*it)->unload();
};
String CUnloadAllStaticObjectCommand::description() const
{
	String desc;
	desc += "Unload All static object";
	return desc;
};
//------------------------------------------------------------------
void CQuitCommand::execute()
{
	CClientApp::instance()->shutdown();
};
String CQuitCommand::description() const
{
	String desc;
	desc += "Shutdown Signal received";
	return desc;
};
//------------------------------------------------------------------
void CRightRotateCameraCommand::execute()
{
	CClientInterface* instance = CClientInterface::instance();
	
	Radian rotX = instance->getRotateX();
	Radian rotY = instance->getRotateY();

	Real time_last_update = instance->getTimeLastUpdate();
	instance->setRotateScale(Degree(0.1));
	//if (time_last_update == 0)
	//	instance->setRotateScale(Degree(0.1));
	//// Otherwise scale movement units by time passed since last frame
	//else
	//	instance->setRotateScale( instance->getRotateSpeed() * time_last_update );
	
	if( CClientApp::instance()->inGame() )
	{
		//rotX -= instance->getRotateScale();
		rotX = -0.05;
		// in-eyes
		//if( rotX < Radian(-2 * Math::PI/3) )
		//	rotX = Radian(-2 * Math::PI/3);
		//rotX += CClientWorld::instance()->getOwnChar()->getRotation();
		//instance->getPlayerCam()->setPosition(CClientWorld::instance()->getOwnChar()->getPosition() + Vector3(0,CHAR_SIZE,0));
		//instance->getPlayerCam()->setOrientation(Quaternion::IDENTITY);
		instance->getPlayerCam()->yaw(rotX);
		_log(_info,_loc,_("%g %g"), rotX.valueDegrees(), rotY.valueDegrees());
		instance->setRotateX(rotX);
	}	
}

String CRightRotateCameraCommand::description()const
{
	String desc;
	desc += "Right rotate camera";
	return desc;
}

void CLeftRotateCameraCommand::execute()
{
	CClientInterface* instance = CClientInterface::instance();

	Radian rotX = instance->getRotateX();
	Radian rotY = instance->getRotateY();

	Real time_last_update = instance->getTimeLastUpdate();
	instance->setRotateScale(Degree(0.1));
	//if (time_last_update == 0)
	//	instance->setRotateScale(Degree(0.1));
	//// Otherwise scale movement units by time passed since last frame
	//else
	//	instance->setRotateScale( instance->getRotateSpeed() * time_last_update );

	if( CClientApp::instance()->inGame() )
	{
		//rotX += instance->getRotateScale();
		rotX = 0.05;
		// in-eyes
		//if( rotX > Radian(2 * Math::PI/3) )
		//	rotX = Radian(2 * Math::PI/3);
		//rotX += CClientWorld::instance()->getOwnChar()->getRotation();
		//instance->getPlayerCam()->setPosition(CClientWorld::instance()->getOwnChar()->getPosition() + Vector3(0,CHAR_SIZE,0));
		//instance->getPlayerCam()->setOrientation(Quaternion::IDENTITY);
		instance->getPlayerCam()->yaw(rotX);
		_log(_info,_loc,_("%g %g"), rotX.valueDegrees(), rotY.valueDegrees());
		instance->setRotateX(rotX);
	}	
}
String CLeftRotateCameraCommand::description()const
{
	String desc;
	desc += "Left rotate camera";
	return desc;
}


void CUpRotateCameraCommand::execute()
{
	CClientInterface* instance = CClientInterface::instance();

	Radian rotX = instance->getRotateX();
	Radian rotY = instance->getRotateY();

	Real time_last_update = instance->getTimeLastUpdate();
	instance->setRotateScale(Degree(0.1));
	//if (time_last_update == 0)
	//	instance->setRotateScale(Degree(0.1));
	//// Otherwise scale movement units by time passed since last frame
	//else
	//	instance->setRotateScale( instance->getRotateSpeed() * time_last_update );
	if( CClientApp::instance()->inGame() )
	{
		//rotY += instance->getRotateScale();

		// in-eyes
		if( rotY > Radian(Math::PI/2 -Math::PI/6) )
			rotY = Radian(Math::PI/2 -Math::PI/6);
		rotY = 0.1;
		//instance->getPlayerCam()->setPosition(CClientWorld::instance()->getOwnChar()->getPosition() + Vector3(0,CHAR_SIZE,0));
		//instance->getPlayerCam()->setOrientation(Quaternion::IDENTITY);
		instance->getPlayerCam()->pitch(rotY);
		_log(_debug,_loc,_("rotX '%g' rotY '%g'"), rotX.valueDegrees(), rotY.valueDegrees());
		instance->setRotateY(rotY);
	}
}
String CUpRotateCameraCommand::description()const
{
	String desc;
	desc += "Up rotate camera";
	return desc;
}

void CDownRotateCameraCommand::execute()
{
	CClientInterface* instance = CClientInterface::instance();
	
	Radian rotX = instance->getRotateX();
	Radian rotY = instance->getRotateY();

	Real time_last_update = instance->getTimeLastUpdate();
	instance->setRotateScale(Degree(0.1));

	//if (time_last_update == 0)
	//	instance->setRotateScale(Degree(0.1));
	//// Otherwise scale movement units by time passed since last frame
	//else
	//	instance->setRotateScale( instance->getRotateSpeed() * time_last_update );
	//
	if( CClientApp::instance()->inGame() )
	{
		//rotY -= instance->getRotateScale();
		rotY = -0.1;
		// in-eyes
		if( rotY < Radian(-Radian(Math::PI/2 -Math::PI/6)) )
			rotY = Radian(-Radian(Math::PI/2 -Math::PI/6));
		
		//instance->getPlayerCam()->setPosition(CClientWorld::instance()->getOwnChar()->getPosition() + Vector3(0,CHAR_SIZE,0));
		//instance->getPlayerCam()->setOrientation(Quaternion::IDENTITY);
		instance->getPlayerCam()->pitch(rotY);
		_log(_debug,_loc,_("rotX '%g' rotY '%g'"), rotX.valueDegrees(), rotY.valueDegrees());
		instance->setRotateY(rotY);
	}
}
String CDownRotateCameraCommand::description()const
{
	String desc;
	desc += "Down rotate camera";
	return desc;
}



void CDecreaseSpeedCameraCommand::execute()
{
	CClientInterface* instance = CClientInterface::instance();
	instance->setCameraSpeed( ((instance->getCameraSpeed()-1) < 1)?1:instance->getCameraSpeed()-1);
}

String CDecreaseSpeedCameraCommand::description()const
{
	String desc;
	desc += "Decrease speed camera.";
	return desc;
}


void CIncreaseSpeedCameraCommand::execute()
{
	CClientInterface* instance = CClientInterface::instance();
	instance->setCameraSpeed( ((instance->getCameraSpeed()+1) >  20 )?20:instance->getCameraSpeed()+1);

}
String CIncreaseSpeedCameraCommand::description()const
{
	String desc;
	desc += "Increase speed camera";
	return desc;
}



//------------------------------------------------------------------

void CForwardMoveObjectCommand::execute()
{
	CClientInterface* instance = CClientInterface::instance();

	Radian rotX = instance->getRotateX();
	Radian rotY = instance->getRotateY();

	Real time_last_update = instance->getTimeLastUpdate();
	//if (time_last_update == 0)
	//	instance->setRotateScale(Degree(0.1));
	//// Otherwise scale movement units by time passed since last frame
	//else
	//	instance->setRotateScale( instance->getRotateSpeed() * time_last_update );
	if( CClientApp::instance()->inGame() )
	{
		//rotY -= instance->getRotateScale();
		// in-eyes
/*
		if( rotY < Radian(-Math::PI/6) )
			rotY = Radian(-Math::PI/6);
		else if( rotY > Radian(Math::PI/2) )
			rotY = Math::PI/2;
		if( rotX < Radian(-2 * Math::PI/3) )
			rotX = Radian(-2 * Math::PI/3);
		else if( rotX > Radian(2 * Math::PI/3) )
			rotX = Radian(2 * Math::PI/3);

		rotX += CClientWorld::instance()->getOwnChar()->getRotation();
		*/
		COwnPlayer* ownPlayer = CDynamicCollection::getSingletonPtr()->getOwnPlayer();
	//////instance->getPlayerCam()->setPosition( ownPlayer->getPosition3D() + Vector3(0,CHAR_SIZE,0));
	//////instance->getPlayerCam()->setOrientation(Quaternion::IDENTITY);
		//instance->getPlayerCam()->pitch(rotY);
		//instance->getPlayerCam()->yaw(rotX);
		_log(_info,_loc,_("%g %g"), rotX.valueDegrees(), rotY.valueDegrees());
		//instance->setRotateX(rotX);
		//instance->setRotateY(rotY);

		Vector2 dir(1,0);
		ownPlayer->walk(time_last_update,&dir);
	}
}
String CForwardMoveObjectCommand::description()const
{
	String desc;
	desc += "Move forward own char";
	return desc;
}

//------------------------------------------------------------------

void CBackMoveObjectCommand::execute()
{
	CClientInterface* instance = CClientInterface::instance();
	Real time_last_update = instance->getTimeLastUpdate();
	if( CClientApp::instance()->inGame() )
	{
		COwnPlayer* ownPlayer = CDynamicCollection::getSingletonPtr()->getOwnPlayer();
		Vector2 dir(-1,0);
		ownPlayer->walk(time_last_update,&dir);
	}
}
String CBackMoveObjectCommand::description()const
{
	String desc;
	desc += "Move back own char";
	return desc;
}

//------------------------------------------------------------------

void CLeftMoveObjectCommand::execute()
{
	CClientInterface* instance = CClientInterface::instance();
	Real time_last_update = instance->getTimeLastUpdate();
	if( CClientApp::instance()->inGame() )
	{
		COwnPlayer* ownPlayer = CDynamicCollection::getSingletonPtr()->getOwnPlayer();
		Vector2 dir(0,-1);
		ownPlayer->walk(time_last_update,&dir);
	}
}
String CLeftMoveObjectCommand::description()const
{
	String desc;
	desc += "Move left own char";
	return desc;
}

//------------------------------------------------------------------

void CRightMoveObjectCommand::execute()
{
	CClientInterface* instance = CClientInterface::instance();
	Real time_last_update = instance->getTimeLastUpdate();
	if( CClientApp::instance()->inGame() )
	{
		COwnPlayer* ownPlayer = CDynamicCollection::getSingletonPtr()->getOwnPlayer();
		Vector2 dir(0,1);
		ownPlayer->walk(time_last_update,&dir);
	}
}
String CRightMoveObjectCommand::description()const
{
	String desc;
	desc += "Move right own char";
	return desc;
}

void CLeftRotateObjectCommand::execute()
{
	CClientInterface* instance = CClientInterface::instance();

	Radian rotX = instance->getRotateX();
	Radian rotY = instance->getRotateY();

	Real time_last_update = instance->getTimeLastUpdate();
	instance->setRotateScale(Degree(1));
	//if (time_last_update == 0)
	//	instance->setRotateScale(Degree(0.1));
	//// Otherwise scale movement units by time passed since last frame
	//else
	//	instance->setRotateScale( instance->getRotateSpeed() * time_last_update );
	if( CClientApp::instance()->inGame() )
	{
		_log(_info,_loc,_("%g %g"), rotX.valueDegrees(), rotY.valueDegrees());
		COwnPlayer* ownPlayer = CDynamicCollection::getSingletonPtr()->getOwnPlayer();
		ownPlayer->rotate( instance->getRotateScale());
	}
}
String CLeftRotateObjectCommand::description()const
{
	String desc;
	desc += "Left rotate own char";
	return desc;	
}


void CRightRotateObjectCommand::execute()
{
	CClientInterface* instance = CClientInterface::instance();

	Radian rotX = instance->getRotateX();
	Radian rotY = instance->getRotateY();

	Real time_last_update = instance->getTimeLastUpdate();
	instance->setRotateScale(Degree(1));
	//if (time_last_update == 0)
	//	instance->setRotateScale(Degree(0.1));
	//// Otherwise scale movement units by time passed since last frame
	//else
	//	instance->setRotateScale( instance->getRotateSpeed() * time_last_update );
	if( CClientApp::instance()->inGame() )
	{
		_log(_info,_loc,_("%g %g"), rotX.valueDegrees(), rotY.valueDegrees());
		COwnPlayer* ownPlayer = CDynamicCollection::getSingletonPtr()->getOwnPlayer();
		ownPlayer->rotate(- instance->getRotateScale());
	}
}
String CRightRotateObjectCommand::description()const
{
	String desc;
	desc += "Right rotate own char";
	return desc;
}


void CLookCenterCameraCommand::execute()
{
	CClientInterface* instance = CClientInterface::instance();
	instance->setRotateX(Radian(0));
	instance->setRotateY(Radian(0));
	COwnPlayer* ownPlayer = CDynamicCollection::getSingletonPtr()->getOwnPlayer();
	instance->getPlayerCam()->setPosition(ownPlayer->getPosition3D() + Vector3(0,CHAR_SIZE,0));
	instance->getPlayerCam()->setOrientation( ownPlayer->getOrientation() );
	
}
String CLookCenterCameraCommand::description()const
{
	String desc;
	desc += "Look center camera";
	return desc;
}

void CForwardMoveCameraCommand::execute()
{
	CClientInterface* instance = CClientInterface::instance();

	Radian rotX = instance->getRotateX();
	Radian rotY = instance->getRotateY();

	Real time_last_update = instance->getTimeLastUpdate();
	if( CClientApp::instance()->inGame() )
	{
		Vector3 look = Vector3::ZERO;
		look.z -= instance->getCameraSpeed();
		instance->getPlayerCam()->moveRelative(look);
	}

}
String CForwardMoveCameraCommand::description()const
{
	String desc;
	desc += "Move forward camera";
	return desc;
}

void CBackMoveCameraCommand::execute()
{
	CClientInterface* instance = CClientInterface::instance();

	Real time_last_update = instance->getTimeLastUpdate();
	if( CClientApp::instance()->inGame() )
	{
		Vector3 look = Vector3::ZERO;
		look.z += instance->getCameraSpeed();;
		instance->getPlayerCam()->moveRelative(look);
	}

}
String CBackMoveCameraCommand::description()const
{
	String desc;
	desc += "Move back camera";
	return desc;
}
//------------------------------------------------------------------
void CLeftRotateFollowObjectCommand::execute()
{
	
}
String CLeftRotateFollowObjectCommand::description()const
{
	String desc;
	desc += "Move back camera";
	return desc;
}
//------------------------------------------------------------------
void CRightRotateFollowObjectCommand::execute()
{

}
String CRightRotateFollowObjectCommand::description()const
{
	String desc;
	desc += "Move back camera";
	return desc;
}
//------------------------------------------------------------------
void CForwardMoveFollowObjectCommand::execute()
{
	// ------ убрать все это - вынести в модуль слежения за любым персонажом

	// определить тип взгляда
	// если вне глаз
	// направить камеру на персонажа
	// определить расстояние до персонажа
	// если расстояния отлчиаются от заданого
	// выполнить команду выставки определеного расстояния
	// интерполируя изменения расстояние, т.е. это затянется на несколько шагов
}
String CForwardMoveFollowObjectCommand::description()const
{
	String desc;
	desc += "Move forvard for object camera";
	return desc;
}
//------------------------------------------------------------------
void CBackMoveFollowObjectCommand::execute()
{

}
String CBackMoveFollowObjectCommand::description()const
{
	String desc;
	desc += "Move back for object camera";
	return desc;
}



//------------------------------------------------------------------
void CShowCurrentTriangleNavMeshInfoCommand::execute()
{
	ireon::nav::Point p = CDynamicCollection::getSingletonPtr()->getOwnPlayer()->getPointOnMesh();
	ireon::nav::Triangle* curentTriangle;
	try
	{
		curentTriangle = CClientWorld::instance()->getZone()->getNavigationMesh()->getTriangleForPoint(p);
	}
	catch (ireon::nav::Mesh::ETriangleNotFound)
	{
		_OUT(1,"Triangle: not found");
		return;
	}
	ireon::nav::Triangle* n1 = curentTriangle->getNeighbor(0);
	ireon::nav::Triangle* n2 = curentTriangle->getNeighbor(1);
	ireon::nav::Triangle* n3 = curentTriangle->getNeighbor(2);
	
	_OUT(1,"Triangle: [%u] Neigbours: [%u, %u, %u]",
		curentTriangle->number,
		n1?n1->number:255,
		n2?n2->number:255,
		n3?n3->number:255
		);
}
String CShowCurrentTriangleNavMeshInfoCommand::description()const
{
	String desc;
	desc += "Show Info for current Triangle on navigation mesh";
	return desc;
}
