/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/
#include "OgreColourValue.h"

namespace Ogre {

    ColourValue ColourValue::Black = ColourValue(0.0,0.0,0.0);
    ColourValue ColourValue::White = ColourValue(1.0,1.0,1.0);
    ColourValue ColourValue::Red = ColourValue(1.0,0.0,0.0);
    ColourValue ColourValue::Green = ColourValue(0.0,1.0,0.0);
    ColourValue ColourValue::Blue = ColourValue(0.0,0.0,1.0);

    //---------------------------------------------------------------------
    RGBA ColourValue::getAsRGBA(void) const
    {
        uint8 val8;
        uint32 val32 = 0;

        // Convert to 32bit pattern
        // (RGBA = 8888)

        // Red
        val8 = static_cast<uint8>(r * 255);
        val32 = val8 << 24;

        // Green
        val8 = static_cast<uint8>(g * 255);
        val32 += val8 << 16;

        // Blue
        val8 = static_cast<uint8>(b * 255);
        val32 += val8 << 8;

        // Alpha
        val8 = static_cast<uint8>(a * 255);
        val32 += val8;

        return val32;
    }
    //---------------------------------------------------------------------
    ARGB ColourValue::getAsARGB(void) const
    {
        uint8 val8;
        uint32 val32 = 0;

        // Convert to 32bit pattern
        // (ARGB = 8888)

        // Alpha
        val8 = static_cast<uint8>(a * 255);
        val32 = val8 << 24;

        // Red
        val8 = static_cast<uint8>(r * 255);
        val32 += val8 << 16;

        // Green
        val8 = static_cast<uint8>(g * 255);
        val32 += val8 << 8;

        // Blue
        val8 = static_cast<uint8>(b * 255);
        val32 += val8;


        return val32;
    }
    //---------------------------------------------------------------------
    ABGR ColourValue::getAsABGR(void) const
    {
        uint8 val8;
        uint32 val32 = 0;

        // Convert to 32bit pattern
        // (ABRG = 8888)

        // Alpha
        val8 = static_cast<uint8>(a * 255);
        val32 = val8 << 24;

        // Blue
        val8 = static_cast<uint8>(b * 255);
        val32 += val8 << 16;

        // Green
        val8 = static_cast<uint8>(g * 255);
        val32 += val8 << 8;

        // Red
        val8 = static_cast<uint8>(r * 255);
        val32 += val8;


        return val32;
    }
    //---------------------------------------------------------------------
    void ColourValue::setAsRGBA(const RGBA val)
    {
        uint32 val32 = val;

        // Convert from 32bit pattern
        // (RGBA = 8888)

        // Red
        r = static_cast<uint8>(val32 >> 24) / 255.0f;

        // Green
        g = static_cast<uint8>(val32 >> 16) / 255.0f;

        // Blue
        b = static_cast<uint8>(val32 >> 8) / 255.0f;

        // Alpha
        a = static_cast<uint8>(val32) / 255.0f;
    }
    //---------------------------------------------------------------------
    void ColourValue::setAsARGB(const ARGB val)
    {
        uint32 val32 = val;

        // Convert from 32bit pattern
        // (ARGB = 8888)

        // Alpha
        a = static_cast<uint8>(val32 >> 24) / 255.0f;

        // Red
        r = static_cast<uint8>(val32 >> 16) / 255.0f;

        // Green
        g = static_cast<uint8>(val32 >> 8) / 255.0f;

        // Blue
        b = static_cast<uint8>(val32) / 255.0f;
    }
    //---------------------------------------------------------------------
    void ColourValue::setAsABGR(const ABGR val)
    {
        uint32 val32 = val;

        // Convert from 32bit pattern
        // (ABGR = 8888)

        // Alpha
        a = static_cast<uint8>(val32 >> 24) / 255.0f;

        // Blue
        b = static_cast<uint8>(val32 >> 16) / 255.0f;

        // Green
        g = static_cast<uint8>(val32 >> 8) / 255.0f;

        // Red
        r = static_cast<uint8>(val32) / 255.0f;
    }
    //---------------------------------------------------------------------
    bool ColourValue::operator==(const ColourValue& rhs) const
    {
        return (r == rhs.r &&
            g == rhs.g &&
            b == rhs.b &&
            a == rhs.a);
    }
    //---------------------------------------------------------------------
    bool ColourValue::operator!=(const ColourValue& rhs) const
    {
        return !(*this == rhs);
    }

}

