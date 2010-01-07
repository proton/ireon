/**
  * @file char_player.cpp
  * Player's character
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: char_player.cpp 822 2006-11-22 13:34:49Z mip $
 
  *  See LICENSE for details
  */
#include "stdafx.h"
#include "common/world/dynamic_object/character.h"
#include "common/world/generic_object/prototype.h"
#include "common/world/managers/dynamic_collection.h"
#include "ireon_client/world/client_world.h"
 
CCharacter::CCharacter(CPrototype* proto):
CDynamicObject(proto)
{
	type_ = CObject::PLAYER;
	setWalkSpeed(0.5);
};

CCharacter::~CCharacter()
{
};

CCharacter* CCharacter::productCallback(CPrototype* proto)
{
	assert(proto);
	CCharacter* characterObject = new CCharacter(proto);
	assert(characterObject);
	CDynamicCollection::getSingletonPtr()->addObject(characterObject);
	return characterObject;
}

void CCharacter::update(Real time)
{
	CDynamicObject::update(time);
};

//------------------------------------------------------

COwnPlayer::COwnPlayer(CPrototype* proto):
CCharacter(proto),
m_stop(true),
m_sinceLastSendPos(0),
m_velocity(1),
m_difference(Vector2::ZERO),
m_correction(0),
m_rotated(false),
m_blocked(false)
{
	type_ = CObject::OWN_PLAYER;
};

COwnPlayer::~COwnPlayer()
{
};

COwnPlayer* COwnPlayer::productCallback(CPrototype* proto)
{
	assert(proto);
	COwnPlayer* characterObject = new COwnPlayer(proto);
	assert(characterObject);
	CDynamicCollection::getSingletonPtr()->addObject(characterObject);
	return characterObject;
}

void COwnPlayer::walk(Real time, Vector2 *direction)
{
	/// We mustn't walk with own character in this way
	/// Direction mus be changed by rotate()
	//assert(direction == 0);
	setRotation(m_rotation);
	///Change velocity due to difference in coordinates
	setWalkSpeed(m_velocity + m_correction);
	CCharacter::walk(time, direction);
};



void COwnPlayer::update(Real time)
{
	// берем нормаль в точке x,y - проверить x, z может быть ???!!!!
	Vector3 normal = I_WORLD->getNormalAt( getPosition3D().x , getPosition3D().y );
	// если не идем и не послали позицию и не повернулись
	if( !m_walk && !m_sinceLastSendPos && !m_rotated)
		m_stop = true;
	// мы идем или послали позицию или повернулись
	else
	{
		// нужно остановиться
		if( m_stop )
		{
			///Send to server that we started walk
			//CClientApp::instance()->getWorldConn()->sendCoords(getPosition2(),m_rotation,0);
			m_sinceLastSendPos = 0;
			m_rotated = false;
			m_stop = false;
			m_blocked = false;
		};
		m_sinceLastSendPos += time;
		if( m_sinceLastSendPos > 0.450 || ( m_rotated && m_walk && m_sinceLastSendPos >= 0.100 ))
		{
			uint t = static_cast<uint>(m_sinceLastSendPos * 1000);
			/// Send our position to server
			if( t > 500 )
				t = 500;
//			CClientApp::instance()->getWorldConn()->sendCoords(getPosition2(),m_rotation,t);
			m_sinceLastSendPos = 0;
			m_rotated = false;
		};
		/// Compute correction
		if( !m_difference.isZeroLength() )
		{ 
			/// Originally (m_rotation == 0) we turned to (0,-1)
			/// Correction is minus projection of m_difference to move direction
			///
			///  -y
			///  ^
			///  |
			///  |
			///  |------> x
			///  |
			///  |
			///  V
			///  y(z in 3d)
			/// correction = -m_difference * direction = -m_difference * (-sin(m_rotation),-cos(m_rotation))
			m_correction = m_difference.dotProduct(Vector2(Math::Sin(m_rotation,true),Math::Cos(m_rotation,true)));
		} else
			m_correction = 0;
	}
	CCharacter::update(time);
};

void COwnPlayer::rotate(Degree degree)
{
	setRotation(getRotation() + Radian(degree));
	m_rotated = true;
};

void COwnPlayer::setDifference(const Vector2 &dif)
{
	m_difference = dif;
	if( (m_difference.squaredLength() > getWalkSpeed() * getWalkSpeed()) && !m_blocked )
	{
		setPosition2D(getPosition2D() - m_difference);
		m_difference = Vector2::ZERO;
// 		CClientApp::instance()->getWorldConn()->sendCoords(getPosition2(),getRotation(),0);
		/// We must begin new move after this
		m_stop = true;
		m_blocked = true;
		m_sinceLastSendPos = 0;
		m_rotated = false;
	};
};
