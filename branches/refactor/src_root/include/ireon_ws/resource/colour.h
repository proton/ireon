/* Copyright (C) 2005 ireon.org developers council
 * $Id: colour.h 288 2005-11-26 09:46:52Z zak $

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
 *
 * Based on Ogre::ColourValue, original copyright information follows
 * Copyright (c) 2000-2005 The OGRE Team
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 */

/**
 * @file colour.h
 * Colour
 */
#ifndef _COLOUR_H
#define _COLOUR_H

struct Colour
{
	Colour(float red = 1.0f,
				 float green = 1.0f,
				 float blue = 1.0f,
				 float alpha = 1.0f ) :
				 r(red), g(green), b(blue), a(alpha)
				{ }

	float r;
	float g;
	float b;
	float a;

	uint getRGBA() const;
	uint getARGB() const;
	uint getABGR() const;
	
	void setRGBA(uint val);
	void setARGB(uint val);
	void setABGR(uint val);

	inline bool operator == (const Colour& rhs) const
	{
		return (r == rhs.r && b == rhs.b && g == rhs.g && a == rhs.a );
	};

	inline bool operator != (const Colour& rhs) const
	{
		return !operator==(rhs);
	};

	inline Colour operator + ( const Colour& rkVector ) const
	{
		Colour kSum;

		kSum.r = r + rkVector.r;
		kSum.g = g + rkVector.g;
		kSum.b = b + rkVector.b;
		kSum.a = a + rkVector.a;

		return kSum;
	}

	inline Colour operator - ( const Colour& rkVector ) const
	{
		Colour kDiff;

		kDiff.r = r - rkVector.r;
		kDiff.g = g - rkVector.g;
		kDiff.b = b - rkVector.b;
		kDiff.a = a - rkVector.a;

		return kDiff;
	}

	inline Colour operator * (const float fScalar ) const
	{
		Colour kProd;

		kProd.r = fScalar*r;
		kProd.g = fScalar*g;
		kProd.b = fScalar*b;
		kProd.a = fScalar*a;

		return kProd;
	}

	inline Colour operator * ( const Colour& rhs) const
	{
		Colour kProd;

		kProd.r = rhs.r * r;
		kProd.g = rhs.g * g;
		kProd.b = rhs.b * b;
		kProd.a = rhs.a * a;

		return kProd;
	}

	inline Colour operator / ( const Colour& rhs) const
	{
		Colour kProd;

		kProd.r = rhs.r / r;
		kProd.g = rhs.g / g;
		kProd.b = rhs.b / b;
		kProd.a = rhs.a / a;

		return kProd;
	}

	inline Colour operator / (const float fScalar ) const
	{
		assert( fScalar != 0.0 );

		Colour kDiv;

		float fInv = 1.0 / fScalar;
		kDiv.r = r * fInv;
		kDiv.g = g * fInv;
		kDiv.b = b * fInv;
		kDiv.a = a * fInv;

		return kDiv;
	}

	inline friend Colour operator * (const float fScalar, const Colour& rkVector )
	{
		Colour kProd;

		kProd.r = fScalar * rkVector.r;
		kProd.g = fScalar * rkVector.g;
		kProd.b = fScalar * rkVector.b;
		kProd.a = fScalar * rkVector.a;

		return kProd;
	}

	inline Colour& operator += ( const Colour& rkVector )
	{
		r += rkVector.r;
		g += rkVector.g;
		b += rkVector.b;
		a += rkVector.a;

		return *this;
	}

	inline Colour& operator -= ( const Colour& rkVector )
	{
		r -= rkVector.r;
		g -= rkVector.g;
		b -= rkVector.b;
		a -= rkVector.a;

		return *this;
	}

	inline Colour& operator *= (const float fScalar )
	{
		r *= fScalar;
		g *= fScalar;
		b *= fScalar;
		a *= fScalar;
		return *this;
	}

	inline Colour& operator /= (const float fScalar )
	{
		assert( fScalar != 0.0 );

		float fInv = 1.0 / fScalar;

		r *= fInv;
		g *= fInv;
		b *= fInv;
		a *= fInv;

		return *this;
	}	

	static Colour Black;
	static Colour White;
	static Colour Red;
	static Colour Green;
	static Colour Blue;
};

#endif
