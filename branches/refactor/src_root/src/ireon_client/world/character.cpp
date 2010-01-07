/* Copyright (C) 2005 ireon.org developers council
 * $Id: character.cpp 433 2005-12-20 20:19:15Z zak $

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
#include "OgreOdeMeshInformer.h"

CCharacter::CCharacter(const String &name, const String& mesh):
CObject(name),
m_amj(NULL),
m_transform(NULL),
m_walk(false),
m_walkAnim(NULL),
m_jumpAnim(NULL),
m_jump(false),
m_fightId(0),
m_timeToNextHit(0)
{
	m_type = CObject::NONE;
	setMass(1);
	m_body = new OgreOde::Body(name + "_Body");
	m_body->setMass(OgreOde::CapsuleMass(getMass(),CHAR_SIZE/6,Vector3(0,1,0),2*CHAR_SIZE/3));
	m_body->setAutoSleep(false);
	m_geometry = new OgreOde::CapsuleGeometry(CHAR_SIZE/6,2*CHAR_SIZE/3);
	m_transform = new OgreOde::TransformGeometry(OgreOde::World::getSingleton().getDefaultSpace());
	m_transform->setEncapsulatedGeometry(m_geometry);
	m_transform->setBody(m_body);
	m_transform->setObject(this);
	Quaternion q;
	q.FromAngleAxis(Radian(Degree(90)),Vector3::UNIT_X);
	m_geometry->setOrientation(q);
	m_geometry->setPosition(Vector3(0,CHAR_SIZE/2,0));
	/// Creating joint to avoid character's rotation
	m_amj = new OgreOde::AngularMotorJoint();
	m_amj->attach(m_body);
	m_amj->setAxisCount(3);
	m_amj->setMode(OgreOde::AngularMotorJoint::Mode_EulerAngularMotor);
	m_amj->setAxis(0,OgreOde::AngularMotorJoint::RelativeOrientation_GlobalFrame,Vector3(1,0,0));
	m_amj->setAxis(1,OgreOde::AngularMotorJoint::RelativeOrientation_GlobalFrame,Vector3(0,0,1));
	m_amj->setAxis(2,OgreOde::AngularMotorJoint::RelativeOrientation_GlobalFrame,Vector3(0,1,0));
	m_amj->setAngle(0,0);
	m_amj->setAngle(1,0);
	m_amj->setAngle(2,0);
	m_amj->setParameter(OgreOde::Joint::Parameter_LowStop,-0,1);
	m_amj->setParameter(OgreOde::Joint::Parameter_LowStop,-0,2);
	m_amj->setParameter(OgreOde::Joint::Parameter_LowStop,-dInfinity,3);
	m_amj->setParameter(OgreOde::Joint::Parameter_HighStop,0,1);
	m_amj->setParameter(OgreOde::Joint::Parameter_HighStop,0,2);
	m_amj->setParameter(OgreOde::Joint::Parameter_HighStop,dInfinity,3);

	m_entity = CWorld::instance()->sceneManager()->createEntity(name,mesh);
	m_entity->setCastShadows(true);
	
	m_node = CWorld::instance()->sceneManager()->getRootSceneNode()->createChildSceneNode();
	m_node->attachObject(m_body);
	m_entityNode = m_node->createChildSceneNode();
	m_entityNode->attachObject(m_entity);
	Real size = OgreOde::MeshInformer::getSize(m_entity->getMesh().get()).y;
	Real scale = CHAR_SIZE / size;
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
};

CCharacter::~CCharacter()
{
	if( m_amj )
	{
		m_amj->detach();
		delete m_amj;
	}
	if( m_transform )
		delete m_transform;

	if( m_info )
		CInterface::instance()->destroyWindow(m_info);
};

void CCharacter::setCharData(ClientCharData* data)
{
	assert(data);
	setWalkSpeed(data->m_velocity);
	setFightSpeed(data->m_fightSpeed);
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

void CCharacter::walk(Vector2 *direction)
{
	if( direction )
		setRotation(Math::ATan2(-direction->x,-direction->y));
	Vector3 vel = m_body->getLinearVelocity();
	vel.x = 0;
	vel.z = -getWalkSpeed();
	/// To avoid jumping when walk up the hill
	if( !m_jump && (vel.y > 0) )
		vel.y = 0;
	Matrix3 rot;
	getOrientation().ToRotationMatrix(rot);
	vel = rot * vel;
	m_body->setLinearVelocity(vel);
	m_walk = true;
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

void CCharacter::setFightSpeed(byte speed)
{
	m_fightSpeed = speed;
	m_timeToNextHit = 0;
	m_hitSpeed = m_hitAnim->getLength(); // Hit in 1 second
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
	Vector3 vel = m_body->getLinearVelocity();
	///Stop free rotation
	m_body->setAngularVelocity(Vector3(0,0,0));
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
				vel.x = 0;
				vel.z = 0;
				if( (vel.y > 0) && !m_jump )
					vel.y = 0;
				m_body->setLinearVelocity(vel);
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
			{
				walk(&toWayPoint);
				vel = m_body->getLinearVelocity();
			}
		};
		if( noWalkAnim )
			walkAnimStep(time,true);
		else
			walkAnimStep(time,false);
	}
	else
	{
		Real velLen = vel.squaredLength();
		if( velLen > 0 )
		{
			vel.x = 0;
			vel.z = 0;
			if( (vel.y > 0) && !m_jump )
				vel.y = 0;
			m_body->setLinearVelocity(vel);
		};
		if( m_walkAnim->getTimePosition() != getAnimStandPos() )
			walkAnimStep(time,true);
	}
	if( (m_noGravityPos - m_body->getPosition()).squaredLength() > CHAR_SIZE / 50 )
 		m_body->setAffectedByGravity(true);
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
			m_fight = ch;
		}
	}
	if( m_fight )
	{
		if( m_closeCombat )
		{
			lookAt(m_fight->getPosition2());
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

bool CCharacter::processCollision(OgreOde::Contact* contact)
{
	if( !contact->getFirstGeometry()->getObject() || !contact->getSecondGeometry()->getObject() )
	{
		if( m_body->getAffectedByGravity() )
		{
			/// Collision with landscape
			m_body->setAffectedByGravity(false);
			m_noGravityPos = m_body->getPosition();
		}
	}
	contact->setCoulombFriction(0);
	contact->setBouncyness(0);
	return true;
};

void CCharacter::jump()
{
	uint velocity = 40;
	Real time = -2.1 * velocity / OgreOde::World::getSingleton().getGravity().y;
	m_jumpSpeed = m_jumpAnim->getLength() / time;
	m_jumpAnim->setEnabled(true);
	m_jumpAnim->setTimePosition(0);
	Vector3 vel = m_body->getLinearVelocity();
	vel.y += velocity;
	m_body->setLinearVelocity(vel);
	m_jump = true;
};

void CCharacter::setRotation(Radian rot)
{
	Quaternion q;
	q.FromAngleAxis(rot,Vector3::UNIT_Y);
	m_body->setOrientation(q);
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
	if( ch )
		m_fight = ch;
	else
		m_fightId = enemy;
	m_closeCombat = closeCombat;
};

void CCharacter::stopFight()
{
	CLog::instance()->log(CLog::msgLvlInfo,"(%s): stop fight.\n",getName());
	m_fightId = 0;
	m_closeCombat = false;
	m_fight.reset();
};
