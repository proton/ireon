#include "OgreOdeWorld.h"
#include "OgreOdeBody.h"
#include "OgreOdeMass.h"

#include "OgreOdeDebugObject.h"

#include "OgreOdeGeometry.h"

using namespace OgreOde;

int Body::_body_count = 0;
const String Body::MovableType = "OgreOde::Body";

Body::Body(const String& name)
{
	_body_count++;
	_body = dBodyCreate(World::getSingleton().getWorldID()); 
	
	_debug_node = 0;
	_mass = new Mass();

	dBodySetData(_body,(void*)this);
	World::getSingleton().getBodyList().registerItem(this);

	if(name == StringUtil::BLANK)
	{
		_name = MovableType + StringConverter::toString(_body_count);
	}
	else _name = name;

	_linear_damping = World::getSingleton()._linear_damping;
	_angular_damping = World::getSingleton()._angular_damping;

	_user_data = 0;
}

void Body::_notifyAttached(Node* parent,bool isTagPoint)
{
	MovableObject::_notifyAttached(parent,isTagPoint);

	if(parent)
	{
		Body* other_body = World::getSingleton().findBody(static_cast<SceneNode*>(parent));
		if((other_body)&&(other_body != this))
		{
			static_cast<SceneNode*>(parent)->detachObject(other_body);
		}
		setPosition(parent->getPosition());
		setOrientation(parent->getOrientation());
	}
}

void Body::_updateRenderQueue(RenderQueue* queue)
{
}


void Body::_notifyCurrentCamera(Camera* camera)
{
}

void Body::destroyDebugNode()
{
	if(_debug_node)
	{
		World::getSingleton().notifyGeometry(this);

		SceneNode* sn = static_cast<SceneNode*>(_debug_node);
		static_cast<SceneNode*>(sn->getParent())->removeAndDestroyChild(sn->getName());
		_debug_node = 0;
	}
}

void Body::addDebugNode(Node* node)
{
	if(!_debug_node)
	{
		_debug_node = World::getSingleton()._manager->getRootSceneNode()->createChild(_name + String("_Debug"));
	}
	_debug_node->addChild(node);
}

void Body::setPosition(const Vector3& position)
{
	dBodySetPosition(_body,(dReal)position.x,(dReal)position.y,(dReal)position.z); 

	if(mParentNode) mParentNode->setPosition(position);
}

void Body::setOrientation(const Quaternion& orientation)
{
	dQuaternion q;
	q[0] = (dReal)orientation.w;
	q[1] = (dReal)orientation.x;
	q[2] = (dReal)orientation.y;
	q[3] = (dReal)orientation.z;
	dBodySetQuaternion(_body,q); 

	if(mParentNode) mParentNode->setOrientation(orientation);
}

void Body::setLinearVelocity(const Vector3& linear_velocity)
{
	dBodySetLinearVel(_body,(dReal)linear_velocity.x,(dReal)linear_velocity.y,(dReal)linear_velocity.z); 
}

void Body::setAngularVelocity(const Vector3& angular_velocity)
{
	dBodySetAngularVel(_body,(dReal)angular_velocity.x,(dReal)angular_velocity.y,(dReal)angular_velocity.z); 
}

void Body::setTorque(const Vector3& torque)
{
	dBodySetTorque(_body,(dReal)torque.x,(dReal)torque.y,(dReal)torque.z); 
}

const Vector3& Body::getPosition()
{
	const dReal* position = dBodyGetPosition(_body);
	_position.x = (Real)position[0];
	_position.y = (Real)position[1];
	_position.z = (Real)position[2];
	return _position;
}

const Quaternion& Body::getOrientation()
{
	const dReal* orientation = dBodyGetQuaternion(_body); 
	_orientation.w = (Real)orientation[0];
	_orientation.x = (Real)orientation[1];
	_orientation.y = (Real)orientation[2];
	_orientation.z = (Real)orientation[3];
	return _orientation;
}

const Vector3& Body::getLinearVelocity()
{
	const dReal* linear_velocity = dBodyGetLinearVel(_body); 
	_linear_vel.x = (Real)linear_velocity[0];
	_linear_vel.y = (Real)linear_velocity[1];
	_linear_vel.z = (Real)linear_velocity[2];
	return _linear_vel;
}

const Vector3& Body::getAngularVelocity()
{
	const dReal* angular_velocity = dBodyGetAngularVel(_body); 
	_angular_vel.x = (Real)angular_velocity[0];
	_angular_vel.y = (Real)angular_velocity[1];
	_angular_vel.z = (Real)angular_velocity[2];
	return _angular_vel;
}

const String& Body::getMovableType() const
{
	return MovableType;
}

const AxisAlignedBox& Body::getBoundingBox(void) const
{
	return _bounding_box;
}

Real Body::getBoundingRadius(void) const
{
	return 0.0;
}

void Body::setMass(const Mass& mass)
{
	dBodySetMass(_body,mass.getMassPtr());
}

const Mass& Body::getMass()
{
	dMass mass;
	dBodyGetMass(_body,&mass); 
	*_mass = &mass;
	return *_mass;
}

void Body::addForce(const Vector3& force)
{
	dBodyAddForce(_body,(dReal)force.x,(dReal)force.y,(dReal)force.z);
}

void Body::addTorque(const Vector3& torque)
{
	dBodyAddTorque(_body,(dReal)torque.x,(dReal)torque.y,(dReal)torque.z);
}

void Body::addRelativeForce(const Vector3& force)
{
	dBodyAddRelForce(_body,(dReal)force.x,(dReal)force.y,(dReal)force.z);
}

void Body::addRelativeTorque(const Vector3& torque)
{
	dBodyAddRelTorque(_body,(dReal)torque.x,(dReal)torque.y,(dReal)torque.z);
}

void Body::addForceAt(const Vector3& force,const Vector3& position)
{
	dBodyAddForceAtPos(_body,(dReal)force.x,(dReal)force.y,(dReal)force.z,(dReal)position.x,(dReal)position.y,(dReal)position.z); 
}

void Body::addForceAtRelative(const Vector3& force,const Vector3& position)
{
	dBodyAddForceAtRelPos(_body,(dReal)force.x,(dReal)force.y,(dReal)force.z,(dReal)position.x,(dReal)position.y,(dReal)position.z);
}

void Body::addRelativeForceAt(const Vector3& force,const Vector3& position)
{
    dBodyAddRelForceAtPos(_body,(dReal)force.x,(dReal)force.y,(dReal)force.z,(dReal)position.x,(dReal)position.y,(dReal)position.z); 
}

void Body::addRelativeForceAtRelative(const Vector3& force,const Vector3& position)
{
	dBodyAddRelForceAtRelPos(_body,(dReal)force.x,(dReal)force.y,(dReal)force.z,(dReal)position.x,(dReal)position.y,(dReal)position.z); 
}

const Vector3& Body::getForce()
{
	const dReal* force = dBodyGetForce(_body); 
	_force.x = force[0];
	_force.y = force[1];
	_force.z = force[2];
	return _force;
}

const Vector3& Body::getTorque()
{
	const dReal* torque = dBodyGetTorque(_body); 
	_torque.x = torque[0];
	_torque.y = torque[1];
	_torque.z = torque[2];
	return _torque;
}

/*
Given a point on a body, get that point's position in the world
*/
Vector3 Body::getPointWorldPosition(const Vector3& position)
{
	dVector3 result;
	dBodyGetRelPointPos(_body,(dReal)position.x,(dReal)position.y,(dReal)position.z,result);
	return Vector3(result[0],result[1],result[2]);
}

/*
Given a point on a body, get that point's velocity in the world
*/
Vector3 Body::getPointWorldVelocity(const Vector3& position)
{
	dVector3 result;
	dBodyGetRelPointVel(_body,(dReal)position.x,(dReal)position.y,(dReal)position.z,result);
	return Vector3(result[0],result[1],result[2]);
}

/*
Given a point (in the world), get that point's velocity in the world with respect to the body
i.e. convert the global point to a relative point on the body and compute the velocity of that
point on the body
*/
Vector3 Body::getPointVelocity(const Vector3& position)
{
	dVector3 result;
	dBodyGetPointVel(_body,(dReal)position.x,(dReal)position.y,(dReal)position.z,result); 
	return Vector3(result[0],result[1],result[2]);
}

Vector3 Body::getPointBodyPosition(const Vector3& position)
{
	dVector3 result;
	dBodyGetPosRelPoint(_body,(dReal)position.x,(dReal)position.y,(dReal)position.z,result); 
	return Vector3(result[0],result[1],result[2]);
}

Vector3 Body::getVectorToWorld(const Vector3& vector)
{
	dVector3 result;
	dBodyVectorToWorld(_body,(dReal)vector.x,(dReal)vector.y,(dReal)vector.z,result);
	return Vector3(result[0],result[1],result[2]);
}

Vector3 Body::getVectorFromWorld(const Vector3& vector)
{
	dVector3 result;
	dBodyVectorFromWorld(_body,(dReal)vector.x,(dReal)vector.y,(dReal)vector.z,result); 
	return Vector3(result[0],result[1],result[2]);
}

void Body::wake()
{
	dBodyEnable(_body); 
}

void Body::sleep()
{
	dBodyDisable(_body); 
}

bool Body::isAwake()
{
	return ((dBodyIsEnabled(_body))?true:false); 
}

void Body::setAutoSleep(bool auto_disable)
{
	dBodySetAutoDisableFlag(_body,((auto_disable)?1:0)); 
}

bool Body::getAutoSleep()
{
	return ((dBodyGetAutoDisableFlag(_body))?true:false); 
}

void Body::setAutoSleepLinearThreshold(Real linear_threshold)
{
	dBodySetAutoDisableLinearThreshold(_body,(dReal)linear_threshold); 
}

Real Body::getAutoSleepLinearThreshold()
{
	return (Real)dBodyGetAutoDisableLinearThreshold(_body); 
}

void Body::setAutoSleepAngularThreshold(Real angular_threshold)
{
	dBodySetAutoDisableAngularThreshold(_body,(dReal)angular_threshold); 
}

Real Body::getAutoSleepAngularThreshold()
{
	return (Real)dBodyGetAutoDisableAngularThreshold(_body); 
}

void Body::setAutoSleepSteps(int steps)
{
	dBodySetAutoDisableSteps(_body,steps); 
}

int Body::getAutoSleepSteps()
{
	return dBodyGetAutoDisableSteps(_body); 
}

void Body::setAutoSleepTime(Real time)
{
	dBodySetAutoDisableTime(_body,(dReal)time); 
}

Real Body::getAutoSleepTime()
{
	return (Real)dBodyGetAutoDisableTime(_body); 
}

void Body::setAutoSleepDefaults()
{
	dBodySetAutoDisableDefaults(_body);
}

void Body::setFiniteRotationMode(bool on)
{
	dBodySetFiniteRotationMode(_body,((on)?1:0)); 
}

bool Body::getFiniteRotationMode()
{
	return ((dBodyGetFiniteRotationMode(_body))?true:false); 
}

void Body::setFiniteRotationAxis(const Vector3& axis)
{
	dBodySetFiniteRotationAxis(_body,(dReal)axis.x,(dReal)axis.y,(dReal)axis.z); 
}

const Vector3& Body::getFiniteRotationAxis()
{
	dVector3 result;
	dBodyGetFiniteRotationAxis(_body,result); 
	_finite_axis.x = result[0];
	_finite_axis.x = result[1];
	_finite_axis.x = result[2];
	return _finite_axis;
}

int Body::getJointCount()
{
	return dBodyGetNumJoints(_body); 
}

Joint* Body::getJoint(int index)
{
	return (Joint*)World::getSingleton().getJointList().findItem((unsigned int)dBodyGetJoint(_body,index)); 
}

int Body::getGeometryCount()
{
	std::map<unsigned long,MaintainedItem*>::iterator i = World::getSingleton()._geometry_list._map.begin();
	std::map<unsigned long,MaintainedItem*>::iterator end = World::getSingleton()._geometry_list._map.end();

	int rc = 0;

	for(;i != end;++i)
	{
		if(this == ((Geometry*)i->second)->getBody()) ++rc;
	}

	return rc;
}

Geometry* Body::getGeometry(int index)
{
	std::map<unsigned long,MaintainedItem*>::iterator i = World::getSingleton()._geometry_list._map.begin();
	std::map<unsigned long,MaintainedItem*>::iterator end = World::getSingleton()._geometry_list._map.end();

	Geometry* rc = 0;
	int idx = 0;

	for(;i != end;++i)
	{
		if(this == ((Geometry*)i->second)->getBody())
		{
			rc = (Geometry*)i->second;
			++idx;

			if(idx > index) break;
		}
	}

	return rc;
}

void Body::setAffectedByGravity(bool on)
{
	dBodySetGravityMode(_body,((on)?1:0)); 
}

bool Body::getAffectedByGravity()
{
	return ((dBodyGetGravityMode(_body))?true:false); 
}

dBodyID Body::getBodyID() const
{
	return _body;
}

const String& Body::getName(void) const
{
	return _name;
}

void Body::updateParentNode()
{
	if(mParentNode)
	{
		mParentNode->setPosition(getPosition());
		mParentNode->setOrientation(getOrientation());
	}

	if(_debug_node)
	{
		_debug_node->setPosition(getPosition());
		_debug_node->setOrientation(getOrientation());
		
		recursiveSetMode(static_cast<SceneNode*>(_debug_node));
	}
}

void Body::deriveLocation()
{
	if(mParentNode)
	{
		setPosition(mParentNode->getPosition());
		setOrientation(mParentNode->getOrientation());
	}
}

void Body::recursiveSetMode(SceneNode* node)
{
	for(int i = 0;i < node->numChildren();i++)
	{
		recursiveSetMode(static_cast<SceneNode*>(node->getChild(i)));		
	}

	for(int j = 0;j < node->numAttachedObjects();j++)
	{
		static_cast<DebugObject*>(node->getAttachedObject(j))->setMode((isAwake())?DebugObject::Mode_Enabled:DebugObject::Mode_Disabled);
	}
}

void Body::setDamping(Real linear_damping,Real angular_damping)
{
	_linear_damping = -(dReal)linear_damping;
	_angular_damping = -(dReal)angular_damping;
}

Real Body::getLinearDamping()
{
	return -(Real)_linear_damping;
}

Real Body::getAngularDamping()
{
	return -(Real)_angular_damping;
}

void Body::applyDamping()
{
	if(dBodyIsEnabled(_body))
	{
		const dReal* v;

		if(_linear_damping < 0.0)
		{
			v = dBodyGetLinearVel(_body);
			dBodyAddForce(_body,v[0] * _linear_damping,v[1] * _linear_damping,v[2] * _linear_damping);
		}

		if(_angular_damping < 0.0)
		{
			v = dBodyGetAngularVel(_body);
			dBodyAddTorque(_body,v[0] * _angular_damping,v[1] * _angular_damping,v[2] * _angular_damping);
		}
	}
}

void Body::sync()
{
	if((dBodyIsEnabled(_body))||(_debug_node))
	{
		applyDamping();
		updateParentNode();
	}
}

void Body::setDebug(bool debug)
{
	destroyDebugNode();
}

void Body::setUserData(unsigned long user_data)
{
	_user_data = user_data;
}

unsigned long Body::getUserData()
{
	return _user_data;
}

unsigned long Body::getID()
{
	return (unsigned long)_body;
}

Body::~Body()
{
	destroyDebugNode();
	delete _mass;

	World::getSingleton().getBodyList().unregisterItem((unsigned int)_body);
	dBodyDestroy(_body);
}
