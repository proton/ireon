/**
 * @file char_player.cpp
 * Player's character
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: char_player.cpp 579 2006-03-26 12:58:49Z zak $

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
#include "stdafx.h"
#include "world/world.h"
#include "world/char_player.h"
#include "db/client_char_data.h"
//#include "net/player_client.h"


CCharPlayer::CCharPlayer(ClientCharPlayerData* data, const String& mesh):
CCharacter(data->m_name,mesh,data)
{
	m_type = CObject::PLAYER;
	assert(data);
	setWalkSpeed(data->m_velocity);
	m_data = data;
};

CCharPlayer::~CCharPlayer()
{

};

void CCharPlayer::update(Real time)
{
	CCharacter::update(time);
};

COwnCharPlayer::COwnCharPlayer(ClientOwnCharData* data, const String& mesh):
CCharPlayer(data,mesh),
m_stop(true),
m_sinceLastSendPos(0),
m_velocity(0),
m_difference(Vector2::ZERO),
m_correction(0),
m_rotated(false),
m_blocked(false)
{
	m_type = CObject::OWN_PLAYER;
	assert(data);
	m_velocity = data->m_velocity;
	m_data = data;
};

COwnCharPlayer::~COwnCharPlayer()
{
};

void COwnCharPlayer::walk(Real time, Vector2 *direction)
{
	/// We mustn't walk with own character in this way
	/// Direction mus be changed by rotate()
	assert(direction == 0);
	setRotation(m_rotation);
	///Change velocity due to difference in coordinates
	setWalkSpeed(m_velocity + m_correction);
	CCharacter::walk(time, direction);
};


void COwnCharPlayer::setPosition(const Vector3& position)
{
	CObject::setPosition(position);
};


void COwnCharPlayer::update(Real time)
{
	Vector3 normal = PagingLandScapeData2DManager::getSingleton().getNormal(getPosition().x,getPosition().z);
	if( !m_walk && !m_sinceLastSendPos && !m_rotated)
		m_stop = true;
	else
	{
		if( m_stop )
		{
			///Send to server that we started walk
//			CClientApp::instance()->getWorldConn()->sendCoords(getPosition2(),m_rotation,0);
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
	CCharPlayer::update(time);
};

void COwnCharPlayer::rotate(Degree degree)
{
	setRotation(getRotation() + Radian(degree));
	m_rotated = true;
};

void COwnCharPlayer::setDifference(const Vector2 &dif)
{
	m_difference = dif;
	if( (m_difference.squaredLength() > getWalkSpeed() * getWalkSpeed()) && !m_blocked )
	{
		setPosition(getPosition2() - m_difference);
		m_difference = Vector2::ZERO;
//		CClientApp::instance()->getWorldConn()->sendCoords(getPosition2(),getRotation(),0);
		/// We must begin new move after this
		m_stop = true;
		m_blocked = true;
		m_sinceLastSendPos = 0;
		m_rotated = false;
	};
};
