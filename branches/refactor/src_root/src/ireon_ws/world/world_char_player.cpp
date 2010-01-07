/* Copyright (C) 2005 ireon.org developers council
 * $Id: world_char_player.cpp 433 2005-12-20 20:19:15Z zak $

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
 * @file world_char_player.cpp
 * Player's character
 */

#include "stdafx.h"
#include "world/world_char_player.h"
#include "db/client_char_data.h"
#include "db/world_db.h"
#include "world/world.h"
#include "net/ws_player_context.h"



CWorldCharPlayer::CWorldCharPlayer():
m_sinceLastMove(0),
m_context(NULL),
m_difference(Vector2::ZERO),
m_blocked(false)
{
};

CWorldCharPlayer::~CWorldCharPlayer()
{
};

void CWorldCharPlayer::serialize(CData& data)
{
	CSynchronizer::serialize(data);
	if( data.store() )
	{
		data << m_id;
		data << m_accountId;
		data << m_name;
		data << m_startPos;
	} else
	{
		data >> m_id;
		data >> m_accountId;
		data >> m_name;
		data >> m_startPos;
	}
};

void CWorldCharPlayer::init()
{
	m_startPos = Vector2(-20,-60);
};

void CWorldCharPlayer::toClientOwnCharData(ClientOwnCharData& d)
{
	toClientCharPlayerData(d);
};

void CWorldCharPlayer::toClientCharPlayerData(ClientCharPlayerData &data)
{
	data.m_id = m_id;
	data.m_name = m_name;
	toClientCharData(&data);
};

void CWorldCharPlayer::fromClientOwnCharData(ClientOwnCharData &data)
{
	m_id = data.m_id;
	m_name = data.m_name;
};

void CWorldCharPlayer::update(uint time)
{
	m_sinceLastMove += time;
    while( m_wayPoints.size() && m_wayPoints.front().m_time <= m_sinceLastMove )
	{
		WayPoint p = m_wayPoints.front();
		m_difference = p.m_pos - getPos();
		if( p.m_time > MAX_SEND_POS_TIME + 1)
			CLog::instance()->log(CLog::msgLvlDebug,_("Sending position interval too big (%d).\n"),p.m_time);
		else if( p.m_time == 0 ) 
		{
			if( m_sinceLastMove < MAX_SEND_POS_TIME )
			{
				p.m_time = m_sinceLastMove;
				moveTo(p);
			}
			CLog::instance()->log(0x08,CLog::msgLvlInfo,"Walk. Pulse %d. Id = %d. Begin move\n",(uint)CWorld::m_pulse,getId());
			m_sinceLastMove = 0; /// Begin move
			m_blocked = false;
		}
		else if( m_sinceLastMove > MAX_LAG_TIME)
		{
			CLog::instance()->log(0x08,CLog::msgLvlInfo,"Walk. Pulse %d. Id = %d. Max lag exceeded.\n",(uint)CWorld::m_pulse,getId());
			//clearWP(); /// Not move, erase all wp until client will start new move
		}
		else if( !m_blocked )
		{
			moveTo(p);
			m_sinceLastMove -= p.m_time;
		}
		m_wayPoints.pop_front();
	};
	if( m_context )
	{
		if( m_moved )
			updatePosition();
		if( m_blocked )
			m_difference = Vector2::ZERO;
		else if( m_difference.squaredLength() > getVelocity() * getVelocity() )
			m_blocked = true;
		m_context->sendCharactersPositions();
	}
	CWorldCharacter::update(time);
};

void CWorldCharPlayer::moveTo(const WayPoint& p)
{
	m_moved = true;
	CLog::instance()->log(0x08,CLog::msgLvlInfo,"Walk. Pulse %ld. Id = %d. Time = %d, StartPos = (%f,%f), DestPos = (%f,%f), ",(uint)CWorld::m_pulse,getId(),p.m_time,getPos().x,getPos().y,p.m_pos.x, p.m_pos.y);
	Vector2 dif = p.m_pos - getPos();
	if( dif.x == 0 && dif.y == 0 )
	{
//		CLog::instance()->log(0x08,CLog::msgLvlInfo,"already there.\n");
		setRotation(p.m_rotation);
		return; /// We're already there
	}
	float len = getVelocity() * ((float)p.m_time / 1000.0f);
	float len2 = len * len;
	if( dif.squaredLength() > getVelocity() * getVelocity() )
	{
//		CLog::instance()->log(0x08,CLog::msgLvlInfo,"distance too big. Velocity = %f\n",getVelocity());
		return; /// Wrong point, might be some error ?
	}
	float height = CWorld::instance()->heightAt(p.m_pos.x,p.m_pos.y) - CWorld::instance()->heightAt(getPos().x,getPos().y);
	if( height > 0 )
	{
		//            /  |  
		//           /   | 
		//          /    | 
		//         /     |        
		//        /      | 
		//       /       |   
		//      /        | height
		//     /.        |  
		//    / |`.      |   
		//   /  |  `.    |    
		//  /   |    `.  |    
		// /a   |len   `.|  
		// ---------------
		// |    |        |
		// |    |        |
		//cur|new pos |p.pos
		// new len = len * cos(a)^2 = len * (len / sqrt(len^2 + height^2))^2
		len = len * len2 / (len2 + height * height);
	}
	float diflen = dif.length();
	if( diflen > len )
	{
		Vector2 newdif = dif * (len / diflen);
		setPos(getPos() + newdif );
	}
	else
		setPos(getPos() + dif);
	setRotation(p.m_rotation);
//	CLog::instance()->log(0x08,CLog::msgLvlInfo," Result = (%f,%f) Len = %f.\n",getPos().x,getPos().y,len);
	m_difference = p.m_pos - getPos();
};

void CWorldCharPlayer::compute()
{
	m_velocity = 10;
	m_fightSpeed = 20; /// One hit in 2 seconds
	CWorldCharacter::compute();
};

void CWorldCharPlayer::setContext(CWSPlayerContext *context)
{
	m_context = context;
	// If this character lost connection, then next time it'll be
	// connected we'll send to client current coordinates, so
	// difference will be zero
	if( !context )
		m_difference = Vector2::ZERO;
};

void CWorldCharPlayer::noticeCharOut(uint id)
{
	m_playersOut.insert(id);
	CLog::instance()->log(0x10,CLog::msgLvlInfo,"(%s): Char out. Id = %d.\n",getName(),id);
	CWorldCharacter::noticeCharOut(id);
}

void CWorldCharPlayer::noticeCharIn(const CharacterPtr& ch)
{
	std::map<uint,CharacterPtr>::iterator it = m_playersIn.find(ch->getId());
	if( it != m_playersIn.end() )
		(*it).second = ch;
	else
		m_playersIn[ch->getId()] = ch;
	/// If char just arrived, we must to send its position
	noticeCharMove(ch->getId(),ch->getPos(),ch->getRotation());
	if( ch->isFighting() )
		noticeCharFight(ch->getId(),ch->getEnemy()->getId(),ch->inCloseCombat());
	CLog::instance()->log(0x10,CLog::msgLvlInfo,"(%s): Char in. Id = %d.\n",getName(),ch->getId());
	CWorldCharacter::noticeCharIn(ch);
}

void CWorldCharPlayer::noticeCharMove(uint id, const Vector2& pos, byte rotation)
{
	std::map<uint,WorldPosition>::iterator it = m_playersPos.find(id);
	if( it != m_playersPos.end() )
	{
		(*it).second.m_pos = pos;
		(*it).second.m_rotation = rotation;
	}
	else
	{
		m_playersPos[id].m_pos = pos;
		m_playersPos[id].m_rotation = rotation;
	}
//	CLog::instance()->log(0x10,CLog::msgLvlTrace,"(%s): Char moved. Id = %d. New pos = (%f,%f)\n",getName(),id,pos.x,pos.y);
	CWorldCharacter::noticeCharMove(id,pos,rotation);
}

void CWorldCharPlayer::onEnterWorld()
{
	setPos(m_startPos);
	CWorldCharacter::onEnterWorld();
};

void CWorldCharPlayer::onLeaveWorld()
{
	CWorldCharacter::onLeaveWorld();
};
