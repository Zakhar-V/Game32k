
//----------------------------------------------------------------------------//
// Font
//----------------------------------------------------------------------------//

class Font : public NonCopyable
{
public:
	Font(const char* _name, uint _size, float _width = 0.5f, bool _italic = false);
	~Font(void);
	void SetColor(const Color& _color) { m_color = _color; }
	Vec2 Draw(const Vec2& _pos, const char* _text, int _length = -1);
	void BeginDraw(void);
	static void EndDraw(void);

protected:
	struct Char { Vec2 size; Vec2 tc[2]; } m_chars[256];
	uint m_texture;
	float m_height;
	Color m_color;
};

//----------------------------------------------------------------------------//
// Font
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Font::Font(const char* _name, uint _size, float _width, bool _italic)
{
	int _tw = int(_width * 1000) / 100;
	HFONT _fnt = CreateFontA(-int(_size), 0, 0, 0, _tw * 100, _italic, 0, 0, RUSSIAN_CHARSET, OUT_OUTLINE_PRECIS, CLIP_TT_ALWAYS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _name);
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
	m_height = (float)_height;
	for (uint i = 0; i < 256; ++i)
	{
		CharRect& _ch = _chars[i];
		_ch.w = int(_abc[i].abcfB) + 2;
		_ch.h = _height;
	}

	uint _tex_width = 32, _tex_height = 32;
	for (;;)
	{
		bool _placed = true;
		for (uint x = 0, y = 0, i = 0; i < 256; ++i)
		{
			CharRect& _ch = _chars[i];
			if (x + _ch.w > _tex_width)
			{
				x = 0;
				y += _height;
				if (y + _height > _tex_height)
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
		if (_tex_width <= _tex_height)
			_tex_width <<= 1;
		else
			_tex_height <<= 1;
	}

	BITMAPINFO _bmi;
	memset(&_bmi, 0, sizeof(_bmi));
	_bmi.bmiHeader.biSize = sizeof(_bmi);
	_bmi.bmiHeader.biWidth = _tex_width;
	_bmi.bmiHeader.biHeight = -int(_tex_height);
	_bmi.bmiHeader.biBitCount = 32;
	_bmi.bmiHeader.biCompression = BI_RGB;
	_bmi.bmiHeader.biPlanes = 1;

	Color* _img;
	HBITMAP _bm = CreateDIBSection(_dc, &_bmi, DIB_RGB_COLORS, (void**)&_img, 0, 0);
	//memset(_img, 0, _tex_width * _tex_height * sizeof(Color));

	float _itw = 1.0f / float(_tex_width);
	float _ith = 1.0f / float(_tex_height);
	SelectObject(_dc, _bm);
	for (uint i = 0; i < 256; ++i)
	{
		uint8 c = (uint8)i;
		CharRect& _cr = _chars[i];
		Char& _ch = m_chars[i];
		TextOutA(_dc, _cr.x - (int(_abc[i].abcfA) - 1), _cr.y, (const char*)&c, 1);
		_ch.size.Set((float)_cr.w, (float)_cr.h);
		_ch.tc[0].Set(_cr.x * _itw, _cr.y * _ith);
		_ch.tc[1].Set((_cr.x + _cr.w) * _itw, (_cr.y + _cr.h) * _ith);
	}
	GdiFlush();

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _tex_width, _tex_height, 0, GL_ABGR_EXT, GL_UNSIGNED_BYTE, _img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	DeleteObject(_bm);
	DeleteDC(_dc);
	DeleteObject(_fnt);
}
//----------------------------------------------------------------------------//
Font::~Font(void)
{
	//do nothing
}
//----------------------------------------------------------------------------//
Vec2 Font::Draw(const Vec2& _pos, const char* _text, int _length)
{
	Vec2 _p = _pos;
	glBegin(GL_QUADS);
	for (int i = 0; _length < 0 ? *_text : i < _length; ++i)
	{
		if (*_text == '\n' || *_text == '\r')
		{
			if (*_text == '\n' && _text[1] == '\r')	++_text;
			++_text;
			_p.x = _pos.x;
			_p.y += m_height;
			continue;
		}
		const Font::Char& _ch = m_chars[uint8(*_text++)];
		glTexCoord2f(_ch.tc[0].x, _ch.tc[0].y); glVertex2f(_p.x, _p.y);	// lt
		glTexCoord2f(_ch.tc[0].x, _ch.tc[1].y); glVertex2f(_p.x, _p.y + _ch.size.y); // lb
		glTexCoord2f(_ch.tc[1].x, _ch.tc[1].y); glVertex2f(_p.x + _ch.size.x, _p.y + _ch.size.y); // rb
		glTexCoord2f(_ch.tc[1].x, _ch.tc[0].y); glVertex2f(_p.x + _ch.size.x, _p.y); // rt;
		_p.x += _ch.size.x;
	}
	glEnd();
	return _p;
}
//----------------------------------------------------------------------------//
void Font::BeginDraw(void)
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glColor4ubv(m_color.v);
}
//----------------------------------------------------------------------------//
void Font::EndDraw(void)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}
//----------------------------------------------------------------------------//

