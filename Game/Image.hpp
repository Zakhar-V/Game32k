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
		Vec2 tc[2];	// lt, rb
	}
	chars[256];
	float height;
};

//----------------------------------------------------------------------------//
// PixelFormat
//----------------------------------------------------------------------------//

enum PixelFormat : uint16
{
	PF_RGB8,
	PF_RGBA8,
	PF_RGB10A2,
	PF_R32F,
	PF_RG32F,
	PF_RGB32F,
	PF_RGBA32F,
	PF_RGBA16F,
	PF_RG11B10F,
	PF_D24S8,
	PF_DXT1,
	PF_DXT5,
};

uint BitsPerPixel(PixelFormat _format);
bool IsCompressed(PixelFormat _format);

//----------------------------------------------------------------------------//
// PixelBox
//----------------------------------------------------------------------------//

class PixelBox : public NonCopyable
{
public:
	PixelBox(void);
	~PixelBox(void);
	void Realloc(PixelFormat _format, uint _width, uint _height, uint _depth = 1);
	uint Width(void) { return m_width; }
	uint Height(void) { return m_height; }
	uint Depth(void) { return m_depth; }
	uint Bpp(void) { return BitsPerPixel(m_format); }
	PixelFormat Format(void) { return m_format; }
	uint8* Data(void) { return m_data; }

protected:

	PixelFormat m_format;
	uint m_width;
	uint m_height;
	uint m_depth;
	uint8* m_data;
};

//----------------------------------------------------------------------------//
// Image
//----------------------------------------------------------------------------//

class Image : public RefCounted
{
public:
	Image(void);
	~Image(void);
	void Realloc(uint _width, uint _height, uint _channels);
	uint Width(void) { return m_width; }
	uint Height(void) { return m_height; }
	const Vec2& InvSize(void) { return m_invSize; }
	PixelFormat Format(void) { return (PixelFormat)(PF_R32F + (m_channels-1)); }
	float* Data(void) { return m_data; }
	uint Bpp(void) { return m_channels * sizeof(float); }

	Vec2 GetCoord(const Vec2& _tc, bool _repeat = true);
	Color Sample(const Vec2& _tc, bool _smoothed = true, bool _repeat = true);

	void CreateBitmapFont(FontInfo& _info, const char* _name, uint _fheight, float _fwidth = 0.5f, bool _italic = false);
	int CreateNoize(uint _size, uint _channels, int _rseed = 0);
	void CreatePerlin(uint _size, float _scale = 1, const Vec2& _offset = 0, int _rseed = 100, uint _iterations = 4);


protected:
	uint m_width;
	uint m_height;
	Vec2 m_invSize;
	uint m_channels;
	float* m_data;
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
