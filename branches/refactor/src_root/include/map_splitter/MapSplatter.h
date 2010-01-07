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
MapSplatter.h
@brief
Utility that pre-calculate splatting by
pre-calculating coverage map, A Color Map and a Base Map.
*/
#ifndef MapSplatter_H
#define MapSplatter_H

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
class MapSplatter
{
public:
    MapSplatter();
    ~MapSplatter();

    /**
    * \return pointer to image containing Base Map, that will be blend with
    * splat maps at realtime.
    */
    Image *getBaseMap();
    /**
    * \return pointer to image containing a Color Map that can be use to 
    * Map a whole Tile
    */
    Image *getColorMap();
    /**
    * \return pointer to image containing Coverage Map, each color channel
    * containing alpha channel for splating a particular Map
    */
    Image *getCoverageMap();
    /**
    * \return pointer to image containing Alpha Map that
    * splats a particular Map
    */
    Image *MapSplatter::getAlphaMap(uint i);

    /**
    * \return pointer to image containing RGB Map that
    * splats a particular Map
    */
    Image *MapSplatter::getRGBMap(uint i);
    /**
    * @remarks 
    * empty all maps and prepare for a new set of calculation.
    */
    void reset ();

private :

    void CalcMapSplats ();

    bool mAlreadyComputed;

    Image BaseMap;   //Base map
    Image ColorMap;    //color map
    Image SplatMap[4]; // source Splats Maps
    Image destAlphaMap[4]; //destination Alpha Maps
    Image destRGBMap[4]; //destination RGB Maps
    Image CoverageMap; // Synthesise the 4 alpha map as one image
    ColourValue matColor[4];// average color value of splats map
    Real matHeight[1]; // Height value upon which splat is chosen
    Vector3 mScale; // landscape scale value
    uint mWidth; // landscape width value
    uint mHeight; // landscape height value
    /**
     *
     * \param out Base Color Value for this point
     * \param x Position on map
     * \param z Position on map
     * \param alpha1 factor of predominance of splat-map 1 at this point
     * \param alpha2 factor of predominance of splat-map 2 at this point
     * \param alpha3 factor of predominance of splat-map 3 at this point
     * \param alpha4 factor of predominance of splat-map 4 at this point
     */
    void BuildPoint(ColourValue& out, int x, int z, Real& alpha1, 
    Real& alpha2, Real& alpha3, Real& alpha4);

    /**
     *
     * \param Base Color Value returned
     * \param percentaje of color 1 to interpolate to color 2
     * \param index1 index of splat maps average color to interpolate with
     * \param index2 index of splat maps average color to interpolate with
     */
    void InterpolateColour(ColourValue& out, const Real percentaje, 
                        const int index1, const int index2);  

    /**
     *
     * \param alpha1 factor of predominance of splat-map 1 at this point
     * \param alpha2 factor of predominance of splat-map 1 at this point 
     * \param alpha3 factor of predominance of splat-map 1 at this point 
     * \param alpha4 factor of predominance of splat-map 1 at this point 
     * \param percentaje percentaje of  alpha 1 to interpolate to  alpha 2 
     * \param index1 index of splat maps alpha to interpolate 
     * \param index2 index of splat maps alpha to interpolate 
     */
    void InterpolateAlpha(Real& alpha1, Real& alpha2, Real& alpha3, 
                        Real& alpha4, const Real percentaje, 
                        const int index1, const int index2);
};
}
#endif //MapSplatter_H

