/**
  * @file client_interface.cpp
  * Client interface manager
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: client_interface.cpp 856 2007-05-03 05:24:30Z mip $
 
  *  See LICENSE for details
  */
 
#include "stdafx.h"
#include "ireon_client/interface/client_interface.h"
#include "ireon_client/client_app.h"
#include "ireon_client/world/client_world.h"
#include "common/world/managers/dynamic_collection.h"
#include "common/world/dynamic_object/character.h"

#pragma warning(push)
#pragma warning(disable : 4100 4201 4244 4251 4275) 
#include "Ogre/main/OgreCursor.h"
//#include "Ogre/paging_landscape/OgrePagingLandScapeRaySceneQuery.h"
#pragma warning(pop)

#include "ireon_client/interface/client_camera.h"

 CClientInterface* CClientInterface::m_singleton = 0;
 
 CClientInterface::CClientInterface():
 m_prevCamPos(Vector3::ZERO),
 m_firstPerson(false),
 m_enableControl(true),
 camSpeed_(10)
 {
 };
 
 CClientInterface::~CClientInterface()
 {
 };
 
 bool CClientInterface::init(Ogre::RenderWindow* win)
 {
 	_log(_info,_loc,_("Client interface initialization..."));
 
 	CInterface::init(win);
 
 	m_camR = CHAR_SIZE * 4;
 	m_diffY = 0;
	
 	_log(_info,_loc,_("Client interface initialization complete."));
 	return true;
 };
 
 
 void CClientInterface::pulseUpdate(Real time)
 {
	time_last_update_ = time;
	CEGUI::System::getSingletonPtr()->injectTimePulse(time);


	
		getCamera()->update();
	/*	
 	// If this is the first frame, pick a speed
 	if (time == 0)
 	{
		m_rotScale = (Ogre::Real) 0.1;
 	}
 	// Otherwise scale movement units by time passed since last frame
 	else
 		m_rotScale = m_rotateSpeed * time;

	if( CClientApp::instance()->inGame() && m_enableControl)
 	{

		if( isKeyPressed(Ogre::KC_UP) )
 			m_rotY -= m_rotScale;
 
 		if( isKeyPressed(Ogre::KC_DOWN) )
 			m_rotY += m_rotScale;
 
 		if( m_keysPressed.find(Ogre::KC_LEFT) != m_keysPressed.end() )
 			m_rotX -= m_rotScale;
 
 		if( m_keysPressed.find(Ogre::KC_RIGHT) != m_keysPressed.end())
 			m_rotX += m_rotScale;
 	
		if( !m_firstPerson )
 		{
 			if( m_diffY > m_rotScale )
 			{
 				m_rotY -= m_rotScale;
 				m_diffY -= m_rotScale;
 			} else
 			{
 				m_rotY -= m_diffY;
 				m_diffY = 0;
 			}
 			if( m_rotY < -Radian(Math::PI/(Ogre::Real) 2.1) )
 				m_rotY = -Radian(Math::PI/(Ogre::Real)2.1);
 			else if( m_rotY > -Radian(Math::PI/50) )
 				m_rotY = -Math::PI/50;
 			if( m_mousePressed.find(RBUTTON) == m_mousePressed.end() )
 			{
 				if( Math::Abs(m_rotX) > Radian(Math::PI) )
 				{
 					m_rotX = m_rotX - 2 * Radian(Math::PI) * static_cast<int>((m_rotX / Math::PI / 2).valueRadians());
 					if( m_rotX > Radian(Math::PI) )
 						m_rotX -= 2 * Radian(Math::PI);
 					else if( m_rotX < Radian(-Math::PI) )
 						m_rotX += 2 * Radian(Math::PI);
 				}
 				if( m_rotX > Radian(0) )
 					m_rotX = std::max(Radian(0),m_rotX - 5 * m_rotScale);
 				else
 					m_rotX = std::min(Radian(0),m_rotX + 5 * m_rotScale);
 			};
 			Vector3 chHead = CClientWorld::instance()->getOwnChar()->getPosition() + Vector3(0,CHAR_SIZE,0);
 			Radian rotX = m_rotX + CClientWorld::instance()->getOwnChar()->getRotation();
 			Quaternion q(Quaternion::IDENTITY);
 			Quaternion q1(Radian(m_rotY),Vector3::UNIT_X);
 			q = q1 * q;
 			q1.FromAngleAxis(Radian(rotX),Vector3::UNIT_Y);
 			q = q1 * q;
 			Vector3 dir = q * Vector3::NEGATIVE_UNIT_Z;
 			Vector3 toPos = chHead - dir * m_camR;
 			if( m_prevCamPos == Vector3::ZERO || m_mousePressed.find(RBUTTON) != m_mousePressed.end() )
 				m_prevCamPos = toPos;
 			else
 			{
 				Vector3 diff = toPos - m_prevCamPos;
 				Real diffLength = diff.length();
 				if( diffLength )
 				{
 					diff *= time;
 					Real len = diff.length();
 					Real minLen = (Ogre::Real) 1.6 * m_camR * time;
 					if( len < minLen )
 						diff *= std::min(minLen,diffLength) / len;
 					if( len > diffLength )
 						diff *= diffLength / len;
 					m_prevCamPos += diff;
 				}
 			}
 			getPlayerCam()->setPosition(m_prevCamPos);
 			//m_prevCamPos = toPos;
 			Real camR = (m_prevCamPos - chHead).length();
 			getPlayerCam()->lookAt(chHead);
 			if( camR > 1.1 * m_camR )
 				camR = (Ogre::Real) 1.1 * m_camR;
 			dir = getPlayerCam()->getDirection();
 			m_prevCamPos = chHead - dir * camR;
 			Vector3 up = getPlayerCam()->getUp();
 			Vector3 right = getPlayerCam()->getRight();
 			Vector3 dots[5];
 			while(1)
			{
 				dots[0] = chHead - dir * camR;
 				Vector3 center = dots[0] + dir * getPlayerCam()->getNearClipDistance();
 				dots[1] = center + up * getPlayerCam()->getNearClipDistance();
 				dots[2] = center - up * getPlayerCam()->getNearClipDistance();
 				dots[3] = center + right * getPlayerCam()->getNearClipDistance();
 				dots[4] = center - right * getPlayerCam()->getNearClipDistance();
 				bool outside = true;
 				for( uint i = 0; i < 5; i++ )
 					if( CClientWorld::instance()->heightAt(dots[i]) > dots[i].y ) /// Dot is under landscape
 						outside = false;
 				if( outside )
 					break;
 				m_diffY -= Radian((Ogre::Real) 0.005);
 				getPlayerCam()->pitch(-Radian((Ogre::Real)0.005));
 				m_rotY -= Radian((Ogre::Real) 0.005);
 				if( m_rotY < -Radian(Math::PI/(Ogre::Real)2.1) )
 					break;
 				dir = getPlayerCam()->getDirection();
 				up = getPlayerCam()->getUp();
 				right = getPlayerCam()->getRight();
 			};
 			getPlayerCam()->setPosition(dots[0]);
 		}
		else // in-eye view
 		{
 			// Ограничения на повороты камеры
			//
			if( m_rotY < Radian(-Math::PI/6) )
 				m_rotY = Radian(-Math::PI/6);
 			else if( m_rotY > Radian(Math::PI/2) )
 				m_rotY = Math::PI/2;
 			if( m_rotX < Radian(-2 * Math::PI/3) )
 				m_rotX = Radian(-2 * Math::PI/3);
 			else if( m_rotX > Radian(2 * Math::PI/3) )
 				m_rotX = Radian(2 * Math::PI/3);
 			
			// при нажатии правой клавиши уменьшаем угол поворота камеры на утроенный скейл
			if( m_mousePressed.find(RBUTTON) == m_mousePressed.end() )
 			{
 				if( m_rotX > Radian(0) )
 					m_rotX = std::max(Radian(0),m_rotX - 3 * m_rotScale);
 				else
 					m_rotX = std::min(Radian(0),m_rotX + 3 * m_rotScale);
 			};
 			// прибавляем к повороту чара угол поворота камеры
			Radian rotX = m_rotX + CClientWorld::instance()->getOwnChar()->getRotation();
 			// устанавливаем камеру на уровне головы чара
			getPlayerCam()->setPosition(CClientWorld::instance()->getOwnChar()->getPosition() + Vector3(0,CHAR_SIZE,0));
 			//устанавливаем камеру в положение без вращения 
			getPlayerCam()->setOrientation(Quaternion::IDENTITY);
			// вращаем голову вверх-вниз
 			getPlayerCam()->pitch(m_rotY);
			// вращаем голову влево-вправо
			getPlayerCam()->yaw(rotX);
 		}
 		/// Find highlighted character
 		//Ray ray = getPlayerCam()->getCameraToViewportRay(m_eventProcessor->getInputReader()->getCursor()->getX(),m_eventProcessor->getInputReader()->getCursor()->getY());
 		//m_query->setQueryMask(Ogre::RSQ_Entities);
		
 		//m_query->setRay(ray);
 		RaySceneQueryResult& r = m_query->execute();
 		RaySceneQueryResult::iterator it = r.begin();
 		m_highlight = CharacterPtr();
 		for (RaySceneQueryResult::iterator it = r.begin(); it != r.end(); ++it )
 		{
 			if( it->movable != 0 && it->movable->getParentSceneNode() && it->movable->getParentSceneNode()->getParentSceneNode() )
 			{
 				m_highlight = CClientWorld::instance()->findCharacter(it->movable->getParentSceneNode()->getParentSceneNode());
 				if( m_highlight && m_highlight != CClientWorld::instance()->getOwnChar() )
 					break;
 			}
 		} 
 	} else
 	{
 		m_target.reset();
 		m_highlight.reset();
 	}
 
	
 	if( CClientApp::instance()->inGame() && m_enableControl)
 	{
 		/// Moving character
		/// TODO: FIXME: replace with isKeyPressed()
 		if( isKeyPressed(Ogre::KC_W) )
 			CClientWorld::instance()->getOwnChar()->walk(time);
 
 		if( isKeyPressed(Ogre::KC_A)  )
 			CClientWorld::instance()->getOwnChar()->rotate(m_rotScale);
 		
 		if( isKeyPressed(Ogre::KC_D)  )
 			CClientWorld::instance()->getOwnChar()->rotate(-m_rotScale);
 	}
 
 	CClientWorld::instance()->updateCharactersInfo();
	*/
 };
 
 bool CClientInterface::frameStarted(const Ogre::FrameEvent &evt)
 {
 	if( !CInterface::frameStarted(evt) )
 		return false;
 	return true;
 };
 
 void CClientInterface::mouseMoved (Ogre::MouseEvent *e)
 {
 	//CInterface::mouseMoved(e);
 
 	//if( CClientApp::instance()->inGame() )
 	//{
 	//	if( m_mousePressed.find(RBUTTON) != m_mousePressed.end() || m_firstPerson )
 	//	{
 	//		m_rotY += Ogre::Degree(-e->getRelY() * 200);
 	//	}
 	//	if( m_mousePressed.find(RBUTTON) != m_mousePressed.end() )
 	//	{
 	//		m_rotX += Ogre::Degree(-e->getRelX() * 200);
 	//	}
 	//	m_camR -= e->getRelZ() * CHAR_SIZE * 2;
 	//	if( m_camR > CHAR_SIZE * 15 )
 	//		m_camR = CHAR_SIZE * 15;
 	//	if( m_camR < CHAR_SIZE * 2 )
 	//		m_camR = CHAR_SIZE * 2;
 	//}
 }
 
 CharacterPtr CClientInterface::getTarget()
 {
 	if( !m_target || !CClientWorld::instance()->findCharacter(m_target->getId()) )
 		m_target.reset();
 	return m_target;
 };
 
 void CClientInterface::nextTarget()
 {
 	std::vector<CharacterPtr> targets;
 	CClientWorld::instance()->makeVisibleList(targets,true);
 	if( targets.empty() )
 	{
 		m_target = CharacterPtr();
 		return;
 	}
 	if( !m_target )
 	{
 		m_target = targets[0];
 		return;
 	}
 	bool found = false;
 	for( std::vector<CharacterPtr>::iterator it = targets.begin(); it != targets.end(); ++it )
 		if( (*it) == m_target )
 		{
 			if( ++it != targets.end() )
 			{
 				found = true;
 				m_target = *it;
 			}
 			break;
 		};
 	if( !found )
 		m_target = targets[0];
 }
 
 void CClientInterface::mousePressedNoGUI(MouseEvent* e)
 {
 /*	switch (e->getButtonID())
  	{
  	        case e->BUTTON0_MASK:
  	                if( CClientApp::instance()->inGame() )
  	                {
  	                        if( m_highlight )
  	                                m_target = m_highlight;
  	                }
  	                break;
  	        case e->BUTTON1_MASK:
  	                break;
  	        case e->BUTTON2_MASK:
  	                break;
  	}*/
 };
/*
CInterface::getUpdateTime()
{
return m_time;
};*/