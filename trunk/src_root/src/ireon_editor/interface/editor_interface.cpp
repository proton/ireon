/* Copyright (C) 2005 ireon.org developers council
  * $Id: editor_interface.cpp 752 2006-09-23 19:12:44Z zak $
 
  *  See LICENSE for details
  */
 
 /**
  * @file editor_interface.cpp
  * Editor interface manager
  */
 
 #include "stdafx.h"
 
 #include "interface/editor_interface.h"
 #include "editor_app.h"
 #include "world/editor_world.h"
 
 CEditorInterface* CEditorInterface::m_singleton = 0;
 
 CEditorInterface::CEditorInterface():
 m_enableControl(true)
 {
 };
 
 CEditorInterface::~CEditorInterface()
 {
 };
 
 bool CEditorInterface::init(Ogre::RenderWindow* win)
 {
 	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,"Editor interface initialization...\n");
 
 	CInterface::init(win);
 
 	m_moveSpeed = 10;
 
 	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,"Editor interface initialization complete.\n");
 	return true;
 };
 
 
 bool CEditorInterface::frameStarted(const Ogre::FrameEvent &evt)
 {
 	CInterface::frameStarted(evt);
 
 	return true;
 };
 
 void CEditorInterface::mouseMoved (Ogre::MouseEvent *e)
 {
 	if( m_mousePressed.find(RBUTTON) != m_mousePressed.end()  )
 	{
 		m_rotY += Ogre::Degree(-e->getRelY() * 200);
 	}
 	if( m_mousePressed.find(RBUTTON) != m_mousePressed.end() )
 	{
 		m_rotX += Ogre::Degree(-e->getRelX() * 200);
 	}
 
 
 	CInterface::mouseMoved(e);
 }
 
 void CEditorInterface::mousePressedNoGUI(Ogre::MouseEvent *e)
 {
 	switch (e->getButtonID())
 	{
 		case e->BUTTON0_MASK:
 			break;
 		case e->BUTTON1_MASK:
 			break;
 		case e->BUTTON2_MASK:
 			break;
 	}
 }
 
 void CEditorInterface::pulseUpdate(Real time)
 {
 	// If this is the first frame, pick a speed
 	if (time == 0)
 	{
 		m_rotScale = 0.1;
 		m_moveScale = 0.1;
 	}
 	// Otherwise scale movement units by time passed since last frame
 	else
 	{
 		m_rotScale = m_rotateSpeed * time;
 		m_moveScale = m_moveSpeed * time * ((isKeyPressed(KC_LSHIFT) || isKeyPressed(KC_RSHIFT)) ? 20 : 1 );
 	}

	if (!m_enableControl) // WARNING !!! this may do code unwork bottom this line
		return;
 
 	Vector3 move(Vector3::ZERO);	
 
 	if( isKeyPressed(Ogre::KC_UP) )
 		m_rotY -= m_rotScale;
 
 	if( isKeyPressed(Ogre::KC_DOWN) )
 		m_rotY += m_rotScale;
 
 	if( isKeyPressed(Ogre::KC_LEFT) )
 		m_rotX += m_rotScale;
 
 	if( isKeyPressed(Ogre::KC_RIGHT) )
 		m_rotX -= m_rotScale;
 
 	if( isKeyPressed(Ogre::KC_W) )
 		move.z -= m_moveScale;
 
 	if( isKeyPressed(Ogre::KC_S) )
 		move.z += m_moveScale;
 
 	if( isKeyPressed(Ogre::KC_A) )
 		move.x -= m_moveScale;
 	
 	if( isKeyPressed(Ogre::KC_D) )
 		move.x += m_moveScale;
 
 	if( Math::Abs(m_rotX) > Radian(Math::PI) )
 	{
 		m_rotX = m_rotX - 2 * Radian(Math::PI) * static_cast<int>((m_rotX / Math::PI / 2).valueRadians());
 		if( m_rotX > Radian(Math::PI) )
 			m_rotX -= 2 * Radian(Math::PI);
 		else if( m_rotX < Radian(-Math::PI) )
 			m_rotX += 2 * Radian(Math::PI);
 	}
 
 	if( Math::Abs(m_rotY) > Radian(Math::PI) )
 	{
 		m_rotY = m_rotY - 2 * Radian(Math::PI) * static_cast<int>((m_rotY / Math::PI / 2).valueRadians());
 		if( m_rotY > Radian(Math::PI) )
 			m_rotY -= 2 * Radian(Math::PI);
 		else if( m_rotY < Radian(-Math::PI) )
 			m_rotY += 2 * Radian(Math::PI);
 	}
 
 	Quaternion q(Quaternion::IDENTITY);
 	Quaternion q1(Radian(m_rotY),Vector3::UNIT_X);
 	q = q1 * q;
 	q1.FromAngleAxis(Radian(m_rotX),Vector3::UNIT_Y);
 	q = q1 * q;
 	getPlayerCam()->setOrientation(q);
 
 	getPlayerCam()->moveRelative(move);
 };
 
 void CEditorInterface::keyPressedNoGUI(KeyEvent *e)
 {
 	if (e->getKey() == KC_R)
 	{
 		m_sceneDetailIndex = (m_sceneDetailIndex+1)%3 ;
 		switch(m_sceneDetailIndex) {
 			case 0 : getPlayerCam()->setDetailLevel(SDL_SOLID) ; break ;
 			case 1 : getPlayerCam()->setDetailLevel(SDL_WIREFRAME) ; break ;
 			case 2 : getPlayerCam()->setDetailLevel(SDL_POINTS) ; break ;
 		}
 	}
 };
