#include "Image.hpp"
#include <Windows.h>

//----------------------------------------------------------------------------//
// PixelFormat
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
uint BitsPerPixel(PixelFormat _format)
{
	static const uint BPP[] =
	{
		24, // PF_RGB8
		32, // PF_RGBA8
		32, // PF_D24S8
		4, // PF_DXT1
		8, // PF_DXT5
	};
	return BPP[_format];
}
//----------------------------------------------------------------------------//
bool IsCompressed(PixelFormat _format)
{
	return _format == PF_DXT1 || _format == PF_DXT5;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Image
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Image::Image(void) :
	m_format(PF_RGBA8),
	m_size(0),
	m_data(nullptr)
{
}
//----------------------------------------------------------------------------//
Image::~Image(void)
{
	delete[] m_data;
}
//----------------------------------------------------------------------------//
void Image::Realloc(PixelFormat _format, uint _width, uint _height)
{
	delete[] m_data;
	m_format = _format;
	m_size.Set(_width, _height);
	m_data = new uint8[(_width * _height * BitsPerPixel(_format)) >> 3];
}
//----------------------------------------------------------------------------//
void Image::RgbaToArgb(void* _dst, const void* _src, uint _numPixels)
{
	uint32* _dstp = (uint32*)_dst;
	uint32* _dste = _dstp + _numPixels;
	const uint32* _srcp = (const uint32*)_src;
	while (_dstp < _dste)
	{
		uint32 _color = *_srcp++;
		*_dstp++ = (_color >> 8) | (_color << 24);
	}
}
//----------------------------------------------------------------------------//
void Image::RgbToXrgb(void* _dst, const void* _src, uint _numPixels, uint8 _alpha)
{
	uint32* _dstp = (uint32*)_dst;
	uint32* _dste = _dstp + _numPixels;
	const uint8* _srcp = (const uint8*)_src;
	uint _a = _alpha << 24;
	while (_dstp < _dste)
	{
		uint8 _r = _srcp[0];
		uint8 _g = _srcp[1];
		uint8 _b = _srcp[2];
		_srcp += 3;
		*_dstp++ = (_r << 16) | (_g << 8) | _b | _a;
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Generator 
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
ImagePtr CreateBitmapFont(FontInfo& _info, const char* _name, uint _fheight, float _fwidth, bool _italic)
{
	int _tw = int(_fwidth * 1000) / 100;
	HFONT _fnt = CreateFontA(-int(_fheight), 0, 0, 0, _tw * 100, _italic, 0, 0, RUSSIAN_CHARSET, OUT_OUTLINE_PRECIS, CLIP_TT_ALWAYS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _name);
	HDC _dc = CreateCompatibleDC(0);
	SetTextColor(_dc, RGB(255, 255, 255));
	SetBkColor(_dc, RGB(0, 0, 0));
	SetBkMode(_dc, TRANSPARENT);
	SetMapMode(_dc, MM_TEXT);
	SetTextAlign(_dc, TA_TOP);
	SelectObject(_dc, _fnt);

	ABCFLOAT _abc[256];
	TEXTMETRICA	_tm;
	GetTextMetricsA(_dc, &_tm);
	GetCharABCWidthsFloatA(_dc, 0, 0xff, _abc);

	struct CharRect { uint x, y, w, h; } _chars[256];
	uint _height = _tm.tmHeight;
	_info.height = (float)_height;
	for (uint i = 0; i < 256; ++i)
	{
		CharRect& _ch = _chars[i];
		_ch.w = int(_abc[i].abcfB) + 2;
		_ch.h = _height;
	}

	uint _texWidth = 32, _texHeight = 32;
	for (;;)
	{
		bool _placed = true;
		for (uint x = 0, y = 0, i = 0; i < 256; ++i)
		{
			CharRect& _ch = _chars[i];
			if (x + _ch.w > _texWidth)
			{
				x = 0;
				y += _height;
				if (y + _height > _texHeight)
				{
					_placed = false;
					break;
				}
			}
			_ch.x = x;
			_ch.y = y;
			x += _ch.w;
		}
		if (_placed)
			break;
		if (_texWidth <= _texHeight)
			_texWidth <<= 1;
		else
			_texHeight <<= 1;
	}

	BITMAPINFO _bmi;
	memset(&_bmi, 0, sizeof(_bmi));
	_bmi.bmiHeader.biSize = sizeof(_bmi);
	_bmi.bmiHeader.biWidth = _texWidth;
	_bmi.bmiHeader.biHeight = -int(_texHeight);
	_bmi.bmiHeader.biBitCount = 32;
	_bmi.bmiHeader.biCompression = BI_RGB;
	_bmi.bmiHeader.biPlanes = 1;

	Color* _bmp;
	HBITMAP _bm = CreateDIBSection(_dc, &_bmi, DIB_RGB_COLORS, (void**)&_bmp, 0, 0);
	//memset(_img, 0, _tex_width * _tex_height * sizeof(Color));

	float _itw = 1.0f / float(_texWidth);
	float _ith = 1.0f / float(_texHeight);
	SelectObject(_dc, _bm);
	for (uint i = 0; i < 256; ++i)
	{
		uint8 c = (uint8)i;
		CharRect& _cr = _chars[i];
		FontInfo::Char& _ch = _info.chars[i];
		TextOutA(_dc, _cr.x - (int(_abc[i].abcfA) - 1), _cr.y, (const char*)&c, 1);
		_ch.size.Set((float)_cr.w, (float)_cr.h);
		_ch.tc[0].Set(_cr.x * _itw, _cr.y * _ith);
		_ch.tc[1].Set((_cr.x + _cr.w) * _itw, (_cr.y + _cr.h) * _ith);
	}
	GdiFlush();

	ImagePtr _img = new Image();
	_img->Realloc(PF_RGBA8, _texWidth, _texHeight);
	Color* _dstp = (Color*)_img->Pixels();
	Color* _dste = _dstp + _texWidth * _texHeight;
	while (_dstp < _dste)
	{
		*_dstp++ = _bmp->Abgr(); // abgr --> rgba
		++_bmp;
	}

	DeleteObject(_bm);
	DeleteDC(_dc);
	DeleteObject(_fnt);

	return _img;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//


