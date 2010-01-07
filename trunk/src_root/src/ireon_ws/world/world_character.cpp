/* Copyright (C) 2005 ireon.org developers council
 * $Id: world_character.cpp 806 2006-11-08 14:02:44Z zak $

 *  See LICENSE for details
 */

/**
 * @file world_character.cpp
 * Character
 */

#include "ireon_ws/stdafx.h"
#include "ireon_ws/world/world_character.h"
#include "ireon_ws/world/ws_world.h"
//#include "ireon_ws/world/world_char_player.h"
//#include "ireon_ws/world/world_char_mob.h"
//#include "common/db/client_char_data.h"


// -----------------------------------------------------------------

CWSEntityBase::~CWSEntityBase() { delete pos_; }

// -----------------------------------------------------------------

CWSCharGroup::CWSCharGroup():
CWSEntityBase(WSWorld::instance()->generateId())
{
}

// -----------------------------------------------------------------

void CWSCharGroup::update(ushort time)
{
	for( std::list<WSEntityPtr>::iterator it = components_.begin(); it != components_.end(); ++it )
		(*it)->update(time);
}

// -----------------------------------------------------------------

void CWSCharGroup::removeComponent(uint id)
{
	for( std::list<WSEntityPtr>::iterator it = components_.begin(); it != components_.end(); ++it )
		if ((*it)->getId() == id)
			components_.erase(it);
		else
			(*it)->removeComponent(id);
}

// -----------------------------------------------------------------

uint CWSCharGroup::getOverallSize()
{
	uint overallSize = 0;
	for( std::list<WSEntityPtr>::iterator it = components_.begin(); it != components_.end(); ++it)
		overallSize += (*it)->getOverallSize();
	return	overallSize + components_.size();
}

// -----------------------------------------------------------------

WSEntityPtr CWSCharGroup::findComponentById(uint id)
{
	for( std::list<WSEntityPtr>::iterator it = components_.begin(); it != components_.end(); ++it)
		if ((*it)->getId() == id)
			return (*it);
	throw EComponentNotFound();
}

// -----------------------------------------------------------------
// -----------------------------------------------------------------
// -----------------------------------------------------------------

#if 0
CWSCharacter::CWSCharacter(uint id):
m_inWorld(false),
m_rotation(0),
m_moved(false),
m_needCloseCombat(false),
m_nextHitPulse(0),
CWSEntityBase(id)
{
};

CWSCharacter::~CWSCharacter()
{
};

void CWSCharacter::toClientCharData(ClientCharData* data)
{
	assert(data);
	if( !data )
		return;
	data->m_velocity = m_velocity;
	data->m_fightSpeed = m_fightSpeed;
	data->m_percentHP = m_stats.getPercentHP();
};

void CWSCharacter::setPos(const Vector2& pos)
{
	if( m_inWorld )
	{
		WorldPage* p1 = WSWorld::instance()->getPageByCoords(m_pos.x,m_pos.y);
		WorldPage* p2 = WSWorld::instance()->getPageByCoords(pos.x,pos.y);
		if( p1 != p2 )
		{
			if( p1 )
				p1->removeCharacter(m_ptr);
			if( p2 )
				p2->insertCharacter(m_ptr);
			else
				CLog::instance()->log(CLog::msgLvlError,__FUNCTION__,_("Try to insert character with id %d (%s) to world, but there is no page for its coordinates (%f,%f).\n"),getId(),getName(),pos.x,pos.y);
		}
	}
	m_moved = true;
	m_pos = pos;
};

bool CWSCharacter::seeCharacter(const CWSCharacter* ch, float& distance)
{
	if( !ch )
		return false;
	if( !distance )
		distance = (ch->getPos() - getPos()).squaredLength();
	if( distance < (300 * 300) )
		return true;
	return false;
};

void CWSCharacter::updatePosition()
{
	updateVisible();
	if( m_moved )
		for( std::vector<CharacterPtr>::iterator it = m_seeMe.begin(); it != m_seeMe.end(); it++ )
			(*it)->noticeCharMove(getId(),getPos(),getRotation());
	m_moved = false;
};

void CWSCharacter::updateVisible()
{
	std::vector<CharacterPtr> iSeeOut(m_iSee.begin(),m_iSee.end());
	std::vector<CharacterPtr> seeMeOut(m_seeMe.begin(),m_seeMe.end());
	m_iSee.clear();
	m_seeMe.clear();

	uint x,z;
	WSWorld::instance()->getPageIndices(getPos().x,getPos().y,x,z);
	for( uint i = ( (x > 1) ? (x - 1) : 0); i <= x + 1; ++i )
		for( uint j = ( (z > 1) ? (z - 1) : 0); j <= z + 1; ++j )
		{
			WorldPage* p = WSWorld::instance()->getPage(i,j);
			if( !p )
				continue;
			std::list<CharacterPtr>::iterator it;
			std::vector<CharacterPtr>::iterator f;
			for( it = p->m_players.begin(); it != p->m_players.end(); ++it )
			{
				if( *it == m_ptr )
					continue;
				float distance = 0;
				if( seeCharacter((*it).get(),distance) )
				{
					m_iSee.push_back(*it);
					for( f = iSeeOut.begin(); f != iSeeOut.end(); ++f)
						if( *f == *it )
							break;
					if( f != iSeeOut.end() )
						iSeeOut.erase(f);
					else
					{
						noticeCharIn(*it);
						/// Add us to m_seeMe vector of another character
						/// to avoid double noticeCharIn
						(*it)->m_seeMe.push_back(m_ptr);
					}
				}
				if( (*it)->seeCharacter(this,distance) )
				{
					m_seeMe.push_back(*it);
					for( f = seeMeOut.begin(); f != seeMeOut.end(); ++f)
						if( *f == *it )
							break;
					if( f != seeMeOut.end() )
						seeMeOut.erase(f);
					else
					{
						(*it)->noticeCharIn(m_ptr);
						/// Add us to m_iSee vector of another character
						/// to avoid double noticeCharIn
						(*it)->m_iSee.push_back(m_ptr);
					}
				}
			};
		};
	std::vector<CharacterPtr>::iterator it;
	for( it = iSeeOut.begin(); it != iSeeOut.end(); ++it )
	{
		noticeCharOut((*it)->getId());
		/// Remove us from m_seeMe vector of another character
		/// to avoid double noticeCharOut
		for( std::vector<CharacterPtr>::iterator it1 = (*it)->m_seeMe.begin(); it1 != (*it)->m_seeMe.end(); ++it1 )
			if( (*it1) == m_ptr )
			{
				(*it)->m_seeMe.erase(it1);
				break;
			};
	}
	for( it = seeMeOut.begin(); it != seeMeOut.end(); ++it )
	{
		(*it)->noticeCharOut(getId());
		for( std::vector<CharacterPtr>::iterator it1 = (*it)->m_iSee.begin(); it1 != (*it)->m_iSee.end(); ++it1 )
			if( (*it1) == m_ptr )
			{
				(*it)->m_iSee.erase(it1);
				break;
			};
	}
};

void CWSCharacter::clearVisible()
{
	std::vector<CharacterPtr>::iterator it;
	for( it = m_iSee.begin(); it != m_iSee.end(); ++it )
	{
		for( std::vector<CharacterPtr>::iterator it1 = (*it)->m_seeMe.begin(); it1 != (*it)->m_seeMe.end(); ++it1 )
			if( (*it1) == m_ptr )
			{
				(*it)->m_seeMe.erase(it1);
				break;
			};
	}
	for(it = m_seeMe.begin(); it != m_seeMe.end(); ++it )
	{
		(*it)->noticeCharOut(getId());
		for( std::vector<CharacterPtr>::iterator it1 = (*it)->m_iSee.begin(); it1 != (*it)->m_iSee.end(); ++it1 )
			if( (*it1) == m_ptr )
			{
				(*it)->m_iSee.erase(it1);
				break;
			};
	}
	m_iSee.clear();
	m_seeMe.clear();
};

void CWSCharacter::onEnterWorld()
{
	compute();
	updatePosition();
	WorldPage* p1 = WSWorld::instance()->getPageByCoords(getPos().x,getPos().y);
	if( p1 )
	{
		p1->removeCharacter(m_ptr);
		p1->insertCharacter(m_ptr);
	}
};

void CWSCharacter::onLeaveWorld()
{
	stopFight();
	clearVisible();
	WorldPage* p1 = WSWorld::instance()->getPageByCoords(getPos().x,getPos().y);
	if( p1 )
		p1->removeCharacter(m_ptr);
};

void CWSCharacter::update(uint time)
{
	if( needCloseCombat() )
	{
		if( !m_enemy )
			m_needCloseCombat = false;
		else if( inCloseBattleRange(m_enemy->getPos()) )
		{
			startFight(m_enemy,true);
			m_needCloseCombat = false;
		}
	}
//	if( m_nextHitPulse <= WSWorld::m_pulse && isFighting() )
	if( isFighting() )
	{
		if( inCloseCombat() )
		{
			hit(m_enemy);
//			m_nextHitPulse = WSWorld::m_pulse + m_fightSpeed;
		}
	}
};

bool CWSCharacter::startFight(const CharacterPtr &ch, bool closeCombat)
{
	if( !ch )
		return false;
	if( !m_ptr )
		return false;
	if( ch == m_ptr )
		return false;
	std::list<CharacterPtr>::iterator it;
	if( m_enemy )
		for( it = m_enemy->m_fightWithMe.begin(); it != m_enemy->m_fightWithMe.end(); ++it )
			if( *it == m_ptr )
			{
				m_enemy->m_fightWithMe.erase(it);
				break;
			};
	if( (!ch->isFighting() || !ch->needCloseCombat()) &&
		ch->attackIsClose())
		closeCombat = true;
	if( attackIsClose() )
		closeCombat = true;
	m_enemy = ch;
	for( it = ch->m_fightWithMe.begin(); it != ch->m_fightWithMe.end(); ++it )
		if( *it == m_ptr )
		{
			ch->m_fightWithMe.erase(it);
			break;
		};
	ch->m_fightWithMe.push_back(m_ptr);
	m_needCloseCombat = false;
	m_closeCombat = false;
	if( closeCombat )
	{
		if( inCloseBattleRange(ch->getPos()) )
			m_closeCombat = true;
		else
			m_needCloseCombat = true;
	};
	if( !ch->isFighting() || (!ch->inCloseCombat() && m_closeCombat) )
		ch->startFight(m_ptr,true);
	for( std::vector<CharacterPtr>::iterator vis = m_seeMe.begin(); vis != m_seeMe.end(); ++vis )
		(*vis)->noticeCharFight(getId(),m_enemy->getId(),inCloseCombat());
	noticeCharFight(getId(),m_enemy->getId(),inCloseCombat());
	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,"Fight: %d with %d. Close = %d\n",getId(),m_enemy->getId(),inCloseCombat());
	return true;
};

void CWSCharacter::stopFight()
{
	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,"Stop fight: %d\n",getId());
	if( !isFighting() )
		return;
	std::list<CharacterPtr>::iterator it;
	for( it = m_enemy->m_fightWithMe.begin(); it != m_enemy->m_fightWithMe.end(); ++it )
		if( *it == m_ptr )
		{
			m_enemy->m_fightWithMe.erase(it);
			break;
		};
	m_enemy.reset();
	noticeIStopFight();
	m_closeCombat = false;
	m_needCloseCombat = false;

	std::list<CharacterPtr>::iterator enemy;
	for( enemy = m_fightWithMe.begin(); enemy != m_fightWithMe.end(); ++enemy )
	{
		(*enemy)->m_enemy.reset();
		(*enemy)->noticeIStopFight();
		(*enemy)->updateFightPos();
	}
	m_fightWithMe.clear();
};

void CWSCharacter::updateFightPos()
{
	if( isFighting() && inCloseCombat() )
		return;
	m_needCloseCombat = false;
	m_closeCombat = false;
	if( m_fightWithMe.empty() )
		return;
	bool foundClose = false;
	std::list<CharacterPtr>::iterator it;
	for( it = m_fightWithMe.begin(); it != m_fightWithMe.end(); ++it )
		if( (*it)->inCloseCombat() )
		{
			foundClose = true;
			break;
		}
	if( foundClose )
	{
		startFight(*it,true);
		return;
	} else if( isFighting() )
		return;
	startFight(*m_fightWithMe.begin(),attackIsClose());
};

bool CWSCharacter::attackIsClose()
{
	return true;
};

void CWSCharacter::noticeIStopFight()
{
	for( std::vector<CharacterPtr>::iterator vis = m_seeMe.begin(); vis != m_seeMe.end(); ++vis )
		(*vis)->noticeCharStopFight(getId());
	noticeCharStopFight(getId());
};

bool CWSCharacter::canWalk(const Vector2 &pos)
{
	if( inCloseCombat() && isFighting() )
		if( (pos - getEnemy()->getPos()).squaredLength() > CLOSE_COMBAT_DISTANCE * CLOSE_COMBAT_DISTANCE )
			return false;
	Vector3 normal3 = WSWorld::instance()->normalAt(pos.x,pos.y);
	Vector2 move = pos - getPos();
	Vector2 normal2(normal3.x,normal3.z);
	if( normal3.y < IMPASSABLE && normal2.dotProduct(move) < 0 )
		return false;
	return true;
};

bool CWSCharacter::changeHP(int hp, const CharacterPtr& who)
{
	if( hp > 0 )
		m_stats.setHP(m_stats.getHP() + hp);
	else
	{
		if( m_stats.getHP() > -hp )
			m_stats.setHP(m_stats.getHP() + hp);
		else
		{
			die(who);
			return false;
		}
	}
	return true;
};

bool CWSCharacter::hit(const CharacterPtr& ch)
{
	int hit = rand() % 20 + 5;
// 	if( mob() )
// 		hit /= 3;
	if (ch->changeHP(-hit,m_ptr) )
	{
		for( std::vector<CharacterPtr>::iterator it = m_seeMe.begin(); it != m_seeMe.end(); ++it )
			(*it)->noticeCharHit(getId(),ch->getId(),ch->getStats().getPercentHP(),0);
		noticeCharHit(getId(),ch->getId(),ch->getStats().getPercentHP(),0);
		//CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,"%s hitted by %s for %d.\n",ch->getName(),getName(),hit);
	}
	return true;
};

void CWSCharacter::die(const CharacterPtr& killer)
{
	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,"%s is DEAD (killed by %s).\n",getName(),killer ? killer->getName() : "nobody");
	stopFight();
	m_stats.setHP(m_stats.getMaxHP());
// 	setMana(getMaxMana());
	uint killerId = killer ? killer->getId() : 0;
	for( std::vector<CharacterPtr>::iterator it = m_seeMe.begin(); it != m_seeMe.end(); ++it )
		(*it)->noticeCharDied(getId(),killerId);
	noticeCharDied(getId(),killerId);
};

#endif
