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
MapMergeModulater.h
@brief
Utility that split big map in tiles, pre-calculate splatting by
pre-calculating coverage map and pre-calculate normals.
*/
#ifndef MapMergeModulater_H
#define MapMergeModulater_H


#include "Ogre.h"
#include "OgreImage.h"
#include "OgreString.h"
#include "OgreSingleton.h"


#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeData2D.h"

namespace Ogre
{

/**
 * \ingroup Tool_MapMergeModulater
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
class MapMergeModulater
{
    public :
        MapMergeModulater();
        ~MapMergeModulater();

        /**
        * put gray data as alpha in an RGBA image, therefore copying the 
        * RGB data of theRGB image.
        * (Adding heightmap in normal map... for example)
        * \param *GrayImage 
        * \param *RgbImage
        * \return Rgba Image
        */
        Image* MergeGrayAndRgbInRgba (Image *GrayImage, Image *RgbaImage);
        /**
        * Merge Gray And RGBA in an RGBA image
        * replacing the RGB data of the RGBA image
        * (adding light on a Alphamap for example...)
        * \param *GrayImage 
        * \param *RgbImage 
        */
        void MergeGrayAndRgbaInRgba (Image *GrayImage, Image *RgbaImage);
        /**
        * Merge Alpha And RGB in an RGBA image
        * (adding alpha on a black image for example...)
        * \param *AlphaImage 
        * \param *RgbImage 
        */
        Image *MergeAlphaAndRgbInRgba(Image *AlphaImage, Image *RgbImage);
        /**
        * Modulate Gray And RGB in an RGB image
        * (adding light on a colormap for example...)
        * \param *GrayImage 
        * \param *RgbImage 
        */
        void ModulateGrayAndRgbInRgb( Image *GrayImage, Image *RgbImage, 
            uint factor);
        /**
        * Modulate Gray And RGBA in an RGBA image
        * (adding light on a basemap for example...)
        * \param *GrayImage 
        * \param *RgbaImage 
        */
        void ModulateGrayAndRgbaInRgba (Image *GrayImage, Image *RgbaImage, 
            uint factor);
};
}
#endif //MapMergeModulater_H
