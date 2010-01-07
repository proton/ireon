#include "MapUtil.h"
#include "MapNormaler.h"

namespace Ogre
{

//----------------------------------------------------------------------
MapNormaler::MapNormaler()
{
    reset();
}
//----------------------------------------------------------------------
MapNormaler::~MapNormaler()
{
    delete[] Normals;
}

//----------------------------------------------------------------------
Vector3 *MapNormaler::getNormalData()
{
    return Normals;
}
//----------------------------------------------------------------------
Image *MapNormaler::getNormalMap()
{ 
    if (mAlreadyComputed == false)
    {
        CalcNormalMap();
        mAlreadyComputed = true;
    }
    return &NormalMap;
}
//----------------------------------------------------------------------
void MapNormaler::reset()
{
    mAlreadyComputed = false;
}

//----------------------------------------------------------------------
const Vector3 MapNormaler::getNormalAt (const uint x, const uint z)
{      
    if (mAlreadyComputed == false)
    {
        CalcNormalMap();
        mAlreadyComputed = true;
    }
    return Normals[( z * mWidth) + x];
}

//----------------------------------------------------------------------
void MapNormaler::CalcNormalMap()
{
    const uint  height = MapUtil::getSingleton().getMapHeight ();
    const uint  width = MapUtil::getSingleton().getMapWidth ();

    mHeight = height;
    mWidth = width;

    const uint size = height * width;

    uchar *NormalData = new uchar[size * 3];
    Normals = new Vector3 [size];

    std::cout << "normal Map Calc : ";

    // First General method : (9 adds and 6 muls + a normalization)
    //        *---v3--*
    //        |   |   |
    //        |   |   |
    //        v1--X--v2
    //        |   |   |
    //        |   |   |
    //        *---v4--*
    //
    //        U = v2 - v1;
    //        V = v4 - v3;
    //        N = Cross(U, V);
    //        N.normalise;
    //
    // BUT IN CASE OF A HEIGHTMAP : 
    //
    //   if you do some math by hand before you code, 
    //   you can see that N is immediately given by 
    //  Approximation (2 adds and a normalization)
    // 
    //        N = Vector3(z[x-1][y] - z[x+1][y], z[x][y-1] - z[x][y+1], 2); 
    //        N.normalise();
    //
    // or even using SOBEL operator VERY accurate! 
    // (14 adds and a normalization)
    //
    //       N = Vector3 (z[x-1][y-1] + z[x-1][y] + z[x-1][y] + z[x-1][y+1] - z[x+1][y-1] - z[x+1][y] - z[x+1][y] - z[x+1][y+1], 
    //                     z[x-1][y-1] + z[x][y-1] + z[x][y-1] + z[x+1][y-1] - z[x-1][y+1] - z[x][y+1] - z[x][y+1] - z[x+1][y+1], 
    //                     8);
    //       N.normalize();



    // the divider make sure we do respect proportion  (height and width proportional to y )
	const Real Divider = 1 / MapUtil::getSingleton ().scale.x;

    const Real *heightmap = MapUtil::getSingleton().getHeightData ();
	uint k = 0;
	uint j = 0;
	for( ; k < width; k++ )
	{
		Normals[k].x = 0;
		Normals[k].y = 1;
		Normals[k].z = 0;
		NormalData[j++] = static_cast <uchar> ((Normals[k].x + 1.0f) * 127.5f);
		NormalData[j++] = static_cast <uchar> ((Normals[k].y + 1.0f) * 127.5f);
		NormalData[j++] = static_cast <uchar> ((Normals[k].z + 1.0f) * 127.5f);
	}
    uint curr_row = width;
    for (uint z = 1; z < height; z++)
    {
        DEBUG_PROGRESS_OUTPUT(".")
/*		Normals[k].x = 0;
		Normals[k].y = 1;
		Normals[k].z = 0;
		NormalData[j++] = static_cast <uchar> ((Normals[k].x + 1.0f) * 127.5f);
		NormalData[j++] = static_cast <uchar> ((Normals[k].y + 1.0f) * 127.5f);
		NormalData[j++] = static_cast <uchar> ((Normals[k].z + 1.0f) * 127.5f);
        k++;*/
        for (uint x = 1; x < width; x++)
        {
            // Fast SOBEL filter and mathematically correct
            Normals[k].x =   (heightmap[x-1 + curr_row-width] + heightmap[x-1 + curr_row]
                            + heightmap[x-1 + curr_row]       + heightmap[x-1 + curr_row+width]
                            - heightmap[x+1 + curr_row-width] - heightmap[x+1 + curr_row] 
                            - heightmap[x+1 + curr_row]       - heightmap[x+1 + curr_row+width])
                            * Divider; 

            Normals[k].z =   (heightmap[x-1 + curr_row-width] + heightmap[x +   curr_row-width]
                            + heightmap[x +   curr_row-width] + heightmap[x+1 + curr_row-width]
                            - heightmap[x-1 + curr_row+width] - heightmap[x +   curr_row+width]
                            - heightmap[x +   curr_row+width] - heightmap[x+1 + curr_row+width])
                            * Divider;

            Normals[k].y = 8.0f; 

            // very Fast SOBEL filter
            // (not mathematically correct + approximation)
            // But gives same result as above 
            //Normals[k].x = heightmap[x - 1  + z)   - heightmap[x + 1 + z);
            //Normals[k].z = heightmap[x +      z-1) - heightmap[x     + z + 1);
            //Normals[k].y = 2.0f; 

            
            Normals[k].normalise ();

            NormalData[j++] = static_cast <uchar> ((Normals[k].x + 1.0f) * 127.5f);
            NormalData[j++] = static_cast <uchar> ((Normals[k].y + 1.0f) * 127.5f);
            NormalData[j++] = static_cast <uchar> ((Normals[k].z + 1.0f) * 127.5f);

            k++;
        }
        curr_row += width;
		Normals[k].x = 0;
		Normals[k].y = 1;
		Normals[k].z = 0;
		NormalData[j++] = static_cast <uchar> ((Normals[k].x + 1.0f) * 127.5f);
		NormalData[j++] = static_cast <uchar> ((Normals[k].y + 1.0f) * 127.5f);
		NormalData[j++] = static_cast <uchar> ((Normals[k].z + 1.0f) * 127.5f);
        k++;
    }
    NormalMap.loadDynamicImage(NormalData, static_cast <size_t> (width),
                                static_cast <size_t> (height), 1, PF_BYTE_RGB, true);
    std::cout << "\n";
}

}//namespace Ogre
