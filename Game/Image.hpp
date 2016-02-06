#pragma once

#include "Math.hpp"
#include "Object.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

typedef Ptr<class Image> ImagePtr;

struct FontInfo
{
	struct Char
	{
		Vec2 size;
		Vec2 tc[2];
	}
	chars[256];
	float height;
};

//----------------------------------------------------------------------------//
// PixelFormat
//----------------------------------------------------------------------------//

enum PixelFormat
{
	PF_RGB8,
	PF_RGBA8, // do not use for render targets
	PF_D24S8,
	PF_DXT1,
	PF_DXT5,
};

uint BitsPerPixel(PixelFormat _format);
bool IsCompressed(PixelFormat _format);

//----------------------------------------------------------------------------//
// Image
//----------------------------------------------------------------------------//

class Image : public RefCounted
{
public:
	Image(void);
	~Image(void);
	void Realloc(PixelFormat _format, uint _width, uint _height);
	const Vec2i& Size(void) { return m_size; }
	PixelFormat Format(void) { return m_format; }
	uint8* Pixels(void) { return m_data; }
	uint Bpp(void) { return BitsPerPixel(m_format); }
	//Color Sample(float _x, float _y, bool _nearest, int _addressing)

	static void RgbaToArgb(void* _dst, const void* _src, uint _numPixels);
	static void RgbToXrgb(void* _dst, const void* _src, uint _numPixels, uint8 _alpha = 0xff);

protected:
	PixelFormat m_format;
	Vec2i m_size;
	uint8* m_data;
};

//----------------------------------------------------------------------------//
// Generator
//----------------------------------------------------------------------------//

ImagePtr CreateBitmapFont(FontInfo& _info, const char* _name, uint _fheight, float _fwidth = 0.5f, bool _italic = false);


//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
