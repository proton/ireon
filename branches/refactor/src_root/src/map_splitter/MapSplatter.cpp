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
MapSplatter.cpp
@brief
Utility that pre-calculate splating by
pre-calculating coverage map, A Color Map and a Base Map.
*/
#include "MapUtil.h"
#include "MapSplatter.h"
#include "MapBlurrer.h"

#include <iostream>

namespace Ogre
{
//----------------------------------------------------------------------
MapSplatter::MapSplatter()
{
    reset();
}
//----------------------------------------------------------------------
MapSplatter::~MapSplatter()
{
   
}
//----------------------------------------------------------------------
Image *MapSplatter::getBaseMap()
{
    CalcMapSplats ();
    return &BaseMap;
}
//----------------------------------------------------------------------
Image *MapSplatter::getCoverageMap()
{
    CalcMapSplats ();
    return &CoverageMap;
}
//----------------------------------------------------------------------
Image *MapSplatter::getColorMap()
{
    CalcMapSplats ();
    return &ColorMap;
}
//----------------------------------------------------------------------
Image *MapSplatter::getAlphaMap(uint i)
{
    CalcMapSplats ();
    return &destAlphaMap[i];
}
//----------------------------------------------------------------------
Image *MapSplatter::getRGBMap(uint i)
{
    CalcMapSplats ();
    return &destRGBMap[i];
}
//----------------------------------------------------------------------
void MapSplatter::reset()
{
    mAlreadyComputed = false;  
}


//----------------------------------------------------------------------
void MapSplatter::CalcMapSplats()
{
    if (mAlreadyComputed)
        return;

    const bool b_Base = MapUtil::getSingleton().mGenBaseMap || 
        MapUtil::getSingleton().mGenLitColorMapGenerate;
    
    const bool b_RGB = MapUtil::getSingleton().mGenRGBMaps;

    const bool b_Color = MapUtil::getSingleton().mGenColorMapGenerate || 
        MapUtil::getSingleton().mGenLitColorMapGenerate;

    const bool b_Alpha = MapUtil::getSingleton().mGenAlphaMaps || 
        MapUtil::getSingleton().mGenAlphaSplatLightMaps  ||
        MapUtil::getSingleton().mGenAlphaSplatRGBAMaps;

    const bool b_Coverage = MapUtil::getSingleton().mGenCoverageMap;
    ///
    if (b_Color)
    {
        std::cout << "Loading Splats Maps \n";

        //snow  = 0
        MapUtil::getSingleton().load(MapUtil::getSingleton().Splat_Filename_0, 
            &SplatMap[0]);
        //sand  = 1
        MapUtil::getSingleton().load(MapUtil::getSingleton().Splat_Filename_1, 
            &SplatMap[1]);
        //grass = 2
        MapUtil::getSingleton().load(MapUtil::getSingleton().Splat_Filename_2, 
            &SplatMap[2]);
        //rock  = 3
        MapUtil::getSingleton().load(MapUtil::getSingleton().Splat_Filename_3, 
            &SplatMap[3]);
    }

     
    const uint terrain_height = MapUtil::getSingleton().getMapHeight ();
    const uint terrain_width = MapUtil::getSingleton().getMapWidth ();

    const uint size1 = terrain_height * terrain_width;
    const uint size3 = size1 + size1 + size1;
    const uint size4 = size3 + size1;

    uchar *BaseData;
    uchar *ColorData;
    uchar *CoverageData;
    uchar *destAlphaMapData0;
    uchar *destAlphaMapData1;
    uchar *destAlphaMapData2;
    uchar *destAlphaMapData3;
    uchar *destRGBMapData0;
    uchar *destRGBMapData1;
    uchar *destRGBMapData2;
    uchar *destRGBMapData3;

    if (b_Base) 
    {
        //std::cout << "Base ";
         // Create the base image 
        uchar *dataRGB = new uchar[size3];
        BaseMap.loadDynamicImage (dataRGB, (terrain_width), 
            (terrain_height), 1, PF_BYTE_RGB, true);
        BaseData = BaseMap.getData();
    }
    if (b_Color) 
    {
        //std::cout << "Color ";
        // Create the color texture
        uchar *dataRGB = new uchar[size3];
        ColorMap.loadDynamicImage (dataRGB, (terrain_width), 
            (terrain_height), 1, PF_BYTE_RGB, true);
        ColorData = ColorMap.getData();
    }

    if (b_Coverage) 
    {  
        //std::cout << "Coverage ";
        uchar *dataRGBA = new uchar[size4];  
        CoverageMap.loadDynamicImage (dataRGBA, (terrain_width), 
            (terrain_height), 1, PF_BYTE_RGBA, true);
        CoverageData = CoverageMap.getData();
    } 
    if (b_Alpha) 
    {      
        //std::cout << "Alpha ";
        // alpha maps used as coverage
        //without having to decompose it in real-time

        // create transparent Alpha images
        // loadDynamicImage  does not allocate and memcpy data
        uchar *dataGray = new uchar[size1];
        memset (dataGray, 0, size1);
        destAlphaMap[0].loadDynamicImage (dataGray, (terrain_width), 
            (terrain_height), 1, PF_A8, true);

        dataGray = new uchar[size1];
        memset (dataGray, 0, size1);
        destAlphaMap[1].loadDynamicImage (dataGray, (terrain_width), 
            (terrain_height), 1, PF_A8, true);

        dataGray = new uchar[size1];
        memset (dataGray, 0, size1);
        destAlphaMap[2].loadDynamicImage (dataGray, (terrain_width), 
            (terrain_height), 1, PF_A8, true); 

        dataGray = new uchar[size1];
        memset (dataGray, 0, size1);
        destAlphaMap[3].loadDynamicImage (dataGray, (terrain_width), 
            (terrain_height), 1, PF_A8, true);

        destAlphaMapData0 = destAlphaMap[0].getData ();
        destAlphaMapData1 = destAlphaMap[1].getData ();
        destAlphaMapData2 = destAlphaMap[2].getData ();
        destAlphaMapData3 = destAlphaMap[3].getData ();
    }
    if (b_RGB)
    {
        //std::cout << "RGB ";
        // rgb maps used as coverage
        // without having to decompose it in real-time
        // actually they are gray images...

        // create back rgb images
        // loadDynamicImage  does allocate and memcpy data, so no need
        // to reallocate a new pointer using DC
        uchar *dataRGB = new uchar[size3];
        destRGBMap[0].loadDynamicImage (dataRGB, (terrain_width), 
            (terrain_height), 1, PF_BYTE_RGB, true);
        dataRGB = new uchar[size3];
        destRGBMap[1].loadDynamicImage (dataRGB, (terrain_width), 
            (terrain_height), 1, PF_BYTE_RGB, true);
        dataRGB = new uchar[size3];
        destRGBMap[2].loadDynamicImage (dataRGB, (terrain_width), 
            (terrain_height), 1, PF_BYTE_RGB, true);
        dataRGB = new uchar[size3];
        destRGBMap[3].loadDynamicImage (dataRGB, (terrain_width), 
            (terrain_height), 1, PF_BYTE_RGB, true);

        destRGBMapData0 = destRGBMap[0].getData ();
        destRGBMapData1 = destRGBMap[1].getData ();
        destRGBMapData2 = destRGBMap[2].getData ();
        destRGBMapData3 = destRGBMap[3].getData ();
    }

    // each splatting texture can have different 
    // height, width and pixel size
    uchar *SplatMapData0 = SplatMap[0].getData();
    uchar *SplatMapData1 = SplatMap[1].getData();
    uchar *SplatMapData2 = SplatMap[2].getData();
    uchar *SplatMapData3 = SplatMap[3].getData();

    uint SplatMapWidth0;
    uint SplatMapWidth1;
    uint SplatMapWidth2;
    uint SplatMapWidth3;

    uint SplatMapHeight0;
    uint SplatMapHeight1;
    uint SplatMapHeight2;
    uint SplatMapHeight3;

    uint SplatMapbpp0;
    uint SplatMapbpp1;
    uint SplatMapbpp2;
    uint SplatMapbpp3;

    if (b_Color)
    {
       SplatMapWidth0 = static_cast <uint> (SplatMap[0].getWidth ());
       SplatMapWidth1 = static_cast <uint> (SplatMap[1].getWidth ());
       SplatMapWidth2 = static_cast <uint> (SplatMap[2].getWidth ());
       SplatMapWidth3 = static_cast <uint> (SplatMap[3].getWidth ());

       SplatMapHeight0 = static_cast <uint> (SplatMap[0].getHeight ());
       SplatMapHeight1 = static_cast <uint> (SplatMap[1].getHeight ());
       SplatMapHeight2 = static_cast <uint> (SplatMap[2].getHeight ());
       SplatMapHeight3 = static_cast <uint> (SplatMap[3].getHeight ());

       SplatMapbpp0 = static_cast <uint> (PixelUtil::getNumElemBytes (SplatMap[0].getFormat ()));
       SplatMapbpp1 = static_cast <uint> (PixelUtil::getNumElemBytes (SplatMap[1].getFormat ()));
       SplatMapbpp2 = static_cast <uint> (PixelUtil::getNumElemBytes (SplatMap[2].getFormat ()));
       SplatMapbpp3 = static_cast <uint> (PixelUtil::getNumElemBytes (SplatMap[3].getFormat ()));
    }
    bool bSplatMap0NotUsed = true;
    bool bSplatMap1NotUsed = true;
    bool bSplatMap2NotUsed = true;
    bool bSplatMap3NotUsed = true;

    uint SplatMap_index_pixel0 = 0;
    uint SplatMap_index_pixel1 = 0;
    uint SplatMap_index_pixel2 = 0;
    uint SplatMap_index_pixel3 = 0;

    uint index_1pixel = 0;
    uint index_3pixel = 0;
    uint index_4pixel = 0;


            matColor[0] = MapUtil::getSingleton().matColor[0];
            matColor[1] = MapUtil::getSingleton().matColor[1];
            matColor[2] = MapUtil::getSingleton().matColor[2];
            matColor[3] = MapUtil::getSingleton().matColor[3];

            mScale  = MapUtil::getSingleton().scale;

            matHeight[1] = MapUtil::getSingleton().matHeight[1];
            matHeight[2] = MapUtil::getSingleton().matHeight[2];
     // from http://www.tayloredmktg.com/rgb/

            ColourValue colors[4];
          
            colors[0] = MapUtil::getSingleton().matColor[0];
            colors[1] = MapUtil::getSingleton().matColor[1];
            colors[2] = MapUtil::getSingleton().matColor[2];
            colors[3] = MapUtil::getSingleton().matColor[3];


            Real heights[3];
            const Real maxHeight = MapUtil::getSingleton().getMaxHeight ();

            heights[0] = 0.0f;
            heights[1] = matHeight[1];
            heights[2] = MapUtil::getSingleton().getMaxHeight ();

            Real dividers[3];	
            dividers[0] = 1.0f;

            if (heights[1] > 0)
                dividers[1] = 1 / heights[1];
            else 
                dividers[1] = 0.0f;

            if ((heights[2]  - heights[1]) > 0)
                dividers[2] = 1 / (heights[2]  - heights[1]);
            else 
                dividers[2] = 0.0f;

            Real alpha[3];

    // This texture will be used as a base color for the terrain, 
    // it will fake the splat for distant renderables.
    std::cout << "Map Calc : ";
    uint j = 0;
    
    //const Real * const heightmap = MapUtil::getSingleton().getHeightData ();
    const Vector3 *  const normal = MapUtil::getSingleton().getNormalData ();
    const uchar bScale = 255;

    uint curr_row = 0;
    while (j < terrain_height)
    { 
        DEBUG_PROGRESS_OUTPUT(".")
        uint i = 0;
        while (i < terrain_width)
       {
                //slope of current point (the y value of the normal)
                const Real Slope1 = normal[i + curr_row].y;
				Real Slope = (2.0f * Ogre::Math::ACos(Slope1) / Ogre::Math::PI).valueRadians();
				Slope *= heights[2];

                uint indx = 1;
                while (Slope > heights[indx])
                    indx++;  

                const Real interpol = (Slope  - heights[indx - 1]) * dividers[indx];  
                

                //ColourValue color;
                alpha[0] = 0.0f;
                alpha[1] = 0.0f;
                alpha[2] = 0.0f;

		/*               if (Slope < 0.05f)// speed-up as it's invisible
                {
                    const Real B = (1.0f - interpol);
                    const Real C = interpol;

                    alpha[indx - 1] = B;
                    alpha[indx] = C; 

                    if (b_Base)
                    { 
                        BaseData[ index_3pixel     ] = static_cast <uchar> ((colors[indx - 1].r * B + colors[indx].r * C)* bScale);
                        BaseData[ index_3pixel + 1 ] = static_cast <uchar> ((colors[indx - 1].g * B + colors[indx].g * C)* bScale);
                        BaseData[ index_3pixel + 2 ] = static_cast <uchar> ((colors[indx - 1].b * B + colors[indx].b * C)* bScale);
                    }
                }
                else */
                {
                    const Real A = (1.0f);
                    const Real B = A * (1.0f - interpol);
                    const Real C = A * interpol;
                    
                    alpha[indx - 1] = B;
                    alpha[indx] = C; 

//                    alpha[ 2 ] = alpha[ 2 ] + Slope;
//                    alpha[ 2 ] = alpha[ 2 ] > 1.0f ? 1.0f : alpha[ 2 ];
                   
                    if (b_Base)
                    { 
                        BaseData[ index_3pixel    ] = static_cast <uchar> ((colors[indx - 1].r * B + colors[indx].r * C)* bScale);
                        BaseData[ index_3pixel + 1] = static_cast <uchar> ((colors[indx - 1].g * B + colors[indx].g * C)* bScale);
                        BaseData[ index_3pixel + 2] = static_cast <uchar> ((colors[indx - 1].b * B + colors[indx].b * C)* bScale);            
                    }
                }

                const Real alpha0 = alpha[ 0 ] / 2;
                const Real alpha1 = alpha[ 1 ] / 2;
                const Real alpha2 = alpha[ 2 ] / 2;

                const uchar splat1 = static_cast <uchar> (alpha0 * bScale);
                const uchar splat2 = static_cast <uchar> (alpha1 * bScale);
                const uchar splat3 = static_cast <uchar> (alpha2 * bScale);

            if (b_Color)
            {
                //const uchar inv_alpha = static_cast <uchar>(( 1.0 - ( alpha0 + alpha1 + alpha2 + alpha3 ) ) * Bscale);
            
                //Red
                ColorData[index_3pixel] = static_cast <uchar>((//color.r * inv_alpha +
                                SplatMapData0[SplatMap_index_pixel0] * alpha0 + 
                                SplatMapData1[SplatMap_index_pixel1] * alpha1 + 
                                SplatMapData2[SplatMap_index_pixel2] * alpha2));  
                //Green
                ColorData[index_3pixel + 1] = static_cast <uchar>((//color.g * inv_alpha +
                            SplatMapData0[SplatMap_index_pixel0 + 1] * alpha0 + 
                            SplatMapData1[SplatMap_index_pixel1 + 1] * alpha1 + 
                            SplatMapData2[SplatMap_index_pixel2 + 1] * alpha2));
                //Blue
                ColorData[index_3pixel + 2] = static_cast <uchar>((//color.b * inv_alpha +
                            SplatMapData0[SplatMap_index_pixel0 + 2] * alpha0 + 
                            SplatMapData1[SplatMap_index_pixel1 + 2] * alpha1 + 
                            SplatMapData2[SplatMap_index_pixel2 + 2] * alpha2));
             
            }
           

            // Generate the alpha map 1
            if (bSplatMap0NotUsed && alpha0 - 0.05f > 0)
                bSplatMap0NotUsed  = false;

            // Generate the alpha map 2
            if (bSplatMap1NotUsed && alpha1 - 0.05f > 0)
                bSplatMap1NotUsed  = false;

            // Generate the alpha map 3
            if (bSplatMap2NotUsed && alpha2 - 0.05f > 0)
                bSplatMap2NotUsed  = false;


            if (b_Coverage)
            {
                CoverageData[index_4pixel] = splat1;// stored as Red
                CoverageData[index_4pixel + 1] = splat2;// stored as Green
                CoverageData[index_4pixel + 2] = splat3;// stored as Blue
            }
            if (b_Alpha)
            {
                destAlphaMapData0[index_1pixel] = splat1;
                destAlphaMapData1[index_1pixel] = splat2;
                destAlphaMapData2[index_1pixel] = splat3;
            }
            if (b_RGB)
            {
                destRGBMapData0[index_3pixel] = splat1;
                destRGBMapData0[index_3pixel + 1] = splat1;
                destRGBMapData0[index_3pixel + 2] = splat1;

                destRGBMapData1[index_3pixel] = splat2;
                destRGBMapData1[index_3pixel + 1] = splat2;
                destRGBMapData1[index_3pixel + 2] = splat2;

                destRGBMapData2[index_3pixel] = splat3;
                destRGBMapData2[index_3pixel + 1] = splat3;
                destRGBMapData2[index_3pixel + 2] = splat3;
            }
            // don't move it a the end 
            // it will false following computation
            // of position in splats maps
            i++;
            index_1pixel ++;
            index_3pixel += 3;
            index_4pixel += 4;

            ///
            if (b_Color)
            {
                SplatMap_index_pixel0 += SplatMapbpp0;
                if ((i % SplatMapWidth0) == 0)
                    SplatMap_index_pixel0 -= SplatMapWidth0*SplatMapbpp0;
                SplatMap_index_pixel1 += SplatMapbpp1;
                if ((i % SplatMapWidth1) == 0)
                    SplatMap_index_pixel1 -= SplatMapWidth1*SplatMapbpp1;
                SplatMap_index_pixel2 += SplatMapbpp2;
                if ((i % SplatMapWidth2) == 0)
                    SplatMap_index_pixel2 -= SplatMapWidth2*SplatMapbpp2;
            }
        }
        curr_row += terrain_width;
        j++;
        if (b_Color)
        {
            SplatMap_index_pixel0 = (j % SplatMapHeight0) * SplatMapWidth0 * SplatMapbpp0;
            SplatMap_index_pixel1 = (j % SplatMapHeight1) * SplatMapWidth1 * SplatMapbpp1;
            SplatMap_index_pixel2 = (j % SplatMapHeight2) * SplatMapWidth2 * SplatMapbpp2;
        }
    }
    // use bAlphaNotUsed to change coverage from 4 channel to less...
   
    std::cout << "\n";

    mAlreadyComputed = true;
}
//-----------------------------------------------------------------------
void MapSplatter::BuildPoint(ColourValue& out, int x, int z, Real& alpha1, 
                           Real& alpha2, Real& alpha3, Real& alpha4)
{
    // Init the colour
    
    alpha1 = 0.0f;
    alpha2 = 0.0f;
    alpha3 = 0.0f;
    alpha4 = 0.0f;


#define SNOW  0
#define SAND  1
#define GRASS 2
#define ROCK  3
    // Ask for the current height value and the 8 surrounding values
    Real height[9];
    if ( x == 0 )
    {
        x = 1;
    }
    else if (x == int (mWidth))
    {
        x = mWidth - 1;
    }
    if ( z == 0 )
    {
        z = 1;
    }
    else if (z == int (mHeight))
    {
        z = mHeight - 1;
    }
//    /*
//            h1
//      h0*---*---*h2
//        |0 /|2 /|
//        | / | / |
//        |/ 1|/ 3|
//      h3*---h4--*h5
//        |7 /|4 /|
//        | / | / |
//        |/ 6|/ 5|
//      h6*---*--*h8
//            h7
//            */  
    height[0] = MapUtil::getSingleton().getHeight( x - 1, z - 1 );		// Top-Left
    height[1] = MapUtil::getSingleton().getHeight( x, z - 1 );			// Top-Center
    height[2] = MapUtil::getSingleton().getHeight( x + 1, z - 1 );		// Top-Right
    height[3] = MapUtil::getSingleton().getHeight( x - 1, z );			// Left
    height[4] = MapUtil::getSingleton().getHeight( x, z );				// Current Point
    height[5] = MapUtil::getSingleton().getHeight( x + 1, z );			// Right
    height[6] = MapUtil::getSingleton().getHeight( x - 1, z + 1 );		// Bottom-Left
    height[7] = MapUtil::getSingleton().getHeight( x, z + 1 );			// Bottom-Center
    height[8] = MapUtil::getSingleton().getHeight( x + 1, z + 1 );		// Bottom-Right
   
    // Weight( pt1 , pt2 ) = 1 - DistanceSquared(pt1,pt2) / (1.75)^2
    
    //The sloppy test
    
	Real sloppy[8];
    const Real dx = MapUtil::getSingleton().scale.x;
	const Real dz = MapUtil::getSingleton().scale.z;
    const Real inv_dxdz = 1.0 / (dx+dz);
    const Real inv_dx = 1.0 / dx;
    const Real inv_dz = 1.0 / dz;

	sloppy[0] = Math::Abs ( height[0] - height[4] ) * inv_dxdz;
	sloppy[1] = Math::Abs ( height[1] - height[4] ) * inv_dz;
	sloppy[2] = Math::Abs ( height[2] - height[4] ) * inv_dxdz;
	sloppy[3] = Math::Abs ( height[3] - height[4] ) * inv_dx;
	sloppy[4] = Math::Abs ( height[5] - height[4] ) * inv_dx;
	sloppy[5] = Math::Abs ( height[6] - height[4] ) * inv_dxdz;
	sloppy[6] = Math::Abs ( height[7] - height[4] ) * inv_dz;
	sloppy[7] = Math::Abs ( height[8] - height[4] ) * inv_dxdz;
    
    out = ColourValue(0.0f, 0.0f, 0.0f, 0.0f);
    for ( uint i = 0; i < 7; i++ )
    {    
        if ( height[i] < matHeight[0] )
         {
            if ( sloppy[i] < 0.2f )
            {
                // grass-grass
                InterpolateColour( out, 1.0f, GRASS, GRASS );
                InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 1.0f, GRASS, GRASS );
            }
            if ( sloppy[i] >= 0.15f && sloppy[i] < 0.4f )
            {
                // sand-grass
                InterpolateColour( out, 0.25f, SAND, GRASS );
                InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.25f, SAND, GRASS );
            }
            if ( sloppy[i] >= 0.3f && sloppy[i] < 0.65f )
            {
                // sand-sand
                InterpolateColour( out, 1.0f, SAND, SAND );
                InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 1.0f, SAND, SAND );
            }
            if ( sloppy[i] >= 0.55f && sloppy[i] < 0.75f )
            {
                // sand-rock
                InterpolateColour( out, 0.75f, SAND, ROCK );
                InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.75f, SAND, ROCK );
            }
            if ( sloppy[i] >= 0.70 )
            {
                // rock-rock
                InterpolateColour( out, 1.0, ROCK, ROCK );
                InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 1.0f, ROCK, ROCK );
            }
        }
        else if ( height[i] < matHeight[1] )
        {
            if ( sloppy[i] < 0.15 )
            {
                // grass-snow
                InterpolateColour( out, 0.25f, GRASS, SNOW );
                InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.25f, GRASS, SNOW );
            }
            if ( sloppy[i] >= 0.10 && sloppy[i] < 0.45 )
            {
                // snow-sand
                InterpolateColour( out, 0.65f, SNOW, SAND );
                InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.65f, SNOW, SAND );
            }
            if ( sloppy[i] >= 0.25f && sloppy[i] < 0.65f )
            {
                // snow-rock
                InterpolateColour( out, 0.5, SNOW, ROCK );
                InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.5f, SNOW, ROCK );
            }
            if ( sloppy[i] >= 0.50f && sloppy[i] < 0.75f )
            {
                // snow-rock
                InterpolateColour( out, 0.75f, SNOW, ROCK );
                InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.75f, SNOW, ROCK );
            }
            if ( sloppy[i] >= 0.70 )
             {
                // rock-rock
                InterpolateColour( out, 1.0f, ROCK, ROCK );
                InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 1.0f, ROCK, ROCK );
            }
        }
        else
        {
            if ( sloppy[i] < 0.15f )
            {
                // snow-snow
                InterpolateColour( out, 1.0f, SNOW, SNOW );
                InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 1.0f, 0, 0 );
            }
            if ( sloppy[i] >= 0.1f && sloppy[i] < 0.45f )
            {
                // snow-sand
                InterpolateColour( out, 0.35f, SNOW, SAND );
                InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.35f, 0, 1 );
            }
            if ( sloppy[i] >= 0.25f && sloppy[i] < 0.65f )
            {
                // snow-rock
                InterpolateColour( out, 0.5f, SNOW, ROCK );
                InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.5f, 0, 3 );
            }
            if ( sloppy[i] >= 0.5f && sloppy[i] < 0.75f )
            {
                // snow-rock
                InterpolateColour( out, 0.75f, SNOW, ROCK );
                InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 0.75f, 0, 3 );
            }
            if ( sloppy[i] >= 0.7f )
            {
                // rock-rock
                InterpolateColour( out, 1.0f, ROCK, ROCK );
                InterpolateAlpha( alpha1, alpha2, alpha3, alpha4, 1.0f, 3, 3 );
            }
        }
     }
 }

//-----------------------------------------------------------------------
void MapSplatter::InterpolateColour(ColourValue& out, const Real percentaje, 
                                const int index1, const int index2)
{
    if (percentaje < 1.0)
    {   
        const Real tmp = 1 - percentaje;
        out.r = ( out.r + ( percentaje * matColor[index1].r + tmp * matColor[index2].r ) ) * 0.5f;
        out.g = ( out.g + ( percentaje * matColor[index1].g + tmp * matColor[index2].g ) ) * 0.5f;
        out.b = ( out.b + ( percentaje * matColor[index1].b + tmp * matColor[index2].b ) ) * 0.5f;
    }
    else if (index1 == index2)
    {
        out.r = matColor[index1].r;
        out.g = matColor[index1].g;
        out.b = matColor[index1].b;
    }
}

//-----------------------------------------------------------------------
void MapSplatter::InterpolateAlpha(Real& alpha1, Real& alpha2, Real& alpha3, 
                                 Real& alpha4, const Real percentaje, 
                                const int index1, const int index2)
    {
    if ( index1 == index2 )
     {
        if ( index1 == 0 )
        {
            alpha1 += 1.0;
        }
        else if ( index1 == 1 )
        {
            alpha2 += 1.0;
        }
        else if ( index1 == 2 )
         {
            alpha3 += 1.0;
        }
        else
        {
            alpha4 += 1.0;
        }
    }
    else
    {
        if ( index2 == 0 )
        {
            alpha1 += percentaje;
        }
        else if ( index2 == 1 )
        {
            alpha2 += percentaje;
         }
        else if ( index2 == 2 )
        {
            alpha3 += percentaje;
        }
        else
        {
            alpha4 += percentaje;
        }

        if ( index1 == 0 )
        {
            alpha1 += 1.0 - percentaje;
        }
        else if ( index1 == 1 )
        {
            alpha2 += 1.0 - percentaje;
        }
        else if ( index1 == 2 )
        {
            alpha3 += 1.0 - percentaje;
        }
        else
        {
            alpha4 += 1.0 - percentaje;
         }
    }
    //snow  = 0
    //sand  = 1
    //grass = 2
    //rock  = 3
    alpha1 *= ( 4 * 0.2f);
    alpha2 *= ( 3 * 0.2f);
    alpha3 *= ( 4 * 0.2f);
    alpha4 *= ( 2 * 0.2f);
    // Normalize
    const Real total = 1 / (( alpha1 + alpha2 + alpha3 + alpha4 ) * 2);
    alpha1 = alpha1 * total;
    alpha2 = alpha2 * total;
    alpha3 = alpha3 * total;
    alpha4 = alpha4 * total;
}
}//namespace Ogre
