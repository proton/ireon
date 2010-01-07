/* Copyright (C) 2005 ireon.org developers council
  * $Id: character.cpp 822 2006-11-22 13:34:49Z mip $
 
  *  See LICENSE for details
  */
 
 /**
  * @file character.cpp
  * Character
  */
#include "stdafx.h"
#include "common/world/dynamic_object/dynamic.h"
#include "common/world/generic_object/prototype.h"
#include "common/world/managers/dynamic_collection.h"
//#include "common/interface/window.h"
#include "common/world/OgreOdeMeshInformer.h"
//#include "world/static_object/client_static.h"
//#include "common/db/client_char_data.h"
//#include "ireon_client/interface/client_interface.h"
#include "ireon_client/world/client_world.h"
#include "common/interface/event_manager.h"
#include "common/world/mesh.h"
#include "common/world/client_zone.h"
CDynamicObject::CDynamicObject(CPrototype* proto):
CObject(proto->getName()),
proto_(proto),
m_walkSpeed(0.5),
m_walk(false),
m_walkAnim(NULL),
m_jumpAnim(NULL),
m_jump(false),
m_fightId(0),
m_timeToNextHit(0),
m_dead(false)
{
	type_ = CObject::NONE;
	node_ = I_WORLD->sceneManager()->getRootSceneNode()->createChildSceneNode();
	/*Quaternion q;
	q.FromAngleAxis(Radian(Degree(90)),Vector3::UNIT_Z);
	node_->setOrientation(q);*/
	
	

	node_->setPosition(Vector3::ZERO);

	entity_ = I_WORLD->sceneManager()->createEntity(getUniqueName(name_.c_str()),proto_->getMeshName());
	entity_->setCastShadows(true);

	entityNode_ = node_->createChildSceneNode();
	entityNode_->attachObject(entity_);
	Vector3 size = OgreOde::MeshInformer::getSize(entity_->getMesh().get());
	Vector3 min = Vector3::ZERO - size/2;
	min.y = 0;
	Vector3 max = Vector3::ZERO + size/2;
	max.y = size.y;
	entity_->getMesh()->_setBounds(AxisAlignedBox(min,max));
	Real scale = CHAR_SIZE / size.y;
	entityNode_->setScale(Vector3(scale,scale,scale)); /// All characters 2 units in height

	Quaternion q;
	q.FromAngleAxis(Radian(Degree(180)),Vector3::UNIT_Y);
	entityNode_->setOrientation(q);

	AnimationStateSet* animSet = entity_->getAllAnimationStates();
	assert(animSet);
	// ogre 1.2.2 

	//animSet->getAnimationState("Walk");


	AnimationStateIterator it = animSet->getAnimationStateIterator();
	
	if( animSet->hasAnimationState("Walk")  )
	{
		m_walkAnim = entity_->getAnimationState("Walk");
		m_walkAnim->setEnabled(false);
		m_animStandPos[0] = m_walkAnim->getLength() / 4;
		m_animStandPos[1] = m_walkAnim->getLength() / 4 * 3;
		m_walkAnim->setTimePosition(m_animStandPos[0]);
		/// 1 cycle of animation will be in CHAR_SIZE length
		//m_stepLsen = CHAR_SIZE / m_walkAnim->getLength();
	} else
		CLog::instance()->log(CLog::msgLvlCritical,__FUNCTION__,"Character %s hasn't walk animation.\n",getName());

	if( animSet->hasAnimationState("JumpNoHeight") )
	{
		m_jumpAnim = entity_->getAnimationState("JumpNoHeight");
		m_jumpAnim->setEnabled(false);
		m_jumpAnim->setTimePosition(0);
		m_jumpAnim->setLoop(false);
	} else
		CLog::instance()->log(CLog::msgLvlCritical,__FUNCTION__,"Character %s hasn't jump animation.\n",getName());


	if( animSet->hasAnimationState("Attack1")  )
	{
		m_hitAnim = entity_->getAnimationState("Attack1");
		m_hitAnim->setEnabled(false);
		m_hitAnim->setTimePosition(0);
		m_hitAnim->setLoop(false);
	} else
		CLog::instance()->log(CLog::msgLvlCritical,__FUNCTION__,"Character %s hasn't hit animation.\n",getName());

	if( animSet->hasAnimationState("Die1")  )
	{
		m_dieAnim = entity_->getAnimationState("Die1");
		m_dieAnim->setEnabled(false);
		m_dieAnim->setTimePosition(0);
		m_dieAnim->setLoop(false);
		m_dieSpeed = m_dieAnim->getLength();
	} else 	if( animSet->hasAnimationState("Death1") )
	{
		m_dieAnim = entity_->getAnimationState("Death1");
		m_dieAnim->setEnabled(false);
		m_dieAnim->setTimePosition(0);
		m_dieAnim->setLoop(false);
		m_dieSpeed = m_dieAnim->getLength();
	} else 	if( animSet->hasAnimationState("Collapse") )
	{
		m_dieAnim = entity_->getAnimationState("Collapse");
		m_dieAnim->setEnabled(false);
		m_dieAnim->setTimePosition(0);
		m_dieAnim->setLoop(false);
		m_dieSpeed = m_dieAnim->getLength();
	} else 
		CLog::instance()->log(CLog::msgLvlCritical,__FUNCTION__,"Character %s hasn't die animation.\n",getName());

	setWalkSpeed(0.5);
	m_timeToNextHit = 0;
	m_hitSpeed = m_hitAnim->getLength(); // Hit in 1 second
};

CDynamicObject::~CDynamicObject()
{
	//if( m_info )
	//	I_INTERFACE->destroyWindow(m_info);
};

CDynamicObject* CDynamicObject::productCallback(CPrototype* proto)
{
	assert(proto);
	CDynamicObject* dynamicObject = new CDynamicObject(proto);
	assert(dynamicObject);
	CDynamicCollection::getSingletonPtr()->addObject(dynamicObject);
	return dynamicObject;
}

uint CDynamicObject::getId()
{
	return 0;
};
 
byte CDynamicObject::getPercentHP()
{
	return percentHP_;
};

void CDynamicObject::setPercentHP(byte percent)
{
	percentHP_ = percent;
};

byte CDynamicObject::getFightSpeed()
{
	return fightSpeed_;
};

Real CDynamicObject::getAnimStandPos()
{
	Real pos = m_walkAnim->getTimePosition();
	if( pos <= m_animStandPos[0] )
		return m_animStandPos[0];
	if (pos <= m_animStandPos[1] )
		return m_animStandPos[1];
	return 100;
};

void CDynamicObject::walk(Real time, Vector2 *direction)
{
	//Vector3 pos = getPosition3D();
	//if( direction )
	//	setRotation(Math::ATan2(-direction->x,-direction->y));
	//m_walk = true;
	//if( !m_jump )
	//{
	//	Vector3 to = Vector3(pos.x + Math::Sin(getRotation(),true) * time * getWalkSpeed(),0,pos.z + Math::Cos(getRotation(),true) * time * getWalkSpeed());
	//	to.y = I_WORLD->heightAt(to);
	//	Vector2 norm;
	//	if( canWalk(to,norm) )
	//		setPosition3D(to);
	//	else
	//	{
	//		Vector2 to2(to.x,to.z);
	//		Vector2 pos2(pos.x,pos.z);
	//		Vector2 diff = to2-pos2;
	//		Vector2 position2 = pos2 + diff - norm * diff.dotProduct(norm);
	//		to.x = position2.x;
	//		to.z = position2.y;
	//		to.y = I_WORLD->heightAt(to);
	//		if( canWalk(to,norm) )
	//			setPosition3D(to);
	//	}
	//}

	// nav mesh used
	//Vector3 curentPosition2D = getPosition3D();
	
	ireon::nav::Point	direction2D;
	direction2D = ireon::nav::Point( Math::Sin(getRotation(),true), 0, Math::Cos(getRotation(),true) );
	ireon::nav::Point curentPosition2D = CClientWorld::instance()->getZone()->getNavigationMesh()->GetPointOnMesh(posOnMesh_);

	Vector3 perp = direction2D.toVector3().crossProduct(Vector3::UNIT_X);
	perp = perp.crossProduct(direction2D.toVector3()).normalisedCopy();
	Vector3 perp2 = perp.crossProduct(direction2D.toVector3()).normalisedCopy();
	Matrix3 sk2sk(	-perp.x,  perp2.x, direction2D.x, 
					-perp.y, perp2.y, direction2D.y, 
					-perp.z, perp2.z , direction2D.z );
	if( direction )
	{
		//setRotation(Math::ATan2(-direction->x,-direction->y));
		direction2D = sk2sk.Transpose() * Vector3(direction->y, 0, direction->x);
	}		
	_log(_info,_loc,"------------------------------------------------");

	_log(_info,_loc,"Direction = (%12.4f, %12.4f, %12.4f) \n",(float)direction2D.x, (float)direction2D.y, (float)direction2D.z);
	_log(_info,_loc,"CurPos = (%12.4f, %12.4f, %12.4f)\n",(float)curentPosition2D.x, (float)curentPosition2D.y, (float)curentPosition2D.z );

	ireon::nav::Triangle* curentTriangle = CClientWorld::instance()->getZone()->getNavigationMesh()->getTriangleForPoint(curentPosition2D);
	_log(_info,_loc,"Cur Tri [ %u ] = %s\n",curentTriangle->number,curentTriangle->toText().c_str());

	ireon::nav::Position curentPositionOnMesh2D(curentPosition2D, curentTriangle);
	
	m_walk = true;
	// time - Real (s), need - short (ms)
	
	_log(_info,_loc,"--------");

	ireon::nav::Position newPositionOnMesh2D( CClientWorld::instance()->getZone()->getNavigationMesh()->walk(curentPositionOnMesh2D, direction2D * m_walkSpeed, 1) );
	ireon::nav::Point newPosition3D = newPositionOnMesh2D.getPoint3D();
	// mesh coor == ogre coor 
	setPosition3D(newPosition3D.toVector3());
	_log(_info,_loc,"NewPos = %s\n",newPosition3D.toText().c_str());
	_log(_info,_loc,"New Tri [ %u ] = %s\n",curentTriangle->number,newPositionOnMesh2D.tri->toText().c_str());

	
};

void CDynamicObject::lookAt(const Vector2& to)
{
	Vector2 dir = to - getPosition2D();
	setRotation(Math::ATan2(-dir.x,-dir.y));
};

void CDynamicObject::setWalkSpeed(Real speed)
{
	m_walkSpeed = speed;
	m_stepLen = speed / 2 / m_walkAnim->getLength();
};

void CDynamicObject::walkAnimStep(const Real& time, bool toStand)
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

void CDynamicObject::update(Real time)
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
				I_WORLD->removeDead(getId());
		}
		return;
	}
	if( isWalking() && !m_jump )
	{
		bool noWalkAnim = false;
		if( !m_walk )
		{
			/// Walking by waypoints
			Vector2 pos2 = getPosition2D();
			Vector2 toWayPoint = m_wayPoints[0].m_pos - pos2;
			if( toWayPoint.squaredLength() < getWalkSpeed() * getWalkSpeed() * time * time )
			{
				setPosition2D(m_wayPoints[0].m_pos);
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
	/*if( m_fightId )
	{
		CharacterPtr ch = I_WORLD->findCharacter(m_fightId);
		if( ch )
		{
			m_fightId = 0;
			m_enemy = ch;
		}
	}*/
	if( m_enemy )
	{
		if( m_closeCombat )
		{
			lookAt(m_enemy->getPosition2D());
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
	arg.m_ch = I_WORLD->findCharacter(getId());
	arg.m_time = time;
	if( arg.m_ch )
		CEventManager::instance()->execute(ON_CHAR_UPDATE,&arg);
};

void CDynamicObject::jump()
{
/*	uint velocity = 40;
	Real time = -2.1 * velocity / OgreOde::World::getSingleton().getGravity().y;
	m_jumpSpeed = m_jumpAnim->getLength() / time;
	m_jumpAnim->setEnabled(true);
	m_jumpAnim->setTimePosition(0);
	m_jump = true;*/
};

void CDynamicObject::setRotation(Radian rot)
{
	Quaternion q;
	q.FromAngleAxis(rot,Vector3::UNIT_Y);
	setOrientation(q);
	m_rotation = rot;
};

bool CDynamicObject::visible()
{
	return I_WORLD->pointIsVisible(getPosition3D() + Vector3(0,CHAR_SIZE,0));
};

void CDynamicObject::startFight(uint enemy, bool closeCombat)
{
	_log(_info,_loc,"(%s): start fight with %d in %s.\n",getName(),enemy,closeCombat ? "close combat" : "ranged combat");
	CharacterPtr ch = I_WORLD->findCharacter(enemy);
	m_fightId = 0;
	if( isDead() || (ch && ch->isDead()) )
		return;
	/*if( ch )
		m_enemy = ch;
	else
		m_fightId = enemy;*/
	m_closeCombat = closeCombat;
};

void CDynamicObject::stopFight()
{
	_log(_info,_loc,_("(%s): stop fight.\n"),getName());
	m_fightId = 0;
	m_closeCombat = false;
	m_enemy = NULL;
};

void CDynamicObject::die()
{
	stopFight();
	m_walkAnim->setTimePosition(getAnimStandPos());
	m_walkAnim->setEnabled(false);
	m_jump = false;
	clearWayPoints();
	m_hitAnim->setEnabled(false);
	m_dieAnim->setEnabled(true);
	m_dead = true;
	CharacterPtr me = I_WORLD->findCharacter(getId());
	if( !me )
		return;
	I_WORLD->addDead(me);
	I_WORLD->removeCharacter(getId());
	m_timeToDisappear = 10;
	CharacterEventArg arg;
	arg.m_ch = me;
	CEventManager::instance()->execute(ON_CHAR_DIE,&arg);
};

Vector2 getVectorForPair(const Vector2& pos1, const Vector2& pos2)
{
	const Real sqrt2 = 0.7071067811865475;
	uint x1, x2, z1, z2;
	bool up1, up2;
	// TODO: find analog in new version PLSM2
	x1 = 0;
	x2 = 0;
	z1 = 0;
	z2 = 0;
	up1 = false;
	up2 = false;
	
	//PagingLandScapeData2DManager::getSingleton().getTriInfo(pos1.x,pos1.y,x1,z1,up1);
	//PagingLandScapeData2DManager::getSingleton().getTriInfo(pos2.x,pos2.y,x2,z2,up2);
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

bool CDynamicObject::canWalk(const Vector3& to, Vector2& norm)
{
	Vector2 to2 = Vector2(to.x,to.z);
	Vector2 pos2 = getPosition2D();
	Vector3 normal = I_WORLD->getNormalAt(to2.x,to2.y);
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
	CWorld::CharMapIterator it = I_WORLD->getFirstChar();
	for(; !I_WORLD->lastChar(it); it = I_WORLD->getNextChar() )
		if( (*it).second.get() != this )
			if( ((*it).second->getPosition2D() - to2).squaredLength() < CHAR_SIZE * CHAR_SIZE / 16 )
			{
				norm = pos2 - (*it).second->getPosition2D();
				norm.normalise();
				return false;
			};
//	CWorld::StaticVectorIterator vec = I_WORLD->getFirstStatic();
//	for(; !I_WORLD->lastStatic(vec); vec = I_WORLD->getNextStatic() )
//		if( !(*vec)->canWalk(pos2,to2,norm) )
//			return false;
	return true;
};
