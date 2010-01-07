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
MapBlender.cpp
@brief
Utility that split big map in tiles, pre-calculate splatting by
pre-calculating coverage map and pre-calculate normals.
*/

#include "MapUtil.h"
#include "MapBlender.h"


namespace Ogre
{
    //-----------------------------------------------------------------------
    MapBlender::MapBlender()
    {
    }
    //-----------------------------------------------------------------------
    MapBlender::~MapBlender()
    { 
    }
    //-----------------------------------------------------------------------
    void MapBlender::blendColor (uchar *DataDest, uchar *DataA, Real FactorA, 
                                                  uchar *DataB, Real FactorB)
    {
        for (uint curr_bpp = 0; curr_bpp < mBpp; curr_bpp++)
        {      
            DataDest[curr_bpp] = static_cast <uchar>  (static_cast <uint> (DataA[curr_bpp]) * FactorA + 
                                        static_cast <uint> (DataB[curr_bpp]) * FactorB);
        }
    }
    //-----------------------------------------------------------------------
    Image *MapBlender::blend (Image *AImage, Image *BImage)
    {
        uchar *DataA = AImage->getData();
        uchar *DataB = BImage->getData();
        mwidth = static_cast <uint> (AImage->getWidth ());
        mheight = static_cast <uint> (AImage->getHeight ()); 
        mBpp = PixelUtil::getNumElemBytes (AImage->getFormat ());

        if ((PixelUtil::getNumElemBytes (BImage->getFormat ()) !=  
             PixelUtil::getNumElemBytes (AImage->getFormat ())) ||
            mwidth != BImage->getWidth () ||
            mheight != BImage->getHeight ())
        {
            String err = "Error: Invalid parameters, No way to Blend";
            OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, err, "MapBlender::blend" );
        }
        uchar *FinalData = new uchar[mwidth * mheight * mBpp];
        uint row_count = 0;
        const uint image_width_size = mwidth * mBpp;
        const uint bpp = static_cast <uint> (mBpp);
        for (uint y = 0; y < mheight; y++)	
        {
            uint col_count = row_count;
            for (uint x = 0; x < mwidth; x++)
            {
                blendColor (&FinalData[col_count], 
                            &DataA[col_count], 0.5f,
                            &DataB[col_count], 0.5f);
                col_count += bpp;  
            }
            row_count += image_width_size;
        }
        Image *FinalImage = new Image();
        PixelFormat pxlfmt;
        switch  (mBpp)
        {
            case 1:
                pxlfmt = PF_A8;
                break;
            case 3:
                pxlfmt = PF_BYTE_RGB;
                break;
            case 4:
                pxlfmt = PF_BYTE_RGBA;
                break;
        }    
        FinalImage->loadDynamicImage  (FinalData, static_cast <ushort>(mwidth), 
            static_cast <ushort>(mheight), 1, pxlfmt, true); 
        return FinalImage;
    }

 
}//namespace Ogre
