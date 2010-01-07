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
MapHorizon3.h
@brief

http://www.dgp.toronto.edu/people/JamesStewart/papers/egwr97.html
there's a link to fast horizon computing

*/
#ifndef MapHorizon3_H
#define MapHorizon3_H

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
    class MapHorizon3
    {
    public:
        MapHorizon3();
        ~MapHorizon3();

        void calcHorizonMap ();
        Image *getHorizonMap();

        Image *getLightMap();
        void calcLightMap();

    private:
      
        uint mW;
        uint mH;

        Image *mHorizonImage;
        Image *mLightImage;

        Real *mShadowMapDatapos;
        Real *mShadowMapDataneg;
        Real *mHeightMapData;
    };
}
#endif //MapHorizon3_H
