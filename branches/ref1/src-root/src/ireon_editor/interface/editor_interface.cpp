/* Copyright (C) 2005 ireon.org developers council
 * $Id: interface.cpp 510 2006-02-26 21:09:40Z zak $

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
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

CEditorInterface::CEditorInterface()
{
};

CEditorInterface::~CEditorInterface()
{
};

bool CEditorInterface::init(Ogre::RenderWindow* win)
{
	CLog::instance()->log(CLog::msgLvlInfo,"Editor interface initialization...\n");

	CInterface::init(win);

	m_moveSpeed = 10;

	CLog::instance()->log(CLog::msgLvlInfo,"Editor interface initialization complete.\n");
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

	Vector3 move(Vector3::ZERO);	

	if( m_keysPressed.find(Ogre::KC_UP) != m_keysPressed.end() )
		m_rotY -= m_rotScale;

	if( m_keysPressed.find(Ogre::KC_DOWN) != m_keysPressed.end() )
		m_rotY += m_rotScale;

	if( m_keysPressed.find(Ogre::KC_LEFT) != m_keysPressed.end() )
		m_rotX += m_rotScale;

	if( m_keysPressed.find(Ogre::KC_RIGHT) != m_keysPressed.end())
		m_rotX -= m_rotScale;

	if( m_keysPressed.find(Ogre::KC_W) != m_keysPressed.end() )
		move.z -= m_moveScale;

	if( m_keysPressed.find(Ogre::KC_S) != m_keysPressed.end() )
		move.z += m_moveScale;

	if( m_keysPressed.find(Ogre::KC_A) != m_keysPressed.end() )
		move.x -= m_moveScale;
	
	if( m_keysPressed.find(Ogre::KC_D) != m_keysPressed.end() )
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
