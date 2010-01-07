#ifndef _OGREODEMASS_H_
#define _OGREODEMASS_H_

#include "OgreOdePreReqs.h"

namespace OgreOde 
{
	class _OgreOdeExport Mass
	{
		friend class Body;

	public:
		Mass();
		Mass(Real mass,const Vector3& center_of_gravity,const Matrix3& intertia_matrix); 
		virtual ~Mass();

		inline Mass(const Mass& other)
		{
			memcpy(&_mass,&(other._mass),sizeof(_mass));
		}

		inline Mass& operator=(const Mass& other)
		{
			memcpy(&_mass,&(other._mass),sizeof(_mass));
			return *this;
		}

		inline Mass& operator=(const dMass* other)
		{
			assert(other);
			memcpy(&_mass,other,sizeof(_mass));
			return *this;
		}

		void adjust(Real mass); 
		void translate(const Vector3& offset); 
		void rotate(const Quaternion& orientation); 
		void add(const Mass& other); 

	protected:
		const dMass* getMassPtr() const;
		dMass _mass;
	};

	class _OgreOdeExport SphereMass:public Mass
	{
	public:
		SphereMass();
		SphereMass(Real mass,Real radius);
		~SphereMass();

		void setDensity(Real density,Real radius);
	};

	class _OgreOdeExport CapsuleMass:public Mass
	{
	public:
		CapsuleMass();
		CapsuleMass(Real mass,Real radius,const Vector3& direction,Real length);
		~CapsuleMass();

		void setDensity(Real density,Real radius,const Vector3& direction,Real length);
	};

	class _OgreOdeExport CylinderMass:public Mass
	{
	public:
		CylinderMass();
		CylinderMass(Real mass,const Vector3& direction,Real radius,Real length);
		~CylinderMass();

		void setDensity(Real density,const Vector3& direction,Real radius,Real length);
	};

	class _OgreOdeExport BoxMass:public Mass
	{
	public:
		BoxMass();
		BoxMass(Real mass,const Vector3& size);
		~BoxMass();

		void setDensity(Real density,const Vector3& size);
	};
}

#endif
