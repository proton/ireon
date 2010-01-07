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
MapVis.cpp
@brief
Utility that pre-calculate hierarchical visibility
*/
#include "MapUtil.h"
#include "MapVis.h"
#include "OgrePagingLandScapeOptions.h"

#include <iostream>

namespace Ogre
{

MapVis::MapVis()
{
    patchW = patchH = mVisDataZ = 0;
    mVisDataDZ = 0;
    mVisData = (uchar*)0;
    minData = 0;
    maxData = 0;
}


MapVis::~MapVis()
{
    if(mVisData)
        delete mVisData;
    if (minData)
        delete minData;
    if (maxData)
        delete maxData;
}

void MapVis::CalcMinMax()
{
    if (minData)
        delete minData;
    minData = new uchar[patchW*patchH];

    if(maxData)
        delete maxData;
    maxData = new uchar[patchW*patchH];

    uint x0_current = 0;
    uint y0_current = 0;
    for (uint x0 = 0; x0 < patchW; x0++)
    { 
        uint row_current = 0;
        for(uint y0 = 0; y0 < patchH; y0++)
        {
            uint min = 255, max = 0;

            for (uint x1 = x0_current; x1 <= x0_current + 16; x1++)
            {
                for (uint y1 = y0_current; y1 <= y0_current + 16; y1++)
                {
                    uint terrain_height = uint (MapUtil::getSingleton ().getHeight (x1, y1));
                    if (terrain_height < min)
                        min = terrain_height;
                    if (terrain_height > max)
                        max = terrain_height;
                }
            }

            minData[x0 + row_current] = min;
            maxData[x0 + row_current] = max;
            y0_current += 16;
            row_current =+ patchW;
        }
        x0_current += 16;
     }
       
}
void MapVis::calc (uint nZ, uint nDZ)
{
    mVisDataW = MapUtil::getSingleton().getMapWidth ();
    mVisDataH = MapUtil::getSingleton().getMapHeight ();

    patchW = (mVisDataW-1) / 16;
    patchH = (mVisDataH-1) / 16;

    CalcMinMax();

    mVisDataZ = nZ;
    mVisDataDZ = nDZ;

    mVisDataXY = patchW * patchH; 
    mVisDataX2Y = mVisDataXY * patchW;
    mVisDataX2Y2 = mVisDataX2Y * patchH;

    uint	numData = mVisDataZ * mVisDataX2Y2;
    uint    dataCount = 0;
    uint   dstX, dstY, srcX, srcY, srcZ, srcZC;


    mVisDataSize = (numData+7)/8;			// +7 => round upwards
    mVisData = new uchar[mVisDataSize+1];
    memset(mVisData, 0, mVisDataSize);

    std::cout << "Visibility Map Calc : \n";

    srcZ = mVisDataDZ - 1;
    for(srcZC = 0; srcZC < mVisDataZ; srcZC++)
    {

        for (srcY = 0; srcY < patchH; srcY++)
        {
            DEBUG_PROGRESS_OUTPUT(".")
            for (srcX = 0; srcX < patchW; srcX++)
            {
                uint row_widthYpatchW = 0;

                for( dstY = 0; dstY < patchH; dstY++)
                {
                    for (dstX = 0; dstX < patchW; dstX++)
                    {
                        if (srcZ > 500)
                            srcZ = srcZ;

                        uchar	hMax = maxData[dstX + row_widthYpatchW];

                        float	p;   
                        float	dx = (float) dstX - srcX;
                        float	dy = (float) dstY - srcY;
                        float	dz = (float) hMax - srcZ;

                        float	stepf = 1.0f / 
                            (float)sqrt((long double) ((dx)*(dx) + (dy)*(dy)));


                        setVis (srcX, srcY, srcZC, 
                                    dstX, dstY, 
                                    true);

                        for (p = 0.0f; p <= 1.0f; p += stepf)
                        {
                            int x = (int) (srcX + dx*p);
                            int y = (int) (srcY + dy*p);
                            int z = (int) (srcZ + dz*p);

                            if (z < minData[x + y*patchW])
                            {
                                //vis = 0;
                                setVis (srcX, srcY, srcZC, 
                                            dstX, dstY, 
                                            false);
                                break;
                            }
                        }
                        dataCount++;
                    }
                    row_widthYpatchW += patchW;
                }
            }
        }
        srcZ += mVisDataDZ;
        std::cout << "\n";
    }
    std::cout << "Done\n";
}



void MapVis::setVis(int nX0, int nY0, int nZ0, 
                        int nX1, int nY1, 
                        bool nValue)
{
    uint idx = nZ0 * mVisDataX2Y2 + 
               nY0 * mVisDataX2Y + 
               nX0 * mVisDataXY + 
               nY1 * patchW + 
               nX1;

    int	 midx = idx >> 3;
    int  sidx = idx & 7;

    uchar	mask = (uchar)(1 << sidx);

    if(nValue)
        mVisData[midx] |= mask;
    else
        mVisData[midx] &= ~mask;
}


bool MapVis::isVis(int nX0, int nY0, int nZ0, 
                       int nX1, int nY1) const
{
    if (nX0 < 0 || nX0 >= (signed)patchW || 
        nY0 < 0 || nY0 >= (signed)patchH || 
        nZ0 < 0 || nZ0 >= (signed)mVisDataZ ||
        nX1 < 0 || nX1 >= (signed)patchW || 
        nY1 < 0 || nY1 >= (signed)patchH)//patchW?
        return true;

    uint	idx = nZ0 * mVisDataX2Y2 + 
                  nY0 * mVisDataX2Y + 
                  nX0 * mVisDataXY + 
                  nY1 * patchW + 
                  nX1;
    int	midx = idx>>3;
    int	sidx = idx&7;
    uchar	mask = (uchar)(1 << sidx);

    return (mVisData[midx] & mask) != 0;
}


bool MapVis::save(const char* nFileName)
{
    if(!mVisData)
        return false;

   std::ofstream outfile;

    outfile.open (nFileName, std::ios::binary);

    outfile << patchW;
    outfile << patchH;
    outfile << mVisDataZ;
    outfile << mVisDataDZ;

    outfile.write ( reinterpret_cast<char*> (mVisData), 
                    mVisDataSize);

    outfile.close ();

    return true;
}


}//ogre namespace
