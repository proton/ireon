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
MapMergeModulater.cpp
@brief
Utility that split big map in tiles, pre-calculate splatting by
pre-calculating coverage map and pre-calculate normals.
*/

#include "MapUtil.h"
#include "MapMergeModulater.h"

#include <iostream>
#include <fstream>
#include <vector>

#include "OgreImageCodec.h"
#include "OgreILCodecs.h"


#include "fileutils.h"


namespace Ogre
{
    //-----------------------------------------------------------------------
    MapMergeModulater::MapMergeModulater()
    {
    }
    //-----------------------------------------------------------------------
    MapMergeModulater::~MapMergeModulater()
    { 
    }
    //-----------------------------------------------------------------------
    void MapMergeModulater::ModulateGrayAndRgbInRgb(Image *GrayImage, Image *RgbImage, 
                                uint factor)
    {
        uchar *DataRGB = RgbImage->getData();
        uchar *DataGray = GrayImage->getData();
        const uint image_width = static_cast <uint> (RgbImage->getWidth ());
        const uint image_height = static_cast <uint> (RgbImage->getHeight ()); 

        if (PixelUtil::getNumElemBytes (GrayImage->getFormat ()) != 1  ||
            PixelUtil::getNumElemBytes (RgbImage->getFormat ()) != 3 ||
            image_width != GrayImage->getWidth () ||
            image_height != GrayImage->getHeight ())
        {
                String err = "Error: Invalid parameters, No way to modulate";
                OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, err, "MapMergeModulater::ModulateGrayAndRgbInRgb" );
        }
        uint row_count_rgb = 0;
        uint row_count_gray = 0;
        const uint image_width_rgb_size = image_width * 3;
        const Real modulate = (Real) factor / 255.0f;
        for (uint j = 0; j < image_height; j++)
        {   
            uint col_count_rgb = row_count_rgb;
            for (uint i = 0; i < image_width; i++)
            {               
                const Real Gray = DataGray[row_count_gray + i] * modulate;

                Real Color = DataRGB[ col_count_rgb] * Gray;
                if (Color > 255.0f)
                    Color = 255.0f;
                DataRGB[col_count_rgb] = static_cast <uchar>(Color);

                Color = DataRGB[ col_count_rgb + 1] * Gray;
                if (Color > 255.0f)
                    Color = 255.0f;
                DataRGB[col_count_rgb + 1] = static_cast <uchar>(Color);

                Color = DataRGB[ col_count_rgb + 2]  * Gray;
                if (Color > 255.0f)
                    Color = 255.0f;
                DataRGB[col_count_rgb + 2] = static_cast <uchar>(Color);

                col_count_rgb += 3;
            }
            row_count_gray += image_width;
            row_count_rgb += image_width_rgb_size;
        }
    }
    //-----------------------------------------------------------------------
    void MapMergeModulater::ModulateGrayAndRgbaInRgba (Image *GrayImage, Image *RgbaImage, 
                                                    uint factor)
    {
        uchar *DataRGBA = RgbaImage->getData();
        uchar *DataGray = GrayImage->getData();
        const uint image_width = static_cast <uint> (RgbaImage->getWidth ());
        const uint image_height = static_cast <uint> (RgbaImage->getHeight ()); 

        if (PixelUtil::getNumElemBytes (GrayImage->getFormat ()) != 1  ||
            PixelUtil::getNumElemBytes (RgbaImage->getFormat ()) != 4 ||
            image_width != GrayImage->getWidth () ||
            image_height != GrayImage->getHeight ())
            {
                String err = "Error: Invalid parameters, No way to modulate";
                OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, err, "MapMergeModulater::ModulateGrayAndRgbInRgb" );
            }

        uint row_count_rgba = 0;
        uint row_count_gray = 0;
        const uint image_width_rgba_size = image_width * 4;
        const Real modulate = (Real) factor / 255.0f;
        for (uint j = 0; j < image_height; j++)
        {   
            uint col_count_rgba = row_count_rgba;
            for (uint i = 0; i < image_width; i++)
            {               
                const Real Gray = DataGray[row_count_gray + i] * modulate;

                Real Color = DataRGBA[ col_count_rgba] * Gray;
                if (Color > 255.0f)
                    Color = 255.0f;
                DataRGBA[col_count_rgba] = static_cast <uchar>(Color);

                Color = DataRGBA[ col_count_rgba + 1] * Gray;
                if (Color > 255.0f)
                    Color = 255.0f;
                DataRGBA[col_count_rgba + 1] = static_cast <uchar>(Color);

                Color = DataRGBA[ col_count_rgba + 2]  * Gray;
                if (Color > 255.0f)
                    Color = 255.0f;
                DataRGBA[col_count_rgba + 2] = static_cast <uchar>(Color);

                col_count_rgba += 4;
            }
            row_count_gray += image_width;
            row_count_rgba += image_width_rgba_size;
        }

    }
    //-----------------------------------------------------------------------
    void MapMergeModulater::MergeGrayAndRgbaInRgba (Image *GrayImage, Image *RgbaImage)
    {
        uchar *DataRGBA = RgbaImage->getData();
        uchar *DataGray = GrayImage->getData();
        const uint image_width = static_cast <uint> (RgbaImage->getWidth ());
        const uint image_height = static_cast <uint> (RgbaImage->getHeight ()); 

        if (PixelUtil::getNumElemBytes (GrayImage->getFormat ()) != 1  ||
            PixelUtil::getNumElemBytes (RgbaImage->getFormat ()) != 4 ||
            image_width != GrayImage->getWidth () ||
            image_height != GrayImage->getHeight ())
         {
            String err = "Error: Invalid parameters, No way to Merge";
            OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, err, "MapMergeModulater::MergeGrayAndRgbaInRgba" );
        }

        uint row_count_rgba = 0;
        uint row_count_gray = 0;
        const uint image_width_rgba_size = image_width * 4;
        for (uint j = 0; j < image_height; j++)
        {   
            uint col_count_rgba = row_count_rgba;
            for (uint i = 0; i < image_width; i++)
             {
                DataRGBA[col_count_rgba] = DataGray[row_count_gray + i];
                DataRGBA[col_count_rgba + 1] = DataGray[row_count_gray + i];
                DataRGBA[col_count_rgba + 2] = DataGray[row_count_gray + i];

                col_count_rgba += 4;
            }
            row_count_rgba += image_width_rgba_size;
            row_count_gray += image_width;
        }
    }
    //-----------------------------------------------------------------------
    Image *MapMergeModulater::MergeGrayAndRgbInRgba(Image *GrayImage, Image *RgbImage)
    {
        uchar *DataRGB = RgbImage->getData();
        uchar *DataGray = GrayImage->getData();
        const uint image_width = static_cast <uint> (RgbImage->getWidth ());
        const uint image_height = static_cast <uint> (RgbImage->getHeight ()); 

        if (PixelUtil::getNumElemBytes (GrayImage->getFormat ()) != 1  ||
            PixelUtil::getNumElemBytes (RgbImage->getFormat ()) != 3 ||
            image_width != GrayImage->getWidth () ||
            image_height != GrayImage->getHeight ())
        {
            String err = "Error: Invalid parameters, No way to merge";
            OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, err, "MapMergeModulater::MergeGrayAndRgbInRgba" );
        }


        uchar *DataRGBA = new uchar[ image_width*image_height*4 ];   
  DataRGBA = new uchar[image_width*image_height];
        uint row_count_rgb = 0;
        uint row_count_rgba = 0;
        uint row_count_gray = 0;
        const uint image_width_rgb_size = image_width * 3;
        const uint image_width_rgba_size = image_width * 4;
        for (uint j = 0; j < image_height; j++)
        {   
            uint col_count_rgb = row_count_rgb;
            uint col_count_rgba = row_count_rgba;
            for (uint i = 0; i < image_width; i++)
            {               
                DataRGBA[col_count_rgba++] = DataRGB[col_count_rgb++];
                DataRGBA[col_count_rgba++] = DataRGB[col_count_rgb++];
                DataRGBA[col_count_rgba++] = DataRGB[col_count_rgb++];

                DataRGBA[col_count_rgba++] = DataGray[row_count_gray + i];

            }
            row_count_gray += image_width;
            row_count_rgb += image_width_rgb_size;
            row_count_rgba += image_width_rgba_size;
        } 

        Image *RgbaImage = new Image();
        RgbaImage->loadDynamicImage (DataRGBA, static_cast <size_t> (image_width),
                                     static_cast <size_t> (image_height), 1, PF_BYTE_RGBA, true); 
        
        return RgbaImage;
    }
    //-----------------------------------------------------------------------
    Image *MapMergeModulater::MergeAlphaAndRgbInRgba(Image *AlphaImage, Image *RgbImage)
    {
        uchar *DataRGB = RgbImage->getData();
        uchar *DataAlpha = AlphaImage->getData();
        const uint image_width = static_cast <uint> (RgbImage->getWidth ());
        const uint image_height = static_cast <uint> (RgbImage->getHeight ()); 

        if (
            PixelUtil::getNumElemBytes (AlphaImage->getFormat ()) != 1  ||
            PixelUtil::getNumElemBytes (RgbImage->getFormat ()) != 3 ||
            image_width != AlphaImage->getWidth () ||
            image_height != AlphaImage->getHeight ())
        {
            String err = "Error: Invalid parameters, No way to merge";
            OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, err, "MapMergeModulater::MergeAlphaAndRgbInRgba" );
        }

        uchar *DataRGBA = new uchar[image_width*image_height*4];

        uint row_count_rgb = 0;
        uint row_count_rgba = 0;
        uint row_count_Alpha = 0;
        const uint image_width_rgb_size = image_width * 3;
        const uint image_width_rgba_size = image_width * 4;
        for (uint j = 0; j < image_height; j++)
        {   
            uint col_count_rgb = row_count_rgb;
            uint col_count_rgba = row_count_rgba;
            for (uint i = 0; i < image_width; i++)
            {               

                DataRGBA[col_count_rgba] = DataRGB[col_count_rgb];
                DataRGBA[col_count_rgba + 1] = DataRGB[col_count_rgb + 1];
                DataRGBA[col_count_rgba + 2] = DataRGB[col_count_rgb + 2];

                DataRGBA[col_count_rgba + 3] = DataAlpha[row_count_Alpha + i];

                col_count_rgb += 3;
                col_count_rgba += 4;
            }
            row_count_Alpha += image_width;
            row_count_rgb += image_width_rgb_size;
            row_count_rgba += image_width_rgba_size;
        } 
        
        Image *RgbaImage = new Image();
        RgbaImage->loadDynamicImage  (DataRGBA, static_cast <size_t> (image_width), 
            static_cast <size_t> (image_height), 1, PF_BYTE_RGBA, true); 
        return RgbaImage;
    }
   
}//namespace Ogre
