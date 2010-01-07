/* Copyright (C) 2005 ireon.org developers council
 * $Id: image.h 782 2006-10-09 18:51:33Z zak $

 *  See LICENSE for details
 */

/**
 * @file image.h
 * Image class
 */
#ifndef _IMAGE_H
#define _IMAGE_H

#include "ireon_ws/resource/colour.h"

enum ImageFormatFlags
{
	IFF_HAS_ALPHA = 1 << 0,
	IFF_LUMINANCE = 1 << 1
};

struct ImageFormatDescription
{
	byte m_pixelSize;
	byte m_flags;

	byte rbits,gbits,bbits,abits;
	byte rshift, gshift, bshift, ashift;
};

#include "common/log_mixin.h"

class CImage : private CLogMixin
{
public:

	CImage();

	CImage(const CImage &img);

	virtual ~CImage();

	CImage& operator = (const CImage& img);

	enum Format
	{
        /// Unknown pixel format.
        IF_UNKNOWN = 0,
        /// 8-bit pixel format, all bits luminace.
        IF_L8,
        /// 16-bit pixel format, all bits luminace.
        IF_L16,
        /// 8-bit pixel format, all bits alpha.
        IF_A8,
		/// 2 byte pixel format, 1 byte luminance, 1 byte alpha
		IF_A8L8,
        /// 24-bit pixel format, 8 bits for red, green and blue.
        IF_R8G8B8,
        /// 24-bit pixel format, 8 bits for blue, green and red.
        IF_B8G8R8,
        /// 32-bit pixel format, 8 bits for alpha, red, green and blue.
        IF_A8R8G8B8,
        /// 32-bit pixel format, 8 bits for blue, green, red and alpha.
        IF_A8B8G8R8,
        /// 32-bit pixel format, 8 bits for blue, green, red and alpha.
        IF_B8G8R8A8,
		/// 32-bit pixel format, 8 bits for red, green, blue and alpha.
		IF_R8G8B8A8,
		// Number of pixel formats currently defined
        IF_COUNT
	};

	void load(const String& file);

	void load(DataPtr data, const String& type);

//	void save(const String& file);

	byte* getData() const {return m_buffer;};

	size_t getSize() const {return this->m_size;}

	size_t getWidth() const {return m_width;}

	size_t getHeight() const {return m_height;}

	Format getFormat() const {return m_format;}

	/// Bytes per pixel
	byte getPixelSize() const {return m_pixelSize;};

	bool hasAlpha() const;

	Colour getColourAt(uint x, uint y);

	void initializeIl();

	Format ilFormat2ImageFormat(int format, int type);

protected:

        // The width of the image in pixels
        size_t m_width;
        // The height of the image in pixels
        size_t m_height;
        // The size of the image buffer
        size_t m_size;
		// The pixel format of the image
        Format m_format;

		// The number of bytes per pixel
        byte m_pixelSize;
        byte* m_buffer;

		static ImageFormatDescription m_formatDesc[IF_COUNT];

		static bool m_ilInitialized;
};


#endif
