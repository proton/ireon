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
MapEqualizer.cpp
@brief
Utility make a map more coherent.
erosion code => Portions Copyright (C) Jason Shankel, 2000
*/

#include "MapUtil.h"
#include "MapEqualizer.h"


namespace Ogre
{
    //-----------------------------------------------------------------------
    MapEqualizer::MapEqualizer()
    {
    }
    //-----------------------------------------------------------------------
    MapEqualizer::~MapEqualizer()
    { 
    }
    //-----------------------------------------------------------------------
    void MapEqualizer::equalize ()
    {
        mwidth = MapUtil::getSingleton().getMapWidth ();
        mheight = MapUtil::getSingleton().getMapHeight ();
        mColorsNum = (MapUtil::getSingleton().mExt == "raw")? 65535: 255;

        const double divider = 1.0f / MapUtil::getSingleton().getMaxTheoHeight() * mColorsNum;
        Real *hdata = MapUtil::getSingleton().getHeightData ();
        const uint size = mwidth*mheight;
        uchar *uData = new uchar[size];
        uint i;
        for (i = 0; i < size; i++ )            
        { 
            uData[i] = uchar  (hdata[i] * divider);
        }


        uint *freq = new uint[mColorsNum];
        memset (freq, 0, mColorsNum*sizeof(uint));
        for (i = 0; i < size; i++ )            
        { 
                freq[uData[i]] += 1;
        }

        uint *Cumulfreq = freq;
        for (i = 1; i < mColorsNum; i++)
        {           
            Cumulfreq[i] = Cumulfreq[i - 1] + freq[i];
        }

        const double factor = static_cast <double> (MapUtil::getSingleton().getMaxTheoHeight()) / size;
        uint cur_pos = 0;
        for (uint z = 0; z < mheight; z++)
        {
            for ( uint x = 0; x < mwidth; x++)
            {
                const uint pos = cur_pos + x;
                hdata[pos] = Cumulfreq[uData[pos]] * factor;
            }
            cur_pos += mwidth;
        }
        delete[] freq;
    } 


    
    //-----------------------------------------------------------------------
    ErodeFault::ErodeFault()
    {
    }
    //-----------------------------------------------------------------------
    ErodeFault::~ErodeFault()
    { 
    }
    //-----------------------------------------------------------------------
    void ErodeFault::ErodeOneDirection(float * const band, 
                                const int stride, 
                                const uint count, 
                                const float filter) const
    {
	    uint i;
        int j = stride;
	    float v = band[0];
	    for (i = 0; i < count-1; i++)
	    {
		    band[j] = filter*v + (1-filter)*band[j];
		    v = band[j];
		    j += stride;
	    }
    }
    //-----------------------------------------------------------------------
    void ErodeFault::Erode(Real * const field, const uint size,const float filter) const
    {
	    uint i;

	    /*
	    Erode rows left to right
	    */
	    for (i=0;i<size;i++)
	    {
		    ErodeOneDirection( &field[size*i], 1, size, filter);
	    }
    	
	    /*
	    Erode rows right to left
	    */
	    for (i=0;i<size;i++)
	    {
		    ErodeOneDirection (&field[size*i+size-1], -1, size, filter);
	    }

	    /*
	    Erode columns top to bottom
	    */
	    for (i=0;i<size;i++)
	    {
		    ErodeOneDirection (&field[i], (int)size, size, filter);
	    }

	    /*
	    Erode columns bottom to top
	    */
	    for (i=0;i<size;i++)
	    {
		    ErodeOneDirection (&field[size*(size-1)+i], -((int)(size)), size, filter);
	    }
    }
    //-----------------------------------------------------------------------
    void ErodeFault::MakeTerrainFault(Real * const field, const uint size, const uint iterations,
                                         const uint maxDelta, const uint minDelta,
                                         const uint iterationsPerFilter,
                                         const float filter,
                                const Real scale) const
    {
        const uint fieldSize = size * size;
	    /*
	    Clear the height field
	    */
        memset (field, 0, fieldSize);

        uint i;
        uint x2, y2;
        Real inv_iter = 1.0f / iterations;
	    for (i = 0; i < iterations; i++)
	    {
		    /*
		    Calculate the dHeight for this iteration
		    (linear interpolation from maxDelta to minDelta
		    */
		    const Real dHeight = maxDelta - ((maxDelta - minDelta) * i) * inv_iter;
    		assert (maxDelta - ((maxDelta - minDelta) * i) * inv_iter > 0);
		    /*
		    Pick two random points on the field for the line
		    (make sure they're not identical)
		    */
		    const uint x1 = rand()%size;
		    const uint y1 = rand()%size;
    		
		    do
		    {
			    x2 = rand()%size;
			    y2 = rand()%size;
		    }while (x2 == x1 && y2 == y1);

    		
		    /*
		    dx1,dy1 is a vector in the direction of the line
		    */
		    const int dx1 = x2 - x1;
		    const int dy1 = y2 - y1;
    		
		    for (x2 = 0; x2 < size; x2++)
            {
		        for (y2 = 0; y2 < size; y2++)
		        {
			        /*
			        dx2,dy2 is a vector from x1,y1 to the candidate point
			        */
			        const int dx2 = x2-x1;
			        const int dy2 = y2-y1;
        			
			        /*
			        if z component of the cross product is 'up', then elevate this point
			        */
			        if (dx2*dy1 - dx1*dy2 > 0)
			        {
				        field[x2+size*y2] += dHeight;
			        }

		        }
            }

		    /*
		    Erode terrain
		    */
		    if (iterationsPerFilter!=0 && (i%iterationsPerFilter)==0)
			    Erode(field, size, filter);
	    }

	    /*
	    Normalize terrain (height field values in the range 0-1)
	    */
	    normalize (field, size, scale);

    }
    //-----------------------------------------------------------------------
    float ErodeFault::RangedRandom(const float v1,const float v2) const
    {
	    return v1 + (v2-v1)*(rand())/((float)RAND_MAX);
    }
    //-----------------------------------------------------------------------
    void ErodeFault::normalize (Real * const field, const uint size,
                                const Real scale) const
    {
        /*
	    Find the maximum and minimum values in the height field
	    */ 
        const uint fieldSize = size*size;
	    Real maxVal = field[0];
	    Real minVal = field[0];
        uint i;
	    for (i = 1; i < fieldSize; i++)
	    {
		    if (field[i] > maxVal) 
		    {
			    maxVal = field[i];
		    }
		    else if (field[i] < minVal) 
		    {
			    minVal = field[i];
		    }
	    }

	    /*
	    Find the altitude range (dh)
	    */
        const Real minFinal = minVal;
	    if (maxVal <= minFinal) 
            return;
	    const Real inv_dh = scale / (maxVal - minFinal);

	    /*
	    Scale all the values so they fill exactly the range 0-1
	    */
	    for (i=0; i < fieldSize; i++)
	    {
		    field[i] = (field[i] - minFinal) * inv_dh;
            assert (field[i] <= scale && field[i] >= 0.0f);
	    }
    } 
    //-----------------------------------------------------------------------
    void ErodeFault::MakeTerrainPlasma(Real *  const field, 
                                        const uint size, 
                                        const float rough,
                                const Real scale) const
    {

	    /*
	    Since the terrain wraps, all 4 "corners" are represented by the value at 0,0,
		    so seeding the heightfield is very straightforward
	    Note that it doesn't matter what we use for a seed value, since we're going to
		    renormalize the terrain after we're done
	    */
	    field[0] = 0;

        const float r = (float)pow (2.0f, -1 * rough);

	    uint i, j, rectSize = size;
	    float dh = (float)rectSize * 0.5f;
	    while(rectSize > 0)
	    {
    		
		    /*
		    Diamond step -

		    Find the values at the center of the rectangles by averaging the values at 
		    the corners and adding a random offset:


		    a.....b
		    .     .  
		    .  e  .
		    .     .
		    c.....d   

		    e  = (a+b+c+d)/4 + random

		    In the code below:
		    a = (i,j)
		    b = (ni,j)
		    c = (i,nj)
		    d = (ni,nj)
		    e = (mi,mj)

		    */
    		const uint c_rectSize = rectSize;
    		const float c_dh = dh;

    		const float maxSize = size - (c_rectSize * 0.5f);

		    for (i = 0; i < maxSize; i += c_rectSize)
            {
		        for (j = 0; j < maxSize; j += c_rectSize)
		        {
        			
			        const uint ni = (i+c_rectSize)%size;
			        const uint nj = (j+c_rectSize)%size;

			        const uint mi = (i+c_rectSize * 0.5f);
			        const uint mj = (j+c_rectSize * 0.5f);
                    

                    assert (i+j*size < size*size);
                    assert (ni+j*size < size*size);
                    assert (ni+nj*size < size*size);
                    assert (i+nj*size < size*size);

                    assert (mi+mj*size < size*size);

			        field[mi+mj*size] = (field[i+j*size] + field[ni+j*size] +
                                         field[i+nj*size] + field[ni+nj*size]) * 0.25f 
                                         + RangedRandom(-c_dh * 0.5f, c_dh * 0.5f);
		        }
            }
		    /*
		    Square step -

		    Find the values on the left and top sides of each rectangle
		    The right and bottom sides are the left and top sides of the neighboring rectangles,
		    so we don't need to calculate them

		    The height field wraps, so we're never left hanging.  The right side of the last
			    rectangle in a row is the left side of the first rectangle in the row.  The bottom
			    side of the last rectangle in a column is the top side of the first rectangle in
			    the column

                .......
		        .     .
		        .     .
		        .  d  .
		        .     .
		        .     .
		    ......a..g..b
		    .     .     .
		    .     .     .
		    .  e  h  f  .
		    .     .     .
		    .     .     .
		    ......c......

		    g = (d+f+a+b)/4 + random
		    h = (a+c+e+f)/4 + random
    		
		    In the code below:
			    a = (i,j) 
			    b = (ni,j) 
			    c = (i,nj) 
			    d = (mi,pmj) 
			    e = (pmi,mj) 
			    f = (mi,mj) 
			    g = (mi,j)
			    h = (i,mj)

		    */
		    for (i = 0; i < maxSize; i += c_rectSize)
            { 
		        for (j = 0; j < maxSize; j += c_rectSize)
		        {

			        const uint ni = (i+c_rectSize)%size;
			        const uint nj = (j+c_rectSize)%size;

			        const uint mi = (i+c_rectSize * 0.5f);
			        const uint mj = (j+c_rectSize * 0.5f);

			        const uint pmi = (i- (uint)(c_rectSize * 0.5f) + size)%size;
			        const uint pmj = (j- (uint)(c_rectSize * 0.5f) + size)%size;

        			
                    assert (i+j*size < size*size);
                    assert (ni+j*size < size*size);
                    assert (i+nj*size < size*size);

                    assert (mi+pmj*size < size*size);
                    assert (mi+mj*size < size*size);
                    assert (pmi+mj*size < size*size);
                    assert (mi+mj*size < size*size);

                    assert (mi+j*size < size*size);
                    assert (i+mj*size < size*size);

			        /*
			        Calculate the square value for the top side of the rectangle
			        */
			        field[mi+j*size] = (field[i+j*size] + field[ni+j*size] + 
                                        field[mi+pmj*size] + field[mi+mj*size])  * 0.25f 
                                        + RangedRandom(-c_dh * 0.5f, c_dh * 0.5f);

        			
			        /*
			        Calculate the square value for the left side of the rectangle
			        */
			        field[i+mj*size] = (field[i+j*size] + field[i+nj*size] + 
                                        field[pmi+mj*size] + field[mi+mj*size])  * 0.25f + 
                                        RangedRandom(-c_dh * 0.5f, c_dh * 0.5f);

		        } 
            }

		    /*
		    Setup values for next iteration
		    At this point, the height field has valid values 
            at each of the coordinates that fall on a rectSize/2 boundary
		    */
		    rectSize *= 0.5f;
		    dh *= r;
	    }
	    /*
	    Normalize terrain so minimum value is 0 and maximum value is 1
	    */
	    normalize(field, size, scale);
    }

    
    //-----------------------------------------------------------------------
    void ErodeFault::MakeTerrainDeposition(Real *  const field, const uint size, const uint jumps,
                                            const uint peakWalk, const uint minParticlesPerJump, 
                                            const uint maxParticlesPerJump,
                                            const float caldera,
                                const Real scale) const
    {
	    uint x, y, i, j, p, px, py, minx, miny, maxx, maxy, sx, sy, done;
	    uint dx[] = {0,1,0,size-1,1,1,size-1,size-1};
	    uint dy[] = {1,0,size-1,0,size-1,1,size-1,1};
	    float ch,ph;

	    uint *calderaMap = new uint[size*size];
	    /*
	    Clear out height field
	    */
		memset (field, 0, size*size);
    	
	    for (p = 0; p < jumps; p++)
	    {
    		
		    /*
		    Pick a random spot
		    */
		    x = rand()%size;
		    y = rand()%size;

		    /*
		    px,py will track where we form the caldera
		    */
		    px = x;
		    py = y;
    		
    		
		    /*
		    Determine how many particles we're going to drop
		    */
		    const uint numParticles = (uint) RangedRandom((float)minParticlesPerJump,
                                                          (float)maxParticlesPerJump);
    		
		    for (i = 0; i < numParticles; i++)
		    {

			    /*
			    If it's time to move the drop point, agitate it in a random direction
			    */
			    if (peakWalk != 0 && (i%peakWalk) == 0)
			    {
				    const uint m = rand()%8;
				    x = (x+dx[m]+size)%size;
				    y = (y+dy[m]+size)%size;
			    }


			    /*
			    Drop the particle
			    */
			    field[x+y*size] += 1.0f;
    			
    			
			    /*
			    Now agitate it until it settles
			    */
			    sx = x;
			    sy = y;
			    done = 0;
    			
			    while (done == 0)
			    {
				    done = 1;
    				
				    /*
				    Pick a random neighbor to start inspecting
				    */
				    const uint m = rand();
				    for (j = 0; j < 8 ;j++)
				    {
					    const uint tx = (sx+dx[(j+m)%8])%size;
					    const uint ty = (sy+dy[(j+m)%8])%size;
    					
					    /*
					    If we can move to this neighbor, do it
					    */
					    if (field[tx+ty*size] + 1.0f < field[sx+sy*size])
					    {
						    field[tx+ty*size] += 1.0f;
						    field[sx+sy*size] -= 1.0f;
						    sx = tx;
						    sy = ty;
						    done = 0;
						    break;
					    }
				    }
			    }

			    /*
			    Check to see if our latest point is higher than the caldera point 
			    If so, move the caldera point here
			    */
			    if (field[sx+size*sy] > field[px+size*py])
			    {
				    px = sx;
				    py = sy;
			    }
		    }

		    /*
		    Now that we're done with the peak, invert the caldera
		    ch is the caldera cutoff altitude
		    ph is the height at the caldera start point

		    Note that this technique will not create a caldera at every
			    peak, rather it will create one caldera per jump.  It is
			    possible for the algorithm to generate multiple peaks per jump.
		    */
		    ph = field[px+size*py];
		    ch = ph*(1.0f-caldera);


		    /*
		    We're going to do a flood fill, so we use an array of integers to mark visited locations
		        0 = unmarked, unvisited
		        1 = marked, unvisited
		        2 = marked, visited
		    */
		    memset(calderaMap,0,size*size*sizeof(int));

		    minx = px;
		    maxx = px;
		    miny = py;
		    maxy = py;

		    /*
		    Mark the start location for the caldera
		    */
		    calderaMap[px+size*py] = 1;

		    done = 0;

		    while (done == 0)
		    {
			    done = 1;
			    const uint c_sx = minx;
			    const uint c_sy = miny;
			    const uint tx = maxx;
			    const uint ty = maxy;
    			
			    /*
			    Examine the bounding rectangle looking for unvisited neighbors
			    */
			    for (x = c_sx; x <= tx; x++)
                {
			        for (y = c_sy; y <= ty; y++)
			        {
				        px = (x+size)%size;
				        py = (y+size)%size;

				        /*
				        If this cell is marked but unvisited, check it out
				        */
				        if(calderaMap[px+size*py] == 1)
				        {
					        /*
					        Mark cell as visited
					        */
					        calderaMap[px+size*py] = 2;

					        /*
					        If this cell should be inverted, invert it and inspect neighbors
					        Mark any unmarked, unvisited neighbor
					        Don't invert any cells whose height exceeds our initial caldera height
					        This prevents small peaks from destroying large ones
					        */
					        if(field[px+size*py] > ch && field[px+size*py] <= ph)
					        {
						        done = 0;
						        field[px+size*py] = 2*ch - field[px+size*py];
        					
						        /*
						        Left and right neighbors
						        */
						        px = (px+1)%size;
						        if(calderaMap[px+size*py]==0)
						        {
							        if (x+1 > maxx) maxx = x+1;
							        calderaMap[px+size*py] = 1;
						        }
						        px = (px+size-2)%size;
						        if(calderaMap[px+size*py]==0)
						        {
							        if (x-1 < minx) minx = x-1;
							        calderaMap[px+size*py] = 1;
						        }

						        /*
						        Top and bottom neighbors
						        */
						        px = (x+size)%size;
						        py = (py+1)%size;
						        if(calderaMap[px+size*py] == 0)
						        {
							        if (y+1 > maxy) 
                                        maxy = y+1;
							        calderaMap[px+size*py] = 1;
						        }
						        py = (py+size-2)%size;
						        if(calderaMap[px+size*py] == 0)
						        {
							        if (y-1 < miny) 
                                        miny = y-1;
							        calderaMap[px+size*py] = 1;
						        }
					        }

				        }
			        }
                }
            }

	    }

	    delete [] calderaMap;
    	
	    /*
	    Calderas increase aliasing, so erode terrain with a filter value proportional to the prominence of the caldera
	    */
	    Erode (field, size, caldera);
    	
	    normalize(field, size, scale);
    	
    }


    /*
	    The Universe Development Kit
	    Copyright (C) 2000  Sean O'Neil
	    soneil@home.com

	    This library is free software; you can redistribute it and/or
	    modify it under the terms of the GNU Lesser General Public
	    License as published by the Free Software Foundation; either
	    version 2.1 of the License, or (at your option) any later version.

	    This library is distributed in the hope that it will be useful,
	    but WITHOUT ANY WARRANTY; without even the implied warranty of
	    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	    Lesser General Public License for more details.

	    You should have received a copy of the GNU Lesser General Public
	    License along with this library; if not, write to the Free Software
	    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
    */
    void CNoise::Init(const uint nDimensions, const uint nSeed)
    {
	    m_nDimensions = MIN(nDimensions, MAX_DIMENSIONS);
	    CRandom r(nSeed);

	    uint i, j;
	    for(i=0; i<256; i++)
	    {
		    m_nMap[i] = i;
		    for(j=0; j<m_nDimensions; j++)
			    m_nBuffer[i][j] = (float)r.RandomD(-0.5, 0.5);
		    Normalize(m_nBuffer[i], m_nDimensions);
	    }

        uchar k;
	    while(--i)
	    {
		    const int l = r.RandomI(0, 255);
		    SWAP(m_nMap[i], m_nMap[j], k);
	    }
	    _fpreset();	// Bug in CRandom! Causes messed up floating point operations!
    }

    float CNoise::Noise(float * const f) const 
    {
	    int n[MAX_DIMENSIONS];			// Indexes to pass to lattice function
	    float r[MAX_DIMENSIONS];		// Remainders to pass to lattice function
	    float w[MAX_DIMENSIONS];		// Cubic values to pass to interpolation function

	    for(uint i=0; i<m_nDimensions; i++)
	    {
		    n[i] = Floor(f[i]);
		    r[i] = f[i] - n[i];
		    w[i] = Cubic(r[i]);
	    }

	    float fValue;
	    switch(m_nDimensions)
	    {
		    case 1:
			    fValue = Lerp(Lattice(n[0], r[0]),
						    Lattice(n[0]+1, r[0]-1),
						    w[0]);
			    break;
		    case 2:
			    fValue = Lerp(Lerp(Lattice(n[0], r[0], n[1], r[1]),
							    Lattice(n[0]+1, r[0]-1, n[1], r[1]),
							    w[0]),
						    Lerp(Lattice(n[0], r[0], n[1]+1, r[1]-1),
							    Lattice(n[0]+1, r[0]-1, n[1]+1, r[1]-1),
							    w[0]),
						    w[1]);
			    break;
		    case 3:
			    fValue = Lerp(Lerp(Lerp(Lattice(n[0], r[0], n[1], r[1], n[2], r[2]),
									    Lattice(n[0]+1, r[0]-1, n[1], r[1], n[2], r[2]),
									    w[0]),
							    Lerp(Lattice(n[0], r[0], n[1]+1, r[1]-1, n[2], r[2]),
									    Lattice(n[0]+1, r[0]-1, n[1]+1, r[1]-1, n[2], r[2]),
									    w[0]),
							    w[1]),
						    Lerp(Lerp(Lattice(n[0], r[0], n[1], r[1], n[2]+1, r[2]-1),
									    Lattice(n[0]+1, r[0]-1, n[1], r[1], n[2]+1, r[2]-1),
									    w[0]),
							    Lerp(Lattice(n[0], r[0], n[1]+1, r[1]-1, n[2]+1, r[2]-1),
									    Lattice(n[0]+1, r[0]-1, n[1]+1, r[1]-1, n[2]+1, r[2]-1),
									    w[0]),
							    w[1]),
						    w[2]);
			    break;
		    case 4:
			    fValue = Lerp(Lerp(Lerp(Lerp(Lattice(n[0], r[0], n[1], r[1], n[2], r[2], n[3], r[3]),
										    Lattice(n[0]+1, r[0]-1, n[1], r[1], n[2], r[2], n[3], r[3]),
										    w[0]),
									    Lerp(Lattice(n[0], r[0], n[1]+1, r[1]-1, n[2], r[2], n[3], r[3]),
										    Lattice(n[0]+1, r[0]-1, n[1]+1, r[1]-1, n[2], r[2], n[3], r[3]),
										    w[0]),
									    w[1]),
									    Lerp(Lerp(Lattice(n[0], r[0], n[1], r[1], n[2]+1, r[2]-1, n[3], r[3]),
										    Lattice(n[0]+1, r[0]-1, n[1], r[1], n[2]+1, r[2]-1, n[3], r[3]),
										    w[0]),
									    Lerp(Lattice(n[0], r[0], n[1]+1, r[1]-1, n[2]+1, r[2]-1),
										    Lattice(n[0]+1, r[0]-1, n[1]+1, r[1]-1, n[2]+1, r[2]-1, n[3], r[3]),
										    w[0]),
									    w[1]),
							    w[2]),
						    Lerp(Lerp(Lerp(Lattice(n[0], r[0], n[1], r[1], n[2], r[2], n[3]+1, r[3]-1),
										    Lattice(n[0]+1, r[0]-1, n[1], r[1], n[2], r[2], n[3]+1, r[3]-1),
										    w[0]),
									    Lerp(Lattice(n[0], r[0], n[1]+1, r[1]-1, n[2], r[2], n[3]+1, r[3]-1),
										    Lattice(n[0]+1, r[0]-1, n[1]+1, r[1]-1, n[2], r[2], n[3]+1, r[3]-1),
										    w[0]),
									    w[1]),
									    Lerp(Lerp(Lattice(n[0], r[0], n[1], r[1], n[2]+1, r[2]-1, n[3]+1, r[3]-1),
										    Lattice(n[0]+1, r[0]-1, n[1], r[1], n[2]+1, r[2]-1, n[3]+1, r[3]-1),
										    w[0]),
									    Lerp(Lattice(n[0], r[0], n[1]+1, r[1]-1, n[2]+1, r[2]-1),
										    Lattice(n[0]+1, r[0]-1, n[1]+1, r[1]-1, n[2]+1, r[2]-1, n[3]+1, r[3]-1),
										    w[0]),
									    w[1]),
							    w[2]),
						    w[3]);
			    break;
	    }
	    return CLAMP(-0.99999f, 0.99999f, fValue);
    }

    float CFractal::fBm(float *f, const float fOctaves) const 
    {
	    // Initialize locals
	    float fValue = 0;
	    float fTemp[MAX_DIMENSIONS];
        uint i;
	    for(i=0; i<m_nDimensions; i++)
		    fTemp[i] = f[i];

	    // Inner loop of spectral construction, where the fractal is built
	    for(i=0; i<fOctaves; i++)
	    {
		    fValue += Noise(fTemp) * m_fExponent[i];
		    for(uint j=0; j<m_nDimensions; j++)
			    fTemp[j] *= m_fLacunarity;
	    }

	    // Take care of remainder in fOctaves
	    const float cfOctaves = fOctaves - (int)fOctaves;
	    if(cfOctaves > DELTA)
		    fValue += cfOctaves * Noise(fTemp) * m_fExponent[i];
	    return CLAMP(-0.99999f, 0.99999f, fValue);
    }

    float CFractal::fBmTest(float *f, const float fOctaves) const 
    {
	    // Initialize locals
	    float fValue = 0;
	    float fTemp[MAX_DIMENSIONS];
        uint i;
	    for(i=0; i<m_nDimensions; i++)
		    fTemp[i] = f[i] * 2;

	    //fOctaves *= Abs(Noise(fTemp)) + 1.0f;
	    //fOctaves = Clamp(2, 16, fOctaves);

	    // Inner loop of spectral construction, where the fractal is built
	    for(i=0; i<fOctaves; i++)
	    {
		    fValue += Noise(fTemp) * m_fExponent[i];
		    for(uint j=0; j<m_nDimensions; j++)
			    fTemp[j] *= m_fLacunarity;
	    }

	    // Take care of remainder in fOctaves
	    const float cfOctaves = fOctaves - (int)fOctaves;
	    if(cfOctaves > DELTA)
		    fValue += cfOctaves * Noise(fTemp) * m_fExponent[i];

	    if(fValue <= 0.0f)
		    return (float)-pow(-fValue, 0.7f);
	    return (float)pow(fValue, 1 + Noise(fTemp) * fValue);
    }

    float CFractal::Turbulence(float *f, const float fOctaves) const 
    {
	    // Initialize locals
	    float fValue = 0;
	    float fTemp[MAX_DIMENSIONS];
        uint i;
	    for(i=0; i<m_nDimensions; i++)
		    fTemp[i] = f[i];

	    // Inner loop of spectral construction, where the fractal is built
	    for(i=0; i<fOctaves; i++)
	    {
		    fValue += Abs(Noise(fTemp)) * m_fExponent[i];
		    for(uint j=0; j<m_nDimensions; j++)
			    fTemp[j] *= m_fLacunarity;
	    }

	    // Take care of remainder in fOctaves
	    const float cfOctaves = fOctaves - (int)fOctaves;
	    if(cfOctaves > DELTA)
		    fValue += cfOctaves * Abs(Noise(fTemp) * m_fExponent[i]);
	    return CLAMP(-0.99999f, 0.99999f, fValue);
    }

    float CFractal::Multifractal(float *f, const float fOctaves, 
                                const float fOffset) const 
    {
	    // Initialize locals
	    float fValue = 1;
	    float fTemp[MAX_DIMENSIONS];
	    uint i;
	    for(i=0; i<m_nDimensions; i++)
		    fTemp[i] = f[i];

	    // Inner loop of spectral construction, where the fractal is built
	    for(i=0; i<fOctaves; i++)
	    {
		    fValue *= Noise(fTemp) * m_fExponent[i] + fOffset;
		    for(uint j=0; j<m_nDimensions; j++)
			    fTemp[j] *= m_fLacunarity;
	    }

	    // Take care of remainder in fOctaves (shouldn't that be a multiply?)
	    const float cfOctaves = fOctaves - (int)fOctaves;
	    if(cfOctaves > DELTA)
		    fValue *= cfOctaves * (Noise(fTemp) * m_fExponent[i] + fOffset);
	    return CLAMP(-0.99999f, 0.99999f, fValue);
    }

    float CFractal::Heterofractal(float *f, const float fOctaves, 
                                const float fOffset) const 
    {
	    // Initialize locals
	    float fValue = Noise(f) + fOffset;
	    float fTemp[MAX_DIMENSIONS];
        uint i;
	    for(i=0; i<m_nDimensions; i++)
		    fTemp[i] = f[i] * m_fLacunarity;

	    // Inner loop of spectral construction, where the fractal is built
	    for(i=1; i<fOctaves; i++)
	    {
		    fValue += (Noise(fTemp) + fOffset) * m_fExponent[i] * fValue;
		    for(uint j=0; j<m_nDimensions; j++)
			    fTemp[j] *= m_fLacunarity;
	    }

	    // Take care of remainder in fOctaves
	    const float cfOctaves = fOctaves - (int)fOctaves;
	    if(cfOctaves > DELTA)
		    fValue += cfOctaves * (Noise(fTemp) + fOffset) * m_fExponent[i] * fValue;
	    return CLAMP(-0.99999f, 0.99999f, fValue);
    }

    float CFractal::HybridMultifractal(float *f, const float fOctaves, 
                                    const float fOffset, const float fGain) const 
    {
	    // Initialize locals
	    float fValue = (Noise(f) + fOffset) * m_fExponent[0];
	    float fWeight = fValue;
	    float fTemp[MAX_DIMENSIONS];
        uint i;
	    for(i=0; i<m_nDimensions; i++)
		    fTemp[i] = f[i] * m_fLacunarity;

	    // Inner loop of spectral construction, where the fractal is built
	    for(i=1; i<fOctaves; i++)
	    {
		    if(fWeight > 1)
			    fWeight = 1;
		    float fSignal = (Noise(fTemp) + fOffset) * m_fExponent[i];
		    fValue += fWeight * fSignal;
		    fWeight *= fGain * fSignal;
		    for(uint j=0; j<m_nDimensions; j++)
			    fTemp[j] *= m_fLacunarity;
	    }

	    // Take care of remainder in fOctaves
	    const float cfOctaves = fOctaves - (int)fOctaves;
	    if(cfOctaves > DELTA)
	    {
		    if(fWeight > 1)
			    fWeight = 1;
		    const float fSignal = (Noise(fTemp) + fOffset) * m_fExponent[i];
		    fValue += cfOctaves * fWeight * fSignal;
	    }
	    return CLAMP(-0.99999f, 0.99999f, fValue);
    }

    float CFractal::RidgedMultifractal(float *f, const float fOctaves, 
                                    const float fOffset, const float fGain) const 
    {
	    // Initialize locals
	    float fSignal = fOffset - Abs(Noise(f));
	    fSignal *= fSignal;
	    float fValue = fSignal;
	    float fTemp[MAX_DIMENSIONS];
        uint i;
	    for(i=0; i<m_nDimensions; i++)
		    fTemp[i] = f[i];

	    // Inner loop of spectral construction, where the fractal is built
	    for(i=1; i<fOctaves; i++)
	    {
		    for(uint j=0; j<m_nDimensions; j++)
			    fTemp[j] *= m_fLacunarity;
		    const float fWeight = Clamp(0, 1, fSignal * fGain);
		    fSignal = fOffset - Abs(Noise(fTemp));
		    fSignal *= fSignal;
		    fSignal *= fWeight;
		    fValue += fSignal * m_fExponent[i];
	    }
	    return CLAMP(-0.99999f, 0.99999f, fValue);
    }

}//namespace Ogre
