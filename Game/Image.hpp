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
	PF_RGBX8,
	PF_RGBA8, // do not use for render targets
	PF_D24S8,
	PF_DXT1,
	PF_DXT5,
};

uint BitsPerPixel(PixelFormat _format);
bool IsCompressed(PixelFormat _format);

void RgbaToArgb(void* _dst, const void* _src, uint _numPixels);
//void RgbxToXrgb(void* _dst, const void* _src, uint _numPixels, uint8 _alpha = 0xff);

//----------------------------------------------------------------------------//
// Image
//----------------------------------------------------------------------------//

//TODO: 3D & compressed

class Image : public RefCounted
{
public:
	Image(void);
	~Image(void);
	void Realloc(PixelFormat _format, uint _width, uint _height);
	uint Width(void) { return m_width; }
	uint Height(void) { return m_height; }
	const Vec2& InvSize(void) { return m_invSize; }
	PixelFormat Format(void) { return m_format; }
	uint8* RawData(void) { return m_data; }
	Color* Pixels(void) { return m_pixels; }
	uint Bpp(void) { return BitsPerPixel(m_format); }
	Vec2 GetCoord(const Vec2& _tc, bool _repeat = true);
	Color Sample(const Vec2& _tc, bool _smoothed = true, bool _repeat = true);
	ImagePtr CreateLod(void);

	void CreateBitmapFont(FontInfo& _info, const char* _name, uint _fheight, float _fwidth = 0.5f, bool _italic = false);
	int CreateNoize(uint _size, int _rseed = 0);
	void CreatePerlin(uint _size, float _scale = 1, const Vec2& _offset = 0, int _rseed = 100, uint _iterations = 4);


protected:
	PixelFormat m_format;
	uint m_width;
	uint m_height;
	Vec2 m_invSize;
	union
	{
		uint8* m_data; // raw
		Color* m_pixels; // rgba
	};
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
