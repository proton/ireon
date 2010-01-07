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
MapSplitter.cpp
@brief
Utility that split big map in tiles, pre-calculate splatting by
pre-calculating coverage map and pre-calculate normals.
*/

#include "MapUtil.h"
#include "MapSplitter.h"

#include <iostream>
#include <fstream>
#include <vector>

#include "OgreImageCodec.h"
#include "OgreILCodecs.h"


#include "fileutils.h"


namespace Ogre
{
    //-----------------------------------------------------------------------
    MapSplitter::MapSplitter()
    {}
    //-----------------------------------------------------------------------
    MapSplitter::~MapSplitter()
    {}
    //-----------------------------------------------------------------------
    void MapSplitter::ComputeTileSize()
   {
        uint tileh = MapUtil::getSingleton().getMapHeight() /
                    MapUtil::getSingleton().world_height;
        uint tilew = MapUtil::getSingleton().getMapWidth() /
            MapUtil::getSingleton().world_width;

        int TileSize = (tilew < tileh)? tilew : tileh;

        int i = 0;
        while ( i < 256 )
        {
            if ( TileSize < ( 1 << i ))
            {
                break;
            }
            i++;
        }
        mPageSize = ( 1 << (i - 1));
   }

    //-----------------------------------------------------------------------
    void MapSplitter::setRawSave (bool option)
    {
        if (option)
        {
            b_SaveAsRaw = true;
            b_SaveAsExt = false;
        }
        else
         {
            b_SaveAsRaw = false;
            b_SaveAsExt = true;
        }
    }
    //-----------------------------------------------------------------------
    void MapSplitter::init ()
    {
        if (MapUtil::getSingleton().mPaged)
        {
            mTileSpacing = 1;
            b_SaveBigFile = false;
            mPageSize = MapUtil::getSingleton().mPageSize;
        } 
        else
        {
            b_SaveBigFile = true;
            mPageSize = 0;
            mTileSpacing = 0;
        }
        setRawSave (StringUtil::endsWith (MapUtil::getSingleton().mExt, "raw", true));
        
       OutDirectory = MapUtil::getSingleton().mOutDirectory;

       // make sure directory exists before adding it to archive manager
       char *olddir = ChangeToDir (const_cast< char * > (OutDirectory.c_str ()));
       RetablishDir (olddir);

    }

    //-----------------------------------------------------------------------
    void MapSplitter::saveAndSplitMap (const String &filename, Image *newImage)
    {
        char *olddir = ChangeToDir (const_cast< char * > (OutDirectory.c_str ()));

        setCurrentMap (newImage);
        if (b_SaveBigFile)
            saveCurrentMap (filename);
        else
            SplitCurrentMap (filename, mPageSize + 1);

        RetablishDir (olddir);
    }
    //-----------------------------------------------------------------------
    void MapSplitter::saveAndSpaceAndSplitMap (const String &filename, Image *newImage)
    {
        char *olddir = ChangeToDir (const_cast< char * > (OutDirectory.c_str ()));

        setCurrentMap (newImage);
        if (b_SaveBigFile)
            saveCurrentMap (filename);
        else
            SplitAndSpaceCurrentMap (filename, mTileSpacing, mPageSize + 1);

        RetablishDir (olddir);
    } 
    //-----------------------------------------------------------------------
    void MapSplitter::saveAndSpaceAndSplitHeightData (const String &filename, const String& outDir,
        uchar *Heightdata, const uint width, const uint height, const int tile_spacing, const int page_size,
                                                const bool saveasRaw, const String &extension)
    {
		OutDirectory=outDir;
        char *olddir = ChangeToDir (const_cast< char * > (OutDirectory.c_str ()));

        SplitAndSpaceHeightData (filename, Heightdata, width, height, tile_spacing, page_size,
            saveasRaw, extension);

        RetablishDir (olddir);
    }
    //-----------------------------------------------------------------------
    void MapSplitter::saveAndSpaceAndSplitHeightData (const String &filename, 
        Real *Heightdata, const uint width, const uint height)
    {
        char *olddir = ChangeToDir (const_cast< char * > (OutDirectory.c_str ()));

        if (b_SaveBigFile)
            saveHeightData (filename, Heightdata, width, height);
        else
            SplitAndSpaceHeightData (filename, Heightdata, width, height, mTileSpacing, mPageSize);

        RetablishDir (olddir);
    }
    //-----------------------------------------------------------------------
    void MapSplitter::SplitAndSpaceHeightData (const String &filename, Real *Heightdata,
                                                const uint width, const uint height, 
                                                const int tile_spacing, const int page_size)
    {
        const uint base_image_width = width;
        const uint base_image_height = height;
        const uint base_image_bypp = b_SaveAsRaw? 2 : 1;
        uchar *BaseData = new uchar[width*height*base_image_bypp];

        if (base_image_bypp == 2)
        {
            uint j = 0;
            const Real divider = (1.0f / MapUtil::getSingleton().getMaxTheoHeight()) * 65535;
            for (uint i = 0; i < width*height; i++ )
            { 
            
                ushort syn = ushort  (Heightdata[i] * divider);
                #if OGRE_ENDIAN == OGRE_ENDIAN_BIG
                    BaseData[j] = uchar ((syn >> 8) & 0xff);
		            BaseData[j+ 1] = uchar (syn & 0xff);
                #else
		            BaseData[j] = uchar (syn & 0xff);
		            BaseData[j+ 1] = uchar ((syn >> 8) & 0xff);
                #endif
                j += 2;            
            }
        }
        else if (base_image_bypp == 1)
        {
            const Real divider = (1.0f / MapUtil::getSingleton().getMaxTheoHeight()) * 256;
            for (uint i = 0; i < width*height; i++ )
            {             
                BaseData[i] = uchar  (Heightdata[i] * divider);
            }
        }
        SplitAndSpaceHeightData (filename, BaseData,
                                 width, height, 
                                 tile_spacing, page_size,
                                 b_SaveAsRaw,
                                 (b_SaveAsRaw)? 
                                    MapUtil::getSingleton().mExt :
                                    MapUtil::getSingleton().mTextureExt);
         
    }
    //-----------------------------------------------------------------------
    void MapSplitter::SplitAndSpaceHeightData (const String &filename, uchar *BaseData,
                                                const uint width, const uint height, 
                                                const int tile_spacing, const int page_size,
                                                const bool saveasRaw, const String &extension)
    {
        const uint base_image_width = width;
        const uint base_image_height = height;
        const uint base_image_bypp = saveasRaw? 2 : 1;

        DEBUG_PROGRESS_OUTPUT ( "Width : " << base_image_width);
        DEBUG_PROGRESS_OUTPUT ("  Height : " << base_image_height);
        DEBUG_PROGRESS_OUTPUT ("  Bypp : " <<  base_image_bypp);
        DEBUG_PROGRESS_OUTPUT ("  Page : " <<  page_size);
		DEBUG_PROGRESS_OUTPUT ("  TT2 " <<  "\n");

        std::cout << filename << "\n";


        String file_name;
        String pure_file_name;

        // Get codec according to filename extension
        std::vector< String > partofname = StringUtil::split (filename, ".");
        String ext_name;

        if (partofname.size() <= 1)
        {
            ext_name = extension;
            pure_file_name = filename;
        }
        else
        {
            ext_name = extension;
            for (uint i = 0; i < partofname.size() - 1; i++)
                pure_file_name += partofname[i] + ".";
        }

        // number of tiles along width and height axis
        const uint height_split_count = base_image_height / (page_size + tile_spacing);
        const uint width_split_count = base_image_width / (page_size + tile_spacing);
        const uint size_split_count = (page_size + tile_spacing) * (page_size + tile_spacing);
        DEBUG_PROGRESS_OUTPUT ("  SplitCount : " <<  width_split_count);

        if ((base_image_height / page_size) == 1 &&
            (base_image_width / page_size) == 1)
        {
            String SplitFileName =  pure_file_name + String(".0.0.") + ext_name;
            save (const_cast< char * > (SplitFileName.c_str()),
                BaseData,
                page_size + tile_spacing,
                page_size + tile_spacing,
                base_image_bypp);
            //saveCurrentMap (filename);
            return;
        }


        uchar *SplittedData = new uchar[size_split_count*base_image_bypp];
        uint i,j;
        for (i = 0; i < height_split_count; i++)
        {
            for (j = 0; j < width_split_count; j++)
            {
                // Blit copy from base image to sub-image
                BlitSubImage (BaseData,
                    SplittedData,
                    j*page_size,
                    i*page_size,
                    page_size + tile_spacing,
                    page_size + tile_spacing,
                    page_size + tile_spacing,
                    page_size + tile_spacing,
                    base_image_width,
                    base_image_bypp);

                // make up filename with tile indexes
                String SplitFileName;
                SplitFileName =  pure_file_name  + StringConverter::toString(i) + String(".") + StringConverter::toString(j) + String(".") + ext_name;
                save (const_cast< char * > (SplitFileName.c_str()),
                    SplittedData,
                    page_size + tile_spacing,
                    page_size + tile_spacing,
                    base_image_bypp);
                DEBUG_PROGRESS_OUTPUT ("created : " << SplitFileName << "\n");
            }
         }      
        //----last row : special treatment : read too far on bottom
        const uint finalheight = base_image_height - (height_split_count * page_size); 
        const uint finalwidth = base_image_width - (width_split_count * page_size);
        for (j = 0; j < width_split_count; j++)
         {
            // Blit copy from base image to sub-image
            BlitAndFillSubImage (BaseData,
                SplittedData,
                j*page_size,
                height_split_count * page_size,
                page_size + tile_spacing,
                page_size + tile_spacing,
                finalwidth,//page_size + tile_spacing,
                finalheight,//page_size,
                base_image_width,
                base_image_bypp);

            // make up filename with tile indexes
            String SplitFileName;
            SplitFileName =  pure_file_name  + StringConverter::toString(height_split_count) + String(".") + StringConverter::toString(j) + String(".") + ext_name;
            save (const_cast< char * > (SplitFileName.c_str()),
                SplittedData,
                page_size + tile_spacing,
                page_size + tile_spacing,
                base_image_bypp);
            DEBUG_PROGRESS_OUTPUT ("created : " << SplitFileName << "\n");
        }
        //----last col : special treatment : read too far on bottom 
        for (i = 0; i < height_split_count; i++)
         {
            // Blit copy from base image to sub-image
            BlitAndFillSubImage (BaseData,
                SplittedData,
                width_split_count*page_size,
                i * page_size,
                page_size + tile_spacing,
                page_size + tile_spacing,
                finalwidth,// page_size,
                finalheight,//page_size + tile_spacing,
                base_image_width,
                base_image_bypp);

            // make up filename with tile indexes
            String SplitFileName;
            SplitFileName =  pure_file_name  + StringConverter::toString(i) + String(".") + StringConverter::toString(width_split_count) + String(".") + ext_name;
            save (const_cast< char * > (SplitFileName.c_str()),
                SplittedData,
                page_size + tile_spacing,
                page_size + tile_spacing,
                base_image_bypp);
            DEBUG_PROGRESS_OUTPUT ("created : " << SplitFileName << "\n");
        }
        //----last row, last col : reads too far on right and bottom
     // Blit copy from base image to sub-image
        BlitAndFillSubImage (BaseData,
            SplittedData,
            width_split_count * page_size,
            height_split_count * page_size,
            page_size + tile_spacing,
            page_size + tile_spacing,
            finalwidth,//page_size,
            finalheight,//page_size,
            base_image_width,
            base_image_bypp);
        // make up filename with tile indexes
        String SplitFileName;
        SplitFileName = pure_file_name +  StringConverter::toString(height_split_count) +
            String(".") + StringConverter::toString(width_split_count) + String(".") + ext_name;
        save (const_cast< char * > (SplitFileName.c_str()),
            SplittedData,
            page_size + tile_spacing,
            page_size + tile_spacing,
            base_image_bypp);
        delete[] SplittedData;
        delete[] BaseData;
        DEBUG_PROGRESS_OUTPUT ("created : " << SplitFileName << "\n");
    }
   //-----------------------------------------------------------------------
    const bool MapSplitter::saveHeightData (const String &filename, Real *Heightdata, 
        uint width, uint height)
    {  
        bool Okraw = false;
        bool Okpng = false;

        if (b_SaveAsRaw)
            Okraw = saveRaw(filename,
                        Heightdata,
                        width,
                        height);
        else if (b_SaveAsExt)
            Okpng = saveExt(filename,
                            Heightdata,
                            width,
                            height, 
                            1);
        return Okpng && Okraw;
    }
    //-----------------------------------------------------------------------
    const bool MapSplitter::saveRaw(const String &filename, Real* hdata,
                                    const uint width, const uint height)
    {
        const uint base_image_bypp = b_SaveAsRaw? 2 : 1;
        uchar *data = new uchar[width * height * base_image_bypp];   
        if (base_image_bypp == 2)
        {
            uint j = 0;
            const Real divider = (1.0f / MapUtil::getSingleton().getMaxTheoHeight()) * 65535;
            for (uint i = 0; i < width*height; i++ )
            {             
                ushort syn = ushort  (hdata[i] * divider);
                #if OGRE_ENDIAN == ENDIAN_BIG
                    data[j] = uchar ((syn >> 8) & 0xff);
		            data[j+ 1] = uchar (syn & 0xff);
                #else
		            data[j] = uchar (syn & 0xff);
		            data[j+ 1] = uchar ((syn >> 8) & 0xff);
                #endif
                j += 2;            
            }
        }
        else if (base_image_bypp == 1)
        {
            const Real divider = (1.0f / MapUtil::getSingleton().getMaxTheoHeight()) * 256;
            for (uint i = 0; i < width*height; i++ )
            { 
            
                data[i] = uchar  (hdata[i] * divider);
            }
        }
        String FileNameRaw;

        // Get codec according to filename extension
        std::vector< String > partofname = StringUtil::split (filename, ".");

        if (partofname.size() <= 1)
            FileNameRaw = filename + ".raw";
        else
        {
            for (uint i = 0; i < partofname.size () - 1; i++)
                FileNameRaw += partofname[i] + ".";
            FileNameRaw += "raw";
        }

        DataStreamPtr image_chunk(new MemoryDataStream ((void*)data,
                        width * height * base_image_bypp * sizeof (uchar), 
                        true)); 

        std::ofstream outfile;
        outfile.open (const_cast< char * > (FileNameRaw.c_str()),
            std::ios::binary);
        // Write out
        outfile << image_chunk->getAsString ();
        outfile.close ();
        return true;
    }
    //-----------------------------------------------------------------------
    const bool MapSplitter::saveExt(const String &filename,  Real* hdata,
                                    const uint width, const uint height,
                                    const uint NumBytesPP)
    {
        ImageCodec::ImageData  *imgData = new ImageCodec::ImageData();

        imgData->width = width;
        imgData->height = height;
        imgData->size = width*height*NumBytesPP*sizeof(uchar);

        if (NumBytesPP == 1)
            imgData->format = PF_A8;
        else if (NumBytesPP == 2)
            imgData->format = PF_L16;
        else if (NumBytesPP == 3)
            imgData->format = PF_BYTE_RGB;
        else if (NumBytesPP == 4)
            imgData->format = PF_BYTE_RGBA;
        imgData->depth =  1;

        uchar *data = new uchar[imgData->size];

        if (NumBytesPP == 2)
        {
            uint j = 0;
            const Real divider = (1.0f / MapUtil::getSingleton().getMaxTheoHeight()) * 65535;
            for (uint i = 0; i < width*height; i++ )
            { 
            
                ushort syn = ushort  (hdata[i] * divider);
                #if OGRE_ENDIAN == ENDIAN_BIG
                    data[j] = uchar ((syn >> 8) & 0xff);
		            data[j+ 1] = uchar (syn & 0xff);
                #else
		            data[j] = uchar (syn & 0xff);
		            data[j+ 1] = uchar ((syn >> 8) & 0xff);
                #endif
                j += 2;            
            }
        }
        else if (NumBytesPP == 1)
        {
            const Real divider = (1.0f / MapUtil::getSingleton().getMaxTheoHeight()) * 256;
            for (uint i = 0; i < width*height; i++ )
            {             
                data[i] = uchar  (hdata[i] * divider);
            }
        }

        // Get codec according to filename extension
        std::vector< String > partofname = StringUtil::split (filename, ".");
        String pure_file_name;
        String ext_name;

        if (partofname.size() <= 1)
        {
            ext_name = (b_SaveAsRaw)? MapUtil::getSingleton().mExt:MapUtil::getSingleton().mTextureExt;;
            pure_file_name = filename;
        }
        else
        {
            ext_name =  (b_SaveAsRaw)? MapUtil::getSingleton().mExt:MapUtil::getSingleton().mTextureExt;;
            for (uint i = 0; i < partofname.size() - 1; i++)
                pure_file_name += partofname[i] + ".";
        }


        Codec * pCodec = Codec::getCodec (
            ext_name);
    
        Codec::CodecDataPtr codecdata (imgData);
        MemoryDataStreamPtr image_chunk(new MemoryDataStream ((void*)data,
                                                                imgData->size, 
                                                                true)); 

        // Write out
        pCodec->codeToFile(image_chunk, pure_file_name + ext_name, codecdata);
        return true;
    }
   //-----------------------------------------------------------------------
    void MapSplitter::setCurrentMap(Image *newImage)
    {
        mCurrentMap = newImage;
    }
    //-----------------------------------------------------------------------
    const bool MapSplitter::save(const String &filename, const uchar * const data,
                                const uint width, const uint height,
                                const uint NumBytesPP)
     {
        bool Okraw = false;
        bool Okpng = false;

        if (b_SaveAsRaw || NumBytesPP == 2)//devil cannot save 16 bits image.
            Okraw = saveRaw(filename, const_cast <uchar*>(data),
                            width, height,
                            NumBytesPP);
        else if (b_SaveAsExt)
            Okpng = saveExt(filename, data,
                            width, height,
                            NumBytesPP);

        return Okpng && Okraw;
    }
    //-----------------------------------------------------------------------
    const bool MapSplitter::saveCurrentMap (const String &filename)
    {
        bool Okraw = false;
        bool Okpng = false;

        if (b_SaveAsRaw)
            Okraw = saveCurrentMapAsRaw(filename);
        else if (b_SaveAsExt)
            Okpng = saveCurrentMapAsExt(filename);
        return Okpng && Okraw;
    }
    //-----------------------------------------------------------------------
    const bool MapSplitter::saveCurrentMapAsRaw(const String &filename)
    {
        return saveRaw(filename,
            mCurrentMap->getData(),
            static_cast <uint> ( mCurrentMap->getWidth()),
            static_cast <uint> ( mCurrentMap->getHeight()),
           static_cast <uint> ( PixelUtil::getNumElemBytes (mCurrentMap->getFormat ())));
    }
    //-----------------------------------------------------------------------
    const bool MapSplitter::saveRaw(const String &filename, uchar *data,
        const uint width, const uint height,
        const uint NumBytesPP)
    {
        String FileNameRaw;

        // Get codec according to filename extension
        std::vector< String > partofname = StringUtil::split (filename, ".");

        if (partofname.size() <= 1)
            FileNameRaw = filename + ".raw";
        else
        {
            for (uint i = 0; i < partofname.size () - 1; i++)
                FileNameRaw += partofname[i] + ".";
            FileNameRaw += "raw";
        }

        DataStreamPtr image_chunk(new MemoryDataStream (data,
                        width*height*NumBytesPP * sizeof (uchar), 
                        false)); 

        std::ofstream outfile;
        outfile.open (const_cast< char * > (FileNameRaw.c_str()),
            std::ios::binary);
        // Write out
        outfile << image_chunk->getAsString ();
        outfile.close ();
        return true;
    }
    //-----------------------------------------------------------------------
    const bool MapSplitter::saveExt(const String &filename, const uchar * const data,
        const uint width, const uint height,
        const uint NumBytesPP)
    {
        // Get codec according to filename extension
        String ext_name = (b_SaveAsRaw)? MapUtil::getSingleton().mExt:MapUtil::getSingleton().mTextureExt;
        std::vector< String > partofname = StringUtil::split (filename, ".");
        String pure_file_name;
        if (partofname.size() <= 1)
        {
            pure_file_name = filename;
        }
        else
        {
            for (uint i = 0; i < partofname.size() - 1; i++)
                pure_file_name += partofname[i] + ".";
        }

        Codec * pCodec = Codec::getCodec (ext_name);
        
        ImageCodec::ImageData *imgData = new ImageCodec::ImageData();;

        imgData->width = width;
        imgData->height = height;
        imgData->size = width*height*NumBytesPP;

        if (NumBytesPP == 1)
            imgData->format = PF_A8;
        else if (NumBytesPP == 2)
            imgData->format = PF_L16;
        else if (NumBytesPP == 3)
            imgData->format = PF_BYTE_RGB;
        else if (NumBytesPP == 4)
            imgData->format = PF_BYTE_RGBA;
        imgData->depth =  1;

        Codec::CodecDataPtr codecdata (imgData);
        MemoryDataStreamPtr image_chunk (new MemoryDataStream ((void*)data,
                                                                imgData->size*sizeof(uchar), 
                                                                false)); 
        // Write out
        pCodec->codeToFile(image_chunk, pure_file_name + ext_name, codecdata);
        return true;
    }
    //-----------------------------------------------------------------------
    const bool MapSplitter::saveCurrentMapAsExt(const String &filename)
     {
        return saveExt(filename,
            mCurrentMap->getData(),
            static_cast <uint> (mCurrentMap->getWidth()),
            static_cast <uint> (mCurrentMap->getHeight()),
            static_cast <uint> (PixelUtil::getNumElemBytes (mCurrentMap->getFormat ())));
    }
    //-----------------------------------------------------------------------
    void MapSplitter::BlitAndFillSubImage (const uchar * const BaseData, 
                                    uchar * const OutData,
                                    uint x, uint y,
                                    uint tile_width, uint tile_height,
                                    uint tile_width_copy, uint tile_height_copy,
                                    uint base_width,
                                    uint bpp)
    {
        const uint src_shift = bpp * base_width;
        const uint dest_shift = bpp * tile_width;
        const uint dest_shift_copy = bpp * tile_width_copy;
        const uint dest_shift_fill = bpp * (tile_width - tile_width_copy);
        const uchar * src = BaseData + y*src_shift + bpp*x;
        uchar *dest = OutData;
        for (uint j = 0; j < tile_height_copy; j++)
        {
            memcpy (dest, src, dest_shift_copy);
            dest += dest_shift_copy;
            src += src_shift;
            memset (dest, *(dest - 1), dest_shift_fill);
            dest += dest_shift_fill;
        }
        const uint fill_height = tile_height - tile_height_copy;
        if (fill_height > 0)
        {
            src = dest -  dest_shift;
            for (uint k = 0; k < fill_height; k++)
            {
                memcpy (dest, src, dest_shift);
                dest += tile_width;
            }
        }
    }
    //-----------------------------------------------------------------------
    void MapSplitter::BlitSubImage (const uchar * const BaseData, 
                                    uchar * const OutData,
                                    const uint x, const uint y,
                                    const uint tile_width, const uint tile_height,
                                    const uint tile_width_copy, const uint tile_height_copy,
                                    const uint base_width,
                                    const uint bpp)
    {
        const uint src_shift = bpp * base_width;
        const uint dest_shift = bpp * tile_width;
        const uint dest_shift_copy = bpp * tile_width_copy;

        const uchar *src = BaseData + y*src_shift + bpp*x;
        uchar *dest = OutData;
        for (uint j = 0; j < tile_height_copy; j++)
        {
            memcpy (dest, src, dest_shift_copy);
            dest += dest_shift;
            src += src_shift;
        }
    }
    //-----------------------------------------------------------------------
    void MapSplitter::SplitAndSpaceCurrentMap(const String &filename, 
                                                const int tile_spacing,
                                                const int page_size)
    {
        const uint base_image_width = static_cast <uint> (mCurrentMap->getWidth ());
        const uint base_image_height = static_cast <uint> (mCurrentMap->getHeight ());
        const uint base_image_bypp = static_cast < uint > (PixelUtil::getNumElemBytes (mCurrentMap->getFormat ()));


        DEBUG_PROGRESS_OUTPUT ( "Width : " << base_image_width);
        DEBUG_PROGRESS_OUTPUT ("  Height : " << base_image_height);
        DEBUG_PROGRESS_OUTPUT ("  Bypp : " <<  base_image_bypp << "\n");
        DEBUG_PROGRESS_OUTPUT ("  Page : " <<  page_size);

        std::cout << filename << "\n";


        // Get codec according to filename extension
        std::vector< String > partofname = StringUtil::split (filename, ".");
        String pure_file_name;
        String ext_name = (b_SaveAsRaw)? MapUtil::getSingleton().mExt:MapUtil::getSingleton().mTextureExt;
        if (partofname.size() <= 1)
        {
            pure_file_name = filename;
        }
        else
        {
            for (uint i = 0; i < partofname.size() - 1; i++)
                pure_file_name += partofname[i] + ".";
        }
        
        // number of tiles along width and height axis
        const uint height_split_count = base_image_height / (page_size);
        const uint width_split_count = base_image_width / (page_size);
        DEBUG_PROGRESS_OUTPUT ("  Split count : " <<  width_split_count << "\n");

		if ( base_image_height / (page_size - 1) == 1 &&
            base_image_width / (page_size - 1) == 1)
        {
            mCurrentMap->resize(base_image_width - 1, base_image_height - 1);
            String SplitFileName =  pure_file_name + String(".0.0.") + ext_name;
            save (SplitFileName,
                    mCurrentMap->getData(),
                    base_image_width - 1, 
                    base_image_height - 1,
                    base_image_bypp);

            return;
        }

        const uchar * const BaseData = mCurrentMap->getData();

        //const uint size_split_count = (page_size + tile_spacing) * (page_size + tile_spacing);
        const uint size_split_count = page_size * page_size;


        uint i,j;
        for (i = 0; i < height_split_count; i++)
        {
            for (j = 0; j < width_split_count; j++)
            {
                uchar *SplittedData = new uchar [size_split_count * base_image_bypp];

                // Blit copy from base image to sub-image
                BlitSubImage (BaseData,
                    SplittedData,
                    j*(page_size - 1),
                    i*(page_size - 1),
                    page_size,
                    page_size,
                    page_size,
                    page_size,
                    base_image_width,
                    base_image_bypp);

                // make up filename with tile indexes
                const String SplitFileName (pure_file_name  + 
                    StringConverter::toString(i) + "." + 
                    StringConverter::toString(j) + "." + 
                    ext_name);

                Image resizer;
                resizer.loadDynamicImage (SplittedData, 
                            page_size, page_size, 1,
                          mCurrentMap->getFormat (),
                          true);

                resizer.resize(page_size - tile_spacing, page_size - tile_spacing);

                save (SplitFileName,
                    resizer.getData(),
                    page_size - tile_spacing, page_size - tile_spacing,
                    base_image_bypp);

//                save (const_cast< char * > (SplitFileName.c_str()),
//                    SplittedData,
//                    page_size + tile_spacing,
//                    page_size + tile_spacing,
//                    base_image_bypp);
                DEBUG_PROGRESS_OUTPUT ("created : " << SplitFileName << "\n");
            }
         }

        const uint finalheight = base_image_height - (height_split_count * page_size); 
        const uint finalwidth = base_image_width - (width_split_count * page_size);

        //----last row : special treatment : read too far on bottom
        for (j = 0; j < width_split_count; j++)
         {
            uchar *SplittedData = new uchar [size_split_count * base_image_bypp];
            // Blit copy from base image to sub-image
            BlitAndFillSubImage (BaseData,
                SplittedData,
                j*(page_size - 1),
                height_split_count * (page_size - 1),
                page_size,
                page_size,
                finalwidth,//page_size + tile_spacing,
                finalheight,//page_size,
                base_image_width,
                base_image_bypp);

            // make up filename with tile indexes
            String SplitFileName;
            SplitFileName =  pure_file_name  + StringConverter::toString(height_split_count) + String(".") + StringConverter::toString(j) + String(".") + ext_name;
            
            
                Image resizer;
                resizer.loadDynamicImage (SplittedData, 
                            page_size, page_size, 1,
                          mCurrentMap->getFormat (),
                          true);

                resizer.resize(page_size - tile_spacing, page_size - tile_spacing);

                save (SplitFileName,
                    resizer.getData(),
                    page_size - tile_spacing, page_size - tile_spacing,
                    base_image_bypp);
            
            
//            save (const_cast< char * > (SplitFileName.c_str()),
//                SplittedData,
//                page_size + tile_spacing,
//                page_size + tile_spacing,
//                base_image_bypp);
            DEBUG_PROGRESS_OUTPUT ("created : " << SplitFileName << "\n");
        }
        //----last col : special treatment : read too far on bottom
        for (i = 0; i < height_split_count; i++)
         {
            uchar *SplittedData = new uchar [size_split_count * base_image_bypp];
            // Blit copy from base image to sub-image
            BlitAndFillSubImage (BaseData,
                SplittedData,
                width_split_count*(page_size - 1),
                i * (page_size - 1),
                page_size,
                page_size,
                finalwidth,//page_size,
                finalheight,//page_size + tile_spacing,
                base_image_width,
                base_image_bypp);

            // make up filename with tile indexes
            String SplitFileName;
            SplitFileName =  pure_file_name  + StringConverter::toString(i) + String(".") + StringConverter::toString(width_split_count) + String(".") + ext_name;
            
            
            
                Image resizer;
                resizer.loadDynamicImage (SplittedData, 
                            page_size, page_size, 1,
                          mCurrentMap->getFormat (),
                          true);

                resizer.resize(page_size - tile_spacing, 
                                page_size - tile_spacing);

                save (SplitFileName,
                    resizer.getData(),
                    page_size - tile_spacing, page_size - tile_spacing,
                    base_image_bypp);
            
//            save (const_cast< char * > (SplitFileName.c_str()),
//                SplittedData,
//                page_size + tile_spacing,
//                page_size + tile_spacing,
//                base_image_bypp);
            DEBUG_PROGRESS_OUTPUT ("created : " << SplitFileName << "\n");
        }
        //----last row, last col : reads too far on right and bottom
        // Blit copy from base image to sub-image
     
        uchar *SplittedData = new uchar [size_split_count * base_image_bypp];
        BlitAndFillSubImage (BaseData,
            SplittedData,
            width_split_count * (page_size - 1),
            height_split_count * (page_size - 1),
            page_size,
            page_size,
            finalwidth,//page_size,
            finalheight,//page_size,
            base_image_width,
            base_image_bypp);

        // make up filename with tile indexes
        String SplitFileName(pure_file_name +  
            StringConverter::toString(height_split_count) + "." + 
            StringConverter::toString(width_split_count) + "." + 
            ext_name);

                Image resizer;
                resizer.loadDynamicImage (SplittedData, 
                            page_size, page_size, 1,
                          mCurrentMap->getFormat (),
                          true);

                resizer.resize(page_size - tile_spacing, 
                                page_size - tile_spacing);

                save (SplitFileName,
                    resizer.getData(),
                    page_size - tile_spacing, page_size - tile_spacing,
                    base_image_bypp);

//        save (const_cast< char * > (SplitFileName.c_str()),
//            SplittedData,
//            page_size + tile_spacing,
//            page_size + tile_spacing,
//            base_image_bypp);



        //delete[] SplittedData;
        DEBUG_PROGRESS_OUTPUT ("created : " << SplitFileName << "\n");
    }

    //-----------------------------------------------------------------------
    void MapSplitter::SplitCurrentMap(const String &filename, const int page_size)
    {
        const uint base_image_width = static_cast <uint> (mCurrentMap->getWidth ());
        const uint base_image_height = static_cast <uint> (mCurrentMap->getHeight ());
        const uint base_image_bypp = static_cast < uint > 
            (PixelUtil::getNumElemBytes (mCurrentMap->getFormat ()));

        DEBUG_PROGRESS_OUTPUT ( "Width : " << base_image_width);
        DEBUG_PROGRESS_OUTPUT ("  Height : " << base_image_height);
        DEBUG_PROGRESS_OUTPUT ("  Bpp : " <<  base_image_bypp << "\n");
        DEBUG_PROGRESS_OUTPUT ("  1 " <<  "\n");
        std::cout << filename << "\n";

        // Get codec according to filename extension
        std::vector< String > partofname = StringUtil::split (filename, ".");
        String ext_name = (b_SaveAsRaw)? MapUtil::getSingleton().mExt:MapUtil::getSingleton().mTextureExt;
        String pure_file_name;
        if (partofname.size() <= 1)
        {
            pure_file_name = filename;
        }
        else
        {
            for (uint i = 0; i < partofname.size() - 1; i++)
                pure_file_name += partofname[i] + ".";
        }

        // number of tiles along width and height axis
        const unsigned int height_split_count = base_image_height / page_size;
        const unsigned int width_split_count = base_image_width / page_size;
        const unsigned int size_split_count = page_size * page_size;


		if ( base_image_height / (page_size - 1) == 1 &&
            base_image_width / (page_size - 1) == 1)
        {  
            mCurrentMap->resize(base_image_width - 1, base_image_height - 1);
            String SplitFileName =  pure_file_name + String(".0.0.") + ext_name;
            save (SplitFileName,
                    mCurrentMap->getData(),
                    base_image_width - 1, 
                    base_image_height - 1,
                    base_image_bypp);
            return;
        }
        const uchar * const BaseData = mCurrentMap->getData();

        Image resizer;
        for (unsigned int i = 0; i <= height_split_count; i++)
        {
            for (unsigned int j = 0; j <= width_split_count; j++)
            {
                uchar *SplittedData = new uchar[size_split_count * base_image_bypp];
                // Blit copy from base image to sub-image
                BlitSubImage (BaseData,
                                SplittedData,
                                j * (page_size - 1),
                                i * (page_size - 1),
                                page_size,
                                page_size,
                                page_size,
                                page_size,
                                base_image_width,
                                base_image_bypp);

                // make up filename with tile indexes
                String SplitFileName (pure_file_name + 
                                        StringConverter::toString(i) + "." + 
                                        StringConverter::toString(j) + "." + 
                                        ext_name);
    

                resizer.loadDynamicImage (SplittedData, 
                            page_size, page_size, 1,
                          mCurrentMap->getFormat (),
                          true);

                resizer.resize(page_size - 1, 
                                page_size - 1);

                save (SplitFileName,
                    resizer.getData(),
                    page_size - 1, page_size - 1,
                    base_image_bypp);

                DEBUG_PROGRESS_OUTPUT ("created : " << SplitFileName << "\n");
            }
        }
    }
}//namespace Ogre
