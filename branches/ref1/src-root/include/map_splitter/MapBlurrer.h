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
MapBlurrer.h
@brief
Utility that Blur big maps.
*/
#ifndef MapBlurrer_H
#define MapBlurrer_H


#include "Ogre.h"
#include "OgreImage.h"
#include "OgreString.h"
#include "OgreSingleton.h"


#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeData2D.h"

namespace Ogre
{

    class Convolver
    {
        private :
            int radius;
            int kernelSize;
            int *kernel;
            int **mult;
            int mSz;
            bool mInit;
        public :
            Convolver(int sz = 2);

            ~Convolver();

            void setRadius();
            void blurRgba (Image  * img);        
            void blurRgb (Image  * img);
            void blurGray (Image  * img);
            void blurReal (Real  * data, uint width, uint height);
    };

    /**
    * \ingroup Tool_MapBlurrer
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
    class MapBlurrer
    {
        public :
            MapBlurrer();
            ~MapBlurrer();

            /**
            * Blur image based on a factor
            * \param *BlurImage Image to blur 
            * \param sigma blur factor
            */
            void gaussianBlur(Image *BlurImage, int sigma);

            /**
            * Blur image based by wheighting height with neighbors
            * \param *BlurImage  Image to blur 
            */
            void AverageBlur(Image *BlurImage);
    };
}
#endif //MapBlurrer_H
