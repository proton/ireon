
 
 /* Copyright (C) 2006 ireon.org developers council
  * $Id: keys.cpp  dox $
 
  *  See LICENSE for details
  */

#include "stdafx.h"
#include "common/interface/keys.h"

void ActionMap::setAction(int key,CommandPtr action )
{
	actionMap_.insert(std::pair<int, CommandPtr>(key, action));

};

ActionMap::actionMapType::iterator ActionMap::getAction(int key)
{
	return actionMap_.find(key);
};


//---------------------------------------------------------------------------
void KeyboardActionMap::setDefaultMap()
{
	setAction(OIS::KC_ESCAPE, CommandPtr(new CQuitCommand()));
	setAction(OIS::KC_W, CommandPtr(new CUpRotateCameraCommand()));
	setAction(OIS::KC_S, CommandPtr(new CDownRotateCameraCommand()));
	setAction(OIS::KC_A, CommandPtr(new CLeftRotateCameraCommand()));
	setAction(OIS::KC_D, CommandPtr(new CRightRotateCameraCommand()));

	setAction(OIS::KC_UP, CommandPtr(new CForwardMoveObjectCommand()));
	setAction(OIS::KC_LEFT, CommandPtr(new CLeftMoveObjectCommand()));
	setAction(OIS::KC_RIGHT, CommandPtr(new CRightMoveObjectCommand()));
	setAction(OIS::KC_DOWN, CommandPtr(new CBackMoveObjectCommand()));

	setAction(OIS::KC_HOME, CommandPtr(new CLookCenterCameraCommand()));

	setAction(OIS::KC_R, CommandPtr(new CForwardMoveCameraCommand()));
	setAction(OIS::KC_F, CommandPtr(new CBackMoveCameraCommand()));


	setAction(OIS::KC_O, CommandPtr(new CDecreaseSpeedCameraCommand()));
	setAction(OIS::KC_P, CommandPtr(new CIncreaseSpeedCameraCommand()));

	setAction(OIS::KC_TAB, CommandPtr(new CShowCurrentTriangleNavMeshInfoCommand()));

}

void KeyboardActionMap::loadMap()
{
}

void KeyboardActionMap::saveMap() 
{

} 

//---------------------------------------------------------------------------

void MouseActionMap::setDefaultMap()
{
	setAction(OIS::MC_UP,		CommandPtr(new CUpRotateCameraCommand()));
	setAction(OIS::MC_DOWN,		CommandPtr(new CDownRotateCameraCommand()));
	setAction(OIS::MC_LEFT,		CommandPtr(new CLeftRotateCameraCommand()));
	setAction(OIS::MC_RIGHT,	CommandPtr(new CRightRotateCameraCommand()));

	//setAction(OIS::MB_Left,		CommandPtr(new CFirstActionCommand()));
	//setAction(OIS::MB_Right,	CommandPtr(new CSecondActionCommand()));
	//setAction(OIS::MB_Middle,	CommandPtr(new CSecondActionCommand()));

}

void MouseActionMap::loadMap()
{

}

void MouseActionMap::saveMap() 
{

} 

