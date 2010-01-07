/* Copyright (C) 2005 ireon.org developers council
 * $Id: image.cpp 782 2006-10-09 18:51:33Z zak $

 *  See LICENSE for details
 *
 * Based on Ogre::Image, original copyright information follows
 * Copyright (c) 2000-2005 The OGRE Team
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 */

/**
 * @file image.cpp
 * Image class
 */

#include "ireon_ws/stdafx.h"
#include "IL/il.h"
#include "common/resource/resource_manager.h"
#include "ireon_ws/resource/image.h"
#include "common/data.h"

bool CImage::m_ilInitialized = false;

CImage::CImage():
		CLogMixin("common.image"),
		m_width(0),
		m_height(0),
		m_size(0),
		m_buffer( NULL )
{
};

CImage::CImage( const CImage &img ):
	CLogMixin("common.image"),
	m_buffer( NULL )
{
	// call assignment operator
	*this = img;
}

CImage::~CImage()
{
	if( m_buffer  )
	{
		delete[] m_buffer;
		m_buffer = NULL;
	}
}

CImage & CImage::operator = ( const CImage &img )
{
	if( m_buffer  )
	{
		delete[] m_buffer;
		m_buffer = NULL;
	}
	m_width = img.m_width;
	m_height = img.m_height;
	m_format = img.m_format;
	m_size = img.m_size;
	m_pixelSize = img.m_pixelSize;
	//Only create/copy when previous data was not dynamic data
	m_buffer = new byte[ m_size ];
	memcpy( m_buffer, img.m_buffer, m_size );
	return *this;
}

ImageFormatDescription CImage::m_formatDesc[CImage::IF_COUNT] =
{
	{
		//IF_UNKNOWN
		0,
		0,
		0,0,0,0,
		0,0,0,0
	},
	{
		//IF_L8
		1,
		IFF_LUMINANCE,
		8,0,0,0,
		0,0,0,0
	},
 	{
		//IF_L16
		2,
		IFF_LUMINANCE,
		16,0,0,0,
		0,0,0,0
	},
	{
		//IF_A8
		1,
		IFF_HAS_ALPHA,
		0,0,0,8,
		0,0,0,0
	},
	{
		//IF_A8L8
		2,
		IFF_HAS_ALPHA,
		8,0,0,8,
		0,0,0,0
	},
	{
		//IF_R8G8B8
		3,
		0,
		8,8,8,0,
		16, 8, 0, 0 
	},
	{
		//IF_B8G8R8
		3,
		0,
		8,8,8,0,
		0, 8, 16, 0 
	},
	{
		//IF_A8R8G8B8
		4,
		IFF_HAS_ALPHA,
		8,8,8,8,
		16, 8, 0, 24 
	},
	{
		//IF_A8B8G8R8
		4,
		IFF_HAS_ALPHA,
		8,8,8,8,
		0, 8, 16, 24 
	},
	{
		//IF_B8G8R8A8
		4,
		IFF_HAS_ALPHA,
		8,8,8,8,
		8, 16, 24, 0 
	},
	{
		//IF_R8G8B8A8
		4,
		IFF_HAS_ALPHA,
		8,8,8,8,
		24, 16, 8, 0 
	}
};

void CImage::initializeIl()
{
	if( !this->m_ilInitialized )
	{
		ilInit();
		ilEnable( IL_FILE_OVERWRITE );
		m_ilInitialized = true;
	}
};

CImage::Format CImage::ilFormat2ImageFormat(ILint ImageFormat, ILint ImageType)
{
	Format fmt = IF_UNKNOWN;
	switch( ImageFormat )
	{
		/* Compressed formats -- ignore type */
		case IL_DXT1:   /*fmt = IF_DXT1;*/ break;
		case IL_DXT2:   /*fmt = IF_DXT2;*/ break;
		case IL_DXT3:   /*fmt = IF_DXT3;*/ break;
		case IL_DXT4:   /*fmt = IF_DXT4;*/ break;
		case IL_DXT5:   /*fmt = IF_DXT5;*/ break;
		/* Normal formats */
	case IL_RGB:
			switch(ImageType)
	    {
			case IL_FLOAT:  /*fmt = IF_FLOAT32_RGB;*/ break;
			case IL_UNSIGNED_SHORT: 
			case IL_SHORT:  /*fmt = IF_SHORT_RGBA;*/ break;
			default:       	fmt = IF_R8G8B8; break;
			}
	    break;
	case IL_BGR:
			switch(ImageType)
	    {
			case IL_FLOAT:  /*fmt = IF_FLOAT32_RGB;*/ break;
			case IL_UNSIGNED_SHORT: 
			case IL_SHORT:  /*fmt = IF_SHORT_RGBA;*/ break;
			default:       	fmt = IF_B8G8R8; break;
			}
	    break;	    
	case IL_RGBA:
			switch(ImageType)
	    {
			case IL_FLOAT:  /*fmt = IF_FLOAT32_RGBA;*/ break;
			case IL_UNSIGNED_SHORT: 
			case IL_SHORT:  /*fmt = IF_SHORT_RGBA;*/ break;
			default:       	fmt = IF_R8G8B8A8; break;
			}
	    break;
	case IL_BGRA:
			switch(ImageType)
	    {
			case IL_FLOAT:  /*fmt = IF_FLOAT32_RGBA;*/ break;
			case IL_UNSIGNED_SHORT: 
			case IL_SHORT:  /*fmt = IF_SHORT_RGBA;*/ break;
			default:       	fmt = IF_B8G8R8A8; break;
			}
	    break;
	case IL_LUMINANCE:
	    switch(ImageType)
	    {
	    case IL_BYTE:
	    case IL_UNSIGNED_BYTE:
		fmt = IF_L8;
		break;
	    default:
		fmt = IF_L16;
	    }
	    break;	    
	case IL_LUMINANCE_ALPHA:
			fmt = IF_A8L8;
	    break;
	}  
	return fmt;	
 };

ILenum ilTypeFromExt(const String& ext)
{
#ifdef IL_TGA
	if (ext=="tga" || ext=="vda" ||
		ext=="icb" || ext=="vst")
		return IL_TGA;
#endif
#ifdef IL_JPG
	if (ext=="jpg" || ext=="jpe" || ext=="jpeg")
		return IL_JPG;
#endif
#ifdef IL_DDS
	if (ext=="dds")
		return IL_DDS;
#endif
#ifdef IL_PNG
	if (ext=="png")
		return IL_PNG;
#endif
#ifdef IL_BMP
	if (ext=="bmp" || ext=="dib")
		return IL_BMP;
#endif
#ifdef IL_GIF
	if (ext=="gif")
		return IL_GIF;
#endif
#ifdef IL_CUT
	if (ext=="cut")
		return IL_CUT;
#endif
#ifdef IL_HDR
	if (ext=="hdr")
		return IL_HDR;
#endif
#ifdef IL_ICO
	if (ext=="ico" || ext=="cur")
		return IL_ICO;
#endif
#ifdef IL_JNG
	if (ext=="jng")
		return IL_JNG;
#endif
#ifdef IL_LIF
	if (ext=="lif")
		return IL_LIF;
#endif
#ifdef IL_MDL
	if (ext=="mdl")
		return IL_MDL;
#endif
#ifdef IL_MNG
	if (ext=="mng" || ext=="jng")
		return IL_MNG;
#endif
#ifdef IL_PCD
	if (ext=="pcd")
		return IL_PCD;
#endif
#ifdef IL_PCX
	if (ext=="pcx")
		return IL_PCX;
#endif
#ifdef IL_PIC
	if (ext=="pic")
		return IL_PIC;
#endif
#ifdef IL_PIX
	if (ext=="pix")
		return IL_PIX;
#endif
#ifdef IL_PNM
	if (ext=="pbm" || ext=="pgm" ||
		ext=="pnm" || ext=="ppm")
			return IL_PNM;
#endif
#ifdef IL_PSD
	if (ext=="psd" || ext=="pdd")
		return IL_PSD;
#endif
#ifdef IL_PSP
	if (ext=="psp")
		return IL_PSP;
#endif
#ifdef IL_PXR
	if (ext=="pxr")
		return IL_PXR;
#endif
#ifdef IL_SGI
	if (ext=="sgi" || ext=="bw" ||
		ext=="rgb" || ext=="rgba")
			return IL_SGI;
#endif
#ifdef IL_TIF
	if (ext=="tif" || ext=="tiff")
		return IL_TIF;
#endif
#ifdef IL_WAL
	if (ext=="wal")
		return IL_WAL;
#endif
#ifdef IL_XPM
	if (ext=="xpm")
		return IL_XPM;
#endif
	
	return IL_TYPE_UNKNOWN;
};

void CImage::load(DataPtr data, const String& type)
{
	if( !data )
	{
		_log (_error, _loc, _("Loading image from empty data stream.") );
		return;
	}

	initializeIl();

	if( m_buffer )
	{
		delete[] m_buffer;
		m_buffer = NULL;
	}

	// DevIL variables
	ILuint imageName;

	ILint imageFormat, imageType;

	// Load the image
	ilGenImages( 1, &imageName );
	ilBindImage( imageName );

	// Put it right side up
	ilEnable(IL_ORIGIN_SET);
	ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_UPPER_LEFT);

	// Keep DXTC(compressed) data if present
	ilSetInteger(IL_KEEP_DXTC_DATA, IL_TRUE);

	int ilType = ilTypeFromExt(type);

	ilLoadL(ilType,static_cast< ILvoid* >(const_cast< char* >(data->data())),static_cast< ILuint >(data->size()));

	// Check if everything was ok
	ILenum PossibleError = ilGetError() ;
	if( PossibleError != IL_NO_ERROR )
	{
		_log (_error, _loc, _("Error loading image of type '%s'."), type.c_str() );
		return;
	}

	imageFormat = ilGetInteger( IL_IMAGE_FORMAT );
	imageType = ilGetInteger( IL_IMAGE_TYPE );

	// Convert image if imageType is incompatible with us (double or long)
	if(imageType != IL_BYTE && imageType != IL_UNSIGNED_BYTE && 
		imageType != IL_FLOAT &&
		imageType != IL_UNSIGNED_SHORT && imageType != IL_SHORT)
	{
		ilConvertImage(imageFormat, IL_FLOAT);
		imageType = IL_FLOAT;
	}
	// Converted paletted images
	if(imageFormat == IL_COLOUR_INDEX)
	{
		ilConvertImage(IL_BGRA, IL_UNSIGNED_BYTE);
		imageFormat = IL_BGRA;
		imageType = IL_UNSIGNED_BYTE;
	}

	// Now sets some variables
	m_pixelSize = ilGetInteger( IL_IMAGE_BYTES_PER_PIXEL ); 
	m_format = ilFormat2ImageFormat( imageFormat, imageType );
	m_width = ilGetInteger( IL_IMAGE_WIDTH );
	m_height = ilGetInteger( IL_IMAGE_HEIGHT );
	m_size = ilGetInteger( IL_IMAGE_SIZE_OF_DATA );

	if(m_format == IF_UNKNOWN)
	{
		_log (_error, _loc, _("Unsupported image format '%s'."), type.c_str() );
		ilDeleteImages( 1, &imageName );
		return;
	}

	m_buffer = new byte[m_size];
	memcpy(m_buffer,ilGetData(),m_size);

	// Restore IL state
	ilDisable(IL_ORIGIN_SET);
	ilDisable(IL_FORMAT_SET);

	ilDeleteImages( 1, &imageName );
};

void CImage::load(const String& file)
{
	String strExt;

	size_t pos = file.find_last_of(".");
	if( pos == String::npos )
	{
		_log (_error, _loc, _("Invalid file name, no extention: '%s'."), file.c_str() );
		return;
	}

	while( pos != file.length() - 1 )
		strExt += file[++pos];

	load(CResourceManager::instance()->load(file),strExt);
};

bool CImage::hasAlpha() const
{
	return ((m_formatDesc[m_format].m_flags & IFF_HAS_ALPHA) != 0);
};

inline float fixedToFloat(uint value, uint bits)
{
	return (float)value/(float)((1<<bits)-1);
}

inline unsigned int intRead(const void *src, int n)
{
	switch(n)
	{
		case 1:
		    return ((byte*)src)[0];
		case 2:
			return ((ushort*)src)[0];
		case 3:
			return ((uint)((byte*)src)[0]<<16)|
				((uint)((byte*)src)[1]<<8)|
				((uint)((byte*)src)[2]);
			return ((uint32)((uint8*)src)[0])|
			    ((uint)((byte*)src)[1]<<8)|
				((uint)((byte*)src)[2]<<16);
		case 4:
			return ((uint*)src)[0];
	} 
 return 0; // ?
}	

Colour CImage::getColourAt(uint x, uint y)
{
	Colour val;	
	const ImageFormatDescription &des = m_formatDesc[m_format];
	// Shortcut for integer formats unpacking
	uint value = intRead(m_buffer + (m_pixelSize * (m_width * y + x)),des.m_pixelSize);
	const uint mask = (1 << des.rbits) - 1;
	if(des.m_flags & IFF_LUMINANCE) 
	{
		// Luminance format -- only rbits used
		val.r = val.g = val.b = fixedToFloat(
			(value & (mask << des.rshift))>>des.rshift, des.rbits);
	} 
	else
	{ 
		val.r = fixedToFloat((value & (mask << des.rshift))>>des.rshift, des.rbits);
		val.g = fixedToFloat((value & (mask << des.gshift))>>des.gshift, des.gbits);
		val.b = fixedToFloat((value & (mask << des.bshift))>>des.bshift, des.bbits);
	}
	if(des.m_flags & IFF_HAS_ALPHA)
	{
		val.a = fixedToFloat((value & (mask << des.ashift))>>des.ashift, des.abits);
	}
	else
	{
		val.a = 1.0f; // No alpha, default a component to full
	}
	return val;
};
