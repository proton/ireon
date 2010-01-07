#ifndef _OGREODEJOINT_H_
#define _OGREODEJOINT_H_

#include "OgreOdePreReqs.h"
#include "OgreOdeMaintainedList.h"

namespace OgreOde 
{
	class _OgreOdeExport JointGroup:public MaintainedItem
	{
		friend class World;
		friend class Joint;

	public:
		JointGroup();
		~JointGroup();

		void empty();

		virtual unsigned long getID();

	protected:
		dJointGroupID getJointGroupID() const;

	protected:
		dJointGroupID _joint_group;
	};

	class _OgreOdeExport Joint:public MaintainedItem
    {
		friend class World;

	public:
		enum Type
		{
			Type_BallJoint					= dJointTypeBall,
			Type_HingeJoint					= dJointTypeHinge,
			Type_SliderJoint				= dJointTypeSlider,
			Type_UniversalJoint				= dJointTypeUniversal,
			Type_VehicleSuspension_Joint	= dJointTypeHinge2,
			Type_FixedJoint					= dJointTypeFixed,
			Type_AngularMotorJoint			= dJointTypeAMotor
#ifdef OGREODE_PLANARJOINT
			,Type_PlanarJoint				= dJointTypePlane2D
#endif
		};

		enum Parameter
		{
			Parameter_LowStop				= dParamLoStop,
			Parameter_HighStop				= dParamHiStop,
			Parameter_MotorVelocity			= dParamVel,
			Parameter_MaximumForce			= dParamFMax,
			Parameter_FudgeFactor			= dParamFudgeFactor,
			Parameter_Bounceyness			= dParamBounce,
			Parameter_CFM					= dParamCFM,
			Parameter_StopERP				= dParamStopERP,
			Parameter_StopCFM				= dParamStopCFM,
			Parameter_SuspensionERP			= dParamSuspensionERP,
			Parameter_SuspensionCFM			= dParamSuspensionCFM 
		};

	public:
		Joint(const JointGroup* group = 0);
		virtual ~Joint();

		Joint::Type getType(); 

		void registerJoint();

		void detach();
		virtual void attach(const Body* body);
		virtual void attach(const Body* body_a,const Body* body_b); 

		Body* getFirstBody(); 
		Body* getSecondBody(); 
		
		static bool areConnected(const Body* body_a,const Body* body_b); 
		static bool areConnectedExcluding(const Body* body_a,const Body* body_b,Joint::Type joint_type); 
	
		void enableFeedback(); 
		void disableFeedback(); 
		const Vector3& getFirstForce();
		const Vector3& getFirstTorque();
		const Vector3& getSecondForce();
		const Vector3& getSecondTorque();

		virtual void setAnchor(const Vector3& position);
		virtual const Vector3& getAnchor();
		virtual const Vector3& getAnchorError();
		virtual void setAxis(const Vector3& axis);
		virtual const Vector3& getAxis();
		virtual void setAdditionalAxis(const Vector3& axis);
		virtual const Vector3& getAdditionalAxis();
		virtual Real getAngle();
		virtual Real getAngleRate();
		virtual Real getPosition();
		virtual Real getPositionRate();

		virtual void setParameter(Joint::Parameter parameter,Real value,int axis = 1);
		virtual Real getParameter(Joint::Parameter parameter,int axis = 1);

		virtual unsigned long getID();

		virtual void addTorque(Real torque,Real torque2 = 0.0,Real torque3 = 0.0);
		virtual void addForce(Real force,Real force2 = 0.0,Real force3 = 0.0);

	protected:
		dJointID getJointID();

		dWorldID getWorldID();
		dJointGroupID getJointGroupID(const JointGroup* group) const;

	protected:
		dJointID _joint;
		dJointFeedback _feedback;
		Vector3 _axis,_anchor,_anchor_error,_additional_axis;
		Vector3 _first_force,_first_torque,_second_force,_second_torque;
	};

	class _OgreOdeExport BallJoint:public Joint
	{
	public:
		BallJoint(const JointGroup* group = 0);
		~BallJoint();

		virtual void setAnchor(const Vector3& position);
		virtual const Vector3& getAnchor();
		virtual const Vector3& getAnchorError();
	};

	class _OgreOdeExport HingeJoint:public Joint
	{
	public:
		HingeJoint(const JointGroup* group = 0);
		~HingeJoint();

		virtual void setAnchor(const Vector3& position);
		virtual const Vector3& getAnchor();
		virtual const Vector3& getAnchorError();
		virtual void setAxis(const Vector3& axis);
		virtual const Vector3& getAxis();
		virtual Real getAngle();
		virtual Real getAngleRate();

		virtual void setParameter(Joint::Parameter parameter,Real value,int axis = 1);
		virtual Real getParameter(Joint::Parameter parameter,int axis = 1);

		virtual void addTorque(Real torque,Real torque2 = 0.0,Real torque3 = 0.0);
	};

	class _OgreOdeExport SliderJoint:public Joint
	{
	public:
		SliderJoint(const JointGroup* group = 0);
		~SliderJoint();

		virtual void setAxis(const Vector3& axis);
		virtual const Vector3& getAxis();
		virtual Real getPosition();
		virtual Real getPositionRate();

		virtual void setParameter(Joint::Parameter parameter,Real value,int axis = 1);
		virtual Real getParameter(Joint::Parameter parameter,int axis = 1);

		virtual void addForce(Real force,Real force2 = 0.0,Real force3 = 0.0);
	};

	class _OgreOdeExport UniversalJoint:public Joint
	{
	public:
		UniversalJoint(const JointGroup* group = 0);
		~UniversalJoint();

		virtual void setAnchor(const Vector3& position);
		virtual const Vector3& getAnchor();
		virtual const Vector3& getAnchorError();
		virtual void setAxis(const Vector3& axis);
		virtual const Vector3& getAxis();
		virtual void setAdditionalAxis(const Vector3& axis);
		virtual const Vector3& getAdditionalAxis();

		virtual void setParameter(Joint::Parameter parameter,Real value,int axis = 1);
		virtual Real getParameter(Joint::Parameter parameter,int axis = 1);

		virtual void addTorque(Real torque,Real torque2 = 0.0,Real torque3 = 0.0);
	};

	class _OgreOdeExport VehicleSuspensionJoint:public Joint
	{
	public:
		VehicleSuspensionJoint(const JointGroup* group = 0);
		~VehicleSuspensionJoint();

		virtual void setAnchor(const Vector3& position);
		virtual const Vector3& getAnchor();
		virtual const Vector3& getAnchorError();
		virtual void setAxis(const Vector3& axis);
		virtual const Vector3& getAxis();
		virtual void setAdditionalAxis(const Vector3& axis);
		virtual const Vector3& getAdditionalAxis();
		virtual Real getAngle();
		virtual Real getAngleRate();
		virtual Real getPositionRate();

		virtual void setParameter(Joint::Parameter parameter,Real value,int axis = 1);
		virtual Real getParameter(Joint::Parameter parameter,int axis = 1);

		virtual void addTorque(Real torque,Real torque2 = 0.0,Real torque3 = 0.0);
	};

	class _OgreOdeExport FixedJoint:public Joint
	{
	public:
		FixedJoint(const JointGroup* group = 0);
		~FixedJoint();

		virtual void attach(const Body* body);
		virtual void attach(const Body* body_a,const Body* body_b); 
	};

	class _OgreOdeExport AngularMotorJoint:public Joint
	{
	public:
		enum Mode
		{
			Mode_UserAngularMotor	= dAMotorUser,
			Mode_EulerAngularMotor	= dAMotorEuler 
		};

		enum RelativeOrientation
		{
			RelativeOrientation_GlobalFrame = 0,
			RelativeOrientation_FirstBody	= 1,		
			RelativeOrientation_SecondBody	= 2
		};

	public:
		AngularMotorJoint(const JointGroup* group = 0);
		~AngularMotorJoint();

		void setMode(AngularMotorJoint::Mode mode);
		AngularMotorJoint::Mode getMode(); 

		void setAxisCount(int axes);
		int getAxisCount(); 

		void setAxis(int axis_number,AngularMotorJoint::RelativeOrientation orientation,const Vector3& axis);
		const Vector3& getAxis(int axis_number);
		AngularMotorJoint::RelativeOrientation getAxisRelativeOrientation(int axis_number); 

		void setAngle(int axis,Real angle); 
		Real getAngle(int axis); 
		Real getAngleRate(int axis); 

		virtual void setParameter(Joint::Parameter parameter,Real value,int axis = 1);
		virtual Real getParameter(Joint::Parameter parameter,int axis = 1);

		virtual void addTorque(Real torque,Real torque2 = 0.0,Real torque3 = 0.0);
	};

#ifdef OGREODE_PLANARJOINT
	class _OgreOdeExport PlanarJoint:public Joint
	{
	public:
		PlanarJoint(const JointGroup* group = 0);
		~PlanarJoint();

		virtual void setParameterX(Joint::Parameter parameter,Real value,int axis = 1);
		virtual void setParameterY(Joint::Parameter parameter,Real value,int axis = 1);
		virtual void setParameterAngle(Joint::Parameter parameter,Real value,int axis = 1);
	};
#endif
}

#endif 
