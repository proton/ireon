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
MapVis.h
@brief

http://www.dgp.toronto.edu/people/JamesStewart/papers/egwr97.html

*/
#ifndef MapVis_H
#define MapVis_H

#include "Ogre.h"

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
 * 
 */
class MapVis
{
public:
    MapVis();
    ~MapVis();

     /**
    * @remarks 
    * empty all maps and prepare for a new set of calculation.
    */
    void reset ();


    bool isVis(int nFromX, int nFromY, int nFromZ, 
                    int nDestX, int nDestY) const;

    void calc(uint nZ, uint nDZ);

    bool save(const char* nFileName);

    int getpatchW() const  {  return patchW;  }
    int getpatchH() const  {  return patchH;  }
    int getDataZ() const  {  return mVisDataZ;  }
    int getDataDZ() const  {  return mVisDataDZ;  }

private:

    void setVis(int nX0, int nY0, int nZ0, 
                    int nX1, int nY1, bool nValue);

    void CalcMinMax();

    uint mVisDataZ, mVisDataDZ;
    uint mVisDataXY, mVisDataX2Y, mVisDataX2Y2;
    uint mVisDataW, mVisDataH, patchW, patchH;

    uchar* mVisData;
    uchar* minData;
    uchar* maxData;

    uint mVisDataSize;
};
}
#endif //MapVis_H
