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
MapHorizon.cpp
@brief
Utility that pre-calculate horizon mapping for real-time
light mapping of terrain
*/
#include "MapUtil.h"
#include "MapHorizon.h"

#include "OgrePagingLandScapeOptions.h"

#include <iostream>

namespace Ogre
{

    MapHorizon::MapHorizon()
    {   
        hMap = 0;
    }
    MapHorizon::~MapHorizon()
    {
        if(!mHorizon.empty())
        { 
            for (uint k = 0; k < mSamples; k++) 
            {
                    delete mHorizon[k];
            }
        }
        mHorizon.clear ();

        if(!mElevation.empty())
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
    Image *MapHorizon::getElevationCubeMap(uint sample)
    {
        return mElevation[sample];
    }
    Image *MapHorizon::getHorizon3DMap(uint sample)
    {
        return mHorizon[sample];
    }

    Vector3 MapHorizon::GetCubeVector(int side, uint x, uint y)
    {
        Real s, t, sc, tc;

        s = (x + 0.5) / mElevationSize;
        t = (y + 0.5) / mElevationSize;
        sc = s*2 - 1;
        tc = t*2 - 1;

        Vector3 CubeVector;
        switch (side) 
        {
            case 0: //CUBE_MAP_POSITIVE_X
                CubeVector = Vector3(1, -tc, -sc);
                break;
            case 1: //CUBE_MAP_NEGATIVE_X
                CubeVector = Vector3(-1, -tc, sc);
                break;
            case 2: //CUBE_MAP_POSITIVE_Y
                CubeVector = Vector3(sc, 1, tc);
                break;
            case 3: //CUBE_MAP_NEGATIVE_Y
                CubeVector = Vector3(sc, -1, -tc);
                break;
            case 4: //CUBE_MAP_POSITIVE_Z
                CubeVector = Vector3(sc, -tc, 1);
                break;
            case 5: //CUBE_MAP_NEGATIVE_Z
                CubeVector = Vector3(-sc, -tc, -1);
                break;
        }

        CubeVector.normalise ();
        return CubeVector;
    }

    void MapHorizon::calcElevationCubeMap(uint size)
    {
        mElevationSize = size;
        mElevation.reserve (mElevationSize);

        std::cout << "Calculating Elevation map";

        uint eBpp = 3;   //2 only are used...


        uchar *pixels = new uchar[mElevationSize * mElevationSize * eBpp];
        for (uint i = 0; i < 6; i++) 
        {
            //ui3 CubeFace = CubeFaces[i];
            uint row_count = 0;
            DEBUG_PROGRESS_OUTPUT(".")
            for (uint y = 0 ; y < mElevationSize; y++)
            {
                //Real cube_t = (y + 0.5)  * twoBysSize - 1;    
                for (uint x = 0 ; x < mElevationSize; x++)
                {
                    Vector3 cubeVector(GetCubeVector(i, x, y));

                    // Calculate elevation:
                    Vector3 e0 (cubeVector.x, cubeVector.y, 0);
                    e0.normalise ();

                    Real elevation = 1 - e0.dotProduct (cubeVector);

                    // Calculate heading:
                    Real heading = Math::ACos (e0.x).valueRadians();

                    if (e0.y < 0)
                        heading = Math::PI + (Math::PI - heading);

                    // Write them to the cube map.
                    pixels[(row_count + x) * eBpp + 0] = uchar ((heading/(2*Math::PI))*255);
                    pixels[(row_count + x) * eBpp + 1] = uchar (elevation * 255);
                    if (eBpp > 2)
                        pixels[(row_count + x) * eBpp + 2] = uchar (0);
                }
                row_count += mElevationSize;
            }

            Image *elevationImage = new Image();
            elevationImage->loadDynamicImage  (pixels, static_cast <size_t>(mElevationSize), 
                static_cast <size_t>(mElevationSize),  1, 
                PF_BYTE_RGB, true);
            mElevation.push_back (elevationImage);
        }
        std::cout << "\n";
    }

    uint MapHorizon::CalcHorizonAtPoint (uint localx, 
                                        uint localy,
                                        Vector3 direction,
                                        Real origin)
    {
        // Scale the direction vector such that the largest component is 1.
        Vector3 dir (direction);

        // Trace a ray through the HeightMap 
        // and keep track of the horizon elevation. 
        int w = int (mW);
        int h = int (mH);
        int x = int (localx);
        int y = int (localy);

        // Move one step further along the ray.
        Vector3 curr_pos (x, y, 0.0f);
        curr_pos += dir; 
        int i = int (Math::Ceil (curr_pos.x));
        int j = int (Math::Ceil  (curr_pos.y));
        uint maxH = 0;
        //Real vscalefactor = vscale / 255;
        //Real originheight = origin * vscalefactor;

        while ((i >= 0) && (i < w) && 
                (j >= 0) && (j < h))
        {
            // Height of the current sample point:
            uint curH = uint (hMap[j*w + i]);

            // If the current sample point is lower than the ray origin, 
            // it can't affect   our horizon, so skip it. 
            if (curH > origin) 
            {
                // Calculate the elevation angle of the current sample point:
                Vector3 flat  (i - x, 
                                j - y, 
                                0); //originheight);
                flat.normalise ();
                Vector3 elev (i - x, 
                                j - y, 
                                //curH*vscalefactor - originheight);
                                curH - origin);
                elev.normalise ();

                // Output: [0=horizontal, 1=vertical].
                curH = int (Math::Ceil ( (1 - flat.dotProduct (elev)) * 255.0f));

                maxH = (curH < maxH)?maxH:curH;

                // If the current sample value is 255, no point beyond it can possibly be
                //   higher, so stop here. 
                if (mHeightMapData[j*w+i] == 255)  
                    i = w;
            }
            // Move one step further along the ray.
            curr_pos += dir;
            i = int (Math::Ceil (curr_pos.x));
            j = int (Math::Ceil (curr_pos.y));
        }
        return maxH;
    }


    void MapHorizon::calcHorizon3DMap(Real vscale,
                                    uint samples, 
                                    float step)
    {
        mHeightMapData = MapUtil::getSingleton().getHeightData ();
        mW = MapUtil::getSingleton().getMapWidth ();
        mH = MapUtil::getSingleton().getMapHeight ();
        mSamples = samples;
        mVscale = vscale;  

        uint w = mW;
        uint h = mH;
        uint size = w * h;

        mElevationHeight = vscale* 256;

        hMap = new Real[size];
        Real *dest = hMap;
        Real *src = mHeightMapData;
        double tmp = 1.0 / 255 * mElevationHeight / step;
        for (uint i = 0; i < size; i++) 
        {
            *dest++ = (Real)*src * tmp;
            src += 1;
        }   

        std::cout << "Calculating horizon map" << "\n";

        mHorizon.reserve (mSamples);

        double rotationAmount = 2 * Math::PI / mSamples;
        double rotationAmountSum = 0;
        
        for (uint k = 0; k < mSamples; k++) 
        {           

            uchar *horizonmap2d = new uchar[size]; 
            uint row_countj = 0; 
            std::cout << StringConverter::toString (k) << " : ";

            Vector3 dir (Math::Sin (rotationAmountSum), 
                        Math::Cos (rotationAmountSum), 
                         0.0f);
               
            dir *= 1.0f / std::max  (Math::Abs (dir.x), Math::Abs (dir.y));

            for (uint j = 0; j <  h; j++)
            {  
                DEBUG_PROGRESS_OUTPUT(".")
                for (uint i = 0; i < w; i++) 
                {
                    horizonmap2d[row_countj + i] = 
                        CalcHorizonAtPoint (i, 
                                            j, 
                                            dir, 
                                            hMap[ row_countj + i]);
                }
                row_countj += w;            
            }
            rotationAmountSum += rotationAmount;

            Image *horizonImage = new Image();
            horizonImage->loadDynamicImage  (horizonmap2d, static_cast <size_t> (w), 
                static_cast <size_t> (h), 1, PF_L8, true);
            mHorizon.push_back (horizonImage);
            std::cout << "\n";
        }
    }


}//ogre namespace
