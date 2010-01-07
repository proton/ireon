//-----------------------------------------------------------------------------
//
// Map Splitter
// Copyright (C) 2004 by Spoke and Tuan kuranes
// Last modified:  23 Jun 2004
//
// Filename: MapSplitter.cpp
//
// Description: takes an image file and splits it into smaller images to used 
//				with Paging Landscape Plugin
//              
//      The images can be of any format 
//		Map Splitter : split large image into tiles for Paging Landscape Plugin.
//		Usage : MapSplitter <file> <tile size> <overlap>
//		tile size and overlap are in pixels
//		Height maps should use 256 and 1
//		Colour maps should use 256 and 0
//		overlap should be 1 for height maps and 0 for colour map
//
//-----------------------------------------------------------------------------
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
***************************************************************************/

// Required include files.
#include "MapUtil.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char **argv)
{

    //SET_TERM_HANDLER;

    Ogre::MapUtil *MapPrecompute = new Ogre::MapUtil ();
    
    MapPrecompute->Load();
    MapPrecompute->process ();

    delete MapPrecompute;
}
