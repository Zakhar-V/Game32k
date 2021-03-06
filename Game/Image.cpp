#include "Image.hpp"
#include <Windows.h>

//----------------------------------------------------------------------------//
// PixelBox
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
PixelBox::PixelBox(void) :
	m_format(PF_RGBA8),
	m_width(0),
	m_height(0),
	m_depth(0),
	m_data(nullptr)
{
}
//----------------------------------------------------------------------------//
PixelBox::~PixelBox(void)
{
}
//----------------------------------------------------------------------------//
void PixelBox::Realloc(PixelFormat _format, uint _width, uint _height, uint _depth)
{
	delete[] m_data;
	m_format = _format;
	m_width = _width;
	m_height = _height;
	m_depth = _depth;
	m_data = new uint8[((_width * _height * BitsPerPixel(_format)) >> 3) * m_depth];
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Image
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Image::Image(void) :
	m_width(0),
	m_height(0),
	m_invSize(0),
	m_channels(1),
	m_data(nullptr)
{
}
//----------------------------------------------------------------------------//
Image::~Image(void)
{
	delete[] m_data;
}
//----------------------------------------------------------------------------//
void Image::Realloc(uint _width, uint _height, uint _channels)
{
	ASSERT(_channels >= 1 && _channels <= 4);
	if (m_width == _width && m_height == _height && m_channels == _channels)
		return;
	delete[] m_data;
	m_width = _width;
	m_height = _height;
	m_channels = _channels;
	m_invSize.Set(1.0f / _width, 1.0f / _height);
	m_data = new float[_width * _height * _channels];
}
//----------------------------------------------------------------------------//
Vec2 Image::GetCoord(const Vec2& _tc, bool _repeat)
{
	Vec2 _coord;
	if (_repeat)
	{
		_coord.x = Wrap(_tc.x * m_width, 0.0f, (float)m_width-1);
		_coord.y = Wrap(_tc.y * m_height, 0.0f, (float)m_height - 1);
	}
	else
	{
		_coord.x = Clamp<float>(_tc.x * m_width, 0.0f, (float)m_width - 1);
		_coord.y = Clamp<float>(_tc.y * m_height, 0.0f, (float)m_height - 1);
	}

	ASSERT((uint)_coord.x <= m_width);
	ASSERT((uint)_coord.y <= m_height);

	return _coord;
}
//----------------------------------------------------------------------------//
Vec4 Image::Sample(const Vec2& _tc, uint _flags, float _divisor)
{
	Vec4 _r = VEC4_ZERO;
	uint _w = m_width * m_channels;
	bool _repeat = (_flags & ISF_Clamp) == 0;

	if (_flags & ISF_Sparse)
	{
		if (_divisor == 0)
			_divisor = 1;
		Vec2 _sectorSize = Vec2((float)m_width, (float)m_height) / (float)_divisor;
		Vec2 _sectorInvSize = 1 / _sectorSize;
		Vec2 _coord = GetCoord(_tc, _repeat); // 0..1 --> 0..size
		Vec2i _sectorIdx = _coord * _sectorInvSize;
		Vec2 _sectorLtCoord = Vec2(_sectorIdx) * _sectorSize;
		Vec2 _sectorRbCoord = _sectorLtCoord + _sectorSize;
		Vec2 _coeff = (_coord - _sectorLtCoord) * _sectorInvSize;
		_sectorLtCoord *= m_invSize; // 0..size --> 0..1
		_sectorRbCoord *= m_invSize; // 0..size --> 0..1

		_flags &= ~ISF_Sparse;
		Vec4 _lt = Sample(_sectorLtCoord, _flags);
		Vec4 _lb = Sample({ _sectorLtCoord.x, _sectorRbCoord.y }, _flags);
		Vec4 _rb = Sample(_sectorRbCoord, _flags);
		Vec4 _rt = Sample({ _sectorRbCoord.x, _sectorLtCoord.y }, _flags);

		for (uint i = 0; i < m_channels; ++i)
		{
			_r[i] = Mix(Mix(_lt[i], _rt[i], _coeff.x), Mix(_lb[i], _rb[i], _coeff.x), _coeff.y);
		}
	}	
	else if (_flags & ISF_Linear)
	{
		Vec2 _coord = GetCoord(_tc, _repeat); // 0..1 --> 0..size
		Vec2i _lt = _coord;
		Vec2i _rb = GetCoord(_tc + m_invSize, _repeat);
		Vec2 _coeff = _coord - _lt;
		uint _ti = _lt.y * _w;
		uint _bi = _rb.y * _w;
		uint _lti = _lt.x + _ti;
		uint _rti = _rb.x + _ti	;
		uint _lbi = _lt.x + _bi;
		uint _rbi = _rb.x + _bi;
		for (uint c = 0; c < m_channels; ++c)
		{
			_r[c] = Mix(Mix(m_data[_lti + c], m_data[_rti + c], _coeff.x), Mix(m_data[_lbi + c], m_data[_rbi + c], _coeff.x), _coeff.y);
		}
	}
	else // nearest
	{
		Vec2i _coord = GetCoord(_tc, _repeat);
		for (uint c = 0; c < m_channels; ++c)
			_r[c] = m_data[_coord.x + _coord.y * _w + c];
	}
	return _r;
}
//----------------------------------------------------------------------------//
void Image::GetMinMax(Vec4& _mn, Vec4& _mx)
{
	_mn = +999999.9f;
	_mx = -999999.9f;
	
	const float* _pixel = m_data;
	const float* _end = m_data + m_width * m_height * m_channels;
	while (_pixel < _end)
	{
		for (uint i = 0; i < m_channels; ++i)
		{
			_mn[i] = Min(_pixel[i], _mn[i]);
			_mx[i] = Max(_pixel[i], _mx[i]);
		}
		_pixel += m_channels;
	}
	printf("min(%f) max(%f)\n", _mn.x, _mx.x);
}
//----------------------------------------------------------------------------//
void Image::CreateBitmapFont(FontInfo& _info, const char* _name, uint _fheight, float _fwidth, bool _italic)
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

	Realloc(_texWidth, _texHeight, 4);
	Vec4* _dstp = (Vec4*)m_data;
	Vec4* _dste = _dstp + _texWidth * _texHeight;
	while (_dstp < _dste)
	{
		_dstp->r = ByteToFloat(_bmp->a);
		_dstp->g = ByteToFloat(_bmp->r);
		_dstp->b = ByteToFloat(_bmp->g);
		_dstp->a = ByteToFloat(_bmp->b);
		++_dstp;
		++_bmp;
	}

	DeleteObject(_bm);
	DeleteDC(_dc);
	DeleteObject(_fnt);
}
//----------------------------------------------------------------------------//
int Image::CreateNoize(uint _size, uint _channels, int _rseed)
{
	Realloc(_size, _size, _channels);
	float* _dstp = m_data;
	float* _dste = _dstp + _size * _size * _channels;
	while (_dstp < _dste)
		*_dstp++ = Rand(&_rseed);
	return _rseed;
}
//----------------------------------------------------------------------------//
void Image::CreatePerlin(uint _size, float _scale, const Vec2& _offset, int _rseed, uint _iterations)
{
	Realloc(_size, _size, 1);
	float* _dstp = m_data;
	for (uint y = 0; y < m_height; ++y)
	{
		for (uint x = 0; x < m_width; ++x)
		{
			*_dstp++ = Perlin2d(x * _scale + _offset.x, y * _scale + _offset.y, _rseed, _iterations);
		}
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//


