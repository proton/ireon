/* Copyright (C) 2005 ireon.org developers council
  * $Id: colour.cpp 687 2006-08-22 17:30:48Z zak $
 
  *  See LICENSE for details
  *
  * Based on Ogre::ColourValue, original copyright information follows
  * Copyright (c) 2000-2005 The OGRE Team
  * This program is free software; you can redistribute it and/or modify it under
  * the terms of the GNU Lesser General Public License as published by the Free Software
  * Foundation; either version 2 of the License, or (at your option) any later
  * version.
  *
  */
 
 /**
  * @file colour.cpp
  * Colour
  */
 #include "ireon_ws/stdafx.h"
 #include "ireon_ws/resource/colour.h"
 
 Colour Colour::Black = Colour(0.0,0.0,0.0);
 Colour Colour::White = Colour(1.0,1.0,1.0);
 Colour Colour::Red = Colour(1.0,0.0,0.0);
 Colour Colour::Green = Colour(0.0,1.0,0.0);
 Colour Colour::Blue = Colour(0.0,0.0,1.0);
 	
 uint Colour::getRGBA(void) const
 {
 	byte val8;
 	uint val32 = 0;
 
 	// Convert to 32bit pattern
 	// (RGBA = 8888)
 
 	// Red
 	val8 = static_cast<byte>(r * 255);
 	val32 = val8 << 24;
 
 	// Green
 	val8 = static_cast<byte>(g * 255);
 	val32 += val8 << 16;
 
 	// Blue
 	val8 = static_cast<byte>(b * 255);
 	val32 += val8 << 8;
 
 	// Alpha
 	val8 = static_cast<byte>(a * 255);
 	val32 += val8;
 
 	return val32;
 }
 	//---------------------------------------------------------------------
 uint Colour::getARGB(void) const
 {
 	byte val8;
 	uint val32 = 0;
 
 	// Convert to 32bit pattern
 	// (ARGB = 8888)
 
 	// Alpha
 	val8 = static_cast<byte>(a * 255);
 	val32 = val8 << 24;
 
 	// Red
 	val8 = static_cast<byte>(r * 255);
 	val32 += val8 << 16;
 
 	// Green
 	val8 = static_cast<byte>(g * 255);
 	val32 += val8 << 8;
 
 	// Blue
 	val8 = static_cast<byte>(b * 255);
 	val32 += val8;
 
 
 	return val32;
 }
 	//---------------------------------------------------------------------
 uint Colour::getABGR(void) const
 {
 	byte val8;
 	uint val32 = 0;
 
 	// Convert to 32bit pattern
 	// (ABRG = 8888)
 
 	// Alpha
 	val8 = static_cast<byte>(a * 255);
 	val32 = val8 << 24;
 
 	// Blue
 	val8 = static_cast<byte>(b * 255);
 	val32 += val8 << 16;
 
 	// Green
 	val8 = static_cast<byte>(g * 255);
 	val32 += val8 << 8;
 
 	// Red
 	val8 = static_cast<byte>(r * 255);
 	val32 += val8;
 
 
 	return val32;
 }
 	//---------------------------------------------------------------------
 void Colour::setRGBA(const uint val)
 {
 	uint val32 = val;
 
 	// Convert from 32bit pattern
 	// (uint = 8888)
 
 	// Red
 	r = static_cast<byte>(val32 >> 24) / 255.0f;
 
 	// Green
 	g = static_cast<byte>(val32 >> 16) / 255.0f;
 
 	// Blue
 	b = static_cast<byte>(val32 >> 8) / 255.0f;
 
 	// Alpha
 	a = static_cast<byte>(val32) / 255.0f;
 }
 	//---------------------------------------------------------------------
 void Colour::setARGB(const uint val)
 {
 	uint val32 = val;
 
 	// Convert from 32bit pattern
 	// (uint = 8888)
 
 	// Alpha
 	a = static_cast<byte>(val32 >> 24) / 255.0f;
 
 	// Red
 	r = static_cast<byte>(val32 >> 16) / 255.0f;
 
 	// Green
 	g = static_cast<byte>(val32 >> 8) / 255.0f;
 
 	// Blue
 	b = static_cast<byte>(val32) / 255.0f;
 }
 	//---------------------------------------------------------------------
 void Colour::setABGR(const uint val)
 {
 	uint val32 = val;
 
 	// Convert from 32bit pattern
 	// (ABGR = 8888)
 
 	// Alpha
 	a = static_cast<byte>(val32 >> 24) / 255.0f;
 
 	// Blue
 	b = static_cast<byte>(val32 >> 16) / 255.0f;
 
 	// Green
 	g = static_cast<byte>(val32 >> 8) / 255.0f;
 
 	// Red
 	r = static_cast<byte>(val32) / 255.0f;
 }
