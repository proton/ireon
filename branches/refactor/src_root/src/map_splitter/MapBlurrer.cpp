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
MapBlurrer.cpp
@brief
Utility that blur maps
*/

#include "MapUtil.h"
#include "MapBlurrer.h"

#include <iostream>
#include <fstream>
#include <vector>

#include "OgreImageCodec.h"
#include "OgreILCodecs.h"


#include "fileutils.h"


namespace Ogre
{

#define uimax(a,b) (a>b?a:b)
#define uimin(a,b) (a>b?b:a)

    //-----------------------------------------------------------------------
    Convolver::Convolver(int sz)
    {
        mSz = sz;
        mInit = false;
    }

    //-----------------------------------------------------------------------
    Convolver::~Convolver()
    {
        if (mInit)
        {
            for (int i = 0; i < kernelSize; i++)
            {
                delete[] mult[i];
            }
            delete[] mult;
            delete[] kernel;
        }
    }

    //-----------------------------------------------------------------------
    void Convolver::setRadius()
    {
        int sz = uimin (uimax (1, mSz), 248);
        if (radius == sz) 
            return;
        radius = sz;

        kernelSize = 1 + sz * 2;

        mult = new int*[kernelSize];
        for (int i = 0; i < kernelSize; i++)
        {
            mult[i] = new int [256];
        }

        kernel = new int[kernelSize];
        int sum = 0;
        for (int i = 1; i < sz; i++)
        {
            int szi = sz  -  i;
            kernel[sz+i] = kernel[szi] = szi*szi;
            sum += kernel[szi] + kernel[szi];
            for (uint j=0; j < 256; j++)
            {
                mult[sz+i][j] = mult[szi][j] = kernel[szi] * j;
            }
        }

        kernel[sz] = sz*sz;
        sum += kernel[sz];
        for (uint j = 0; j<256; j++)
        {
            mult [sz][j] = kernel[sz] * j;
        }
    }
    
    //-----------------------------------------------------------------------
    void Convolver::blurRgba (Image  * img)
    {
        if (!mInit)
            setRadius ();
        const int w = static_cast <int> (img->getHeight());
        const int h = static_cast <int> (img->getWidth()); 
        const int wh = w  *  h;

        int *r = new int[wh];
        int *g = new int[wh];
        int *b = new int[wh];
        int *a = new int[wh];

        uint k = 0;
        uchar *pix = img->getData ();
        for (int i = 0; i < wh; i++ )
        {
            r[i] = pix[k];
            g[i] = pix[k+1];
            b[i] = pix[k+2];
            a[i] = pix[k+3];

            k += 4;
        }

        int *r2 = new int[wh];
        int *g2 = new int[wh];
        int *b2 = new int[wh];
        int *a2 = new int[wh];

        int yi = 0;
        for (int yl = 0; yl < h; yl++ )
        {
            for (int xl = 0; xl < w; xl++ )
            {
                int ri = xl - radius;
                int sum, cr, cg, cb, ca;
                ca = cb = cg = cr = sum = 0;
                for (int i = 0; i < kernelSize; i++ )
                {
                    int read = ri + i;
                    if (read >= 0 && read < w)
                    {
                        read += yi;
                        cr += mult[i][r[read]];
                        cg += mult[i][g[read]];
                        cb += mult[i][b[read]];
                        ca += mult[i][a[read]];
                        sum += kernel[i];
                    }
                }
                ri = yi + xl;
                float invsum = 1.0f / sum;
                r2[ri] = static_cast <int> (cr * invsum);
                g2[ri] = static_cast <int> (cg * invsum);
                b2[ri] = static_cast <int> (cb * invsum);
                a2[ri] = static_cast <int> (ca * invsum);
            }
            yi += w;
        }

        yi = 0;
        uint curr_row = 0;
        for (int yl = 0; yl < h; yl++)
        {
            int ym = yl - radius;
            int riw = ym * w;
            for (int xl = 0; xl < w; xl++)
            {
                int ri = ym;
                int read = xl + riw;
                int sum, cr, cg, cb, ca ;
                ca = cb = cg = cr = sum = 0;
                for (int i = 0; i < kernelSize; i++ )
                {
                    if ( ri < h && ri >= 0)
                    {
                        cr += mult[i][r2[read]];
                        cg += mult[i][g2[read]];
                        cb += mult[i][b2[read]];
                        ca += mult[i][a2[read]];
                        sum += kernel[i];
                    }
                    ri++;
                    read += w;
                }
                float invsum = 1.0f / sum;
                pix[curr_row + 0] = static_cast <uchar> (cr * invsum);
                pix[curr_row + 1] = static_cast <uchar> (cg * invsum);
                pix[curr_row + 2] = static_cast <uchar> (cb * invsum);
                pix[curr_row + 3] = static_cast <uchar> (ca * invsum);
                curr_row += 4;
            }
            yi += w;
        }
    }

    
    //-----------------------------------------------------------------------
    void Convolver::blurRgb (Image  * img)
    {
        if (!mInit)
            setRadius();

        const int w = static_cast <int> (img->getHeight());
        const int h = static_cast <int> (img->getWidth()); 
        const int wh = w  *  h;

        int *r = new int[wh];
        int *g = new int[wh];
        int *b = new int[wh];

        uint k = 0;
        uchar *pix = img->getData ();
        for (int i = 0; i < wh; i++ )
        {
            r[i] = pix[k];
            g[i] = pix[k+1];
            b[i] = pix[k+2];

            k += 3;
        }

        int *r2 = new int[wh];
        int *g2 = new int[wh];
        int *b2 = new int[wh];

        int yi = 0;
        for (int yl = 0; yl < h; yl++ )
        {
            for (int xl = 0; xl < w; xl++ )
            {
                int ri = xl - radius;
                int sum, cr, cg, cb;
                cb = cg = cr = sum = 0;
                for (int i = 0; i < kernelSize; i++ )
                {
                    int read = ri + i;
                    if (read >= 0 && read < w)
                    {
                        read += yi;
                        cr += mult[i][r[read]];
                        cg += mult[i][g[read]];
                        cb += mult[i][b[read]];
                        sum += kernel[i];
                    }
                }
                ri = yi + xl;
                float invsum = 1.0f / sum;
                r2[ri] = static_cast <int> (cr * invsum);
                g2[ri] = static_cast <int> (cg * invsum);
                b2[ri] = static_cast <int> (cb * invsum);
            }
            yi += w;
        }

        yi = 0;
        uint curr_row = 0;
        for (int yl = 0; yl < h; yl++)
        {
            int ym = yl - radius;
            int riw = ym * w;
            for (int xl = 0; xl < w; xl++)
            {
                int ri = ym;
                int read = xl + riw;
                int sum, cr, cg, cb;
                cb = cg = cr = sum = 0;
                for (int i = 0; i < kernelSize; i++ )
                {
                    if ( ri < h && ri >= 0)
                    {
                        cr += mult[i][r2[read]];
                        cg += mult[i][g2[read]];
                        cb += mult[i][b2[read]];
                        sum += kernel[i];
                    }
                    ri++;
                    read += w;
                }
                float invsum = 1.0f / sum;
                pix[curr_row] = static_cast <uchar> (cr * invsum);
                pix[curr_row + 1] = static_cast <uchar> (cg * invsum);
                pix[curr_row + 2] = static_cast <uchar> (cb * invsum);
                curr_row += 3;
            }
            yi += w;
        }
    }
    
    //-----------------------------------------------------------------------
    void Convolver::blurGray (Image  * img)
    {
        if (!mInit)
            setRadius ();

        const int w = static_cast <int> (img->getHeight());
        const int h = static_cast <int> (img->getWidth()); 
        const int wh = w  *  h;

        int *g = new int[wh];

        uint k = 0;
        uchar *pix = img->getData ();
        for (int i = 0; i < wh; i++ )
        {
            g[i] = pix[k];
            k += 1;
        }

        int *g2 = new int[wh];


        int yi = 0;
        for (int yl = 0; yl < h; yl++ )
        {
            for (int xl = 0; xl < w; xl++ )
            {
                int ri = xl - radius;
                int sum, cg;
                cg = sum = 0;
                for (int i = 0; i < kernelSize; i++ )
                {
                    int read = ri + i;
                    if (read >= 0 && read < w)
                    {
                        read += yi;
                        cg += mult[i][g[read]];
                        sum += kernel[i];
                    }
                }
                ri = yi + xl;
                g2[ri] = cg / sum;
            }
            yi += w;
        }

        yi = 0;
        for (int yl = 0; yl < h; yl++)
        {
            int ym = yl - radius;
            int riw = ym * w;
            for (int xl = 0; xl < w; xl++)
            {
                int ri = ym;
                int read = xl + riw;
                int sum, cg;
                cg = sum = 0;
                for (int i = 0; i < kernelSize; i++ )
                {
                    if ( ri < h && ri >= 0)
                    {
                        cg += mult[i][g2[read]];
                        sum += kernel[i];
                    }
                    ri++;
                    read += w;
                }
                pix[(xl + yi)] = uchar (cg / sum);
            }
            yi += w;
        }
    }

    //-----------------------------------------------------------------------
    void Convolver::blurReal (Real  * data, uint width, uint height)
    {
        const uint w = width;
        const uint h = height; 
        const uint wh = w  *  h;
        uint blurneighbors = uint (mSz);

        Real *g = data;
        Real *g2 = new Real[wh];

        uint yi = 0;
        for (uint yl = blurneighbors; yl < (h - blurneighbors); yl++ )
        {
            for (uint xl = blurneighbors; xl < (w - blurneighbors); xl++ )
            {
                Real sum = 0.0f;
                for (uint i = xl - blurneighbors; i < xl + blurneighbors; i++ )
                { 
                    for (uint j = yl - blurneighbors; j < yl + blurneighbors; j++ )
                    {
                        sum += g[i + j*w];
                    }
                    
                }
                g2[xl + yi] = Real (g[xl + yi] / sum);
            }
            yi += w;
        }

        memcpy (data, g2, sizeof (Real) * wh);
    }







    //-----------------------------------------------------------------------
    MapBlurrer::MapBlurrer()
    {
    }
    //-----------------------------------------------------------------------
    MapBlurrer::~MapBlurrer()
    { 
    }
    //-----------------------------------------------------------------------
    void MapBlurrer::AverageBlur(Image *BlurImage)
    {        
        uint  height =  static_cast <uint> (BlurImage->getHeight ());
        uint  width = static_cast <uint> (BlurImage->getWidth ());
        uint Bpp = static_cast <uint> (PixelUtil::getNumElemBytes (BlurImage->getFormat ()));

        uchar *tempImageData = new uchar[width*height*Bpp];
        uchar *BlurImageData = BlurImage->getData ();


        //int w[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
        // or 
        int w[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}} ;


        int totalWeight = 0;
        for (uint i = 0; i < 3; i++)
            for (uint j = 0; j < 3; j++)
                totalWeight += w[i][j];

        Real invTotalW = 1.0f / totalWeight;
        uint row_current = 0;
        for (uint x = 0; x < width; x++)
        {
            for (uint z = 0; z < height; z++)
            {
                /* Show progress */
                DEBUG_PROGRESS_OUTPUT(".")
                uint tempY = 0;
                uint cur_row = row_current;
                for (uint i = 0; i < 3; i++)
                {
                    for (uint j = 0; j < 3; j++)
                        tempY += w[i][j] * BlurImageData [row_current + cur_row - height + z + j - 1];
                    cur_row += height;
                }
                   
                //Save the value into the temporary image data
                BlurImageData[row_current + z] = uchar (tempY *invTotalW);
            }
            row_current += height;
        }

        //Delete temporary memory
        if(tempImageData)
            delete [] tempImageData;
        tempImageData = 0;
    }

   //-----------------------------------------------------------------------
    void MapBlurrer::gaussianBlur(Image *BlurImage, int sigma)
    {
        //Create space to hold the (1D) kernel values
        //Ignore values outside 3 standard deviations
        int kernelRadius = int (3*sigma);
        int kernelSize = kernelRadius * 2 + 1;

        double *kernel = new double[kernelSize];

        //Fill the kernel values

        Real precal_divider =  (1.0f / (2*sigma*sigma)) / (Math::Sqrt(2*Math::PI) * sigma);
        for(int x = -kernelRadius; x <= kernelRadius; ++x)
            kernel[x+kernelRadius] = Math::Exp ( -(x*x) * precal_divider);

        //Normalize the values
        double kernelTotal = 0.0f;

        for(int i = 0; i < kernelSize; ++i)
            kernelTotal += kernel[i];

        for(int i = 0; i < kernelSize; ++i)
            kernel[i] /= kernelTotal;

        uint  height = static_cast <uint> (BlurImage->getHeight ());
        uint  width = static_cast <uint> (BlurImage->getWidth ());
        size_t Bpp = PixelUtil::getNumElemBytes (BlurImage->getFormat ());

        uchar *tempImageData = new uchar[width*height*Bpp];
        uchar *BlurImageData = BlurImage->getData ();

        std::cout << "blurring : ";

        //Apply the kernel in the x direction, saving the data in tempImageData
        //Loop through pixels
        uint row_current = 0;
        const uint bytespp = static_cast <uint> (Bpp);
        int width_size = width*Bpp;
        
        for (uint y = 0; y < height; ++y)
        {   
            /* Show progress */
            DEBUG_PROGRESS_OUTPUT(".")
            for(uint x = 0; x < width; ++x)
            {
                for(uint z = 0; z < Bpp; ++z)
                {
                    //Keep a running total of the pixel's new value
                    double newValue = 0.0;

                    //Loop through values in kernel
                    for(int kx = -kernelRadius; kx <= kernelRadius; ++kx)
                    {
                        //Calculate the pixel column to read
                        int pixelColumn = (x + kx)*Bpp;

                        //If the pixel is outside the image, stop at the edge
                        if(pixelColumn < 0)
                            pixelColumn = 0 + z;

                        if(pixelColumn >= width_size)
                            pixelColumn = width_size - (bytespp - z);

                        pixelColumn += z;//step to the good color channel

                        //Add kernel value*pixel value to the new value
                        newValue += kernel[kx + kernelRadius] * BlurImageData [row_current + pixelColumn];
                    }

                    //Save the value into the temporary image data
                    tempImageData[row_current + x * Bpp + z] = uchar (newValue);
                }
            }
            row_current += width_size;
        }

        //Now apply the kernel in the y direction to tempImageData
        //Loop through pixels
        row_current = 0;
        for (uint y = 0; y < height; ++y)
        { 
            /* Show progress */
            DEBUG_PROGRESS_OUTPUT(".")
            for (uint x = 0; x<width; ++x)
            {
                for (uint z = 0; z < Bpp; ++z)
                {
                    //Keep a running total of the pixel's new value
                    double newValue=0.0;

                    //Loop through values in kernel
                    for (int kx = -kernelRadius; kx <= kernelRadius; ++kx)
                    {
                        //Calculate the pixel row to read
                        int pixelRow = y + kx;

                        //If the pixel is outside the image, stop at the edge
                        if(pixelRow < 0)
                            pixelRow = 0 + z;

                        if(pixelRow >= (int)height)
                            pixelRow = height - (Bpp - z);


                        //Add kernel value*pixel value to the new value
                        newValue += kernel[kx+kernelRadius] * tempImageData[pixelRow * width_size  + x * Bpp + z];
                    }
     
                    //Save the value into the destination image data
                    BlurImageData[row_current + x * Bpp + z]= uchar (newValue);
                }
            }
            row_current += width_size;
        }


        //Delete temporary memory
        if(tempImageData)
            delete [] tempImageData;
        tempImageData=NULL;

        if(kernel)
            delete [] kernel;
        kernel = NULL;
        std::cout << "\n";
    }
}//namespace Ogre

