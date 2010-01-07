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
MapInfinitizer.cpp
@brief
Utility that split big map in tiles, pre-calculate splatting by
pre-calculating coverage map and pre-calculate normals.
*/

#include "MapUtil.h"
#include "MapInfinitizer.h"


namespace Ogre
{
    //-----------------------------------------------------------------------
    MapInfinitizer::MapInfinitizer()
    {
    }
    //-----------------------------------------------------------------------
    MapInfinitizer::~MapInfinitizer()
    { 
    }
    //-----------------------------------------------------------------------
    bool MapInfinitizer::colorsAreDifferrent (Real *DataA, Real *DataB)
    {
        return (fabs (*DataA - *DataB) > 0.05f)? true: false;       
    }

    //-----------------------------------------------------------------------
    bool MapInfinitizer::checkForNeed()
    {
        uint row_count_down = (mheight - mBorderSize) * mwidth;
        uint row_count_up = 0;

        for (uint y = 0; y < mBorderSize; y++)	
        {
            uint col_count_down = row_count_down;
            uint col_count_up = row_count_up;  

            for (uint x = 0; x < mwidth; x++)	
            {
                Real* TargetColorUp = &mData[x * 1];
                Real* TargetColorDown = &mData[mheight * (mwidth - 1) + x];
                if (colorsAreDifferrent(TargetColorUp, TargetColorDown))
                {
                    return true;
                }
                col_count_up += 1;
                col_count_down += 1;
            }
            row_count_up += mwidth;
            row_count_down += mwidth;
        }
        uint row_count = 0;
        for (uint y = 0; y < mheight; y++)	
        {
 
            Real* TargetColorRight = &mData[row_count + mwidth];
            Real* TargetColorLeft = &mData[row_count];

            if (colorsAreDifferrent(TargetColorRight, TargetColorLeft))
             {
                return true;                
            }
            row_count += mwidth;
        }
        std::cout << "no need for Infinitizing, your Map is already infinite\n";
        return false;
    }
    //-----------------------------------------------------------------------
    void MapInfinitizer::lerpColor (Real *DataDest, Real *DataA, Real *DataB, 
                                    Real Factor)
    {       
        *DataDest = *DataA * (1.0f - Factor) + 
                                *DataB * Factor;    
    }
    //-----------------------------------------------------------------------
    void MapInfinitizer::lerpCorners ()
    {
    }
    //-----------------------------------------------------------------------
    void MapInfinitizer::lerpUpAndDown ()
    {
        uint row_count_down = (mheight - mBorderSize) * mwidth;
        uint row_count_up = 0;

        for (uint y = 0; y < mBorderSize; y++)	
        {
            uint col_count_down = row_count_down;
            uint col_count_up = row_count_up;
            
            Real factor1 = (Real) y / mBorderSize; // from 0 to 1    
            Real factor2 = 1.0f - factor1;         // from 1 to 0        

            for (uint x = 0; x < mwidth; x++)	
            {
                Real* TargetColorUp = &mData[x * 1];
                Real* TargetColorDown = &mData[mheight * (mwidth - 1) * 1 + x * 1];

                    // blend on the down side
                    lerpColor (&mData[col_count_down], &mData[col_count_down], 
                                TargetColorUp, factor1);// from 0 to 1
                
                    // blend on the up side
                    
                    lerpColor (&mData[col_count_up], &mData[col_count_up], 
                                TargetColorDown, factor2); // from 1 to 0
            
                col_count_up += 1;
                col_count_down += 1;
            }
            row_count_up += mwidth;
            row_count_down += mwidth;
        }
    }
    //-----------------------------------------------------------------------
    void MapInfinitizer::LerpLeftAndRight ()
    {
        uint row_count = 0;
        for (uint y = 0; y < mheight; y++)	
        {
            uint col_count_left = row_count;
            uint col_count_right = row_count + (mwidth - mBorderSize) * 1;

            //uint col_count_left_inv = row_count + mBorderSize * 1;
            //uint col_count_right_inv = row_count + mwidth;

            Real* TargetColorRight = &mData[row_count + mwidth];
            Real* TargetColorLeft = &mData[row_count];

            for (uint x = 0; x < mBorderSize; x++)
            {
                // from 1 to 0
                Real factor1 = (Real) (mBorderSize - x) / mBorderSize; 
                // from 0 to 1
                Real factor2 = 1.0f - factor1;

                // lerp on the left side
                lerpColor (&mData[col_count_left], 
                            &mData[col_count_left], 
                            TargetColorRight,  
                            factor1); // from 0 to 1

                // lerp on the right side
                lerpColor (&mData[col_count_right], 
                            &mData[col_count_right],  
                            TargetColorLeft, 
                            factor2); // from 1 to 0

                col_count_left += 1;
                col_count_right += 1;

                //col_count_left_inv -= 1;
                //col_count_right_inv -= 1;
            }
      
            row_count += mwidth;
        }
    }   

    //-----------------------------------------------------------------------
    void MapInfinitizer::blendColor (Real *DataDest, Real *DataA, Real FactorA, 
                                                  Real *DataB, Real FactorB)
    {
       
        *DataDest = *DataA * FactorA + *DataB * FactorB;
      
    }
    //-----------------------------------------------------------------------
    void MapInfinitizer::BlendCorners ()
    {

    }
    //-----------------------------------------------------------------------
    void MapInfinitizer::BlendUpAndDown ()
    {
        // example with mBordersize = 5
        //
        //              Direction of copying : up to down 
        //                                          
        //                              ||            
        //                              ||            
        //
        //    UP storing                        DOWN  storing
        //    
        //     -------                         -------         
        //     Border                          Border         
        //     -------                         -------                           
        //        0.5     0.5                   0.0     1.0   
        //        0.6     0.4                   0.1     0.9   
        //        0.7     0.3                   0.2     0.8   
        //        0.8     0.2                   0.3     0.7   
        //        0.9     0.1                   0.4     0.6   
        //        1.0     0.0                   0.5     0.5   
        //              -------                         ------
        //              Border                          Border
        //              -------                         ------
        //       Up       down                  Up        down
        //    
        //
        uint row_count_down = (mheight - mBorderSize) * mwidth;
        uint row_count_up = 0;

        for (uint y = 0; y < mBorderSize; y++)	
        {
            uint col_count_down = row_count_down;
            uint col_count_up = row_count_up;
            
            Real factor1 = Real (y) / mBorderSize2; // from 0 to 0.5    
            Real factor2 = 1.0f - factor1;              // from 1 to 0.5            
            Real factor3 = Real (mBorderSize - y) / mBorderSize2; // from 0.5 to 0
            Real factor4 = 1.0f - factor3               ;// from 0.5 to 1

            for (uint x = 0; x < mwidth; x++)	
            {
                // blend on the up side
            
                blendColor (&mData[col_count_down], 
                            &mData[col_count_down],   factor2, // from 1 to 0.5
                            &mData[col_count_up],     factor1);// from 0 to 0.5
            
                // blend on the up side
                
                blendColor (&mData[col_count_up], 
                            &mData[col_count_down],   factor3,  // from 0.5 to 0
                            &mData[col_count_up],     factor4); // from 0.5 to 1


                col_count_up += 1;
                col_count_down += 1;
            }
            row_count_up += mwidth;
            row_count_down += mwidth;
        }
    }
    //-----------------------------------------------------------------------
    void MapInfinitizer::BlendLeftAndRight ()
    {
        // example with mBordersize = 5
        //
        //        Direction of the copying : left to right
        //                   ===> 
        //
        //                   LEFT  storing                    
        //         --0.5--0.4--0.3--0.2--0.1--0.0||Border||    right factor
        // ||Border||0.5--0.6--0.7--0.8--0.9--1.0              left factor        
        //
        // 
        //
        //                   RIGHT  storing                
        //         --1.0--0.9--0.8--0.7--0.6--0.5||Border||   right factor
        // ||Border||0.0--0.1--0.2--0.3--0.4--0.5             left factor

        uint row_count = 0;
        for (uint y = 0; y < mheight; y++)	
        {
            uint col_count_left = row_count;
            uint col_count_right = row_count + (mwidth - mBorderSize) * 1;

            //uint col_count_left_inv = row_count + mBorderSize * 1;
            //uint col_count_right_inv = row_count + mwidth;
   

            for (uint x = 0; x < mBorderSize; x++)
            {
                // from 0.5 to 0
                Real factor1 = (Real) (mBorderSize - x) / mBorderSize2; 
                // from 0.5 to 1
                Real factor2 = 1.0f - factor1;
                // from 0 to 0.5
                Real factor3 = (Real) x / mBorderSize2;
                // from 1 to 0.5
                Real factor4 = 1.0f - factor3;

                // blend on the left side
                blendColor (&mData[col_count_left], 
                            //&mData[col_count_right_inv], factor1, // from 0.5 to 0
                            &mData[col_count_left],  factor3, // from 0 to 0.5
                            &mData[col_count_left],  factor2); // from 0.5 to 1

                // blend on the right side
                blendColor (&mData[col_count_right], 
                            //&mData[col_count_left_inv],  factor3, // from 0 to 0.5
                            &mData[col_count_left],  factor3, // from 0 to 0.5
                            &mData[col_count_right], factor4); // from 1 to 0.5

                col_count_left += 1;
                col_count_right += 1;

                //col_count_left_inv -= 1;
                //col_count_right_inv -= 1;
            }
            row_count += mwidth;
        }
    }
    //-----------------------------------------------------------------------
    bool MapInfinitizer::Infinitize (uint BorderSize, Real RandFactor, uint BlurFactor)
    {
        mData = MapUtil::getSingleton().getHeightData ();
        mwidth = MapUtil::getSingleton().getMapWidth ();
        mheight = MapUtil::getSingleton().getMapHeight ();
    
        mBorderSize = BorderSize;
        mBorderSize2 = mBorderSize * 2;
        mBlurFactor = BlurFactor;
        mRandFactor = RandFactor;

        if (checkForNeed ())
        {
            std::cout << "Infinitizing.\n";

            lerpUpAndDown ();
            LerpLeftAndRight ();
            //lerpCorners ();

            //BlendUpAndDown ();
            //BlendLeftAndRight ();
            //BlendCorners ();
            return true;
        }
        return false;
    } 
}//namespace Ogre
