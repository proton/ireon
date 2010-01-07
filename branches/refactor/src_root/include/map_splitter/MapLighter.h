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
MapLighter.h
@brief
Utility that pre-calculate splating by
pre-calculating coverage map, A Color Map and a Base Map.
*/
#ifndef MapLighter_H
#define MapLighter_H

#include "Ogre.h"

namespace Ogre
{

/**
 * \ingroup Tool_MapSplitter
 *
 *
 * \par 
 *
 * \version 0.2
 * second version
 *
 * \date 06-07-2004
 *
 * \author tuan.kuranes@free.fr
 *
 * \todo 
 * Add New methods, faster, smaller. Add DXT5 compression to maps.
 * Add Realtime methods to modify part of a map (
 * (SubCoverageCalc)
 * adds unlimited number of splat maps ?
 *
 * \bug 
 * 
 */
class MapLighter
{
public:
    MapLighter();
    ~MapLighter();

    /**
    * \return pointer to image containing Light Map, that will be blended with
    * splat maps at real-time.
    */
    Image *getLightMap(const Vector3 &sun, float amb, float diff, 
                         int Blur);
    /**
    * \return pointer to image containing shadow Map, that will be blended with
    * splat maps at real-time.
    */
    Image *getShadowMap(const Vector3 &sun, float amb, float diff, 
                        int Blur);
    /**
    * @remarks 
    * empty all maps and prepare for a new set of calculation.
    */
    void reset ();

private :
    /*
    CalcMapLight()

    This uses the normal map and shadow map to calculate a light map for the
    landscape. A light intensity is calculated for each point in the heightmap,
    scaled to the range 0 to 255 and stored as an unsigned char. We then apply
    a blurring filter to soften the shadow edges.
    */
    void CalcMapLight (const Vector3 &sun, float amb, float diff, int sigma);

    /*
    CalcMapShadow()

    For each point in the heightmap we cast a ray in the light direction. Any 
    points intersected in x-y which are below the ray in z are deemed to be in
    shadow.
    */
    void CalcMapShadow (const Vector3 &sun);

    bool mAlreadyComputed;

    Image mLightMap;  
    Image mShadowMap;
};
}
#endif //MapLighter_H

