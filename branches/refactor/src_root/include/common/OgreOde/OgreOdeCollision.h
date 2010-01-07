#ifndef _OGREODECOLLISION_H_
#define _OGREODECOLLISION_H_

#include "OgreOdePreReqs.h"

namespace OgreOde 
{
	class _OgreOdeExport CollisionListener
	{
	public:
		CollisionListener();
		virtual ~CollisionListener();

		virtual bool collision(Contact* contact) = 0;
	};

	class _OgreOdeExport Contact
	{
		friend class Geometry;
		friend class ContactMapCollisionListener;
	
	public:
		enum Flag
		{
			Flag_UseAdditionalFriction			= dContactMu2,
			Flag_UseFirstFrictionDirection		= dContactFDir1,
			Flag_SurfaceIsBouncy				= dContactBounce,
			Flag_UseERP							= dContactSoftERP,
			Flag_UseCFM							= dContactSoftCFM,
			Flag_IndependentMotion				= dContactMotion1, 
			Flag_AdditionalIndependentMotion	= dContactMotion2,
			Flag_UseFDS							= dContactSlip1,
			Flag_UseAdditionalFDS				= dContactSlip2,
			Flag_FrictionPyramid				= dContactApprox1_1,
			Flag_AdditionalFrictionPyramid		= dContactApprox1_2,
			Flag_BothFrictionPyramids			= dContactApprox1 
		};

	public:
		Contact();
		~Contact();

		const Vector3& getPosition();
		const Vector3& getNormal();
		Real getPenetrationDepth();

		Geometry* getFirstGeometry();
		Geometry* getSecondGeometry();

		void setFirstFrictionDirection(const Vector3& vector);
		void setFrictionMode(Contact::Flag flag);

		void setCoulombFriction(Real mu,Real additional_mu = -1.0);
		void setBouncyness(Real bouncyness,Real velocity_threshold = -1.0);
		void setSoftness(Real ERP,Real CFM);
		void setIndependentMotion(Real velocity,Real additional_velocity = -1.0);
		void setForceDependentSlip(Real FDS);
		void setAdditionalFDS(Real FDS);

	protected:
		inline Contact& operator=(dContact* contact)
		{
			_contact = contact;
			_contact->surface.mode = 0;
			return *this;
		}

	protected:
		dContact* _contact;
		Vector3 _position,_normal;
	};

	class _OgreOdeExport ContactMapCollisionListener:public CollisionListener
	{
	public:
		ContactMapCollisionListener();
		virtual ~ContactMapCollisionListener();

		virtual bool collision(Contact* contact);

		void createContact(unsigned long materialA,unsigned long materialB); 
		Contact *getContactPtr(unsigned long materialA,unsigned long materialB); 

	protected:
		std::map<unsigned long,std::map<unsigned long,Contact*>* > _map;
	};
}

#endif
