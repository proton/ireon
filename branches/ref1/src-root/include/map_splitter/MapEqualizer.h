/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
***************************************************************************/
/**
@file 
MapEqualizer.h
@brief
Utility that split big map in tiles, pre-calculate splatting by
pre-calculating coverage map and pre-calculate normals.
*/
#ifndef MapEqualizer_H
#define MapEqualizer_H


#include "Ogre.h"
#include "OgreImage.h"
#include "OgreString.h"
#include "OgreSingleton.h"


#include "OgrePagingLandScapeData2D.h"

namespace Ogre
{

/**
 * \ingroup Tool_MapEqualizer
 *
 *
 * \par untested on linux
 *
 * \version 0.2
 * second version
 *
 * \date 06-07-2004
 *
 * \author tuan.kuranes@free.fr
 *
 * \todo 
 *
 * \bug 
 */
class MapEqualizer
{
    public :

        MapEqualizer();
        ~MapEqualizer();

        void equalize ();
        void normalize (Real *field, const uint size);
         /*
          Generate terrain using diamond-square (plasma) algorithm
        */
        void MakeTerrainPlasma(Real * field,int size,float rough);
        /*
         Drop particles, peak, flood fill, erode
        */
        void MapEqualizer::MakeTerrainDeposition(Real * field,int size,int jumps,
                                            int peakWalk, int minParticlesPerJump,int maxParticlesPerJump,
                                            float caldera);
 private:
   
        /*
        Returns a random number between v1 and v2
        */
        float RangedRandom(float v1,float v2);

        void MakeTerrainFault(Real *field,int size,int iterations,
                                        int maxDelta,int minDelta,
                                        int iterationsPerFilter,
                                        float filter);
        
         
        /*
            Erosion filter -
            Erodes a terrain in all 4 directions
        */
        void Erode(Real * field,int size,float filter);
        /*
        Erosion filter -
            FilterHeightBand applies a FIR filter across a row or column of the height field
        */ 
        void ErodeOneDirection(float *band,int stride,int count,float filter);
   
        uint mColorsNum;
        uint mwidth;
        uint mheight;
        Real *mData;
};


/**
 * \ingroup Tool_ErodeFault
 * \author tuan.kuranes@free.fr
 */
class ErodeFault
{
    public :

        ErodeFault();
        ~ErodeFault();

        /**
        * Make the heighfield uses the max of information range
        * does modify heavily heightmap
        * \param field the heightmap
        * \param size field must be square and side is length of a side
        * \param hmin min height in heightfield
        * \param hmax max height in heightfield
        * \param maxColor 256 for 8bits heightmap and 65535 for raw files
        */
        void equalize (Real * const field, 
                        const uint size, 
                        const Real hmin, 
                        const Real hmax, 
                        const uint maxColor) const;

        void normalize (Real * const field, 
                        const uint size,
                        const Real scale) const;
        /*
          Generate terrain using diamond-square (plasma) algorithm
        */
        void MakeTerrainPlasma(Real * const field, 
                                const uint size, 
                                const float rough,
                                const float scale) const;
        /*
         Drop particles, peak, flood fill, erode
        */
        void MakeTerrainDeposition(Real * const field,
                                    const uint size,
                                    const uint jumps,
                                    const uint peakWalk, 
                                    const uint minParticlesPerJump,
                                    const uint maxParticlesPerJump,
                                    const float caldera,
                                    const float scale) const;
        
        void MakeTerrainFault(Real * const field, 
                                const uint size,
                                const uint iterations,
                                const uint maxDelta, 
                                const uint minDelta,
                                const uint iterationsPerFilter,
                                const float filter,
                                const float scale) const;      
        /*
            Erosion filter -
            Erodes a terrain in all 4 directions
        */
        void Erode(Real * const field, const uint size, const float filter) const;
        
 private:
   
        /*
        Returns a random number between v1 and v2
        */
        float RangedRandom(const float v1, const float v2) const;         
        /*
        Erosion filter -
            FilterHeightBand applies a FIR filter across a row or column of the height field
        */ 
        void ErodeOneDirection(float * const band, 
            const int stride,  
            const uint count, 
            const float filter) const;
};


    // Defines
    #define PI					3.14159265359f				// PI
    #define HALF_PI				1.570796326795f				// PI / 2
    #define TWO_PI				6.28318530718f				// PI * 2
    #define LOGHALF				-0.6931471805599f			// log(0.5)
    #define LOGHALFI			-1.442695040889f			// Inverse of log(0.5)
    #define DELTA				1e-6f						// Small number for comparing floating point numbers
    #define MAX_DIMENSIONS		4							// Maximum number of dimensions in a noise object
    #define MAX_OCTAVES			128							// Maximum # of octaves in an fBm object

    // Macros
    #define SQUARE(a)			((a) * (a))
    #define FLOOR(a)			((int)(a) - ((a) < 0 && (a) != (int)(a)))
    #define CEILING(a)			((int)(a) + ((a) > 0 && (a) != (int)(a)))
    #define MIN(a, b)			((a) < (b) ? (a) : (b))
    #define MAX(a, b)			((a) > (b) ? (a) : (b))
    #define ABS(a)				((a) < 0 ? -(a) : (a))
    #define CLAMP(a, b, x)		((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))
    #define LERP(a, b, x)		((a) + (x) * ((b) - (a)))
    #define CUBIC(a)			((a) * (a) * (3 - 2*(a)))
    #define STEP(a, x)			((x) >= (a))
    #define BOXSTEP(a, b, x)	Clamp(0, 1, ((x)-(a))/((b)-(a)))
    #define PULSE(a, b, x)		(((x) >= (a)) - ((x) >= (b)))
    #define GAMMA(a, g)			powf(a, 1/g)
    #define BIAS(a, b)			powf(a, logf(b) * LOGHALFI)
    #define EXPOSE(l, k)		(1 - expf(l * k))
    #define DEGTORAD(x)			((x) * 0.01745329251994f)
    #define RADTODEG(x)			((x) * 57.29577951308f)
    #define SWAP(a, b, t)		t = a; a = b; b = t

    // Inline functions (use instead of macros to avoid performing slow operations twice)
    inline float Square(const float a)					{ return a * a; }
    inline int Floor(const float a)						{ return ((int)a - (a < 0 && a != (int)a)); }
    inline int Ceiling(const float a)						{ return ((int)a + (a > 0 && a != (int)a)); }
    inline float Min(const float a, const float b)				{ return (a < b ? a : b); }
    inline float Max(const float a, const float b)				{ return (a > b ? a : b); }
    inline float Abs(const float a)						{ return (a < 0 ? -a : a); }
    inline float Clamp(const float a, const float b, const float x)	{ return (x < a ? a : (x > b ? b : x)); }
    inline float Lerp(const float a, const float b, const float x)	{ return a + x * (b - a); }
    inline float Cubic(const float a)						{ return a * a * (3 - 2*a); }
    inline float Step(const float a, const float x)				{ return (float)(x >= a); }
    inline float Boxstep(const float a, const float b, const float x)	{ return Clamp(0, 1, (x-a)/(b-a)); }
    inline float Pulse(const float a, const float b, const float x)	{ return (float)((x >= a) - (x >= b)); }
    inline float Gamma(const float a, const float g)			{ return powf(a, 1/g); }
    inline float Bias(const float a, const float b)				{ return powf(a, logf(b) * LOGHALFI); }
    inline float Expose(const float l, const float k)			{ return (1 - expf(-l * k)); }

    inline float Gain(const float a, const float b)
    {
	    if(a <= DELTA)
		    return 0;
	    if(a >= 1-DELTA)
		    return 1;

	    register float p = (logf(1 - b) * LOGHALFI);
	    if(a < 0.5)
		    return powf(2 * a, p) * 0.5f;
	    else
		    return 1 - powf(2 * (1 - a), p) * 0.5f;
    }

    inline float Smoothstep(const float a, const float b, const float x)
    {
	    if(x <= a)
		    return 0;
	    if(x >= b)
		    return 1;
	    return Cubic((x - a) / (b - a));
    }

    inline float Mod(float a, const float b)
    {
	    a -= ((int)(a / b)) * b;
	    if(a < 0)
		    a += b;
	    return a;
    }

    inline void Normalize(float * const f, const uint n)
    {
	    float fMagnitude = 0;
        uint i;
	    for(i=0; i<n; i++)
		    fMagnitude += f[i]*f[i];
	    const float cfMagnitude = 1 / sqrtf(fMagnitude);
	    for(i=0; i<n; i++)
		    f[i] *= cfMagnitude;
    }

    /*******************************************************************************
    * Class: CRandom
    ********************************************************************************
    * This class wraps a random number generator. I plan to implement my own random
    * number generator so I can keep the seeds as member variables (which is more
    * flexible than using statics or globals). I was using one I found on the
    * Internet implemented in assembler, but I was having problems with it so I
    * removed it for this demo.
    *******************************************************************************/
    class CRandom
    {
    public:
	    CRandom()						{}
	    CRandom(unsigned int nSeed)		{ Init(nSeed); }
	    void Init(unsigned int nSeed)	{ srand(nSeed); }
	    double Random()					{ return (double)rand()/(double)RAND_MAX; }
	    double RandomD(const double dMin, const double dMax)
	    {
		    const double dInterval = dMax - dMin;
		    const double d = dInterval * Random();
		    return dMin + MIN(d, dInterval);
	    }
	    unsigned int RandomI(const unsigned int nMin, const unsigned int nMax)
	    {
		    const unsigned int nInterval = nMax - nMin;
		    const unsigned int i = (unsigned int)((nInterval+1.0) * Random());
		    return nMin + MIN(i, nInterval);
	    }
    };

    /*******************************************************************************
    * Class: CNoise
    ********************************************************************************
    * This class implements the Perlin noise function. Initialize it with the number
    * of dimensions (1 to 4) and a random seed. I got the source for the first 3
    * dimensions from "Texturing & Modeling: A Procedural Approach". I added the
    * extra dimension because it may be desirable to use 3 spatial dimensions and
    * one time dimension. The noise buffers are set up as member variables so that
    * there may be several instances of this class in use at the same time, each
    * initialized with different parameters.
    *******************************************************************************/
    class CNoise
    {
    protected:
	    uint m_nDimensions;						// Number of dimensions used by this object
	    unsigned char m_nMap[256];				// Randomized map of indexes into buffer
	    float m_nBuffer[256][MAX_DIMENSIONS];	// Random n-dimensional buffer

	    float Lattice(const uint ix, const float fx, 
            const uint iy=0, const float fy=0, 
            const uint iz=0, const float fz=0, 
            const uint iw=0, const float fw=0) const 
	    {
		    uint n[4] = {ix, iy, iz, iw};
		    float f[4] = {fx, fy, fz, fw};
		    int nIndex = 0;
            uint i;
		    for(i=0; i<m_nDimensions; i++)
			    nIndex = m_nMap[(nIndex + n[i]) & 0xFF];
		    float fValue = 0;
		    for(i=0; i<m_nDimensions; i++)
			    fValue += m_nBuffer[nIndex][i] * f[i];
		    return fValue;
	    }

    public:
	    CNoise()	{}
	    CNoise(const uint nDimensions, const unsigned int nSeed)	{ Init(nDimensions, nSeed); }
	    void Init(const uint nDimensions, const unsigned int nSeed);
	    float Noise(float * const f) const ;
    };

    /*******************************************************************************
    * Class: CFractal
    ********************************************************************************
    * This class implements fBm, or fractal Brownian motion. Since fBm uses Perlin
    * noise, this class is derived from CNoise. Initialize it with the number of
    * dimensions (1 to 4), a random seed, H (roughness ranging from 0 to 1), and
    * the lacunarity (2.0 is often used). Many of the fractal routines came from
    * "Texturing & Modeling: A Procedural Approach". fBmTest() is my own creation,
    * and I created it to generate my first planet.
    *******************************************************************************/
    class CFractal : public CNoise
    {

    public:
	    CFractal()	{}
	    CFractal(const uint nDimensions, const unsigned int nSeed, const float fH, const float fLacunarity)
	    {
		    Init(nDimensions, nSeed, fH, fLacunarity);
	    }
	    void Init(const uint nDimensions, const unsigned int nSeed, const float fH, const float fLacunarity)
	    {
		    CNoise::Init(nDimensions, nSeed);
		    m_fH = fH;
		    m_fLacunarity = fLacunarity;
		    float f = 1;
		    uint i;
		    for(i=0; i<MAX_OCTAVES; i++) 
		    {
			    m_fExponent[i] = powf(f, -m_fH);
			    f *= m_fLacunarity;
		    }
	    }
	    float fBm(float *f, const float fOctaves) const ;
	    float Turbulence(float *f, const float fOctaves) const ;
	    float Multifractal(float *f, const float fOctaves, const float fOffset) const ;
	    float Heterofractal(float *f, const float fOctaves, const float fOffset) const ;
	    float HybridMultifractal(float *f, const float fOctaves, const float fOffset, const float fGain) const ;
	    float RidgedMultifractal(float *f, const float fOctaves, const float fOffset,const  float fThreshold) const ;
	    float fBmTest(float *f, const float fOctaves) const ;

    protected:
	    float m_fH;
	    float m_fLacunarity;
	    float m_fExponent[MAX_OCTAVES];
    };




}
#endif //MapEqualizer_H
