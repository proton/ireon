/* Copyright (C) 2005 ireon.org developers council
 * $Id: world_char_player.cpp 806 2006-11-08 14:02:44Z zak $

 *  See LICENSE for details
 */

/**
 * @file world_char_player.cpp
 * Player's character
 */

#include "ireon_ws/stdafx.h"
#include "ireon_ws/world/world_char_player.h"
#include "common/db/client_char_data.h"
#include "ireon_ws/db/world_db.h"
#include "ireon_ws/world/ws_world.h"
//#include "ireon_ws/net/ws_player_context.h"
#include "common/data.h"


CWorldCharPlayer::CWorldCharPlayer(uint id):
m_sinceLastMove(0),
m_context(NULL),
m_difference(Vector2::ZERO),
m_blocked(false),
CWSCharacter(id)
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
		data.wrt(getStats().getMaxHP());
		/*
		data << 100; //m_baseMaxMana;
		data << 10; //m_baseStr;
		data << 10; //m_baseSta;
		data << 10; //m_baseInt;
		data << 10; //m_baseWis;
		data << 10; //m_baseCon;
		data << 10; //m_baseLuck;
		*/
	} else
	{
		data >> m_id;
		data >> m_accountId;
		data >> m_name;
		data >> m_startPos;
		uint maxHP;
		data >> maxHP;
		getStats().setHP(maxHP);
		getStats().setMaxHP(maxHP);
		/*
		data >> m_baseMaxMana;
		m_curMana = m_baseMaxMana;
		data >> m_baseStr;
		data >> m_baseSta;
		data >> m_baseInt;
		data >> m_baseWis;
		data >> m_baseCon;
		data >> m_baseLuck;
		*/
	}
};

void CWorldCharPlayer::init()
{
	m_startPos = Vector2(-20 ,-60 );
	getStats().reset_stats();
};

void CWorldCharPlayer::toClientOwnCharData(ClientOwnCharData& d)
{
	toClientCharPlayerData(d);
	d.m_HP = getStats().getHP();
	d.m_maxHP = getStats().getMaxHP();
	d.m_mana = 100; //m_curMana;
	d.m_maxMana = 100; //m_maxMana;
	d.m_str = 10; //m_str;
	d.m_sta = 10; //m_sta;
	d.m_int = 10; //m_int;
	d.m_wis = 10; //m_wis;
	d.m_con = 10; //m_con;
	d.m_luck = 10; //m_luck;
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
// 	if( WSWorld::m_playerMovePulse )
// 	{
		while( m_wayPoints.size() && m_wayPoints.front().m_time <= m_sinceLastMove )
		{
			WayPoint p = m_wayPoints.front();
			m_difference = p.m_pos - getPos();
			if( p.m_time > MAX_SEND_POS_TIME + 1)
				CLog::instance()->log(CLog::msgLvlDebug,__FUNCTION__,_("Sending position interval too big (%d).\n"),p.m_time);
			else if( p.m_time == 0 ) 
			{
				if( m_sinceLastMove < MAX_SEND_POS_TIME )
				{
					p.m_time = m_sinceLastMove;
					moveTo(p);
				}
				CLog::instance()->log(CLog::msgLvlDebug,__FUNCTION__, "Walk. Id = %d. Begin move\n", getId());
				m_sinceLastMove = 0; /// Begin move
				m_blocked = false;
			}
			else if( m_sinceLastMove > MAX_LAG_TIME)
			{
				CLog::instance()->log(CLog::msgLvlDebug,__FUNCTION__,"Walk. Id = %d. Max lag exceeded.\n", getId());
				//clearWP(); /// Not move, erase all wp until client will start new move
			}
			else if( !m_blocked )
			{
				moveTo(p);
				m_sinceLastMove -= p.m_time;
			}
			m_wayPoints.pop_front();
		};
		if( m_moved )
			updatePosition();
		if( m_blocked )
			m_difference = Vector2::ZERO;
		else if( m_difference.squaredLength() > getVelocity() * getVelocity() )
			m_blocked = true;
// 	}
	// FIXME -- should not send data on each update
// 	if( m_context )
// 		m_context->sendCharactersPositions();
	CWSCharacter::update(time);
};

void CWorldCharPlayer::moveTo(const WayPoint& p)
{
	m_moved = true;
	CLog::instance()->log(CLog::msgLvlDebug,__FUNCTION__,"Walk. Id = %d. Time = %d, StartPos = (%f,%f), DestPos = (%f,%f), ", getId(),p.m_time,getPos().x,getPos().y,p.m_pos.x, p.m_pos.y);
	Vector2 dif = p.m_pos - getPos();
	if( dif.x == 0 && dif.y == 0 )
	{
//		CLog::instance()->log(0x08,CLog::msgLvlInfo,__FUNCTION__,"already there.\n");
		setRotation(p.m_rotation);
		return; /// We're already there
	}
	float len = getVelocity() * ((float)p.m_time / 1000.0f);
	float len2 = len * len;
	if( dif.squaredLength() > getVelocity() * getVelocity() )
	{
//		CLog::instance()->log(0x08,CLog::msgLvlInfo,__FUNCTION__,"distance too big. Velocity = %f\n",getVelocity());
		return; /// Wrong point, might be some error ?
	}
	float height = WSWorld::instance()->heightAt(p.m_pos.x,p.m_pos.y) - WSWorld::instance()->heightAt(getPos().x,getPos().y);
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
	Vector2 newPos;
	if( diflen > len )
	{
		Vector2 newdif = dif * (len / diflen);
		newPos = getPos() + newdif;
	}
	else
		newPos = getPos() + dif;
	if( canWalk(newPos) )
		setPos(newPos);
	setRotation(p.m_rotation);
//	CLog::instance()->log(0x08,CLog::msgLvlInfo,__FUNCTION__," Result = (%f,%f) Len = %f.\n",getPos().x,getPos().y,len);
	m_difference = p.m_pos - getPos();
};

void CWorldCharPlayer::compute()
{
	m_velocity = 10;
	m_fightSpeed = 20; /// One hit in 2 seconds
/*
	m_maxHP = m_baseMaxHP;
	m_maxMana = m_baseMaxMana;
	m_str = m_baseStr;
	m_sta = m_baseSta;
	m_int = m_baseInt;
	m_wis = m_baseWis;
	m_con = m_baseCon;
	m_luck = m_baseLuck;
*/
	CWSCharacter::compute();
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
	CLog::instance()->log(0x10,CLog::msgLvlInfo,__FUNCTION__,"(%s): Char out. Id = %d.\n",getName(),id);
	CWSCharacter::noticeCharOut(id);
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
	CLog::instance()->log(0x10,CLog::msgLvlInfo,__FUNCTION__,"(%s): Char in. Id = %d.\n",getName(),ch->getId());
	CWSCharacter::noticeCharIn(ch);
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
//	CLog::instance()->log(0x10,CLog::msgLvlTrace,__FUNCTION__,"(%s): Char moved. Id = %d. New pos = (%f,%f)\n",getName(),id,pos.x,pos.y);
	CWSCharacter::noticeCharMove(id,pos,rotation);
}

void CWorldCharPlayer::onEnterWorld()
{
	float dispX = 15;
	float dispY = 15;
	float rndPosX =(float(rand())/RAND_MAX) * dispX;
	rndPosX -= dispX/2;
	float rndPosY = (float(rand())/RAND_MAX) * dispY;
	rndPosY -= dispY/2;
	Ogre::Vector2 pos = m_startPos;
	pos.x += rndPosX;
	pos.y += rndPosY;
		
	setPos(pos);
	CWSCharacter::onEnterWorld();
};

void CWorldCharPlayer::onLeaveWorld()
{
	CWSCharacter::onLeaveWorld();
};

void CWorldCharPlayer::noticeCharHit(uint ch, uint enemy, byte percentHP, byte flags)
{
// 	if( m_context )
// 		m_context->sendHit(ch,enemy,percentHP, flags);
};

void CWorldCharPlayer::noticeCharDied(uint id, uint enemyId)
{
// 	if( m_context )
// 		m_context->sendDeath(id,enemyId);
};

void CWorldCharPlayer::die(const CharacterPtr& killer)
{
	CWSCharacter::die(killer);
// 	if( m_context )
// 		m_context->sendCharactersPositions();
	WSWorld::instance()->removeEntity(getId());
};