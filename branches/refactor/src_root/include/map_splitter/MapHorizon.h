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
MapHorizon.h
@brief

http://www.dgp.toronto.edu/people/JamesStewart/papers/egwr97.html
there's a link to fast horizon computing

*/
#ifndef MapHorizon_H
#define MapHorizon_H

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
    class MapHorizon
    {
        public:
            MapHorizon();
            ~MapHorizon();

            /**
            * @remarks 
            * empty all maps and prepare for a new set of calculation.
            */
            void reset ();

            Image *getElevationCubeMap(uint sample);
            Image *getHorizon3DMap(uint sample);

            void calcHorizon3DMap (Real vscale, uint samples, float step);

            void calcElevationCubeMap (uint size);

        private:

            uint CalcHorizonAtPoint (uint localx, uint localy, 
                                    Vector3 direction, Real origin);

            Vector3 GetCubeVector(int side, uint x, uint y);

            Real mVscale;
            uint mSamples;
            uint mElevationSize;
            Real mElevationHeight; // max height on the heightmap
            uint mW;
            uint mH;

            float *hMap;
            Real *mHeightMapData;
            std::vector <Image*> mHorizon;
            std::vector <Image*> mElevation;

    };
}
#endif //MapHorizon_H
