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
MapBlender.h
@brief
Utility that split big map in tiles, pre-calculate splatting by
pre-calculating coverage map and pre-calculate normals.
*/
#ifndef MapBlender_H
#define MapBlender_H


#include "Ogre.h"
#include "OgreImage.h"
#include "OgreString.h"
#include "OgreSingleton.h"


#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeData2D.h"

namespace Ogre
{

/**
 * \ingroup Tool_MapBlender
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
class MapBlender
{
    public :
        MapBlender();
        ~MapBlender();


        /**
        * Blend uniformely two map
        * \param *AImage 
        * \param *BImage 
        * \return the blended image
        */
        Image *blend (Image *AImage, Image *BImage);

    private :
        /**
         *
         * \param *Dest     where to store result
         * \param *A        Color to blend
         * \param FactorA   blend factor color
         * \param *B        Color to blend
         * \param FactorB   blend factor color
         */
        void blendColor (uchar *Dest, uchar *A, Real FactorA, 
                                 uchar *B, Real FactorB);
       
        size_t mBpp;
        uint mwidth;
        uint mheight;
};
}
#endif //MapBlender_H
