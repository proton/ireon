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
MapInfinitizer.h
@brief
Utility that split big map in tiles, pre-calculate splatting by
pre-calculating coverage map and pre-calculate normals.
*/
#ifndef MapInfinitizer_H
#define MapInfinitizer_H


#include "Ogre.h"
#include "OgreImage.h"
#include "OgreString.h"
#include "OgreSingleton.h"


#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeData2D.h"

namespace Ogre
{

/**
 * \ingroup Tool_MapInfinitizer
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
 */
class MapInfinitizer
{
    public :
        MapInfinitizer();
        ~MapInfinitizer();

        bool Infinitize (uint BorderSize, 
            Real RandFactor, uint BlurFactor);

    private :
        /**
        *  Compare 2 color byte per byte and return true if 
        * they are differrent
        * \param *DataA 
        * \param *DataB 
        * \return 
        */
        bool colorsAreDifferrent (Real *DataA, Real *DataB);
        
        bool checkForNeed();

        void LerpLeftAndRight ();
        void lerpUpAndDown ();
        void lerpCorners ();

        void BlendLeftAndRight ();
        void BlendUpAndDown ();
        void BlendCorners ();

        /**
         *
         * \param *Dest     where to store result
         * \param *A        Color to blend
         * \param FactorA   blend factor color
         * \param *B        Color to blend
         * \param FactorB   blend factor color
         */
        void blendColor (Real *Dest, Real *A, Real FactorA, 
                                 Real *B, Real FactorB);
        /**
         * linear interpolate from A to B color and store in dest
         * \param *DataDest where to store results
         * \param *DataA to interpolate from
         * \param *DataB to interpolate to
         * \param Factor interpolation Factor
         */
        void lerpColor (Real *DataDest, Real *DataA, Real *DataB, 
                        Real Factor);    
    
        uint mBorderSize;
        uint mBorderSize2;
        uint mBlurFactor;
        Real mRandFactor;

        uint mwidth;
        uint mheight;
        Real *mData;
};
}
#endif //MapInfinitizer_H
