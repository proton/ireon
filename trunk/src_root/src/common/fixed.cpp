//////////////////////////////////////////////////////////////////////////
//
//  Fixed Point Math Class
//
//////////////////////////////////////////////////////////////////////////
//
//  Released under GNU license
//		Erik H Gawtry
//			July, 2005	Version 1.0
//
//
//  Algorythms borrowed from:
//		Andrew Ryder, 11 September 2001
//      Joseph Hall, Unknown Date
//
//
//////////////////////////////////////////////////////////////////////////
//
// Written for doing fixed point math on DSP processors
//
//////////////////////////////////////////////////////////////////////////

#include "common\fixed.h"
#include <math.h>

#define	RESOLUTION			10000L //1000000L
#define	RESOLUTION_FLOAT	10000.0 //1000000.0
#define	RESOLUTION_FLOATf	10000.0f //1000000.0f
#define	FLOAT_RESOLUTION	0.00005//0.0000005
#define	FLOAT_RESOLUTIONf	0.00005f//0.0000005f
#define EPS 1
#define _XPI      31416 // 3.1415926535897932384626433832795
#define XPI      fixed(true,_XPI)
#define _X2PI     62832 // 6.283185307179586476925286766559
#define X2PI     fixed(true,_X2PI)
#define _XPIO2    15708 // 1.5707963267948966192313216916398
#define XPIO2    fixed(true,_XPIO2)
#define _XPIO4    78540 // 0.78539816339744830961566084581988
#define XPIO4    fixed(true, _XPIO4)
#define _XLN_E    27183 // 2.71828182845904523536
#define XLN_E    fixed(true,_XLN_E)
#define _XLN_10   23026 // 2.30258509299404568402
#define XLN_10   fixed(true,_XLN_10)
#define sqrt_error   fixed(true, 10) // 0.001

#include "assert.h"
fixed::fixed(void)
{
	m_nVal = 0;
	_debug_func();
}

fixed::fixed(const fixed& fixedVal)
{
	m_nVal = fixedVal.m_nVal;
	_debug_func();
}

fixed::fixed(const fixed* fixedVal)
{
	m_nVal = fixedVal->m_nVal;
	_debug_func();
}

fixed::fixed(bool bInternal, long nVal)
{
	m_nVal = nVal;
	_debug_func();
}

fixed::fixed(long nVal)
{
	m_nVal = nVal*RESOLUTION;
	_debug_func();
}

fixed::fixed(int nVal)
{
	m_nVal = nVal*RESOLUTION;
	_debug_func();
}

fixed::fixed(short nVal)
{
	m_nVal = nVal*RESOLUTION;
	_debug_func();
}

#ifdef FIXED_HAS_DOUBLE
fixed::fixed(double floatVal)
{
	floatVal += FLOAT_RESOLUTION;
	m_nVal = (long)::floor(floatVal*RESOLUTION);
	_debug_func();
}
#endif

fixed::fixed(float floatVal)
{
	floatVal += FLOAT_RESOLUTIONf;
	m_nVal = (long)::floorf(floatVal*RESOLUTION);
	_debug_func();
}

fixed::~fixed(void)
{
}

fixed fixed::operator++(void)
{
	m_nVal += RESOLUTION;
	_debug_func();
	return *this;
}

fixed fixed::operator--(void)
{
	m_nVal -= RESOLUTION;
	_debug_func();
	return *this;
}

fixed& fixed::operator=(fixed fixedVal)
{
	m_nVal = fixedVal.m_nVal;
	_debug_func();
	return *this;
}

fixed& fixed::operator=(float floatVal)
{
	floatVal += FLOAT_RESOLUTIONf;
	m_nVal = (long)::floorf(floatVal*RESOLUTION);
	_debug_func();
	return *this;
}

#ifdef FIXED_HAS_DOUBLE
fixed& fixed::operator=(double floatVal)
{
	floatVal+=FLOAT_RESOLUTION;
	m_nVal = (long)::floor(floatVal*RESOLUTION);
	_debug_func();
	return *this;
}
#endif

fixed& fixed::operator=(int intVal)
{
	m_nVal = intVal*RESOLUTION;
	_debug_func();
	return *this;
}

fixed& fixed::operator=(unsigned int intVal)
{
	m_nVal = intVal*RESOLUTION;
	_debug_func();
	return *this;
}

#ifdef FIXED_HAS_LONG
fixed& fixed::operator=(long longVal)
{
	m_nVal = longVal*RESOLUTION;
	_debug_func();
	return *this;
}

fixed& fixed::operator=(unsigned long longVal)
{
	m_nVal = longVal*RESOLUTION;
	_debug_func();
	return *this;
}
#endif

fixed& fixed::operator=(short shortVal)
{
	m_nVal = shortVal*RESOLUTION;
	_debug_func();
	return *this;
}

fixed& fixed::operator=(unsigned short shortVal)
{
	m_nVal = shortVal*RESOLUTION;
	_debug_func();
	return *this;
}

fixed& fixed::operator=(char charVal)
{
	m_nVal = charVal*RESOLUTION;
	_debug_func();
	return *this;
}

fixed& fixed::operator=(unsigned char charVal)
{
	m_nVal = charVal*RESOLUTION;
	_debug_func();
	return *this;
}

bool fixed::operator==(float floatVal)
{
	floatVal+=FLOAT_RESOLUTIONf;
	return (m_nVal == (long)::floorf(floatVal*RESOLUTION));
}

#ifdef FIXED_HAS_DOUBLE
bool fixed::operator==(double floatVal)
{
	floatVal+=FLOAT_RESOLUTION;
	return (m_nVal == (long)::floor(floatVal*RESOLUTION));
}
#endif

bool fixed::operator==(fixed fixedVal)
{
	return (m_nVal == fixedVal.m_nVal);
}

bool fixed::operator==(int intVal)
{
	return (m_nVal == intVal*RESOLUTION);
}

#ifdef FIXED_HAS_LONG
bool fixed::operator==(long intVal)
{
	return (m_nVal == intVal*RESOLUTION);
}
#endif

bool fixed::operator==(short intVal)
{
	return (m_nVal == intVal*RESOLUTION);
}

bool fixed::lessthan(fixed b)
{
	return (m_nVal < b.m_nVal);
}

bool fixed::lessthanequal(fixed b)
{
	return (m_nVal <= b.m_nVal);
}

bool fixed::operator<(float floatVal)
{
	floatVal+=FLOAT_RESOLUTIONf;
	return (m_nVal < (long)::floorf(floatVal*RESOLUTION));
}

#ifdef FIXED_HAS_DOUBLE
bool fixed::operator<(double floatVal)
{
	floatVal+=FLOAT_RESOLUTION;
	return (m_nVal < (long)::floor(floatVal*RESOLUTION));
}
#endif

bool fixed::operator<(fixed fixedVal)
{
	return (m_nVal < fixedVal.m_nVal);
}

bool fixed::operator<(int intVal)
{
	return (m_nVal < intVal*RESOLUTION);
}

#ifdef FIXED_HAS_LONG
bool fixed::operator<(long intVal)
{
	return (m_nVal < intVal*RESOLUTION);
}
#endif

bool fixed::operator<(short intVal)
{
	return (m_nVal < intVal*RESOLUTION);
}

bool fixed::operator<=(float floatVal)
{
	floatVal+=FLOAT_RESOLUTIONf;
	return (m_nVal <= (long)::floorf(floatVal*RESOLUTION));
}

#ifdef FIXED_HAS_DOUBLE
bool fixed::operator<=(double floatVal)
{
	floatVal+=FLOAT_RESOLUTION;
	return (m_nVal <= (long)::floor(floatVal*RESOLUTION));
}
#endif

bool fixed::operator<=(fixed fixedVal)
{
	return (m_nVal <= fixedVal.m_nVal);
}

bool fixed::operator<=(int intVal)
{
	return (m_nVal <= intVal*RESOLUTION);
}

#ifdef FIXED_HAS_LONG
bool fixed::operator<=(long intVal)
{
	return (m_nVal <= intVal*RESOLUTION);
}
#endif

bool fixed::operator<=(short intVal)
{
	return (m_nVal <= intVal*RESOLUTION);
}

bool fixed::operator>(float floatVal)
{
	floatVal+=FLOAT_RESOLUTIONf;
	return (m_nVal > (long)::floorf(floatVal*RESOLUTION));
}

#ifdef FIXED_HAS_DOUBLE
bool fixed::operator>(double floatVal)
{
	floatVal+=FLOAT_RESOLUTION;
	return (m_nVal > (long)::floor(floatVal*RESOLUTION));
}
#endif

bool fixed::operator>(fixed fixedVal)
{
	return (m_nVal > fixedVal.m_nVal);
}

bool fixed::operator>(int intVal)
{
	return (m_nVal > intVal*RESOLUTION);
}

#ifdef FIXED_HAS_LONG
bool fixed::operator>(long intVal)
{
	return (m_nVal > intVal*RESOLUTION);
}
#endif

bool fixed::operator>(short intVal)
{
	return (m_nVal > intVal*RESOLUTION);
}

bool fixed::operator>=(float floatVal)
{
	floatVal+=FLOAT_RESOLUTIONf;
	return (m_nVal >= (long)::floorf(floatVal*RESOLUTION));
}

#ifdef FIXED_HAS_DOUBLE
bool fixed::operator>=(double floatVal)
{
	floatVal+=FLOAT_RESOLUTION;
	return (m_nVal >= (long)::floor(floatVal*RESOLUTION));
}
#endif

bool fixed::operator>=(fixed fixedVal)
{
	return (m_nVal >= fixedVal.m_nVal);
}

bool fixed::operator>=(int intVal)
{
	return (m_nVal >= intVal*RESOLUTION);
}

#ifdef FIXED_HAS_LONG
bool fixed::operator>=(long intVal)
{
	return (m_nVal >= intVal*RESOLUTION);
}
#endif

bool fixed::operator>=(short intVal)
{
	return (m_nVal >= intVal*RESOLUTION);
}

bool fixed::operator!=(float floatVal)
{
	floatVal+=FLOAT_RESOLUTIONf;
	return (m_nVal != (long)::floorf(floatVal*RESOLUTION));
}

#ifdef FIXED_HAS_DOUBLE
bool fixed::operator!=(double floatVal)
{
	floatVal+=FLOAT_RESOLUTION;
	return (m_nVal != (long)::floor(floatVal*RESOLUTION));
}
#endif

bool fixed::operator!=(fixed fixedVal)
{
	return (m_nVal != fixedVal.m_nVal);
}

bool fixed::operator!=(int intVal)
{
	return (m_nVal != intVal*RESOLUTION);
}

#ifdef FIXED_HAS_LONG
bool fixed::operator!=(long intVal)
{
	return (m_nVal != intVal*RESOLUTION);
}
#endif

bool fixed::operator!=(short intVal)
{
	return (m_nVal != intVal*RESOLUTION);
}

fixed::operator float(void)
{
	return m_nVal/RESOLUTION_FLOATf;
}

fixed::operator float(void) const
{
	return m_nVal/RESOLUTION_FLOATf;
}

#ifdef FIXED_HAS_DOUBLE
fixed::operator double(void)
{
	return m_nVal/RESOLUTION_FLOAT;
}
#endif

#ifdef FIXED_HAS_LONG
long fixed::GetLong(void)
{
	return m_nVal/RESOLUTION;
}

fixed::operator long(void)
{
	return (m_nVal/RESOLUTION);
}
#endif

fixed::operator int(void)
{
	return (int)(m_nVal/RESOLUTION);
}

fixed::operator short(void)
{
	return (short)(m_nVal/RESOLUTION);
}

fixed::operator unsigned short(void)
{
	return (unsigned short)(m_nVal/RESOLUTION);
}

fixed fixed::floor(void)
{
	return (fixed)(m_nVal/RESOLUTION);
}

fixed fixed::floor( int resolution)
{
	return (fixed)(m_nVal/RESOLUTION);
}
fixed fixed::ceil(void)
{
	return (fixed)(m_nVal/RESOLUTION+1);
}

fixed fixed::operator%(fixed fixedVal)
{
	fixed a;
	a.m_nVal = m_nVal%fixedVal.m_nVal;
	a._debug_func();
	return a;
}

#ifdef FIXED_HAS_LONG
fixed fixed::operator%(long longVal)
{
	fixed a;
	a.m_nVal = m_nVal%longVal;
	a._debug_func();
	return a;
}
#endif

fixed fixed::operator%(int intVal)
{
	fixed a;
	a.m_nVal = m_nVal%intVal;
	a._debug_func();
	return a;
}

fixed fixed::operator%(short shortVal)
{
	fixed a;
	a.m_nVal = m_nVal%shortVal;
	a._debug_func();
	return a;
}

bool fixed::equals(fixed b)
{
	float floatVal1 = (float)b;
	floatVal1+=FLOAT_RESOLUTIONf;
	float floatVal2 = (float)*this;
	floatVal2+=FLOAT_RESOLUTIONf;
	return (abs((long)::floorf(floatVal1*RESOLUTION)- (long)::floorf(floatVal2*RESOLUTION)) <= EPS);
	//return (m_nVal == b.m_nVal);
}

bool fixed::equals(fixed b) const
{
	float floatVal1 = (float)b;
	floatVal1+=FLOAT_RESOLUTIONf;
	float floatVal2 = (float)*this;
	floatVal2+=FLOAT_RESOLUTIONf;
	return (abs((long)::floorf(floatVal1*RESOLUTION)- (long)::floorf(floatVal2*RESOLUTION)) <= EPS);

	
}
fixed fixed::add(const fixed b)
{
	fixed a;
	a.m_nVal = m_nVal+b.m_nVal;
	a._debug_func();
	return a;
}
fixed fixed::add(const fixed b) const
{
	fixed a;
	a.m_nVal = m_nVal+b.m_nVal;
	if ( (long long)a.m_nVal != (long long)m_nVal + (long long)b.m_nVal)
		throw EFixedPointMathError();
	a._debug_func();
	return a;
}
fixed fixed::operator+(const fixed b)
{
	return add(b);
}

fixed fixed::operator+(const fixed b) const
{
	return add(b);
}

fixed fixed::subtract(fixed b)
{
	fixed a;
	a.m_nVal = m_nVal-b.m_nVal;
	if ( (long long)a.m_nVal != (long long)m_nVal - (long long)b.m_nVal)
		throw EFixedPointMathError();
	a._debug_func();
	return a;
}

fixed fixed::operator-(fixed b)
{
	return subtract(b);
}

fixed fixed::multiply(const fixed b)
{
	fixed a;
	a.m_nVal = (long)(((long long)m_nVal*b.m_nVal)/RESOLUTION);
	if ((long long)a.m_nVal != (long long)(((long long)m_nVal*b.m_nVal)/RESOLUTION))
		throw EFixedPointMathError();
	a._debug_func();
	return a;
}

fixed fixed::operator*(const fixed b)
{
	return multiply(b);
}

fixed fixed::divide(fixed b)
{
	if( b == 0 ) return fixed(true, 0xFFFFFFFFL);
	fixed a;
	if( m_nVal < 0xFFFFFFFFL/RESOLUTION )
		a.m_nVal = (long)((long long)m_nVal*RESOLUTION/b.m_nVal);
	else
		a.m_nVal = (long)(((long long)m_nVal*RESOLUTION/b.m_nVal));
	if ((long long)a.m_nVal !=  ((long long)m_nVal*RESOLUTION/b.m_nVal))
		throw EFixedPointMathError();
	a._debug_func();
	return a;
}

fixed fixed::operator/(fixed b)
{
	return divide(b);
}

fixed fixed::add(float b)
{
	fixed _b = b;
	return add(_b);
}

fixed fixed::operator+(float b)
{
	return add(b);
}

fixed fixed::subtract(float b)
{
	fixed _b = b;
	return subtract(_b);
}

fixed fixed::operator-(float b)
{
	return subtract(b);
}

fixed fixed::multiply(float b)
{
	fixed _b = b;
	return multiply(_b);
}

fixed fixed::operator*(float b)
{
	return multiply(b);
}

fixed fixed::divide(float b)
{
	fixed _b = b;
	return divide(_b);
}

fixed fixed::operator/(float b)
{
	return divide(b);
}

#ifdef FIXED_HAS_DOUBLE
fixed fixed::add(double b)
{
	fixed _b = b;
	return add(_b);
}

fixed fixed::operator+(double b)
{
	return add(b);
}

fixed fixed::subtract(double b)
{
	fixed _b = b;
	return subtract(_b);
}

fixed fixed::operator-(double b)
{
	return subtract(b);
}

fixed fixed::multiply(double b)
{
	fixed _b = b;
	return multiply(_b);
}

fixed fixed::operator*(double b)
{
	return multiply(b);
}

fixed fixed::divide(double b)
{
	fixed _b = b;
	return divide(_b);
}

fixed fixed::operator/(double b)
{
	return divide(b);
}
#endif

fixed fixed::add(int b)
{
	fixed _b = b;
	return add(_b);
}

fixed fixed::operator+(int b)
{
	return add(b);
}

fixed fixed::subtract(int b)
{
	fixed _b = b;
	return subtract(_b);
}

fixed fixed::operator-(int b)
{
	return subtract(b);
}

fixed fixed::multiply(int b)
{
	fixed _b = b;
	return multiply(_b);
}

fixed fixed::operator*(int b)
{
	return multiply(b);
}

fixed fixed::divide(int b)
{
	fixed _b = b;
	return divide(_b);
}

fixed fixed::operator/(int b)
{
	return divide(b);
}

#ifdef FIXED_HAS_LONG
fixed fixed::add(long b)
{
	fixed _b = b;
	return add(_b);
}

fixed fixed::operator+(long b)
{
	return add(b);
}

fixed fixed::subtract(long b)
{
	fixed _b = b;
	return subtract(_b);
}

fixed fixed::operator-(long b)
{
	return subtract(b);
}

fixed fixed::multiply(long b)
{
	fixed _b = b;
	return multiply(_b);
}

fixed fixed::operator*(long b)
{
	return multiply(b);
}

fixed fixed::divide(long b)
{
	fixed _b = b;
	return divide(_b);
}

fixed fixed::operator/(long b)
{
	return divide(b);
}
#endif

fixed fixed::add(short b)
{
	fixed _b = b;
	return add(_b);
}

fixed fixed::operator+(short b)
{
	return add(b);
}

fixed fixed::subtract(short b)
{
	fixed _b = b;
	return subtract(_b);
}

fixed fixed::operator-(short b)
{
	return subtract(b);
}

fixed fixed::multiply(short b)
{
	fixed _b = b;
	return multiply(_b);
}

fixed fixed::operator*(short b)
{
	return multiply(b);
}

fixed fixed::divide(short b)
{
	fixed _b = b;
	return divide(_b);
}

fixed fixed::operator/(short b)
{
	return divide(b);
}

fixed fixed::operator*=(fixed val)
{
	m_nVal = (long)(((long long)m_nVal*val.m_nVal)/RESOLUTION);
	_debug_func();
	return *this;
}

#ifdef FIXED_HAS_DOUBLE
fixed fixed::operator*=(double val)
{
	m_nVal = (long)(m_nVal*val);
	_debug_func();
	return *this;
}

fixed fixed::operator/=(double val)
{
	m_nVal = (long)(m_nVal/val);
	_debug_func();
	return *this;
}

fixed fixed::operator-=(double val)
{
	m_nVal -= (long)(val*RESOLUTION);
	_debug_func();
	return *this;
}

fixed fixed::operator+=(double val)
{
	m_nVal += (long)(val*RESOLUTION);
	_debug_func();
	return *this;
}

#endif
#ifdef FIXED_HAS_LONG
fixed fixed::operator*=(long val)
{
	m_nVal = (long)((long long)m_nVal*val);
	_debug_func();
	return *this;
}

fixed fixed::operator/=(long val)
{
	m_nVal = (long)(((long long)m_nVal*RESOLUTION)/val);
	_debug_func();
	return *this;
}

fixed fixed::operator-=(long val)
{
	m_nVal -= val*RESOLUTION;
	_debug_func();
	return *this;
}

fixed fixed::operator+=(long val)
{
	m_nVal += val*RESOLUTION;
	_debug_func();
	return *this;
}

#endif
fixed fixed::operator*=(float val)
{
	m_nVal = (long)(m_nVal*val);
	_debug_func();
	return *this;
}

fixed fixed::operator*=(int val)
{
	m_nVal = (long)((long long)m_nVal*val);
	_debug_func();
	return *this;
}

fixed fixed::operator*=(short val)
{
	m_nVal = (long)((long long)m_nVal*val);
	_debug_func();
	return *this;
}

fixed fixed::operator/=(fixed val)
{
	m_nVal = (long)(((long long)m_nVal*RESOLUTION)/val.m_nVal);
	_debug_func();
	return *this;
}

fixed fixed::operator/=(float val)
{
	m_nVal = (long)(m_nVal/val);
	_debug_func();
	return *this;
}

fixed fixed::operator/=(int val)
{
	m_nVal = (long)(((long long)m_nVal*RESOLUTION)/val);
	_debug_func();
	return *this;
}

fixed fixed::operator/=(short val)
{
	m_nVal = (long)(((long long)m_nVal*RESOLUTION)/val);
	_debug_func();
	return *this;
}

fixed fixed::operator-=(fixed val)
{
	m_nVal -= val.m_nVal;
	_debug_func();
	return *this;
}

fixed fixed::operator-=(float val)
{
	m_nVal -= (long)(val*RESOLUTION);
	_debug_func();
	return *this;
}

fixed fixed::operator-=(int val)
{
	m_nVal -= val*RESOLUTION;
	_debug_func();
	return *this;
}

fixed fixed::operator-=(short val)
{
	m_nVal -= val*RESOLUTION;
	_debug_func();
	return *this;
}

fixed fixed::operator+=(fixed val)
{
	m_nVal += val.m_nVal;
	_debug_func();
	return *this;
}

fixed fixed::operator+=(float val)
{
	m_nVal += (long)(val*RESOLUTION);
	_debug_func();
	return *this;
}

fixed fixed::operator+=(int val)
{
	m_nVal += val*RESOLUTION;
	_debug_func();
	return *this;
}

fixed fixed::operator+=(short val)
{
	m_nVal += val*RESOLUTION;
	_debug_func();
	return *this;
}

#ifdef FIXED_HAS_DOUBLE
fixed operator-(double a, fixed b)
{
	fixed _a = a;
	return _a - b;
}
#endif

fixed operator-(float a, fixed b)
{
	fixed _a = a;
	return _a - b;
}

#ifdef FIXED_HAS_LONG
fixed operator-(long a, fixed b)
{
	fixed _a = a;
	return _a - b;
}
#endif

fixed operator-(int a, fixed b)
{
	fixed _a = a;
	return _a - b;
}

fixed operator-(short a, fixed b)
{
	fixed _a = a;
	return _a - b;
}

#ifdef FIXED_HAS_LONG
fixed operator*(fixed a, long b)
{
	return a.multiply(b);
}
#endif

float operator+=(float& a, fixed b)
{
	return a += (float)b;
}

#ifdef FIXED_HAS_DOUBLE
double operator+=(double& a, fixed b)
{
	return a += (double)b;
}
#endif

float operator-=(float& a, fixed b)
{
	return a -= (float)b;
}

#ifdef FIXED_HAS_DOUBLE
double operator-=(double& a, fixed b)
{
	return a -= (double)b;
}

double operator*=(double& a, fixed b)
{
	return a *= (double)b;
}

double operator/=(double& a, fixed b)
{
	return a /= (double)b;
}

bool operator<(double b, fixed a)
{
	return a >= b;
}

#endif

float operator*=(float& a, fixed b)
{
	return a *= (float)b;
}

float operator/=(float& a, fixed b)
{
	return a /= (float)b;
}

bool operator<(float b, fixed a)
{
	return a >= b;
}

#ifdef FIXED_HAS_LONG
bool operator<(long b, fixed a)
{
	return a >= b;
}
#endif

bool operator<(int b, fixed a)
{
	return a >= b;
}

bool operator<(short b, fixed a)
{
	return a >= b;
}

fixed operator-(fixed a)
{
	return 0-a;
}

/*
static
unsigned int isqrt(unsigned long n)
{
    unsigned long i;
    unsigned long k0, k1, nn;

    for (nn = i = n, k0 = 2; i > 0; i >>= 2, k0 <<= 1)
        ;
    nn <<= 2;
    for (;;) 
	{
        k1 = (nn / k0 + k0) >> 1;
        if (((k0 ^ k1) & ~1) == 0)
            break;
        k0 = k1;
    }
    return (unsigned int) ((k1 + 1) >> 1);
}
*/
// see : http://www.imodeitalia.com/download/FP.java.html ?
static
fixed isqrtB(fixed p_Square)
{
	fixed   res;
	fixed   delta;
	fixed   maxError;

	if( p_Square <= 0 )
		return 0;

	/* start at half */
	res = (p_Square / 2);

	/* determine allowable error */
	maxError =  (p_Square * sqrt_error);

	do
	{
		delta =  (( res * res ) - p_Square);
		res -=  delta/(res * 2 );
	}
	while( delta > maxError || delta < -maxError );

	return res;
}

static
long iabs( register long p_Base )
{
	if( p_Base < 0 ) return -p_Base;
	return p_Base;
}

fixed absx( fixed p_Base )
{
	if( p_Base < 0 ) return -p_Base;
	return p_Base;
}
/*
                     (x^h) - 1
   ln(x)  =   lim    -------      
             h -> 0     h

*/

static
fixed iLog2( fixed p_Base )
{   
    fixed w = 0;
	fixed y = 0;
	fixed z = 0;
	int num = 1;
	int dec = 0;

	if( p_Base == 1 )
		return 0;

	for( dec=0 ; absx( p_Base ) >= 2 ; ++dec )
		p_Base /= XLN_E;

	p_Base -= 1;
	z = p_Base;
	y = p_Base;
	w = 1;

	while( y != y + w )
		y += ( w = ( z = 0 - ( z * p_Base ) ) / ( num += 1 ) );

	return y + dec;
}

/*
	calculate the exponential function using the following series :

                          x^2     x^3     x^4     x^5
	exp(x) == 1  +  x  +  ---  +  ---  +  ---  +  ---  ...
                           2!      3!      4!      5!

*/

static
fixed iExp2(fixed p_Base)
{
	fixed w;
	fixed y;
	int num;

	for( w=1, y=1, num=1 ; y != y+w ; ++num )
		y += ( w *= p_Base / num );

	return y;
}

static
fixed ipow( fixed p_Base, fixed p_Power )
{
	if( p_Base < 0 && p_Power%2 != 0 )
		return - iExp2( (p_Power * iLog2( -p_Base )) );
	else
		return iExp2( (p_Power * iLog2(absx( p_Base ))) );
}

static
fixed ilog10( fixed p_Base )
{
	return iLog2( p_Base ) / XLN_10;
}

fixed fixed::sqrt(void)
{
	//return fixed(true, isqrt(m_nVal)*(RESOLUTION/1000L));
	return isqrtB(*this);
}

fixed sqrtx(fixed fixedVal)
{
	return fixedVal.sqrt();
}

fixed fixed::pow(fixed fixedPower)
{
	return ipow(*this, fixedPower);
}

fixed powx(fixed fixedVal, fixed fixedPower)
{
	return fixedVal.pow(fixedPower);
}

fixed fixed::exp(void)
{
	return iExp2(*this);
}

fixed expx(fixed fixedVal)
{
	return fixedVal.exp();
}

fixed fixed::log10(void)
{
	return ilog10(*this);
}

fixed log10x(fixed fixedVal)
{
	return fixedVal.log10();
}

fixed fixed::log(void)
{
	return iLog2(*this);
/*
	Calculate the POW function by calling EXP :

                  Y      A                 
                 X   =  e    where A = Y * log(X)
*/
}

fixed logx(fixed fixedVal)
{
	return fixedVal.log();
}

fixed floorx(fixed fixedVal)
{
	return fixedVal.floor();
}

fixed ceilx(fixed fixedVal)
{
	return fixedVal.ceil();
}

//
// Taylor Algorythm
// x - x^3/3! + x^5/5! - x^7/7! + x^9/9! ........    
//
// Note: Make xresult a float to get more precision
//
// Only accurate from -PI/2 to PI/2

static
fixed _sinx(fixed x)
{
	fixed xpwr;
	long xftl;
	fixed xresult;
	bool positive;

	xresult = 0;
	xpwr = x;
	xftl = 1;
	positive = true;

	// Note: 12! largest for long
	for(int i = 1; i < 7; i+=2)
	{
		if( positive )
			xresult += (xpwr/xftl);
		else
			xresult -= (xpwr/xftl);

		xpwr *= x;
		xpwr *= x;
		xftl *= i+1;
		xftl *= i+2;
		positive = !positive;
	}

	return xresult;
}

fixed fixed::sin(void)
{
	fixed xresult;
	bool bBottom = false;
	static fixed xPI = XPI;
	static fixed x2PI = X2PI;
	static fixed xPIO2 = XPIO2;

	fixed x(true, m_nVal%_X2PI);
	if( x < 0 )
		x += x2PI;

	if( x > xPI )
	{
		bBottom = true;
		x -= xPI;
	}

	if( x <= xPIO2 )
		xresult = _sinx(x);
	else
		xresult = _sinx(xPIO2-(x-xPIO2));

	if( bBottom )
		return -xresult;

	return xresult;
}

fixed sinx(fixed x)
{
	return x.sin();
}

fixed fixed::cos(void)
{
	fixed xresult;
	bool bBottom = false;
	static fixed xPI = XPI;
	static fixed x2PI = X2PI;
	static fixed xPIO2 = XPIO2;

	fixed x(true, (m_nVal+_XPIO2)%_X2PI);
	if( x < 0 )
		x += x2PI;

	if( x > xPI )
	{
		bBottom = true;
		x -= xPI;
	}

	if( x <= xPIO2 )
		xresult = _sinx(x);
	else
		xresult = _sinx(xPIO2-(x-xPIO2));

	if( bBottom )
		return -xresult;

	return xresult;
}

fixed cosx(fixed x)
{
	return x.cos();
}

fixed fixed::tan(void)
{
	return sin()/cos();
}

fixed tanx(fixed x)
{
	return x.tan();
}
