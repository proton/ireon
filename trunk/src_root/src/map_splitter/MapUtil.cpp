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
MapUtil.cpp
@brief
Utility that split big map in tiles, pre-calculate splatting by
pre-calculating coverage map and pre-calculate normals.
*/

#include "MapUtil.h"

#ifndef _MAPEDITOR

#include "OgrePagingLandScapeData2D.h"
#include "OgrePagingLandScapeData2D_HeightField.h"

#endif //_MAPEDITOR

#include <iostream>
#include <fstream>
#include <vector>

#include "fileutils.h"

#include "OgreCommon.h"

#include "OgreVector3.h"
#include "OgreILImageCodec.h"
#include "OgreILCodecs.h"

#include "OgreDynLibManager.h"
#include "OgreDynLib.h"
#include "OgreZip.h"

#include "MapNormaler.h"

#include "MapSplatter.h"

#include "MapLighter.h"

#include "MapVis.h"

#include "MapHorizon.h"
#include "MapHorizon2.h"
#include "MapHorizon3.h"

#include "MapBlender.h"
#include "MapBlurrer.h"
#include "MapSplitter.h"
#include "MapInfinitizer.h"
#include "MapEqualizer.h"
#include "MapMergeModulater.h"

typedef void (*DLL_START_PLUGIN)(void);
typedef void (*DLL_STOP_PLUGIN)(void);

namespace Ogre
{
    
    template<> MapUtil* Singleton<MapUtil>::ms_Singleton = 0;
    //-----------------------------------------------------------------------
    MapUtil* MapUtil::getSingletonPtr(void)
    {
        return ms_Singleton;
    }
    //-----------------------------------------------------------------------
    MapUtil& MapUtil::getSingleton(void)
    {  
        assert( ms_Singleton );  return ( *ms_Singleton );  
    }
    //-----------------------------------------------------------------------
    MapUtil::MapUtil() :
        mGenHeightMap (true),  
        mGenMiniMap (true),
        mGenCoverageMap (true),
        mGenBaseMap (true),
        mGenAlphaMaps (true),
        mGenLightMap (true),
     
        mGenColorMapGenerate (false),
        mGenColorMapSplit (false),
        mGenElevationMap (false),                               
        mGenNormalMap (false),
        mGenEqualize (false),
        mGenInfiniteMap (false),
        mGenHeightNormalMap (false),
        mGenRGBMaps (false),
        mGenAlphaSplatRGBAMaps(false),
        mGenAlphaSplatLightMaps (false),
        mGenShadowMap (false),
        mGenLitColorMapGenerate (false),
        mGenLitColorMapSplit (false),
        mGenLitBaseMap (false),
        mGenPVSMap (false),
        mGenHorizonMap (false),
        mGenZHorizon (false),
        mHeightMapBlurFactor(0.0f),
        mGenMiniMapWidth(256),
        mGenMiniMapHeight(256),
        mColorMapName (StringUtil::BLANK),
        scale(Vector3::UNIT_SCALE),
        mGroupName ("PLSM2"),
        Splat_Filename_0(StringUtil::BLANK),
        Splat_Filename_1(StringUtil::BLANK),
        Splat_Filename_2(StringUtil::BLANK),
        Splat_Filename_3(StringUtil::BLANK),
	    world_height (1),		
	    world_width (1),	
        mPaged (true),
        mOutDirectory (StringUtil::BLANK)
    {    
	    for (uint i = 0; i < 4; i++)
	    {
            matColor[i] = ColourValue::White;
	    }
	    for (uint i = 0; i < 4; i++)
	    {
		    matHeight[i] = 0.0f;
	    }
    }
    //-----------------------------------------------------------------------
    void MapUtil::Load()
    {
#ifndef _MAPEDITOR
        mRoot = new Root("plugins.cfg", "ogre.cfg", "MapSplitter.log");     
        // Load resource paths from config file      
       ConfigFile cf;
        cf.load("resources.cfg");
        // Go through all sections & settings in the file
        ConfigFile::SectionIterator seci = cf.getSectionIterator();

        String secName, typeName, archName;
        while (seci.hasMoreElements())
        {
            secName = seci.peekNextKey();
            ConfigFile::SettingsMultiMap *settings = seci.getNext();
            ConfigFile::SettingsMultiMap::iterator i;
            for (i = settings->begin(); i != settings->end(); ++i)
            {
                typeName = i->first;
                archName = i->second;
                ResourceGroupManager::getSingleton().addResourceLocation(
                    archName, typeName, secName);
            }
        }

        mOptions = new PagingLandScapeOptions();
        mOptions->load("maptool.cfg");
#endif //_MAPEDITOR
    }
    //-----------------------------------------------------------------------
    MapUtil::~MapUtil()
    {          
#ifndef _MAPEDITOR
        delete mOptions;
        delete mRoot;
#endif _MAPEDITOR
    }
    //-----------------------------------------------------------------------
    const Real MapUtil::getMaxTheoHeight ()
    {
        return maxHeight;

    } 
    //-----------------------------------------------------------------------
    Real *MapUtil::getHeightData ()
    {
        return mData;
    }
    //-----------------------------------------------------------------------
    const Real MapUtil::getHeight (const Real x, const Real z)
    {
        assert( mData );
        assert( z < mHeight&& x < mWidth );
        const uint Pos = z * mHeight + x;
        return mData[ Pos ];  
    }

    //-----------------------------------------------------------------------
    void MapUtil::load(const String &filename, Image *ImageMap)
    {
        ImageMap->load (filename, mGroupName);
    }
    //-----------------------------------------------------------------------
    const Vector3 MapUtil::getNormalAt(const Real x, const Real z)
    {  
        return NormalMapper.getNormalAt (uint (x), uint (z));
    }
    //-----------------------------------------------------------------------
    Vector3 *MapUtil::getNormalData()
    {  
        return NormalMapper.getNormalData ();
    }
    //-----------------------------------------------------------------------
    void MapUtil::process()
    { 
#ifndef _MAPEDITOR
        if (mOptions->mBatchMode)
        {
            LandscapeFileNames *_map = mOptions->getLandscapeFileNames();
            LandscapeFileNames::iterator i = _map->begin();
	        LandscapeFileNames::iterator i_end = _map->end();
	        for(;i != i_end; ++i)
	        {	
                mOptions->loadMapOptions(i->second + ".cfg");  
                LoadOptions ();
                LoadDataFromSource();
                processOneMap ();
            }
        }
        else
        {            
            LoadDataFromSource();
            processOneMap ();
        }
#endif// _MAPEDITOR
    }
    //-----------------------------------------------------------------------
    void MapUtil::LoadOptions()
    {
#ifndef _MAPEDITOR
        mFileName = mOptions->landscape_filename + 
                    "." +
                    mOptions->landscape_extension;

        // set codec using file extension
        mPureFilename = mOptions->landscape_filename;
        mExt = mOptions->landscape_export_extension;
        mTextureExt = mOptions->TextureExtension;

        mOptions->getAvgColors();

        mPageSize = mOptions->PageSize - 1;
 

        mSun = mOptions->Sun;
        mAmb = mOptions->Amb;
        mDiff = mOptions->Diff;
        mBlur = mOptions->Blur;
        mHeightMapBlurFactor = mOptions->HeightMapBlurFactor;

        mGenHeightMap =             mOptions->HeightMap;  
        mGenMiniMap =               mOptions->MiniMap;
        mGenCoverageMap =           mOptions->CoverageMap;
        mGenBaseMap =               mOptions->BaseMap;
        mGenAlphaMaps =             mOptions->AlphaMaps;
        mGenLightMap =              mOptions->LightMap;
                                 
        mGenColorMapGenerate =      mOptions->ColorMapGenerate;
        mGenColorMapSplit =         mOptions->ColorMapSplit;
        mGenElevationMap =          mOptions->ElevationMap;                         
        mGenNormalMap  =            mOptions->NormalMap;
        mGenEqualize =              mOptions->Equalize;
        mGenInfiniteMap =           mOptions->InfiniteMap;
        mGenHeightNormalMap =       mOptions->HeightNormalMap;
        mGenRGBMaps =               mOptions->RGBMaps;
        mGenAlphaSplatRGBAMaps;     mOptions->AlphaSplatRGBAMaps;
        mGenAlphaSplatLightMaps =   mOptions->AlphaSplatLightMaps;
        mGenShadowMap =             mOptions->ShadowMap;
        mGenLitColorMapGenerate =   mOptions->LitColorMapGenerate;
        mGenLitColorMapSplit =      mOptions->LitColorMapSplit;
        mGenLitBaseMap =            mOptions->LitBaseMap;
        mGenPVSMap =                mOptions->PVSMap;
        mGenHorizonMap =            mOptions->HorizonMap;
        mGenZHorizon =              mOptions->ZHorizon;

        
        mGenMiniMapWidth  =         mOptions->MiniMapWidth;
        mGenMiniMapHeight  =        mOptions->MiniMapHeight;

        mColorMapName = mOptions->ColorMapName;

        scale  = mOptions->scale;
        mGroupName  = mOptions->groupName;


        Splat_Filename_0 = mOptions->Splat_Filename_0;
        Splat_Filename_1 = mOptions->Splat_Filename_1;
        Splat_Filename_2 = mOptions->Splat_Filename_2;
        Splat_Filename_3 = mOptions->Splat_Filename_3;

        uint i;
        for (i = 0; i < 4; i ++)
        {
            matColor[i] = mOptions->matColor[i];
        }   
        for (i = 0; i < 4; i ++)
        {
            matHeight[i] = mOptions->matHeight[i];
        }

		matColor[0] = ColourValue((Real)122/256,(Real)125/256,(Real)48/256,1);
		matColor[1] = ColourValue((Real)64/256,(Real)46/256,(Real)21/256,1);
		matColor[2] = ColourValue((Real)31/256,(Real)24/256,(Real)10/256,1);

	    world_height = mOptions->world_height;				
	    world_width = mOptions->world_width;	
        mPaged = mOptions->Paged;	
        mOutDirectory  = mOptions->OutDirectory;
#endif // _MAPEDITOR
    }
    //-----------------------------------------------------------------------
    void MapUtil::setMap (const String &mapName, 
        const String &extension, 
        const String &texture_extension, 
        const uint pageSize,
        const String &OutDirectory,
        const String &groupname)
    {
       mFileName = mapName + 
                    "." +
                    extension;   
       mPureFilename = mapName;
       mExt = extension;
       mTextureExt = texture_extension;
       mPageSize = pageSize - 1;
       mOutDirectory = OutDirectory;
        mGroupName = groupname;
    }
    //-----------------------------------------------------------------------
    void MapUtil::setMapColoring (const ColourValue *colors, 
                                   const Real *heights,
                                   const uint numColors)
    {
        uint i;
        for (i = 0; i < numColors; i ++)
        {
            matColor[i] = colors[i];
        }   
        for (i = 0; i < numColors; i ++)
        {
            matHeight[i] = heights[i];
        }
    }
    //-----------------------------------------------------------------------
    void MapUtil::LoadDataFromMemory(uint mapWidth, 
        uint mapHeight, 
        Real *heightData,
        Real max)
    {  
        maxHeight = max;// get theoretic max height (before loading)
        mHeight = mapHeight;
        mWidth = mapWidth;
        mMax = max;
        mData = heightData;
    }
    //-----------------------------------------------------------------------
    void MapUtil::LoadDataFromSource()
    {
#ifndef _MAPEDITOR
        // 
        // Load Heightmap data according to specialized loader 
        // 
        if (StringUtil::endsWith (mOptions->data2DFormat, "heightfield", true))
        {
            mHeightdata	= new PagingLandScapeData2D_HeightField ();
        } 
        else 
        {
            OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS,
                "PageData2D not supplied!", 
                "MapUtil::MapUtil" );
        }

        maxHeight = mHeightdata->getMaxHeight ();// get theoretic max height (before loading)

        mHeightdata->load ();
        mHeight = static_cast <uint> (mHeightdata->getZDimension ());
        mWidth = static_cast <uint> (mHeightdata->getXDimension ());
        mMax = mHeightdata->getMaxHeight ();
        mData = mHeightdata->getHeightData ();
#endif // _MAPEDITOR
    }
    //-----------------------------------------------------------------------
    void MapUtil::processOneMap()
    {   
        MapSplatter SplatMapper;
        MapLighter LightMapper;
        MapBlender Blender;
        MapSplitter Splitter;
        MapMergeModulater MergeModulater;

        mPageX = mWidth / mPageSize;
        mPageZ = mHeight / mPageSize;  

        Splitter.init ();

        if (mGenEqualize)
        { 
            MapEqualizer Equalizer;
            Equalizer.equalize ();
        }

        if (mGenInfiniteMap)
        { 
            MapInfinitizer Infinitizer;
            Infinitizer.Infinitize (static_cast <uint> (mHeight / 10), 
                                    0.0f, 
                                    0);
                    
        }

        const Real Blur = mHeightMapBlurFactor;
        if (Blur > 0.0f)
        {
            Convolver *c = new Convolver (int (Blur));
            c->blurReal (mData, mWidth, mHeight);
        }


        // 
        // Split Heightmap in 2^n + 1 part  
        // 
        if (mGenHeightMap)
        {
            Splitter.setRawSave (StringUtil::match(mExt, "raw", false));
            Splitter.saveAndSpaceAndSplitHeightData (mFileName, 
                                            mData, 
                                            mWidth, 
                                            mHeight); 
            Splitter.setRawSave (StringUtil::match(mTextureExt, "raw", false));
        }

/*        if (mGenMiniMap)
        {
            const uint W = MapUtil::getSingleton().getMapWidth ();
            const uint H  = MapUtil::getSingleton().getMapHeight ();
            const uint hn_map_size = W*H;
            uchar *data = new uchar[hn_map_size];            
            const Real divider = 1.0f / getMaxTheoHeight() * 256;
            Real *hdata = mData;
            for ( uint i = 0; i < hn_map_size; i++ )            
            { 
                data[i] = static_cast <uchar>  (hdata[i] * divider);
            }
            Image grayheightmap;
          
            grayheightmap.loadDynamicImage (data, 
                                            static_cast <size_t> (W),
                                            static_cast <size_t> (H), 
                                            1, PF_L8, true);


            const ushort wtiles = mGenMiniMapWidth;
            const ushort htiles = mGenMiniMapHeight; 

            grayheightmap.resize (wtiles, htiles);

            const String OutDirectory = mOutDirectory;

            char *olddir = ChangeToDir (const_cast< char * > (OutDirectory.c_str ()));
            Splitter.saveExt (mPureFilename + ".Small." + mExt, grayheightmap.getData(), 
                                static_cast <uint> (wtiles), 
                                static_cast <uint> (htiles),
                                1);
            RetablishDir (olddir);

            String MapName = mColorMapName;
            if (MapName != StringUtil::BLANK && MapName != "none")    
            {
                    if (MapName == mFileName)
                        OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
                                "HeightMap File and ColorMap file cannot be the same file", 
                                "MapUtil::ProcessOneFile");
               
                    Image ExistingColorMap;

                    ExistingColorMap.load (MapName, mGroupName);

                    // Get codec according to filename extension
                    size_t colorpos = MapName.find_last_of(".");
                    String colormPureFilename = MapName.substr(0, colorpos + 1);
           
                    ExistingColorMap.resize (wtiles,htiles);

                    olddir = ChangeToDir (const_cast< char * > (OutDirectory.c_str ()));
                    Splitter.saveExt (colormPureFilename + ".Small." + mTextureExt, ExistingColorMap.getData(), 
                                 static_cast <uint> (wtiles), 
                                static_cast <uint> (htiles),
                                3);
                    RetablishDir (olddir);

            }


        }
*/
        // 
        // Split Normal Map + HeightMap in alpha channel in 2^n + 1 part  
        // 
 /*       if (mGenHeightNormalMap)
        {		
            const uint W = MapUtil::getSingleton().getMapWidth ();
            const uint H  = MapUtil::getSingleton().getMapHeight ();
            const uint hn_map_size = W*H;
            uchar *data = new uchar[hn_map_size];            
            const Real divider = 1.0f / getMaxTheoHeight() * 256;
            Real *hdata = mData;
            for ( uint i = 0; i < hn_map_size; i++ )            
            { 
                data[i] = static_cast <uchar>  (hdata[i] * divider);
            }
            Image grayheightmap;
          
            grayheightmap.loadDynamicImage (data, static_cast <size_t> (W),
                                            static_cast <size_t> (H), 1, PF_L8, true);

            Image *NormalMap = MergeModulater.MergeGrayAndRgbInRgba (&grayheightmap,
                                                                NormalMapper.getNormalMap ());
              
            Splitter.saveAndSpaceAndSplitMap (mPureFilename + ".HN." + mExt,
                                                NormalMap);
            delete NormalMap;
        }   */
          
        // 
        // If we does calculate Light or slope We need Normals.
        // 
        if (mGenCoverageMap ||
            mGenBaseMap || 
            mGenRGBMaps || 
            mGenAlphaMaps || 
            mGenAlphaSplatRGBAMaps || 
            mGenAlphaSplatLightMaps || 
            mGenLightMap || 
            mGenShadowMap ||
            mGenLitColorMapGenerate ||
            mGenLitColorMapSplit ||
            mGenLitBaseMap)
        {
            mNormalMap = NormalMapper.getNormalMap ();
            mNormalMapBpp = PixelUtil::getNumElemBytes (mNormalMap->getFormat ());
            mNormalMapData = mNormalMap->getData();
            mNormalMapDataSize = static_cast <uint> (mNormalMap->getSize());
        }

        // 
        // Split Normal Map in 2^n  part  
        // 
        if (mGenNormalMap)
        {
                Splitter.saveAndSpaceAndSplitMap (mPureFilename + String(".N.") + mTextureExt,
                                                    mNormalMap);
        } // if (mGenNormalMap)

        // 
        // Split Light Map in 2^n  part  
        // 
        if (mGenLightMap)
            Splitter.saveAndSplitMap (mPureFilename + String(".Light.") + mTextureExt,
                                    LightMapper.getLightMap (mSun, mAmb, mDiff, mBlur));
        // 
        // Split Shadow Map in 2^n  part (shadow is light map inverted.)
        // 
        if (mGenShadowMap)
            Splitter.saveAndSplitMap (mPureFilename + String(".Shadow.") + mTextureExt,
                                    LightMapper.getShadowMap (mSun, mAmb, mDiff, mBlur));

        // 
        // Generate a Color Map in 2^n  part 
        // 
        if (mGenColorMapGenerate)
            Splitter.saveAndSplitMap (mPureFilename + String(".Color.") + mTextureExt,
                                    SplatMapper.getColorMap ());

        // 
        // Lit Generated Color Map in 2^n  part 
        // 
        if (mGenLitColorMapGenerate)
        { 
            // does modify color map.
            MergeModulater.ModulateGrayAndRgbInRgb (LightMapper.getLightMap (mSun, mAmb, mDiff, mBlur),
                                                    SplatMapper.getColorMap (),
                                                    2);
            Splitter.saveAndSplitMap (mPureFilename + String(".LitColor.") + mTextureExt,
                                    SplatMapper.getColorMap ());
        }


        // 
        // If user want to use its Own Color Map
        //
        if (mGenColorMapSplit || 
            mGenLitColorMapSplit)
        {
            Image ExistingColorMap;

            String MapName = mColorMapName;
            if (MapName == mFileName)
                 OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
                        "HeightMap File and ColorMap file cannot be the same file", 
                        "MapUtil::ProcessOneFile");
       
            ExistingColorMap.load (MapName, mGroupName);
            // Get codec according to filename extension
            size_t colorpos = MapName.find_last_of(".");
            String colormPureFilename = MapName.substr(0, colorpos + 1);

            const uint oldheight = mHeight;
            const uint oldwidth = mWidth;

            ExistingColorMap.resize(static_cast <ushort> (ExistingColorMap.getWidth ()+1), 
                                    static_cast <ushort> (ExistingColorMap.getHeight ()+1));

            mHeight = static_cast <uint> (ExistingColorMap.getHeight ());
            mWidth = static_cast <uint> (ExistingColorMap.getWidth ());

            Splitter.init ();
            Splitter.setRawSave (StringUtil::match(mTextureExt, "raw", false));
            // 
            // Split User Color Map in 2^n  part 
            // 
            if (mGenColorMapSplit)
            {
              Splitter.saveAndSplitMap (colormPureFilename + "." + mTextureExt,
                                        &ExistingColorMap);
            }
            // 
            // Lit User Color Map  in 2^n  part 
            // 
            if (mGenLitColorMapSplit)
            { 
                // does modify colormap.
                MergeModulater.ModulateGrayAndRgbInRgb (LightMapper.getLightMap (mSun, mAmb, mDiff, mBlur),
                                                        &ExistingColorMap,
                                                        2);
                Splitter.saveAndSplitMap (colormPureFilename + String(".Lit.") + mTextureExt,
                                        &ExistingColorMap);
            }   

            mHeight = oldheight;
            mWidth = oldwidth;
            Splitter.init ();
            Splitter.setRawSave (StringUtil::match(mTextureExt, "raw", false));
        }

       
        // 
        // Split Base Map in 2^n  part 
        // 
        if (mGenBaseMap)
            Splitter.saveAndSplitMap (mPureFilename + String(".Base.") + mTextureExt,
                                    SplatMapper.getBaseMap ());
        // 
        // Split Coverage Map in 2^n  part 
        // 
        if (mGenCoverageMap)
        {
            //MapBlurrer Blurrer;

            // blur.
            //Blurrer.gaussianBlur (SplatMapper.getCoverageMap (), 2);

            Splitter.saveAndSplitMap (mPureFilename + String(".Coverage.") + mTextureExt,
                                    SplatMapper.getCoverageMap ());
        }
        // 
        // Split Lit Base Map in 2^n  part 
        // 
        if (mGenLitBaseMap)
        { 
                // does modify Base Map.
                MergeModulater.ModulateGrayAndRgbaInRgba (LightMapper.getLightMap (mSun, mAmb, mDiff, mBlur),
                                                    SplatMapper.getBaseMap (),
                                                    2);

                Splitter.saveAndSplitMap (mPureFilename + String(".LitBase.") + mTextureExt,
                                            SplatMapper.getBaseMap ());
        }

       

        // 
        // Split Alpha Map in 2^n  part 
        // (each alpha is a color channel of coverage Map)
        //
        if (mGenAlphaMaps)
            for (uint i = 0; i < 4; i++)
        {
                String alphaname =  mPureFilename + String(".Alpha.") + 
                                            StringConverter::toString(i) + 
                                            String(".") + mTextureExt;
                Splitter.saveAndSplitMap (alphaname, 
                                        SplatMapper.getAlphaMap (i));
        }

        // 
        // Merge Alpha Map and an empty RGBA map
        // Split it in 2^n  part 
        //
        if (mGenAlphaSplatRGBAMaps)
        {            
            const uint W = MapUtil::getSingleton().getMapWidth ();
            const uint H  = MapUtil::getSingleton().getMapHeight ();
            const uint size = W*H * 3;
            uchar *data = new uchar[size];  
            memset (data, 0, size);

            Image RgbImage;
            RgbImage.loadDynamicImage  (data, static_cast <size_t>(W), 
                static_cast <size_t>(H), 1, PF_BYTE_RGB, true); 
            
            for (uint i = 0; i < 4; i++)
            {
                String alphaname =  mPureFilename + String(".AlphaSplatRGBAMaps.") + StringConverter::toString(i) + 
                    String(".") + mTextureExt;
                // does not modify AlphaMap, allocate a new image
                mAlphaMap[i] = MergeModulater.MergeGrayAndRgbInRgba (
                                                SplatMapper.getAlphaMap (i),
                                                &RgbImage);
                Splitter.saveAndSplitMap (alphaname, 
                                            mAlphaMap[i]);
                if (!mGenAlphaSplatLightMaps)
                    delete mAlphaMap[i];                
            }
        }

        // 
        // Merge Alpha Map and Light map
        // Split it in 2^n  part 
        //
        if (mGenAlphaSplatLightMaps)
            {
                for (uint i = 0; i < 4; i++)
                {
                    String alphaname =  mPureFilename + String(".AlphaSplatLightMaps.") + StringConverter::toString(i) + 
                        String(".") + mTextureExt;
                    // does not modify AlphaMap, allocate a new image
                    // does prepare a new AlphaMap for merging.
                    if (!mGenAlphaSplatRGBAMaps)
                        mAlphaMap[i] = MergeModulater.MergeAlphaAndRgbInRgba (LightMapper.getLightMap (mSun, mAmb, mDiff, mBlur),
                                                SplatMapper.getAlphaMap (i));

                    // does modify mAlphaMap[i]
                    MergeModulater.MergeGrayAndRgbaInRgba (LightMapper.getLightMap (mSun, mAmb, mDiff, mBlur),
                                                            mAlphaMap[i]);

                    Splitter.saveAndSplitMap (alphaname, 
                                                mAlphaMap[i]); 
                    delete mAlphaMap[i];   
                }
            }

        // 
        // Split RGB Map in 2^n  part 
        //       
        if (mGenRGBMaps)
            for (uint i = 0; i < 4; i++)
            {
                String RGBname =  mPureFilename + String(".RGB.") + StringConverter::toString(i) + 
                    String(".") + mTextureExt;
                Splitter.saveAndSplitMap (RGBname, 
                    SplatMapper.getRGBMap (i));
            }


        // 
        // create a Potential Visibility Set map 
        //    
        if (mGenPVSMap)
        {
            MapVis PVSMapper;

            PVSMapper.calc (32, 20);
            PVSMapper.save (const_cast< char * > ((mPureFilename + String(".vis")).c_str()));
        }

       
        // 
        // create a Horizon map 
        // That let us calculate mountains shadows
        //    
        MapHorizon HorizonMapper;
        //MapHorizon2 HorizonMapper;// maybe prettier

        if (mGenHorizonMap)
        {

            uint horizon_samples = 2;
            uint vscale = 7;
            uint steps = 8;
            //vscale = 20; step = 16; horizon_samples = 16;
            //HorizonMapper.calcHorizon3DMap (vscale, horizon_samples, steps);

            //vscale = 7; step = 16; horizon_samples = 128;
            HorizonMapper.calcHorizon3DMap (vscale, horizon_samples, steps);//2
            for (uint k = 0; k < horizon_samples; k++) 
            {  
                String HCMname = mPureFilename + String(".HCM_") + StringConverter::toString(k) + String(".")  + mTextureExt;

                MapBlurrer Blurrer;

                // blur.
                //Blurrer.gaussianBlur (HorizonMapper.getHorizon3DMap (k), 2);

                Splitter.saveAndSplitMap (HCMname, 
                                            HorizonMapper.getHorizon3DMap (k));
            }
        }

        // 
        // create an Elevation Map
        // Used as lookup table to get shadows position 
        //    
        if (mGenElevationMap)
        { 
            HorizonMapper.calcElevationCubeMap (256);

            const String cubename[6] = 
            {
                "rt", "lf", 
                "up", "dn", 
                "fr", "bk"
            };


            for (uint k = 0; k < 6; k++) 
            {
                String Elevname = mPureFilename + String(".Elev_") + cubename[k] + String(".png");
                Splitter.saveAndSplitMap (Elevname, 
                                            HorizonMapper.getElevationCubeMap (k));

            }  
        }

    //
    ////  let us calculate mountains shadows
    ////    
        if (mGenZHorizon)
        {  
            MapHorizon3 HorizonMapper3;

            HorizonMapper3.calcHorizonMap ();
            String hname = mPureFilename + String(".HS.") + mTextureExt;
            Splitter.saveAndSplitMap (hname, HorizonMapper3.getHorizonMap ());

            HorizonMapper3.calcLightMap ();
            String lname = mPureFilename + String(".L.") + mTextureExt;
            Splitter.saveAndSplitMap (lname, HorizonMapper3.getLightMap ());
        }       

        NormalMapper.reset();

#ifndef _MAPEDITOR
        mHeightdata->unload();
        delete mHeightdata;
#endif // _MAPEDITOR
    }


}//namespace Ogre
