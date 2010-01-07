/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: character.cpp 522 2006-03-06 16:49:30Z zak $

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
 * @file character.cpp
 * Character
 */
#include "stdafx.h"
#include "world/character.h"
#include "world/world.h"
#include "interface/interface.h"
#include "world/char_player.h"
#include "world/OgreOdeMeshInformer.h"

CCharacter::CCharacter(const String &name, const String& mesh, ClientCharData* data):
CObject(name),
m_data(data),
m_walk(false),
m_walkAnim(NULL),
m_jumpAnim(NULL),
m_jump(false),
m_fightId(0),
m_timeToNextHit(0),
m_dead(false)
{
	m_type = CObject::NONE;
	m_node = CWorld::instance()->sceneManager()->getRootSceneNode()->createChildSceneNode();
	Quaternion q;
	q.FromAngleAxis(Radian(Degree(90)),Vector3::UNIT_X);
	m_node->setOrientation(q);
	m_node->setPosition(Vector3::ZERO);
	String entityName;
	SceneManager::EntityIterator iter = CWorld::instance()->sceneManager()->getEntityIterator();
	bool found = true;
	while( found )
	{
		entityName = name + StringConverter::toString(rand());
		found = false;
		while( iter.hasMoreElements() )
		{
			if( iter.peekNextKey() == entityName )
			{
				found = true;
				break;
			}
			iter.moveNext();
		}
	}
	m_entity = CWorld::instance()->sceneManager()->createEntity(entityName,mesh);
	m_entity->setCastShadows(true);

	m_entityNode = m_node->createChildSceneNode();
	m_entityNode->attachObject(m_entity);
	Vector3 size = OgreOde::MeshInformer::getSize(m_entity->getMesh().get());
	Vector3 min = Vector3::ZERO - size/2;
	min.y = 0;
	Vector3 max = Vector3::ZERO + size/2;
	max.y = size.y;
	m_entity->getMesh()->_setBounds(AxisAlignedBox(min,max));
	Real scale = CHAR_SIZE / size.y;
	m_entityNode->setScale(Vector3(scale,scale,scale)); /// All characters 2 units in height

	AnimationStateSet* animSet = m_entity->getAllAnimationStates();

	if( animSet->find("Walk") != animSet->end() )
	{
		m_walkAnim = m_entity->getAnimationState("Walk");
		m_walkAnim->setEnabled(false);
		m_animStandPos[0] = m_walkAnim->getLength() / 4;
		m_animStandPos[1] = m_walkAnim->getLength() / 4 * 3;
		m_walkAnim->setTimePosition(m_animStandPos[0]);
		/// 1 cycle of animation will be in CHAR_SIZE length
		//m_stepLsen = CHAR_SIZE / m_walkAnim->getLength();
	} else
		CLog::instance()->log(CLog::msgLvlCritical,"Character %s hasn't walk animation.\n",getName());

	if( animSet->find("JumpNoHeight") != animSet->end() )
	{
		m_jumpAnim = m_entity->getAnimationState("JumpNoHeight");
		m_jumpAnim->setEnabled(false);
		m_jumpAnim->setTimePosition(0);
		m_jumpAnim->setLoop(false);
	} else
		CLog::instance()->log(CLog::msgLvlCritical,"Character %s hasn't jump animation.\n",getName());


	if( animSet->find("Attack1") != animSet->end() )
	{
		m_hitAnim = m_entity->getAnimationState("Attack1");
		m_hitAnim->setEnabled(false);
		m_hitAnim->setTimePosition(0);
		m_hitAnim->setLoop(false);
	} else
		CLog::instance()->log(CLog::msgLvlCritical,"Character %s hasn't hit animation.\n",getName());

	if( animSet->find("Die1") != animSet->end() )
	{
		m_dieAnim = m_entity->getAnimationState("Die1");
		m_dieAnim->setEnabled(false);
		m_dieAnim->setTimePosition(0);
		m_dieAnim->setLoop(false);
		m_dieSpeed = m_dieAnim->getLength();
	} else 	if( animSet->find("Death1") != animSet->end() )
	{
		m_dieAnim = m_entity->getAnimationState("Death1");
		m_dieAnim->setEnabled(false);
		m_dieAnim->setTimePosition(0);
		m_dieAnim->setLoop(false);
		m_dieSpeed = m_dieAnim->getLength();
	} else 	if( animSet->find("Collapse") != animSet->end() )
	{
		m_dieAnim = m_entity->getAnimationState("Collapse");
		m_dieAnim->setEnabled(false);
		m_dieAnim->setTimePosition(0);
		m_dieAnim->setLoop(false);
		m_dieSpeed = m_dieAnim->getLength();
	} else 
		CLog::instance()->log(CLog::msgLvlCritical,"Character %s hasn't die animation.\n",getName());

	setWalkSpeed(data->m_velocity);
	m_timeToNextHit = 0;
	m_hitSpeed = m_hitAnim->getLength(); // Hit in 1 second
};

CCharacter::~CCharacter()
{
	if( m_info )
		CInterface::instance()->destroyWindow(m_info);
	if( m_data )
		delete m_data;
};

uint CCharacter::getId()
{
	return m_data->m_id;
};

byte CCharacter::getPercentHP()
{
	return m_data->m_percentHP;
};

void CCharacter::setPercentHP(byte percent)
{
	m_data->m_percentHP = percent;
};

byte CCharacter::getFightSpeed()
{
	return m_data->m_fightSpeed;
};

Real CCharacter::getAnimStandPos()
{
	Real pos = m_walkAnim->getTimePosition();
	if( pos <= m_animStandPos[0] )
		return m_animStandPos[0];
	if (pos <= m_animStandPos[1] )
		return m_animStandPos[1];
	return 100;
};

void CCharacter::walk(Real time, Vector2 *direction)
{
	Vector3 pos = getPosition();
	if( direction )
		setRotation(Math::ATan2(-direction->x,-direction->y));
	m_walk = true;
	if( !m_jump )
	{
		Vector3 to = Vector3(pos.x - Math::Sin(getRotation(),true) * time * getWalkSpeed(),0,pos.z - Math::Cos(getRotation(),true) * time * getWalkSpeed());
		to.y = CWorld::instance()->heightAt(to);
		Vector2 norm;
		if( canWalk(to,norm) )
			setPosition(to);
		else
		{
			Vector2 to2(to.x,to.z);
			Vector2 pos2(pos.x,pos.z);
			Vector2 diff = to2-pos2;
			Vector2 position2 = pos2 + diff - norm * diff.dotProduct(norm);
			to.x = position2.x;
			to.z = position2.y;
			to.y = CWorld::instance()->heightAt(to);
			if( canWalk(to,norm) )
				setPosition(to);
		}
	}
};

void CCharacter::lookAt(const Vector2& to)
{
	Vector2 dir = to - getPosition2();
	setRotation(Math::ATan2(-dir.x,-dir.y));
};

void CCharacter::setWalkSpeed(Real speed)
{
	m_walkSpeed = speed;
	m_stepLen = speed / 2 / m_walkAnim->getLength();
};

void CCharacter::walkAnimStep(const Real& time, bool toStand)
{
	Real dif = getAnimStandPos() -  m_walkAnim->getTimePosition();
	Real add = time * m_walkSpeed / m_stepLen;
	if( (add > dif) && toStand )
	{
		m_walkAnim->setTimePosition(getAnimStandPos());
		m_walkAnim->setEnabled(false);
	}
	else
	{
		m_walkAnim->setEnabled(true);
		m_walkAnim->addTime(add);
	}
};

void CCharacter::update(Real time)
{
	if( m_dead )
	{
		Real add = time * m_dieSpeed;
		if( m_dieAnim->getLength() - m_dieAnim->getTimePosition() > add )
			m_dieAnim->addTime(add);
		else
			m_dieAnim->setTimePosition(m_dieAnim->getLength());
		if( m_dieAnim->getTimePosition() >= m_dieAnim->getLength() )
		{
			m_timeToDisappear -= time;
			if( m_timeToDisappear <= 0 )
				CWorld::instance()->removeDead(getId());
		}
		return;
	}
	if( isWalking() && !m_jump )
	{
		bool noWalkAnim = false;
		if( !m_walk )
		{
			/// Walking by waypoints
			Vector2 pos2 = getPosition2();
			Vector2 toWayPoint = m_wayPoints[0].m_pos - pos2;
			if( toWayPoint.squaredLength() < getWalkSpeed() * getWalkSpeed() * time * time )
			{
				setPosition(m_wayPoints[0].m_pos);
				if( m_wayPoints[0].m_rotation != Radian(0) && m_wayPoints.size() == 1)
				{
					Radian diff = m_wayPoints[0].m_rotation - getRotation();
					diff = diff - 2 * Radian(Math::PI) * static_cast<int>((diff / Math::PI / 2).valueRadians());
					if( diff > Radian(Math::PI) )
						diff -= 2 * Radian(Math::PI);
					else if( diff < Radian(-Math::PI) )
						diff += 2 * Radian(Math::PI);
					if( diff > Radian(0) )
					{
						if( diff > Radian(Math::PI) * time )
							setRotation(getRotation() + Radian(Math::PI) * time);
						else
						{
							setRotation(m_wayPoints[0].m_rotation);
							m_wayPoints.pop_front();
						}
					} else
					{
						if( diff < -Radian(Math::PI) * time )
							setRotation(getRotation() - Radian(Math::PI) * time);
						else
						{
							setRotation(m_wayPoints[0].m_rotation);
							m_wayPoints.pop_front();
						}
					}
				} else
					m_wayPoints.pop_front();
				noWalkAnim = true;
			} else
				walk(time, &toWayPoint);
		};
		if( noWalkAnim )
			walkAnimStep(time,true);
		else
			walkAnimStep(time,false);
	}
	else
	{
		if( m_walkAnim->getTimePosition() != getAnimStandPos() )
			walkAnimStep(time,true);
	}
	if( m_jump )
	{
		m_jumpAnim->addTime(time * m_jumpSpeed);
		if( m_jumpAnim->getTimePosition() == m_jumpAnim->getLength() )
		{
			m_jump = false;
			m_jumpAnim->setEnabled(false);
		}
	}
	m_walk = false;
	if( m_fightId )
	{
		CharacterPtr ch = CWorld::instance()->findCharacter(m_fightId);
		if( ch )
		{
			m_fightId = 0;
			m_enemy = ch;
		}
	}
	if( m_enemy )
	{
		if( m_closeCombat )
		{
			lookAt(m_enemy->getPosition2());
			m_timeToNextHit -= time;
			if( m_timeToNextHit <= 0 )
			{
				m_hitAnim->setEnabled(true);
				m_timeToNextHit = (Real)getFightSpeed() / 10;
			}
		}
	}
	if( m_hitAnim->getEnabled() )
	{
		Real dif = m_hitAnim->getLength() - m_hitAnim->getTimePosition();
		Real add = m_hitSpeed * time;
		if( add > dif )
		{
			m_hitAnim->setTimePosition(0);
			m_hitAnim->setEnabled(false);
		} else
			m_hitAnim->addTime(add);
	};
	CharUpdateEventArg arg;
	arg.m_ch = CWorld::instance()->findCharacter(getId());
	arg.m_time = time;
	if( !arg.m_ch && CWorld::instance()->getOwnChar().get() == this )
		arg.m_ch = CWorld::instance()->getOwnChar();
	if( arg.m_ch )
		CEventManager::instance()->execute(CEventManager::ON_CHAR_UPDATE,&arg);
};

void CCharacter::jump()
{
/*	uint velocity = 40;
	Real time = -2.1 * velocity / OgreOde::World::getSingleton().getGravity().y;
	m_jumpSpeed = m_jumpAnim->getLength() / time;
	m_jumpAnim->setEnabled(true);
	m_jumpAnim->setTimePosition(0);
	m_jump = true;*/
};

void CCharacter::setRotation(Radian rot)
{
	Quaternion q;
	q.FromAngleAxis(rot,Vector3::UNIT_Y);
	setOrientation(q);
	m_rotation = rot;
};

bool CCharacter::visible()
{
	return CWorld::instance()->pointIsVisible(getPosition() + Vector3(0,CHAR_SIZE,0));
};

void CCharacter::startFight(uint enemy, bool closeCombat)
{
	CLog::instance()->log(CLog::msgLvlInfo,"(%s): start fight with %d in %s.\n",getName(),enemy,closeCombat ? "close combat" : "ranged combat");
	CharacterPtr ch = CWorld::instance()->findCharacter(enemy);
	m_fightId = 0;
	if( isDead() || (ch && ch->isDead()) )
		return;
	if( ch )
		m_enemy = ch;
	else
		m_fightId = enemy;
	m_closeCombat = closeCombat;
};

void CCharacter::stopFight()
{
	CLog::instance()->log(CLog::msgLvlInfo,"(%s): stop fight.\n",getName());
	m_fightId = 0;
	m_closeCombat = false;
	m_enemy.reset();
};

void CCharacter::die()
{
	stopFight();
	m_walkAnim->setTimePosition(getAnimStandPos());
	m_walkAnim->setEnabled(false);
	m_jump = false;
	clearWayPoints();
	m_hitAnim->setEnabled(false);
	m_dieAnim->setEnabled(true);
	m_dead = true;
	CharacterPtr me = CWorld::instance()->findCharacter(getId());
	if( !me )
		return;
	CWorld::instance()->addDead(me);
	CWorld::instance()->removeCharacter(getId());
	m_timeToDisappear = 10;
	CharacterEventArg arg;
	arg.m_ch = me;
	CEventManager::instance()->execute(CEventManager::ON_CHAR_DIE,&arg);
	if( CWorld::instance()->getOwnChar()->getId() == getId() )
	{
		CClientApp::instance()->setState(CClientApp::AS_DEAD);
		CWorld::instance()->getOwnChar()->setPercentHP(0);
	}
};

Vector2 getVectorForPair(const Vector2& pos1, const Vector2& pos2)
{
	const Real sqrt2 = 0.7071067811865475;
	uint x1, x2, z1, z2;
	bool up1, up2;
	PagingLandScapeData2DManager::getSingleton().getTriInfo(pos1.x,pos1.y,x1,z1,up1);
	PagingLandScapeData2DManager::getSingleton().getTriInfo(pos2.x,pos2.y,x2,z2,up2);
	if( up1 )
	{
		if( up2 )
		{
			if( z1 < z2 )
			{
				if( x1 <= x2 )
					return Vector2(-sqrt2,-sqrt2);
				else
					return Vector2(1,0);
			}
			else if( z1 == z2 )
			{
				if( x1 > x2 )
					return Vector2(1,0);
				else
					return Vector2(-sqrt2,-sqrt2);
			} else
				return Vector2(0,1);
		}
		else
		{ //up2 == false
			if( x1 < x2 )
			{
				if( z1 < z2 )
					return Vector2(-sqrt2,-sqrt2);
				else if( z1 == z2 )
					return Vector2(-1,0);
				else
					return Vector2(-1,0);
			} else if( x1 == x2 )
			{
				if( z1 < z2 )
					return Vector2(0,-1);
				else
					return Vector2(sqrt2,sqrt2);
			} else // x1 > x2
			{ 
				if( z1 < z2 )
					return Vector2(0,-1);
				else
					return Vector2(sqrt2,sqrt2);
			}
		}
	} else
	{ //up1 == false
		if( !up2 )
		{
			if( x1 < x2 )
			{
				if( z1 <= z2 )
					return Vector2(-1,0);
				else 
					return Vector2(0,1);
			} else if( x1 == x2 )
			{
				if( z1 > z2 )
					return Vector2(sqrt2,sqrt2);
				else
					return Vector2(0,-1);
			} else // x1 > x2
			{
				if( z1 < z2 )
					return Vector2(1,0);
				else
					return Vector2(sqrt2,sqrt2);
			}
		} else
		{
			///up1 == false up2 == true
			if( x1 < x2 )
			{
				if( z1 > z2 )
					return Vector2(0,1);
				else
					return Vector2(-sqrt2,-sqrt2);
			}
			else if( x1 == x2 )
			{
				if( z1 <= z2 )
					return Vector2(-sqrt2,-sqrt2);
				else
					return Vector2(0,1);
			} else
			{
				/// x1 > x2
				if( z1 > z2 )
					return Vector2(sqrt2,sqrt2);
				else
					return Vector2(1,0);
			}
		}
	}
};

bool CCharacter::canWalk(Vector3 to, Vector2& norm)
{
	Vector2 to2 = Vector2(to.x,to.z);
	Vector2 pos2 = getPosition2();
	Vector3 normal = PagingLandScapeData2DManager::getSingleton().getNormal(to2.x,to2.y);
	if( normal.y < 0.1 )
	{
		Vector2 diff2 = to2 - pos2;
		norm = getVectorForPair(pos2,to2);
		if( Vector2(normal.x,normal.z).dotProduct(to2 - pos2) < 0 )
		{
			return false;
			/*Vector2 position2 = prevPos2 + diff2 - norm2 * diff2.dotProduct(norm2);
			normal = PagingLandScapeData2DManager::getSingleton().getNormal(position2.x,position2.y);
			if( normal.y < 0.1 )
				setPosition(m_prevPosition);
			else
				setPosition(position2);*/
		}
	}
	CWorld::CharMapIterator it = CWorld::instance()->getFirstChar();
	for(; !CWorld::instance()->lastChar(it); it = CWorld::instance()->getNextChar() )
		if( (*it).second.get() != this )
			if( ((*it).second->getPosition2() - to2).squaredLength() < CHAR_SIZE * CHAR_SIZE / 16 )
			{
				norm = pos2 - (*it).second->getPosition2();
				norm.normalise();
				return false;
			};
	return true;
};