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
MapHorizon3.cpp
@brief
Utility that pre-calculate horizon mapping for real-time
light mapping of terrain
*/

#include "MapUtil.h"
#include "MapHorizon3.h"
#include "MapBlurrer.h"



namespace Ogre
{


    MapHorizon3::MapHorizon3()
    {   
        mHorizonImage = 0;
        mLightImage = 0;
        mShadowMapDatapos = 0;
        mShadowMapDataneg = 0;
    }
    MapHorizon3::~MapHorizon3()
    {
        if (mHorizonImage)
            delete mHorizonImage;  

        if (mLightImage)
            delete mLightImage;   
         
        if (mShadowMapDatapos)
            delete[] mShadowMapDatapos;
          
        if (mShadowMapDataneg)
            delete[] mShadowMapDataneg;
    }

    Image *MapHorizon3::getHorizonMap()
    {
        return mHorizonImage;
    }

    Image *MapHorizon3::getLightMap()
    {
        return mLightImage;
    }


    void MapHorizon3::calcLightMap()
    {
        const uint W = MapUtil::getSingleton().getMapWidth ();
        const uint H  = MapUtil::getSingleton().getMapHeight ();

	    // Create the base image
        uchar *BaseData = new uchar[ W * H ];

        //        mSun = MapUtil::getSingleton().Sun;
        //        mAmb = MapUtil::getSingleton().Amb;
        //        mDiff = MapUtil::getSingleton().Diff;
        //        mBlur = MapUtil::getSingleton().Blur;

        //double rotationAmount = 2 * Math::PI / 16.0f;
        //rotationAmount = 4 * rotationAmount;
        //Real LightAngle = rotationAmount;
        //    Vector3 LightDir (1.0f,
        //                      Math::Cos (rotationAmount),
        //	                  0.0f);

        //(float)Math::Sin(0.7f * 300.0f) * 1.55f;
        //1.531; //all shadow
        //0.49; //all shadow

        Real LightAngle = 0.20f;
        Vector3 LightDir(0,
	                    (float) - Math::Cos(LightAngle),
	                    (float) Math::Sin(LightAngle));
        
        //assert(LightDir.y > 0.0f);
      
        LightDir.normalise ();
 
        Real *HorizonAngle;
        if (LightAngle >= 0) 
        {
	    LightAngle = 1.0f - LightAngle;
            HorizonAngle = mShadowMapDatapos;
	} 
        else 
        {
	    LightAngle = 1.0f + LightAngle;
            HorizonAngle = mShadowMapDataneg;
	}
	

	// This texture will be used as a base color for the terrain, it will fake the splat for distant renderables.
	uint curr_row = 0;
        const uchar BScale = 255;
        for(uint nZ = 0; nZ < H ; nZ++) 
        {
            DEBUG_PROGRESS_OUTPUT(".")
            for(uint nX = 0; nX < W; nX++)
            {            
                const uint nVert = curr_row + nX;    

                Real intensity;
                if (LightAngle < HorizonAngle[nVert]) 
                {                            
					intensity = 1.0f - (-10.0f * (LightAngle - HorizonAngle[nVert]));
                    if (intensity > 0.0f) 
                    {
                        // if Vertex is at least partially lighted
                        intensity *= 1.0f;
                        //intensity *= LightDir.dotProduct (MapUtil::getSingleton ().getNormalAt (nX, nZ)); 
					}
                    else
                        intensity = 0.0f; // totally in shadow                             
                }
                else                            
                    // if Vertex is lighted
                    intensity = 1.0f;
                    //intensity = LightDir.dotProduct (MapUtil::getSingleton ().getNormalAt (nX, nZ)); 
               
                //intensity = LightDir.dotProduct (MapUtil::getSingleton ().getNormalAt (nX, nZ));
               
                BaseData[nVert] = static_cast <uchar> (std::max(intensity, 0.0f) * BScale);
                //curr_image_pos += 1;
            }
            curr_row += W;
        }  
        mLightImage = new Image();        
        mLightImage->loadDynamicImage (BaseData, 
            static_cast <size_t> (W), 
            static_cast <size_t> (H), 1, PF_L8, true);
          /* Blur the light map */  
//        int Blur  = 1;
//        if (Blur > 0)
//        {
//            MapBlurrer Blurrer;
//            Blurrer.gaussianBlur(mLightImage, Blur);
//        }

        //Convolver *c = new Convolver(5);
        //c->blurGray (mLightImage);
    }

    void MapHorizon3::calcHorizonMap()
    {
        const Vector3 scale = MapUtil::getSingleton().scale;
        mHeightMapData = MapUtil::getSingleton().getHeightData ();

        const Real *heightmap = mHeightMapData;
        const uint W = MapUtil::getSingleton().getMapWidth ();
        const uint H  = MapUtil::getSingleton().getMapHeight ();

        size_t size = W*H;

        
        mShadowMapDatapos = new Real [size];
        mShadowMapDataneg = new Real [size];
	
        memset (mShadowMapDatapos, 0, sizeof(Real) * size);
        memset (mShadowMapDataneg, 0, sizeof(Real) * size);

        size *= 3;//

        uchar *horizonmap2d = new uchar[size];
        memset (horizonmap2d, 0, size * sizeof (uchar));


        std::cout << "Horizon Map Calc : ";
        const uchar BScale = 255;
        for(uint nX = 0; nX < W; nX++)
        {

            DEBUG_PROGRESS_OUTPUT(".")
            Real LastElevation = 0;
            Real LastElevationinv = 0;

            uint lastelevation_pos = 0;
            uint lastelevationinv_pos = 0;

            for(uint nZ = 0; nZ < H ; nZ++) 
            {
                // positive Z axis shadow
                {
                    if (LastElevation)
                        LastElevation -= scale.z;

                    const uint nVert = nX + nZ * W;
                    const Real height = heightmap[nVert];
                    if (height < LastElevation)
                    {   
                        const Real diffheight = LastElevation - height;
                        const uint pos = static_cast <uint> ((nZ - lastelevation_pos) * scale.z);
                        const Real SunAngleShadow = Math::Abs((static_cast <Real> (diffheight) / Math::Sqrt(pos*pos + diffheight*diffheight)));
                        horizonmap2d[nVert*3 + 0] = static_cast <uchar>(SunAngleShadow * BScale);      
                        mShadowMapDatapos[nVert] = SunAngleShadow;
                    }
                    else
                    {
                        LastElevation = std::max (LastElevation, height);  
                        lastelevation_pos = nZ;
                    }
                }

                // negative Z axis shadow
                {
                    
                    if (LastElevationinv)
                        LastElevationinv -= scale.z;

                    const uint nVert = nX + (H - 1 - nZ) * W;
                    const Real height = heightmap[nVert];
                    if (height < LastElevationinv)
                    {   
                        const Real diffheight = LastElevationinv - height;
                        const uint pos = static_cast <uint> ((lastelevationinv_pos - (H - nZ)) * scale.z);
                        const Real SunAngleShadow = Math::Abs((static_cast <Real> (diffheight) / Math::Sqrt(pos*pos + diffheight*diffheight)));
                        horizonmap2d[nVert*3 + 1] = static_cast <uchar> (SunAngleShadow * BScale);                
                        mShadowMapDataneg[nVert] = SunAngleShadow;
                    }
                    else
                    {
                        LastElevationinv = std::max (LastElevationinv, height); 
                        lastelevationinv_pos = H - nZ;
                    }
                }
            }
        }
        
        mHorizonImage = new Image();
        mHorizonImage->loadDynamicImage (horizonmap2d, static_cast <size_t> (W), 
            static_cast <size_t> (H), 1, PF_BYTE_RGB, true);
        
        //Convolver *c = new Convolver(5);
        //c->blurGray (mHorizonImage);
    }


}//ogre namespace
