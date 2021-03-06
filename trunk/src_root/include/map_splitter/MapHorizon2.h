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
MapHorizon2.h
@brief

http://www.dgp.toronto.edu/people/JamesStewart/papers/egwr97.html
there's a link to fast horizon computing

*/
#ifndef MapHorizon2_H
#define MapHorizon2_H

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
 *
 * \bug 
 * 
 */
class MapHorizon2
{
public:
    MapHorizon2();
    ~MapHorizon2();

     /**
    * @remarks 
    * empty all maps and prepare for a new set of calculation.
    */
    void reset ();

    // cubemap lookup
    Image *getElevationCubeMap(uint sample);
    void calcElevationCubeMap (uint size);

    // 3d texture horizon 
    Image *getHorizon3DMap(uint sample);
    void calcHorizon3DMap (Real vscale, uint samples, float step);


private:
    // 3d texture horizon 
    float height_wrap(int x, int y) ;
    void create_layer (uchar *dest, const Vector3 &dir);

    // CubeMap lookup
    Vector3 getCubeVector(int i,int size,int x,int y); 

    Real mVscale;
    uint mSamples;
    Real mElevationHeight; // max height on the heightmap
    uint mElevationSize;

    float *hMap;
    uint mW;
    uint mH;
    Real *mHeightMapData;
    std::vector <Image*> mHorizon;
    std::vector <Image*> mElevation;

};
}
#endif //MapHorizon2_H

