#pragma once

#include "Math.hpp"
#include "Object.hpp"
#include "Graphics.hpp"

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

enum ImageSampleFlags : uint
{
	ISF_Nearest = 0,
	ISF_Linear = 0x1,
	ISF_Sparse = 0x2,
	ISF_Triangle = 0x4,

	ISF_Repeat = 0,
	ISF_Clamp = 0x8,
};

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
	Vec4 Sample(const Vec2& _tc, uint _flags = ISF_Nearest | ISF_Repeat, float _divisor = 1);

	void GetMinMax(Vec4& _mn, Vec4& _mx);

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
