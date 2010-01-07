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
MapSplitter.h
@brief
Utility that split big map in tiles, pre-calculate splatting by
pre-calculating coverage map and pre-calculate normals.
*/
#ifndef MapSplitter_H
#define MapSplitter_H


#include "Ogre.h"
#include "OgreImage.h"
#include "OgreString.h"
#include "OgreSingleton.h"


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
    */
    class MapSplitter
    {
        public :
            
            MapSplitter();
            ~MapSplitter();
            /*
            * init parameters from option file
            */
            void init ();
            /**
            *
            * \param *newImage image to set as current
            */
            void setCurrentMap (Image *newImage);

            /**
            *
            * \param Filename of map to split and space
            * \param tile_spacing Size of overlapping area of each splitted tile
            * \param tile_size Size of tiles once big map splitted
            */
            void SplitAndSpaceCurrentMap(const String &filename, const int tile_spacing, 
                                        const int tile_size);
            /**
            *
            * \param Filename of map to split
            * \param tile_size Size of tiles once big map splitted
            */
            void SplitCurrentMap(const String &filename, const int tile_size);
            /**
            *
            * \param Filename of map to split
            * \param Image Image to split
            */
            void saveAndSplitMap (const String &filename, Image *newImage);
            /**
            * Split and Space it
            * \param Filename of map to split
            * \param Image Image to split
            */
            void saveAndSpaceAndSplitMap (const String &filename, Image *newImage);
            /**
            * Split and Space it
            * \param Heightdata float data to split and save
            * \param width to split
            * \param height to split
            */
            void saveAndSpaceAndSplitHeightData (const String &filename, 
                    Real *Heightdata, 
                    const uint width, const uint height);
            void saveAndSpaceAndSplitHeightData (const String &filename, 
                    const String& outDir,
                    uchar *Heightdata, 
                    const uint width, const uint height, 
                    const int tile_spacing, const int page_size,
                    const bool saveasRaw, const String &extension);
            /**
            *
            * \param filename Filename of  currentMap to save as image
            * \return 
            */
            const bool saveCurrentMap (const String &filename);

            void SplitAndSpaceHeightData (const String &filename, uchar *Heightdata,
                                                const uint width, const uint height, 
                                                const int tile_spacing, const int page_size,
                                                const bool saveasRaw, const String &extension);
            void saveAndSpaceAndSplitHeightData (const String &filename, 
                                                Real *Heightdata, 
                                                const uint width, const uint height, 
                                                const int tile_spacing, const int tile_size);

            /**
            * make sure mPageSize is 2^n 
            */
            void ComputeTileSize();
            /**
            * splitting and saving files format definition
            * \param option yes if raw is the format to use
            */
            void setRawSave (bool option);

            /**
            *
            * \param filename Filename of Map to save as image file 
            * \param data Pixel data to save
            * \param width Final width of image
            * \param height Final height of image
            * \param NumBytesPP number of Bytes per pixel
            * \return true if done.
            */
            const bool save(const String &filename, const uchar * const data, 
                            const uint width, const uint height,
                            const uint NumBytesPP);          
            /**
            *
            * \param filename Filename of Map to save as image file 
            * \param data Pixel data to save
            * \param width Final width of image
            * \param height Final height of image
            * \param NumBytesPP number of Bytes per pixel
            * \return true if done.
            */
            const bool saveExt(const String &filename, const uchar * const data, 
                                const uint width, const uint height,
                                const uint NumBytesPP);
        private :
            /**
            * copy a rect of source image into a new image.
            * \param *BaseData image data source
            * \param *OutData image data destination to copy data to
            * \param x position in image data source width
            * \param y position in image data source height
            * \param tile_width width of image data destination 
            * \param tile_height height of image data destination 
            * \param tile_width_copy width of image data source being copied
            * \param tile_height_copy height of image data source being copied
            * \param base_width width of image data source
            * \param bpp Number of Bytes Per Pixel of two image (must be the same)
            */
            void BlitSubImage (const uchar * const BaseData, 
                                uchar * const constOutData, 
                                const uint x, const uint y, 
                                const uint tile_width, const uint tile_height, 
                                const uint tile_width_copy, const uint tile_height_copy, 
                                const uint base_width, const uint bpp);   
             /**
            * copy a rect of source image into a new image.
            * \param *BaseData image data source
            * \param *OutData image data destination to copy data to
            * \param x position in image data source width
            * \param y position in image data source height
            * \param tile_width width of image data destination 
            * \param tile_height height of image data destination 
            * \param tile_width_copy width of image data source being copied
            * \param tile_height_copy height of image data source being copied
            * \param base_width width of image data source
            * \param bpp Number of Bytes Per Pixel of two image (must be the same)
            */
            void BlitAndFillSubImage (const uchar * const BaseData, 
                                    uchar * const constOutData, 
                                    const uint x, const uint y, 
                                const uint tile_width, const uint tile_height, 
                                const uint tile_width_copy, const uint tile_height_copy, 
                                const uint base_width, const uint bpp);     
   
            /**
            *
            * \param filename Filename of  currentMap to save as image
            * \return 
            */
            const bool saveCurrentMapAsExt(const String &filename);

            /**
            *
            * \param filename Filename of Map to save as image file 
            * \param data Pixel data to save
            * \param width Final width of image
            * \param height Final height of image
            * \param NumBytesPP number of Bytes per pixel
            * \return true if done.
            */
            const bool saveRaw(const String &filename, uchar  *data,
                const uint width, const uint height, 
                const uint NumBytesPP);
            /**
            *
            * \param filename Filename of  currentMap to save as image
            * \return 
            */
            const bool saveCurrentMapAsRaw(const String &filename);

            const bool saveHeightData (const String &filename, Real *Heightdata, 
                                        const uint width, const uint height);

            const bool saveRaw (const String &filename, Real* data,
                            const uint width, const uint height);

            const bool saveExt (const String &filename,  Real* data,
                                const uint width, const uint height,
                                const uint NumBytesPP);

            void SplitAndSpaceHeightData (const String &filename, Real *Heightdata,
                                                        const uint width, const uint height, 
                                                        const int tile_spacing, const int tile_size);

            bool b_SaveAsRaw;
            bool b_SaveAsExt;
            bool b_SaveBigFile;

            uint mPageSize;
            uint mTileSpacing;

            String OutDirectory;

            Image *mCurrentMap;
};
}
#endif //MapSplitter_H
