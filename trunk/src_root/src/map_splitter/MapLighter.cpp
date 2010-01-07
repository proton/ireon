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
MapLighter.cpp
@brief
Utility that pre-calculate light map.
*/
#include "MapUtil.h"
#include "MapLighter.h"
#include "MapBlurrer.h"
#include "OgrePagingLandScapeOptions.h"

#include <iostream>

namespace Ogre
{
//----------------------------------------------------------------------
MapLighter::MapLighter()
{
    reset();
}
//----------------------------------------------------------------------
MapLighter::~MapLighter()
{
   
}
//----------------------------------------------------------------------
void MapLighter::reset()
{
    mAlreadyComputed = false;
}
//----------------------------------------------------------------------
Image *MapLighter::getShadowMap(const Vector3 &sun, float amb, float diff, 
                                int Blur)
{
    if (mAlreadyComputed)
        return &mShadowMap;

    CalcMapShadow (sun);
    CalcMapLight (sun, amb, diff, Blur);
    mAlreadyComputed = true;

    return &mShadowMap;
}
//----------------------------------------------------------------------
Image *MapLighter::getLightMap(const Vector3 &sun, float amb, float diff, 
                               int Blur)
{
    if (mAlreadyComputed)
        return &mLightMap;

    CalcMapShadow (sun);
    CalcMapLight (sun, amb, diff, Blur);

    mAlreadyComputed = true;
    return &mLightMap;
}



//----------------------------------------------------------------------
void MapLighter::CalcMapShadow (const Vector3 &sun)
{
    uint  height = MapUtil::getSingleton().getMapHeight ();
    uint  width = MapUtil::getSingleton().getMapWidth ();
    Real *heightMap = MapUtil::getSingleton().getHeightData ();

    std::cout << "Shadow Map Calc : ";
	std::cout << "Dir: " << sun.x << "," << sun.y << "," << sun.z << std::endl;

	/* Allocate space for the shadow map */
    const uint size = width * height;
    uchar *ShadowMap = new uchar[size];
    memset (ShadowMap, 0, size * sizeof (uchar));
    // Assign the color texture
    mShadowMap.loadDynamicImage  (ShadowMap, 
        static_cast <size_t> (width), 
        static_cast <size_t> (height), 1, 
        PF_L8, true);

    /* Make sure the light source is normalised */
    const Vector3 LightDirection = sun.normalisedCopy();

//    LightDirection.x /= PagingLandScapeOptions::getSingleton().scale.x;
//    LightDirection.y /= PagingLandScapeOptions::getSingleton().scale.y;
//    LightDirection.z /= PagingLandScapeOptions::getSingleton().scale.z;


    if (LightDirection.y == 0.0f ) 
    {
        /* light vector horizontal*/
        /* all in shadow */
        return;
    }

    /* For each heightmap location */ 
    uint index_pixel = 0;
    for (uint j = 0; j < height; j++) 
     { 
        DEBUG_PROGRESS_OUTPUT(".")
        for (uint i = 0; i < width; i++) 
        {
            /* If already in shadow no need to compute again ?
             what if we store precise height at which it is not shadowed ?*/
            if (ShadowMap[index_pixel] == 0)
            {
                /* a line passing starting from heightmap point 
                    and following light direction*/
                Real terrain_height = heightMap[ index_pixel ];
                    
                if (fabs(LightDirection.x) < fabs(LightDirection.z))
                { 
                    int index_z = 1;
                    float index_x = LightDirection.x / LightDirection.z;
                    float index_y = LightDirection.y / LightDirection.z;
                    if (LightDirection.z < 0) 
                    {
                        index_x = -index_x; 
                        index_y = -index_y;
                        index_z = -index_z; 
                    }
                    int z = j + index_z;
                    float x = i + index_x;
                    terrain_height += index_y;
                    uint zmap = ((uint)z);
                    uint xmap = ((uint)x);
                    while  (//terrain_height >= 0.0f &&
                            xmap < width &&
                            zmap < height)
                    {        
                        if ((zmap < height) && 
                             heightMap[xmap + zmap*width] <= terrain_height)
                             ShadowMap[xmap + zmap*width] = 255; 
                    
                        x += index_x;
                        z += index_z;
                        terrain_height += index_y;

                        zmap = ((uint)z);
                        xmap = ((uint)x);
                     }
                }
                else 
                {
                    int index_x = 1;
                    float index_z = LightDirection.z / LightDirection.x;
                    float index_y = LightDirection.y / LightDirection.x;
                    if (LightDirection.x < 0) 
                    {
                        index_x = -index_x;  
                        index_y = -index_y;
                        index_z = -index_z;
                    }

                    float z = j + index_z;
                    int x = i + index_x;
                    terrain_height += index_y;
                    uint zmap = ((uint)z);
                    uint xmap = ((uint)x);
                    while (//terrain_height >= 0.0f &&
                            xmap < width &&
                            zmap < height)
                    {
                        if ((zmap < height) && 
                            heightMap[xmap + zmap*width] <= terrain_height)
                            ShadowMap[xmap + zmap*width] = 255;   
                        x += index_x; 
                        z += index_z;
                        terrain_height += index_y;
                        zmap = ((uint)z);
                        xmap = ((uint)x);
                    }
                 }
            }
            index_pixel++;
        }
    }

    std::cout << "\n";
}


//----------------------------------------------------------------------
void MapLighter::CalcMapLight(const Vector3 &sun, float amb, float diff, 
                              int Blur)
    {
    Vector3 LightDirection;
    uchar *ShadowMap = mShadowMap.getData ();
    uint  height = MapUtil::getSingleton().getMapHeight ();
    uint  width = MapUtil::getSingleton().getMapWidth ();

    std::cout << "light Map Calc : ";

    // Create the base image
    const uint size = width * height;
    uchar *LightMapData = new uchar[size];
    memset (LightMapData, 0, size * sizeof (uchar));    
    // Assign the color texture
    mLightMap.loadDynamicImage (LightMapData, 
        static_cast <size_t> (width), 
        static_cast <size_t> (height), 
        1, 
        PF_L8, true);

    /* Make sure the light source is normalised */
    LightDirection = sun;  

    //LightDirection.x /= PagingLandScapeOptions::getSingleton().scale.x;
    //LightDirection.y /= PagingLandScapeOptions::getSingleton().scale.y;
    //LightDirection.z /= PagingLandScapeOptions::getSingleton().scale.z;

    LightDirection.normalise ();
    LightDirection = LightDirection * -1.0f;

    const uchar pixelfactor = 255;
    const Real scaleShadow = 1.0f / 255;
    uint index_pixel = 0;
    /* For each point */
    for (uint j = 0; j < height; ++j) 
     {
        /* Show progress */
        DEBUG_PROGRESS_OUTPUT(".")
        for (uint i = 0; i < width; ++i) 
        {
            /* Get the dot product with the light source direction */
            const Vector3 normal = MapUtil::getSingleton ().getNormalAt (i, j);
            float d = LightDirection.dotProduct (normal);
            if (d < 0.0f)
                d = 0.0f;

            /* Calculate illumination model (ambient plus diffuse) */
            const float s = 1.0f - scaleShadow * ShadowMap[index_pixel];
            float l = amb + s * d * diff;
            if (l < 0.0f)
                l = 0.0f;
            else 
                if (l > 1.0f)
                l = 1.0f;

            /* Save in light map */
            LightMapData[index_pixel] = static_cast <uchar> (l * pixelfactor);
            index_pixel++;
        }
     }

    std::cout << "\n";
    /* Blur the light map */  
    if (Blur > 0)
    {
        MapBlurrer Blurrer;
        Blurrer.gaussianBlur(&mLightMap, Blur);
    }

}
}//namespace Ogre
