/* Copyright (C) 2005 ireon.org developers council
* $Id$

*  See LICENSE for details
*/

/**
* @file client_camera.cpp
* LookAt for object, movie move , etc
*/

#include "stdafx.h"
#include "ireon_client/interface/client_camera.h"
#include "ireon_client/world/client_world.h"

#include "common/interface/debug_overlay.h"

void ClientCamera::watch(CObject* target)
{

	if (target_  && target_->getSceneNode()->getChild("third-person-target-node"))
		target_->getSceneNode()->removeAndDestroyChild("third-person-target-node");
	
	target_ = target;
	
	target_->getSceneNode()->createChildSceneNode("third-person-target-node",Vector3(0,0,6));
	target_->getSceneNode()->getChild("third-person-target-node")->setInheritOrientation(false);
	target_->getSceneNode()->getChild("third-person-target-node")->setInheritScale(false);

}

void ClientCamera::setObserveMode(ObserveMode mode)
{
	mode_ = mode;
}

void ClientCamera::update()
{
	const Real MAX_DISTANCE  = 50;
	Vector3 distance;
	SceneNode* aaa = (SceneNode*)target_->getSceneNode()->getChild("third-person-target-node");

	switch (mode_)
	{
	case IN_EYE:
		// �� ��������� ��� Z ����������� ����,
		// �������� �� ��������� �� ����� ������������ ������ ������,
		// �� ��������� �������� �� ��� ������

		break;
	case THIRD_PERSON:
		// ���� ����� - ������ ������ ������ ���������� �� ����������.
		
		//1. ������ ������� �� target_ , �� �������� �� �������� �������
		// ���������� ���������� �� �������� ���� target_
		//distance = target_->getSceneNode()->getChild("third-person-target-node")->getPosition() - CClientWorld::instance()->sceneManager()->getCamera("PlayerCam")->getPosition() ;
		//���� ������ ������ ��� ���� - ����������� ������ ����� 

		/*if (distance.length() != MAX_DISTANCE)
		{
			Real diff = distance.length() - MAX_DISTANCE;
			distance.normalise();
			distance *= diff;
			CClientWorld::instance()->sceneManager()->getCamera("PlayerCam")->move(distance);
		}*/
		//2. ������ �� ������ ������� � ��������� YZ)
		// ����. ����������� �� �����(�.�. ����� , �� ��� ������� ��� taget ��������� �� ������� Z) - ��� ��������� �� ������ ����������� ������ � _target
		// 
		distance = target_->getSceneNode()->getChild("third-person-target-node")->getWorldPosition() - target_->getSceneNode()->getWorldPosition() - Vector3(0,2,0);//0,3,0 - ������ ������
		distance.normalise();
		distance *= -MAX_DISTANCE;
		CClientWorld::instance()->sceneManager()->getCamera("PlayerCam")->setPosition(target_->getSceneNode()->getWorldPosition() + distance);
		if (target_)
			CClientWorld::instance()->sceneManager()->getCamera("PlayerCam")->setAutoTracking(true,(SceneNode*)target_->getSceneNode()->getChild("third-person-target-node"));
		break;
	case FREE_MOVE:

		break;
	}


	_OUT(3,"third-person-target-node: Pos: (%g, %g, %g)",
		target_->getSceneNode()->getChild("third-person-target-node")->getWorldPosition().x,
		target_->getSceneNode()->getChild("third-person-target-node")->getWorldPosition().y,
		target_->getSceneNode()->getChild("third-person-target-node")->getWorldPosition().z
		);

	_OUT(4,"target_: Pos: (%g, %g, %g)",
		target_->getSceneNode()->getWorldPosition().x,
		target_->getSceneNode()->getWorldPosition().y,
		target_->getSceneNode()->getWorldPosition().z
		);
	_OUT(5,"Camera: Pos: (%g, %g, %g)",
		CClientWorld::instance()->sceneManager()->getCamera("PlayerCam")->getPosition().x,
		CClientWorld::instance()->sceneManager()->getCamera("PlayerCam")->getPosition().y,
		CClientWorld::instance()->sceneManager()->getCamera("PlayerCam")->getPosition().z
		);
	
		
}