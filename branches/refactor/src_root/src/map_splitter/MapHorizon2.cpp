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
MapHorizon2.cpp
@brief
Utility that pre-calculate horizon mapping for real-time
light mapping of terrain
*/
#include "MapUtil.h"
#include "MapHorizon2.h"
#include "OgrePagingLandScapeOptions.h"

#include <iostream>

namespace Ogre
{

MapHorizon2::MapHorizon2()
{
   hMap = 0;
}


MapHorizon2::~MapHorizon2()
{
   if(mHorizon.size())
   {
       for (uint k = 0; k < mSamples; k++)
       {
            delete mHorizon[k];
       }
   }
    mHorizon.clear ();

    if(mElevation.size())
    {
        for (uint k = 0; k < 6; k++)
        {
            delete mElevation[k];
        }
    }
    mElevation.clear ();
    if (hMap)
        delete[] hMap;
}
Image *MapHorizon2::getElevationCubeMap(uint sample)
{
    return mElevation[sample];
}
Image *MapHorizon2::getHorizon3DMap(uint sample)
{
    return mHorizon[sample];
}


Vector3 MapHorizon2::getCubeVector(int i,int size,int x,int y)
{
    Real s = ((Real)x + 0.5) / (Real)size * 2.0 - 1.0;
    Real t = ((Real)y + 0.5) / (Real)size * 2.0 - 1.0;
    Vector3 v;
    switch(i)
    {
        case 0: v = Vector3(1.0,-t,-s); break;
        case 1: v = Vector3(-1.0,-t,s); break;
        case 2: v = Vector3(s,1.0,t); break;
        case 3: v = Vector3(s,-1.0,-t); break;
        case 4: v = Vector3(s,-t,1.0); break;
        case 5: v = Vector3(-s,-t,-1.0); break;
    }
    v.normalise();
    return v;
}
void MapHorizon2::calcElevationCubeMap(uint size)
{
    mElevationSize = size;
    mElevation.reserve (mElevationSize);

    std::cout << "Calculating Elevation map";


    uchar *data = new uchar[mElevationSize * mElevationSize * 2];
    for(uint i = 0; i < 6; i++)
    {
        DEBUG_PROGRESS_OUTPUT(".")
        uchar *d = data;
        for(uint y = 0; y < mElevationSize; y++)
        {
            for(uint x = 0; x < mElevationSize; x++)
            {
                Vector3 dir (getCubeVector (i, mElevationSize, x, y));
                Vector3 vec (dir.x, dir.y, 0.0f);
                vec.normalise ();

                Real h =  Math::ACos (Vector3(0,1,0).dotProduct (vec)).valueRadians() / (2.0 * Math::PI);
                if (vec.x < 0)
                    h = 1.0 - h;

                *d++ = uchar ((h * 255));
                *d++ = uchar (((1 - vec.dotProduct (dir)) * 255));
            }
        }
        Image *elevationImage = new Image();

        elevationImage->loadDynamicImage  (data,static_cast <size_t> (mElevationSize), 
           static_cast <size_t> (mElevationSize), 1,  PF_L16, true);
        mElevation.push_back (elevationImage);
      }
    std::cout << "\n";
}



void MapHorizon2::create_layer (uchar *dest, const Vector3 &dir)
{
    Vector3 d = dir;
    Real step = fabs (d.x) > fabs (d.y) ? fabs (d.x) : fabs (d.y);
    d /= step;
    int length = int (Math::Sqrt ((Real)mW * d.x *
                                     (Real)mW * d.x +
                                     (Real)mH * d.y *
                                     (Real)mH * d.y));
    Real dlen =  d.length ();
    for (uint y = 0; y < mH; y++)
    {
        DEBUG_PROGRESS_OUTPUT(".")
        for (uint x = 0; x < mW; x++)
        {
            Vector3 v (x, y, 0.0f);
            *dest = 0;
            Real height_start = height_wrap (x, y);
            Real len = 0;
            Real angle = 0;
            int i = 0;
            int nx = 0;
            int ny = 0;

            const int w = int (mW);
            const int h = int (mH);

            while ( i < length &&
                    (nx < w) &&
                    (ny < h))
            {
                v += d;
                len += dlen;
                nx = int (v.x + 0.5);
                ny = int (v.y + 0.5);
                Real height = height_wrap (nx, ny);
                // If the current sample value is mElevation,
                // no point beyond it can possibly be
                //   higher, so stop here.
                if (height == mElevationHeight)
                    i = length;
                height = (height - height_start) / len;
                if (height > angle)
                    angle = h;
                i++;
            }
            *dest++ = (uchar)((1.0 - Math::Cos (Math::ATan (angle))) * 255.0);
        }
    }
    std::cout << "\n";
}



Real MapHorizon2::height_wrap(int x, int y)
{
    int w = int (mW);
    int h = int (mH);

    while (x < 0)
        x += w;
    while (y < 0)
        y += h;
    if (x >= w)
        x = x % w;
    if (y >= h)
        y = y % h;
    return hMap[y * w + x];
}

void MapHorizon2::calcHorizon3DMap(Real vscale,
                                    uint samples,
                                    Real step)
{
    mHeightMapData = MapUtil::getSingleton().getHeightData ();
    mW = MapUtil::getSingleton().getMapWidth ();
    mH = MapUtil::getSingleton().getMapHeight ();
    mSamples = samples;
    mVscale = vscale;

    mElevationHeight = vscale* 256;

    Real *h = mHeightMapData;
    uint size = mW * mH;
    hMap = new Real[size];
    Real *dest = hMap;
    Real *src = h;
    for (uint i = 0; i < size; i++)
    {
        *dest++ = *src / 255 * mElevationHeight / step; 
        src += 1;
    }

    std::cout << "Calculating horizon map" << "\n";

    mHorizon.reserve (mSamples);
    double rotationAmount = 2 * Math::PI / mSamples;
    double rotationAmountSum = 0;
    for (uint k = 0; k < mSamples; k++)
    {
        std::cout << k << " : ";


        uchar *horizonmap2d = new uchar[size];
        create_layer (horizonmap2d,
                        Vector3 (Math::Sin (rotationAmountSum),
                                 Math::Cos (rotationAmountSum),
                                 0.0f));

        Image *horizonImage = new Image();
        horizonImage->loadDynamicImage  (horizonmap2d,static_cast <size_t> (mW), 
            static_cast <size_t>(mH), 1, PF_L8, true);
        mHorizon.push_back (horizonImage);
        rotationAmountSum += rotationAmount;
    }
    std::cout << "\n";
}


}//ogre namespace
