#ifndef _OGREODEBODY_H_
#define _OGREODEBODY_H_

#include "OgreOdePreReqs.h"

namespace OgreOde 
{
	class _OgreOdeExport Body: public MaintainedItem
		,public MovableObject
    {
		friend class Joint;
		friend class World;
		friend class Geometry;

	public:
		Body(const String& name = StringUtil::BLANK);
		virtual ~Body();

		static const String MovableType;

		void setPosition(const Vector3& position); 
		void setOrientation(const Quaternion& orientation); 
		void setLinearVelocity(const Vector3& linear_velocity); 
		void setAngularVelocity(const Vector3& angular_velocity);
		void setTorque(const Vector3& torque);
		const Vector3& getPosition(); 
		const Quaternion& getOrientation(); 
		const Vector3& getLinearVelocity(); 
		const Vector3& getAngularVelocity(); 

		virtual const String& getMovableType() const; 
		virtual const String& getName(void) const;
		virtual const AxisAlignedBox& getBoundingBox(void) const;
		virtual Real getBoundingRadius(void) const;
        virtual void _updateRenderQueue(RenderQueue* queue);
		virtual void _notifyAttached(Node* parent,bool isTagPoint = false);
		virtual void _notifyCurrentCamera(Camera* camera);

		void updateParentNode();
		void deriveLocation();


		void setMass(const Mass& mass);
		const Mass& getMass();

		void addForce(const Vector3& force); 
		void addTorque(const Vector3& torque); 
		void addRelativeForce(const Vector3& force); 
		void addRelativeTorque(const Vector3& torque); 
		void addForceAt(const Vector3& force,const Vector3& position); 
		void addForceAtRelative(const Vector3& force,const Vector3& position); 
		void addRelativeForceAt(const Vector3& force,const Vector3& position); 
		void addRelativeForceAtRelative(const Vector3& force,const Vector3& position); 

		const Vector3& getForce(); 
		const Vector3& getTorque(); 

		Vector3 getPointWorldPosition(const Vector3& position); 
		Vector3 getPointWorldVelocity(const Vector3& position);
		Vector3 getPointVelocity(const Vector3& position); 
		Vector3 getPointBodyPosition(const Vector3& position);
		Vector3 getVectorToWorld(const Vector3& vector);
		Vector3 getVectorFromWorld(const Vector3& vector);

		void wake();
		void sleep(); 
		bool isAwake(); 
		void setAutoSleep(bool auto_sleep);
		bool getAutoSleep(); 
		void setAutoSleepLinearThreshold(Real linear_threshold);
		Real getAutoSleepLinearThreshold(); 
		void setAutoSleepAngularThreshold(Real angular_threshold);
		Real getAutoSleepAngularThreshold(); 
		void setAutoSleepSteps(int steps);
		int getAutoSleepSteps(); 
		void setAutoSleepTime(Real time);
		Real getAutoSleepTime(); 
		void setAutoSleepDefaults();

		void setFiniteRotationMode(bool on); 
		bool getFiniteRotationMode(); 
		void setFiniteRotationAxis(const Vector3& axis); 
		const Vector3& getFiniteRotationAxis(); 

        int getJointCount(); 
		Joint* getJoint(int index); 

		int getGeometryCount(); 
		Geometry* getGeometry(int index);

		void setAffectedByGravity(bool on);
		bool getAffectedByGravity(); 

		void setDamping(Real linear_damping,Real angular_damping);
		Real getLinearDamping();
		Real getAngularDamping();

		void setUserData(unsigned long user_data);
		unsigned long getUserData();

		virtual unsigned long getID();
		virtual void sync();
		virtual void setDebug(bool debug);

	protected:
		dBodyID getBodyID() const;

		void destroyDebugNode();
		void addDebugNode(Node* node);

		void recursiveSetMode(SceneNode* node);

		void applyDamping();

	protected:
		dBodyID	_body;
		String _name;
		Node* _debug_node;

		static int _body_count;

		Vector3 _position,_linear_vel,_angular_vel,_finite_axis,_force,_torque;
		Quaternion _orientation;
		AxisAlignedBox _bounding_box;
		Mass* _mass;
		dReal _linear_damping,_angular_damping;
		unsigned long _user_data;
	};
}

#endif
