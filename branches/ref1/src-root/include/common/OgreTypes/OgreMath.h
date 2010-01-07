/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/
#ifndef __Math_H__
#define __Math_H__

#include "OgrePrerequisites.h"

namespace Ogre
{
    /** Wrapper class which indicates a given angle value is in Radians.
    @remarks
        Radian values are interchangeable with Degree values, and conversions
        will be done automatically between them.
    */
	class Radian
	{
		Real mRad;

	public:
		explicit Radian ( Real r=0 ) : mRad(r) {}
		Radian ( const Degree& d );
		const Radian& operator = ( const Real& f ) { mRad = f; return *this; }
		const Radian& operator = ( const Radian& r ) { mRad = r.mRad; return *this; }
		const Radian& operator = ( const Degree& d );

		Real valueDegrees() const; // see bottom of this file
		Real valueRadians() const { return mRad; }
		Real valueAngleUnits() const;

		Radian operator + ( const Radian& r ) const { return Radian ( mRad + r.mRad ); }
		Radian operator + ( const Degree& d ) const;
		Radian& operator += ( const Radian& r ) { mRad += r.mRad; return *this; }
		Radian& operator += ( const Degree& d );
		Radian operator - () { return Radian(-mRad); }
		Radian operator - ( const Radian& r ) const { return Radian ( mRad - r.mRad ); }
		Radian operator - ( const Degree& d ) const;
		Radian& operator -= ( const Radian& r ) { mRad -= r.mRad; return *this; }
		Radian& operator -= ( const Degree& d );
		Radian operator * ( Real f ) const { return Radian ( mRad * f ); }
        Radian operator * ( const Radian& f ) const { return Radian ( mRad * f.mRad ); }
		Radian& operator *= ( Real f ) { mRad *= f; return *this; }
		Radian operator / ( Real f ) const { return Radian ( mRad / f ); }
		Radian& operator /= ( Real f ) { mRad /= f; return *this; }

		bool operator <  ( const Radian& r ) const { return mRad <  r.mRad; }
		bool operator <= ( const Radian& r ) const { return mRad <= r.mRad; }
		bool operator == ( const Radian& r ) const { return mRad == r.mRad; }
		bool operator != ( const Radian& r ) const { return mRad != r.mRad; }
		bool operator >= ( const Radian& r ) const { return mRad >= r.mRad; }
		bool operator >  ( const Radian& r ) const { return mRad >  r.mRad; }
	};

    /** Wrapper class which indicates a given angle value is in Degrees.
    @remarks
        Degree values are interchangeable with Radian values, and conversions
        will be done automatically between them.
    */
	class Degree
	{
		Real mDeg; // if you get an error here - make sure to define/typedef 'Real' first

	public:
		explicit Degree ( Real d=0 ) : mDeg(d) {}
		Degree ( const Radian& r ) : mDeg(r.valueDegrees()) {}
		const Degree& operator = ( const Real& f ) { mDeg = f; return *this; }
		const Degree& operator = ( const Degree& d ) { mDeg = d.mDeg; return *this; }
		const Degree& operator = ( const Radian& r ) { mDeg = r.valueDegrees(); return *this; }

		Real valueDegrees() const { return mDeg; }
		Real valueRadians() const; // see bottom of this file
		Real valueAngleUnits() const;

		Degree operator + ( const Degree& d ) const { return Degree ( mDeg + d.mDeg ); }
		Degree operator + ( const Radian& r ) const { return Degree ( mDeg + r.valueDegrees() ); }
		Degree& operator += ( const Degree& d ) { mDeg += d.mDeg; return *this; }
		Degree& operator += ( const Radian& r ) { mDeg += r.valueDegrees(); return *this; }
		Degree operator - () { return Degree(-mDeg); }
		Degree operator - ( const Degree& d ) const { return Degree ( mDeg - d.mDeg ); }
		Degree operator - ( const Radian& r ) const { return Degree ( mDeg - r.valueDegrees() ); }
		Degree& operator -= ( const Degree& d ) { mDeg -= d.mDeg; return *this; }
		Degree& operator -= ( const Radian& r ) { mDeg -= r.valueDegrees(); return *this; }
		Degree operator * ( Real f ) const { return Degree ( mDeg * f ); }
        Degree operator * ( const Degree& f ) const { return Degree ( mDeg * f.mDeg ); }
		Degree& operator *= ( Real f ) { mDeg *= f; return *this; }
		Degree operator / ( Real f ) const { return Degree ( mDeg / f ); }
		Degree& operator /= ( Real f ) { mDeg /= f; return *this; }

		bool operator <  ( const Degree& d ) const { return mDeg <  d.mDeg; }
		bool operator <= ( const Degree& d ) const { return mDeg <= d.mDeg; }
		bool operator == ( const Degree& d ) const { return mDeg == d.mDeg; }
		bool operator != ( const Degree& d ) const { return mDeg != d.mDeg; }
		bool operator >= ( const Degree& d ) const { return mDeg >= d.mDeg; }
		bool operator >  ( const Degree& d ) const { return mDeg >  d.mDeg; }
	};

    /** Wrapper class which identifies a value as the currently default angle 
        type, as defined by Math::setAngleUnit.
    @remarks
        Angle values will be automatically converted between radians and degrees,
        as appropriate.
    */
	class Angle
	{
		Real mAngle;
	public:
		explicit Angle ( Real angle ) : mAngle(angle) {}
		operator Radian();
		operator Degree();
	};

	// these functions could not be defined within the class definition of class
	// Radian because they required class Degree to be defined
	inline Radian::Radian ( const Degree& d ) : mRad(d.valueRadians()) {
	}
	inline const Radian& Radian::operator = ( const Degree& d ) {
		mRad = d.valueRadians(); return *this;
	}
	inline Radian Radian::operator + ( const Degree& d ) const {
		return Radian ( mRad + d.valueRadians() );
	}
	inline Radian& Radian::operator += ( const Degree& d ) {
		mRad += d.valueRadians();
		return *this;
	}
	inline Radian Radian::operator - ( const Degree& d ) const {
		return Radian ( mRad - d.valueRadians() );
	}
	inline Radian& Radian::operator -= ( const Degree& d ) {
		mRad -= d.valueRadians();
		return *this;
	}

    /** Class to provide access to common mathematical functions.
        @remarks
            Most of the maths functions are aliased versions of the C runtime
            library functions. They are aliased here to provide future
            optimisation opportunities, either from faster RTLs or custom
            math approximations.
        @note
            <br>This is based on MgcMath.h from
            <a href="http://www.magic-software.com">Wild Magic</a>.
    */
    class _OgreExport Math 
    {
   public:
       /** The angular units used by the API. This functionality is now deprecated in favor
	       of discreet angular unit types ( see Degree and Radian above ). The only place
		   this functionality is actually still used is when parsing files. Search for
		   usage of the Angle class for those instances
       */
       enum AngleUnit
       {
           AU_DEGREE,
           AU_RADIAN
       };

    protected:
       // angle units used by the api
       static AngleUnit msAngleUnit;

        /// Size of the trig tables as determined by constructor.
        static int mTrigTableSize;

        /// Radian -> index factor value ( mTrigTableSize / 2 * PI )
        static Real mTrigTableFactor;
        static Real* mSinTable;
        static Real* mTanTable;

        /** Private function to build trig tables.
        */
        void buildTrigTables();

		static Real SinTable (Real fValue);
		static Real TanTable (Real fValue);
    public:
        /** Default constructor.
            @param
                trigTableSize Optional parameter to set the size of the
                tables used to implement Sin, Cos, Tan
        */
        Math(unsigned int trigTableSize = 4096);

        /** Default destructor.
        */
        ~Math();

		static inline int IAbs (int iValue) { return ( iValue >= 0 ? iValue : -iValue ); }
		static inline int ICeil (float fValue) { return int(ceil(fValue)); }
		static inline int IFloor (float fValue) { return int(floor(fValue)); }
        static int ISign (int iValue);

		static inline Real Abs (Real fValue) { return Real(fabs(fValue)); }
		static inline Degree Abs (const Degree& dValue) { return Degree(fabs(dValue.valueDegrees())); }
		static inline Radian Abs (const Radian& rValue) { return Radian(fabs(rValue.valueRadians())); }
		static Radian ACos (Real fValue);
		static Radian ASin (Real fValue);
		static inline Radian ATan (Real fValue) { return Radian(atan(fValue)); }
		static inline Radian ATan2 (Real fY, Real fX) { return Radian(atan2(fY,fX)); }
		static inline Real Ceil (Real fValue) { return Real(ceil(fValue)); }

        /** Cosine function.
            @param
                fValue Angle in radians
            @param
                useTables If true, uses lookup tables rather than
                calculation - faster but less accurate.
        */
        static inline Real Cos (const Radian& fValue, bool useTables = false) {
			return (!useTables) ? Real(cos(fValue.valueRadians())) : SinTable(fValue.valueRadians() + HALF_PI);
		}
        /** Cosine function.
            @param
                fValue Angle in radians
            @param
                useTables If true, uses lookup tables rather than
                calculation - faster but less accurate.
        */
        static inline Real Cos (Real fValue, bool useTables = false) {
			return (!useTables) ? Real(cos(fValue)) : SinTable(fValue + HALF_PI);
		}

		static inline Real Exp (Real fValue) { return Real(exp(fValue)); }

		static inline Real Floor (Real fValue) { return Real(floor(fValue)); }

		static inline Real Log (Real fValue) { return Real(log(fValue)); }

		static inline Real Pow (Real fBase, Real fExponent) { return Real(pow(fBase,fExponent)); }

        static Real Sign (Real fValue);
		static inline Radian Sign ( const Radian& rValue )
		{
			return Radian(Sign(rValue.valueRadians()));
		}
		static inline Degree Sign ( const Degree& dValue )
		{
			return Degree(Sign(dValue.valueDegrees()));
		}

        /** Sine function.
            @param
                fValue Angle in radians
            @param
                useTables If true, uses lookup tables rather than
                calculation - faster but less accurate.
        */
        static inline Real Sin (const Radian& fValue, bool useTables = false) {
			return (!useTables) ? Real(sin(fValue.valueRadians())) : SinTable(fValue.valueRadians());
		}
        /** Sine function.
            @param
                fValue Angle in radians
            @param
                useTables If true, uses lookup tables rather than
                calculation - faster but less accurate.
        */
        static inline Real Sin (Real fValue, bool useTables = false) {
			return (!useTables) ? Real(sin(fValue)) : SinTable(fValue);
		}

		static inline Real Sqr (Real fValue) { return fValue*fValue; }

		static inline Real Sqrt (Real fValue) { return Real(sqrt(fValue)); }

        static inline Radian Sqrt (const Radian& fValue) { return Radian(sqrt(fValue.valueRadians())); }

        static inline Degree Sqrt (const Degree& fValue) { return Degree(sqrt(fValue.valueDegrees())); }

        /** Inverse square root i.e. 1 / Sqrt(x), good for vector
            normalisation.
        */
		static Real InvSqrt(Real fValue);

        static Real UnitRandom ();  // in [0,1]

        static Real RangeRandom (Real fLow, Real fHigh);  // in [fLow,fHigh]

        static Real SymmetricRandom ();  // in [-1,1]

        /** Tangent function.
            @param
                fValue Angle in radians
            @param
                useTables If true, uses lookup tables rather than
                calculation - faster but less accurate.
        */
		static inline Real Tan (const Radian& fValue, bool useTables = false) {
			return (!useTables) ? Real(tan(fValue.valueRadians())) : TanTable(fValue.valueRadians());
		}
        /** Tangent function.
            @param
                fValue Angle in radians
            @param
                useTables If true, uses lookup tables rather than
                calculation - faster but less accurate.
        */
		static inline Real Tan (Real fValue, bool useTables = false) {
			return (!useTables) ? Real(tan(fValue)) : TanTable(fValue);
		}

		static inline Real DegreesToRadians(Real degrees) { return degrees * fDeg2Rad; }
        static inline Real RadiansToDegrees(Real radians) { return radians * fRad2Deg; }

       /** These functions used to set the assumed angle units (radians or degrees) 
            expected when using the Angle type.
       @par
            You can set this directly after creating a new Root, and also before/after resource creation,
            depending on whether you want the change to affect resource files.
       */
       static void setAngleUnit(AngleUnit unit);
       /** Get the unit being used for angles. */
       static AngleUnit getAngleUnit(void);

       /** Convert from the current AngleUnit to radians. */
       static Real AngleUnitsToRadians(Real units);
       /** Convert from radians to the current AngleUnit . */
       static Real RadiansToAngleUnits(Real radians);
       /** Convert from the current AngleUnit to degrees. */
       static Real AngleUnitsToDegrees(Real units);
       /** Convert from degrees to the current AngleUnit. */
       static Real DegreesToAngleUnits(Real degrees);

       /** Checks wether a given point is inside a triangle, in a
            2-dimensional (Cartesian) space.
            @remarks
                The vertices of the triangle must be given in either
                trigonometrical (anticlockwise) or inverse trigonometrical
                (clockwise) order.
            @param
                px The X-coordinate of the point.
            @param
                py The Y-coordinate of the point.
            @param
                ax The X-coordinate of the triangle's first vertex.
            @param
                ay The Y-coordinate of the triangle's first vertex.
            @param
                bx The X-coordinate of the triangle's second vertex.
            @param
                by The Y-coordinate of the triangle's second vertex.
            @param
                cx The X-coordinate of the triangle's third vertex.
            @param
                cy The Y-coordinate of the triangle's third vertex.
            @returns
                If the point resides in the triangle, <b>true</b> is
                returned.
            @par
                If the point is outside the triangle, <b>false</b> is
                returned.
        */
        static bool pointInTri2D( Real px, Real pz, Real ax, Real az, Real bx, Real bz, Real cx, Real cz );

 
        /** Compare 2 reals, using tolerance for inaccuracies.
        */
        static bool RealEqual(Real a, Real b,
            Real tolerance = std::numeric_limits<Real>::epsilon());

        static const Real POS_INFINITY;
        static const Real NEG_INFINITY;
        static const Real PI;
        static const Real TWO_PI;
        static const Real HALF_PI;
		static const Real fDeg2Rad;
		static const Real fRad2Deg;

    };

	// these functions must be defined down here, because they rely on the
	// angle unit conversion functions in class Math:

	inline Real Radian::valueDegrees() const
	{
		return Math::RadiansToDegrees ( mRad );
	}

	inline Real Radian::valueAngleUnits() const
	{
		return Math::RadiansToAngleUnits ( mRad );
	}

	inline Real Degree::valueRadians() const
	{
		return Math::DegreesToRadians ( mDeg );
	}

	inline Real Degree::valueAngleUnits() const
	{
		return Math::DegreesToAngleUnits ( mDeg );
	}

	inline Angle::operator Radian()
	{
		return Radian(Math::AngleUnitsToRadians(mAngle));
	}

	inline Angle::operator Degree()
	{
		return Degree(Math::AngleUnitsToDegrees(mAngle));
	}

	inline Radian operator * ( Real a, const Radian& b )
	{
		return Radian ( a * b.valueRadians() );
	}

	inline Radian operator / ( Real a, const Radian& b )
	{
		return Radian ( a / b.valueRadians() );
	}

	inline Degree operator * ( Real a, const Degree& b )
	{
		return Degree ( a * b.valueDegrees() );
	}

	inline Degree operator / ( Real a, const Degree& b )
	{
		return Degree ( a / b.valueDegrees() );
	}

}
#endif
