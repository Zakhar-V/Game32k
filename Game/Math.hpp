#pragma once

#include "Base.hpp"
#include <math.h>

//TODO: прибраться

//----------------------------------------------------------------------------//
// Constants
//----------------------------------------------------------------------------//

static const float EPSILON = 1e-6f;
static const float EPSILON2 = 1e-12f;
static const float PI = 3.1415926535897932384626433832795f;
static const float DEGREES = 57.295779513082320876798154814105f;
static const float RADIANS = 0.01745329251994329576923690768489f;
static const float BYTE2FLOAT = 1.0f / 255.0f;
static const float FLOAT2BYTE = 255.0f;

//----------------------------------------------------------------------------//
// Funcs
//----------------------------------------------------------------------------//

template <typename T> const T& Min(const T& _a, const T& _b) { return _a < _b ? _a : _b; }
template <typename T> const T& Min(const T& _a, const T& _b, const T& _c) { return _a < _b ? (_a < _c ? _a : _c) : (_b < _c ? _b : _c); }
template <typename T> const T& Max(const T& _a, const T& _b) { return _a > _b ? _a : _b; }
template <typename T> const T& Max(const T& _a, const T& _b, const T& _c) { return _a > _b ? (_a > _c ? _a : _c) : (_b > _c ? _b : _c); }
template <typename T> const T Clamp(T _x, T _l, T _u) { return _x > _l ? (_x < _u ? _x : _u) : _l; }
template <typename T> T Mix(const T& _a, const T& _b, float _t) { return _a + (_b - _a) * _t; }

#define USE_CRT_MATH 0

template <typename T> T Abs(T _x) { return abs(_x); }
inline float Abs(float _x) { return fabsf(_x); }
float Radians(float _val) { return _val * RADIANS; }
float Degrees(float _val) { return _val * DEGREES; }
template <typename T> T Sqr(T _x) { return _x * _x; }

// asm math partially ported from http://www.jbox.dk/sanos/source/lib/math/ 

inline float Sqrt(float _x) 
{
#if USE_CRT_MATH
	return sqrtf(_x);
#else
	float _r;
	__asm fld _x
	__asm fsqrt
	__asm fstp _r
	return _r;
#endif
}
inline float RSqrt(float _x)
{ 
#if USE_CRT_MATH
	return 1 / Sqrt(_x);
#else
	float _r;
	__asm rsqrtss xmm0, _x
	__asm movss _r, xmm0
	return _r;
#endif
}
inline float Sin(float _x) 
{
#if USE_CRT_MATH
	return sinf(_x);
#else
	float _r;
	__asm fld _x
	__asm fsin
	__asm fstp _r
	return _r;
#endif
}
inline float Cos(float _x)
{ 
#if USE_CRT_MATH
	return cosf(_x);
#else
	float _r;
	__asm fld _x
	__asm fcos
	__asm fstp _r
	return _r;
#endif
}
inline void SinCos(float _a, float& _s, float& _c) 
{ 
#if USE_CRT_MATH
	_s = sinf(_a), _c = cosf(_a);
#else
	float s, c;
	__asm fld _a
	__asm fsincos
	__asm fstp c
	__asm fstp s
	_s = s;
	_c = c;
#endif
}
inline float Tan(float _x)
{ 
#if USE_CRT_MATH
	return tanf(_x);
#else
	float _r;
	__asm fld _x
	__asm fptan
	__asm fstp _r
	return _r;
#endif
}
inline float ATan2(float _y, float _x) 
{ 
#if USE_CRT_MATH
	return atan2f(_y, _x);
#else
	float _r;
	__asm fld _y
	__asm fld _x
	__asm fpatan
	__asm fstp _r
	return _r;
#endif
}
inline float ASin(float _x)
{ 
#if USE_CRT_MATH
	return asinf(_x);
#else
	//return ATan2(_x, Sqrt(1 - _x * _x));
	float _r;
	__asm fld _x
	__asm fld _x
	__asm fmul st(0), st(0)
	__asm fld1
	__asm fsubr
	__asm fsqrt
	__asm fpatan
	__asm fstp _r
	return _r;
#endif
}
inline float ACos(float _x) 
{ 
#if USE_CRT_MATH
	return acosf(_x);
#else
	//return ATan2(Sqrt(1 - _x * _x), _x);
	float _r;
	__asm fld _x
	__asm fmul st(0), st(0)
	__asm fld1
	__asm fsubr
	__asm fsqrt
	__asm fld _x
	__asm fpatan
	__asm fstp _r
	return _r;
#endif
}
inline float Log2(float _x)
{ 
#if USE_CRT_MATH
	return log2(_x);
#else
	float _r;
	__asm fldln2
	__asm fld _x
	__asm fyl2x
	__asm fstp _r
	return _r;
#endif
}

inline int Log2i(int _x)
{
	return (int)Log2((float)_x);
}

float Floor(float _x)
{
#if USE_CRT_MATH
	return floorf(_x);
#else
	float _r;
	uint16 _ocw, _cw = 0x0763;
	__asm fstcw _ocw
	__asm fldcw _cw
	__asm fld _x
	__asm frndint
	__asm fstp _r
	__asm fldcw _ocw
	return _r;
#endif
}

float Ceil(float _x)
{  
#if USE_CRT_MATH
	return ceilf(_x);
#else
	float _r;
	uint16 _ocw, _cw = 0x0b63;
	__asm fstcw _ocw
	__asm fldcw _cw
	__asm fld _x
	__asm frndint
	__asm fstp _r
	__asm fldcw _ocw
	return _r;
#endif
}

float Wrap(float _x, float _l, float _u)
{
	float _y = _u - _l;
	_x -= _u * floorf(_x / _u);
	if (_x < _l)
		_x = _l - _x;
	return _x;
}


inline uint32_t FirstPow2(uint32_t _val)
{
	--_val |= _val >> 16;
	_val |= _val >> 8;
	_val |= _val >> 4;
	_val |= _val >> 2;
	_val |= _val >> 1;
	return ++_val;
}
inline bool IsPow2(uint32_t _val) { return (_val & (_val - 1)) == 0; }
inline uint8 FloatToByte(float _x) { return (uint8)(_x * FLOAT2BYTE); }
inline float ByteToFloat(uint8 _x) { return _x * BYTE2FLOAT; }
inline uint16_t FloatToHalf(float _value)
{
	union { float f; uint32 i; }_fb = { _value };
	return (uint16)((_fb.i >> 16) & 0x8000) | ((((_fb.i & 0x7f800000) - 0x38000000) >> 13) & 0x7c00) | ((_fb.i >> 13) & 0x03ff);
}
inline float HalfToFloat(uint16 _value)
{
	union { uint32 i; float f; }_fb;
	_fb.i = ((_value & 0x8000) << 16) | (((_value & 0x7c00) + 0x1C000) << 13) | ((_value & 0x03FF) << 13);
	return _fb.f;
}

inline float Cerp(float _a, float _b, float _t)
{
	return Mix(_a, _b, (1 - Cos(_t*PI))*0.5f);
}

//----------------------------------------------------------------------------//
// Vec2
//----------------------------------------------------------------------------//

struct Vec2
{
	Vec2(void) { }
	Vec2(float _s) : x(_s), y(_s) { }
	Vec2(float _x, float _y) : x(_x), y(_y) { }

	Vec2 operator - (void) const { return Vec2(-x, -y); }
	Vec2 operator + (const Vec2& _rhs) const { return Vec2(x + _rhs.x, y + _rhs.y); }
	Vec2 operator - (const Vec2& _rhs) const { return Vec2(x - _rhs.x, y - _rhs.y); }
	Vec2 operator * (const Vec2& _rhs) const { return Vec2(x * _rhs.x, y * _rhs.y); }
	Vec2 operator / (const Vec2& _rhs) const { return Vec2(x / _rhs.x, y / _rhs.y); }
	Vec2 operator * (float _rhs) const { return Vec2(x * _rhs, y * _rhs); }
	Vec2 operator / (float _rhs) const { return Vec2(x / _rhs, y / _rhs); }
	Vec2& operator += (const Vec2& _rhs) { x += _rhs.x, y += _rhs.y; return *this; }
	Vec2& operator -= (const Vec2& _rhs) { x -= _rhs.x, y -= _rhs.y; return *this; }
	Vec2& operator *= (const Vec2& _rhs) { x *= _rhs.x, y *= _rhs.y; return *this; }
	Vec2& operator /= (const Vec2& _rhs) { x /= _rhs.x, y /= _rhs.y; return *this; }
	Vec2& operator *= (float _rhs) { x *= _rhs, y *= _rhs; return *this; }
	Vec2& operator /= (float _rhs) { x /= _rhs, y /= _rhs; return *this; }
	friend Vec2 operator / (float _lhs, const Vec2& _rhs) { return Vec2(_lhs / _rhs.x, _lhs / _rhs.y); }
	friend Vec2 operator * (float _lhs, const Vec2& _rhs) { return Vec2(_lhs * _rhs.x, _lhs * _rhs.y); }

	bool operator == (const Vec2& _rhs) const { return x == _rhs.x && y == _rhs.y; }
	bool operator != (const Vec2& _rhs) const { return x != _rhs.x || y != _rhs.y; }

	Vec2& Set(const Vec2& _v) { return *this = _v; }
	Vec2& Set(float _s) { x = _s, y = _s; return *this; }
	Vec2& Set(float _x, float _y) { x = _x, y = _y; return *this; }

	float x, y;
};

//----------------------------------------------------------------------------//
// Vec2i
//----------------------------------------------------------------------------//

struct Vec2i
{
	Vec2i(void) { }
	Vec2i(int _s) : x(_s), y(_s) { }
	Vec2i(int _x, int _y) : x(_x), y(_y) { }
	Vec2i(const Vec2& _v) : x((int)_v.x), y((int)_v.y) { }
	operator Vec2 (void) const { return Vec2((float)x, (float)y); }

	Vec2i operator - (void) const { return Vec2i(-x, -y); }
	Vec2i operator + (const Vec2i& _rhs) const { return Vec2i(x + _rhs.x, y + _rhs.y); }
	Vec2i operator - (const Vec2i& _rhs) const { return Vec2i(x - _rhs.x, y - _rhs.y); }
	Vec2i operator * (const Vec2i& _rhs) const { return Vec2i(x * _rhs.x, y * _rhs.y); }
	Vec2i operator / (const Vec2i& _rhs) const { return Vec2i(x / _rhs.x, y / _rhs.y); }
	Vec2i operator * (int _rhs) const { return Vec2i(x * _rhs, y * _rhs); }
	Vec2i operator / (int _rhs) const { return Vec2i(x / _rhs, y / _rhs); }
	Vec2i& operator += (const Vec2i& _rhs) { x += _rhs.x, y += _rhs.y; return *this; }
	Vec2i& operator -= (const Vec2i& _rhs) { x -= _rhs.x, y -= _rhs.y; return *this; }
	Vec2i& operator *= (const Vec2i& _rhs) { x *= _rhs.x, y *= _rhs.y; return *this; }
	Vec2i& operator /= (const Vec2i& _rhs) { x /= _rhs.x, y /= _rhs.y; return *this; }
	Vec2i& operator *= (int _rhs) { x *= _rhs, y *= _rhs; return *this; }
	Vec2i& operator /= (int _rhs) { x /= _rhs, y /= _rhs; return *this; }
	friend Vec2i operator / (int _lhs, const Vec2i& _rhs) { return Vec2i(_lhs / _rhs.x, _lhs / _rhs.y); }
	friend Vec2i operator * (int _lhs, const Vec2i& _rhs) { return Vec2i(_lhs * _rhs.x, _lhs * _rhs.y); }

	bool operator == (const Vec2i& _rhs) const { return x == _rhs.x && y == _rhs.y; }
	bool operator != (const Vec2i& _rhs) const { return x != _rhs.x || y != _rhs.y; }

	Vec2i& Set(const Vec2i& _v) { return *this = _v; }
	Vec2i& Set(int _s) { x = _s, y = _s; return *this; }
	Vec2i& Set(int _x, int _y) { x = _x, y = _y; return *this; }

	int x, y;
};

//----------------------------------------------------------------------------//
// Vec3
//----------------------------------------------------------------------------//

struct Vec3
{
	Vec3(void) { }
	Vec3(float _s) : x(_s), y(_s), z(_s) { }
	Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }
	Vec3 Copy(void) const { return *this; }

	const float operator [] (uint _index) const { return (&x)[_index]; }
	float& operator [] (uint _index) { return (&x)[_index]; }
	const float* operator * (void) const { return &x; }
	float* operator * (void) { return &x; }

	Vec3 operator - (void) const { return Vec3(-x, -y, -z); }
	Vec3 operator + (const Vec3& _rhs) const { return Vec3(x + _rhs.x, y + _rhs.y, z + _rhs.z); }
	Vec3 operator - (const Vec3& _rhs) const { return Vec3(x - _rhs.x, y - _rhs.y, z - _rhs.z); }
	Vec3 operator * (const Vec3& _rhs) const { return Vec3(x * _rhs.x, y * _rhs.y, z * _rhs.z); }
	Vec3 operator / (const Vec3& _rhs) const { return Vec3(x / _rhs.x, y / _rhs.y, z / _rhs.z); }
	Vec3 operator * (float _rhs) const { return Vec3(x * _rhs, y * _rhs, z * _rhs); }
	Vec3 operator / (float _rhs) const { return Vec3(x / _rhs, y / _rhs, z / _rhs); }
	Vec3& operator += (const Vec3& _rhs) { x += _rhs.x, y += _rhs.y, z += _rhs.z; return *this; }
	Vec3& operator -= (const Vec3& _rhs) { x -= _rhs.x, y -= _rhs.y, z -= _rhs.z; return *this; }
	Vec3& operator *= (const Vec3& _rhs) { x *= _rhs.x, y *= _rhs.y, z *= _rhs.z; return *this; }
	Vec3& operator /= (const Vec3& _rhs) { x /= _rhs.x, y /= _rhs.y, z /= _rhs.z; return *this; }
	Vec3& operator *= (float _rhs) { x *= _rhs, y *= _rhs, z *= _rhs; return *this; }
	Vec3& operator /= (float _rhs) { x /= _rhs, y /= _rhs, z /= _rhs; return *this; }
	friend Vec3 operator / (float _lhs, const Vec3& _rhs) { return Vec3(_lhs / _rhs.x, _lhs / _rhs.y, _lhs / _rhs.z); }
	friend Vec3 operator * (float _lhs, const Vec3& _rhs) { return Vec3(_lhs * _rhs.x, _lhs * _rhs.y, _lhs * _rhs.z); }

	bool operator == (const Vec3& _rhs) const { return x == _rhs.x && y == _rhs.y && z == _rhs.z; }
	bool operator != (const Vec3& _rhs) const { return x != _rhs.x || y != _rhs.y || z != _rhs.z; }

	Vec3& Set(float _s) { x = _s, y = _s, z = _s; return *this; }
	Vec3& Set(float _x, float _y, float _z) { x = _x, y = _y, z = _z; return *this; }
	Vec3& Set(const Vec3& _other) { return *this = _other; }


	Vec3& SetMin(const Vec3& _a, const Vec3& _b) { x = Min(_a.x, _b.x), y = Min(_a.y, _b.y), z = Min(_a.z, _b.z); return *this; }
	Vec3& SetMin(const Vec3& _other) { return SetMin(*this, _other); }
	Vec3& SetMax(const Vec3& _a, const Vec3& _b) { x = Max(_a.x, _b.x), y = Max(_a.y, _b.y), z = Max(_a.z, _b.z); return *this; }
	Vec3& SetMax(const Vec3& _other) { return SetMax(*this, _other); }
	Vec3& Normalize(void)
	{
		float _l = LengthSq();
		if (_l > EPSILON2)
			*this *= RSqrt(_l);
		return *this;
	}
	Vec3& NormalizeFast(void) { return *this *= LengthInv(); }
	float LengthSq(void) const { return x * x + y * y + z * z; }
	float Length(void) const { return Sqrt(x * x + y * y + z * z); }
	float LengthInv(void) const { return Sqrt(x * x + y * y + z * z); }
	float Dot(const Vec3& _rhs) const { return x * _rhs.x + y * _rhs.y + z * _rhs.z; }
	float AbsDot(const Vec3& _rhs) const { return Abs(x * _rhs.x) + Abs(y * _rhs.y) + Abs(z * _rhs.z); }
	Vec3 Cross(const Vec3& _rhs) const { return Vec3(y * _rhs.z - z * _rhs.y, z * _rhs.x - x * _rhs.z, x * _rhs.y - y * _rhs.x); }
	float Distance(const Vec3& _v) const { return (*this - _v).Length(); }
	float DistanceSq(const Vec3& _v) const { return (*this - _v).LengthSq(); }
	Vec3 MidPoint(const Vec3& _v) const { return (*this + _v) * 0.5f; }
	Vec3 Reflect(const Vec3& _n) const { return Vec3(*this - (2 * Dot(_n) * _n)); }
	Vec3 Perpendicular(void) const
	{
		Vec3 _perp = Cross({1, 0, 0});
		if (_perp.LengthSq() <= EPSILON2)
			_perp = Cross({0, 1, 0});
		return _perp.Normalize();
	}
	float Angle(const Vec3& _v) const
	{
		float _lp = LengthSq() * _v.LengthSq();
		if (_lp > EPSILON2)
			_lp = RSqrt(_lp);
		return ACos(Clamp<float>(Dot(_v) * _lp, -1, 1));
	}

	float x, y, z;
};

const Vec3 VEC3_ZERO(0);
const Vec3 VEC3_ONE(1);

//----------------------------------------------------------------------------//
// Vec4
//----------------------------------------------------------------------------//

struct Vec4
{
	Vec4(void) { }
	Vec4(float _s) : x(_s), y(_s), z(_s), w(_s) { }
	Vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) { }

	//vec4& set(float _x, float _y, float _z, float _w) { x = _x, y = _y, z = _z, w = _w; return *this; }

	union
	{
		struct { float r, g, b, a; };
		struct { float x, y, z, w; };
		struct { float bw[2]; uint bi[2]; }; // for skinning
	};
};

//----------------------------------------------------------------------------//
// Color
//----------------------------------------------------------------------------//

struct Color
{
	Color(void) { }
	//Color(uint _rgba) : r((_rgba >> 24) & 0xff), g((_rgba >> 16) & 0xff), b((_rgba >> 8) & 0xff), a((_rgba >> 0) & 0xff) { }
	Color(uint _rgba) : rgba(_rgba) { }
	Color(uint8 _r, uint8 _g, uint8 _b, uint8 _a = 0xff) : r(_r), g(_g), b(_b), a(_a) { }
	Color(const Vec4& _v) : r(FloatToByte(_v.r)), g(FloatToByte(_v.g)), b(FloatToByte(_v.b)), a(FloatToByte(_v.a)) { }
	operator Vec4 (void) const { return Vec4(ByteToFloat(r), ByteToFloat(g), ByteToFloat(b), ByteToFloat(a)); }

	uint Argb(void) const { return (rgba >> 8) | (a << 24); }
	uint Abgr(void) const { return (a << 24) | (b << 16) | (g << 8) | r; }

	union
	{
		//uint rgba;
		struct { uint8 r, g, b, a; };
		struct { uint8 x, y, z, w; };
		uint8 v[4];
		uint32 rgba;
	};
};

inline Color Mix(const Color& _a, const Color& _b, float _t)
{
	return Color(FloatToByte(Mix(_a.r * 255.f, _b.r * 255.f, _t)),
		FloatToByte(Mix(_a.g * 255.f, _b.g * 255.f, _t)),
		FloatToByte(Mix(_a.b * 255.f, _b.b * 255.f, _t)),
		FloatToByte(Mix(_a.a * 255.f, _b.a * 255.f, _t)));
}


//----------------------------------------------------------------------------//
// Quat
//----------------------------------------------------------------------------//

struct Quat
{
	Quat(void) 
	{
	}

	Quat(float _x, float _z, float _y, float _w) :
		x(_x), y(_y), z(_z), w(_w)
	{
	}
	Quat(float _w) :
		x(0), y(0), z(0), w(_w)
	{
	}
	Quat Copy(void) const 
	{ 
		return *this;
	}

	Quat& Set(const Quat& _other) 
	{
		return *this = _other; 
	}
	Quat& Set(float _x, float _y, float _z, float _w)
	{ 
		x = _x, y = _y, z = _z, w = _w; 
		return *this;
	}
	Quat& Set(float _w) 
	{ 
		x = 0, y = 0, z = 0, w = _w;
		return *this; 
	}
	
	const float operator [] (uint _index) const { return (&x)[_index]; }
	float& operator [] (uint _index) { return (&x)[_index]; }
	const float* operator * (void) const { return &x; }
	float* operator * (void) { return &x; }

	Quat operator - (void) const { return Quat(-x, -y, -z, -w); }

	Quat operator + (const Quat& _rhs) const { return Quat(x + _rhs.x, y + _rhs.y, z + _rhs.z, w + _rhs.w); }
	Quat operator - (const Quat& _rhs) const { return Quat(x - _rhs.x, y - _rhs.y, z - _rhs.z, w - _rhs.w); }
	Quat operator * (const Quat& _rhs) const { return Copy().Mul(_rhs); }
	Quat operator * (float _rhs) const { return Quat(x * _rhs, y * _rhs, z * _rhs, w * _rhs); }
	Quat operator / (float _rhs) const { return Quat(x / _rhs, y / _rhs, z / _rhs, w / _rhs); }
	Quat& operator += (const Quat& _rhs) { x += _rhs.x, y += _rhs.y, z += _rhs.z, w += _rhs.w; return *this; }
	Quat& operator -= (const Quat& _rhs) { x -= _rhs.x, y -= _rhs.y, z -= _rhs.z, w -= _rhs.w; return *this; }
	Quat& operator *= (const Quat& _rhs) { return Mul(_rhs); }
	Quat& operator *= (float _rhs) { x += _rhs, y += _rhs, z += _rhs, w += _rhs; return *this; }
	Quat& operator /= (float _rhs) { x += _rhs, y += _rhs, z += _rhs, w += _rhs; return *this; }
	friend Quat operator * (float _lhs, const Quat& _rhs) { return Quat(_lhs * _rhs.x, _lhs * _rhs.y, _lhs * _rhs.z, _lhs * _rhs.w); }
	friend Vec3 operator * (const Vec3& _lhs, const Quat& _rhs)
	{
		const Vec3& _q = *(const Vec3*)(&_rhs.x);
		Vec3 _uv(_q.Cross(_lhs));
		return _lhs + _uv * 2 * _rhs.w + _q.Cross(_uv) * 2;
	}

	Quat& Mul(const Quat& _rhs)
	{
		return Set(w * _rhs.x + x * _rhs.w + y * _rhs.z - z * _rhs.y,
			w * _rhs.y + y * _rhs.w + z * _rhs.x - x * _rhs.z,
			w * _rhs.z + z * _rhs.w + x * _rhs.y - y * _rhs.x,
			w * _rhs.w - x * _rhs.x - y * _rhs.y - z * _rhs.z);
	}
	float Dot(const Quat& _q) const 
	{ 
		return x * _q.x + y * _q.y + z * _q.z + w * _q.w; 
	}
	Quat& Normalize(void)
	{
		float _l = x * x + y * y + z * z + w * w;
		if (_l > EPSILON2)
			*this *= RSqrt(_l);
		return *this;
	}
	Quat& Inverse(void)
	{
		float _d = Dot(*this);
		if (_d > 0)
			x *= -_d, y *= -_d, z *= -_d, w *= _d;
		else
			x = 0, y = 0, z = 0, w = 1;
		return *this;
	}
	Quat& UnitInverse(void)
	{ 
		x = -x, y = -y, z = -z;
		return *this;
	}

	Quat Nlerp(const Quat& _q, float _t, bool _shortestPath = true) const
	{
		const Quat& _p = *this;
		float _c = _p.Dot(_q);
		Quat _result;
		if (_c < 0 && _shortestPath)
			_result = _p + _t * ((-_q) - _p);
		else
			_result = _p + _t * (_q - _p);
		return _result.Normalize();
	}
	Quat Slerp(const Quat& _q, float _t, bool _shortestPath = true) const
	{
		const Quat& _p = *this;
		float _c = _p.Dot(_q);
		Quat _tmp;
		if (_c < 0 && _shortestPath)
			_c = -_c, _tmp = -_q;
		else
			_tmp = _q;
		if (Abs(_c) < 1 - EPSILON)
		{
			float _s = Sqrt(1 - _c * _c);
			float _angle = ATan2(_s, _c);
			float _invs = 1 / _s;
			float _coeff0 = Sin((1 - _t) * _angle) * _invs;
			float _coeff1 = Sin(_t * _angle) * _invs;
			return _coeff0 * _p + _coeff1 * _tmp;
		}
		return Quat((1 - _t) * _p + _t * _tmp).Normalize();
	}


	void ToRotationMatrix(float* _r0, float* _r1, float* _r2) const
	{
		float _x2 = x + x;
		float _y2 = y + y;
		float _z2 = z + z;
		float _wx = _x2 * w;
		float _wy = _y2 * w;
		float _wz = _z2 * w;
		float _xx = _x2 * x;
		float _xy = _y2 * x;
		float _xz = _z2 * x;
		float _yy = _y2 * y;
		float _yz = _z2 * y;
		float _zz = _z2 * z;
		_r0[0] = 1 - (_yy + _zz);
		_r0[1] = _xy + _wz;
		_r0[2] = _xz - _wy;
		_r1[0] = _xy - _wz;
		_r1[1] = 1 - (_xx + _zz);
		_r1[2] = _yz + _wx;
		_r2[0] = _xz + _wy;
		_r2[1] = _yz - _wx;
		_r2[2] = 1 - (_xx + _yy);
	}
	Quat& FromRotationMatrix(const float* _r0, const float* _r1, const float* _r2)
	{
		int _r;
		float _root = _r0[0] + _r1[1] + _r2[2];
		if (_root > 0)
		{
			_r = 3;
			_root = Sqrt(_root + 1);
			x = _r1[2] - _r2[1];
			y = _r2[0] - _r0[2];
			z = _r0[1] - _r1[0];
		}
		else if (_r0[0] > _r1[1] && _r0[0] > _r2[2])
		{
			_r = 0;
			_root = Sqrt(_r0[0] - _r1[1] - _r2[2] + 1);
			y = _r0[1] + _r1[0];
			z = _r0[2] + _r2[0];
			w = _r1[2] - _r2[1];
		}
		else if (_r1[1] > _r0[0] && _r1[1] > _r2[2])
		{
			_r = 1;
			_root = Sqrt(_r1[1] - _r2[2] - _r0[0] + 1);
			x = _r1[0] + _r0[1];
			z = _r1[2] + _r2[1];
			w = _r2[0] - _r0[2];
		}
		else
		{
			_r = 2;
			_root = Sqrt(_r2[2] - _r0[0] - _r1[1] + 1);
			x = _r2[0] + _r0[2];
			y = _r2[1] + _r1[2];
			w = _r0[1] - _r1[0];
		}

		float _invr = 0.5f / _root;
		for (int i = 0; i < 4; ++i)
			(&x)[i] *= _invr;
		(&x)[_r] = _root * 0.5f;

		return *this;
	}
	Quat& FromAxisAngle(const Vec3& _axis, float _angle)
	{
		float _s, _c;
		SinCos(_angle * 0.5f, _s, _c);
		return Set(_axis.x * _s, _axis.y * _s, _axis.z * _s, _c);
	}

	float x, y, z, w;
};

inline Quat Mix(const Quat& _a, const Quat& _b, float _t)
{
	return _a.Slerp(_b, _t);
}

const Quat QUAT_ZERO(0);
const Quat QUAT_IDENTITY(1);

//----------------------------------------------------------------------------//
// Mat44
//----------------------------------------------------------------------------//

/// Row-major matrix 4x4
struct Mat44
{
	Mat44(void) { }
	explicit Mat44(float _val) { SetIdentity(_val); }
	explicit Mat44(const float* _m44) { FromPtr(_m44); }
	Mat44(float _00, float _01, float _02, float _03,
		float _10, float _11, float _12, float _13,
		float _20, float _21, float _22, float _23,
		float _30, float _31, float _32, float _33) :
		m00(_00), m01(_01), m02(_02), m03(_03),
		m10(_10), m11(_11), m12(_12), m13(_13),
		m20(_20), m21(_21), m22(_22), m23(_23),
		m30(_30), m31(_31), m32(_32), m33(_33)
	{
	}

	Mat44 Copy(void) const { return *this; }
	Mat44& FromPtr(const float* _m44)
	{
		memcpy(v, _m44, sizeof(float) * 16);
		return *this;
	}
	Mat44& Set(const Mat44& _other) { return *this = _other; }
	Mat44& Set(float _00, float _01, float _02, float _03,
		float _10, float _11, float _12, float _13,
		float _20, float _21, float _22, float _23,
		float _30, float _31, float _32, float _33)
	{
		m00 = _00, m01 = _01, m02 = _02, m03 = _03;
		m10 = _10, m11 = _11, m12 = _12, m13 = _13;
		m20 = _20, m21 = _21, m22 = _22, m23 = _23;
		m30 = _30, m31 = _31, m32 = _32, m33 = _33;
		return *this;
	}
	Mat44& SetDiagonal(float _val)
	{
		m00 = _val, m11 = _val, m22 = _val, m33 = _val;
		return *this;
	}
	Mat44& SetZero(void)
	{
		for (uint i = 0; i < 16; ++i)
			v[i] = 0;
		return *this;
	}
	Mat44& SetIdentity(float _val = 1) { return SetZero().SetDiagonal(_val); }

	Vec3& Row(uint _r) { return *(Vec3*)(m[_r]); }
	const Vec3& Row(uint _r) const { return *(Vec3*)(m[_r]); }

	float* operator [] (uint _r) { return m[_r]; }
	const float* operator [] (uint _r) const { return m[_r]; }
	const float* operator * (void) const { return v; }
	float* operator * (void) { return v; }

	Vec3 Translate(const Vec3& _v) const
	{
		return Vec3(_v.x + m03, _v.y + m13, _v.z + m23); 
	}
	Vec3 Transform(const Vec3& _v) const 
	{
		return Vec3(
			m00 * _v.x + m01 * _v.y + m02 * _v.z + m03, 
			m10 * _v.x + m11 * _v.y + m12 * _v.z + m13,
			m20 * _v.x + m21 * _v.y + m22 * _v.z + m23);
	}
	Vec3 TransformVectorAbs(const Vec3& _v) const
	{ 
		return Vec3(
			Abs(m00) * _v.x + Abs(m01) * _v.y + Abs(m02) * _v.z,
			Abs(m10) * _v.x + Abs(m11) * _v.y + Abs(m12) * _v.z, 
			Abs(m20) * _v.x + Abs(m21) * _v.y + Abs(m22) * _v.z);
	}
	Vec3 TransformVector(const Vec3& _v) const 
	{ 
		return Vec3(
			m00 * _v.x + m01 * _v.y + m02 * _v.z,
			m10 * _v.x + m11 * _v.y + m12 * _v.z,
			m20 * _v.x + m21 * _v.y + m22 * _v.z);
	}
	Vec3 TransformProj(const Vec3& _v) const
	{
		float _iw = 1 / (m30 * _v.x + m31 * _v.y + m32 * _v.z + m33);
		return Vec3(
			(m00 * _v.x + m01 * _v.y + m02 * _v.z + m03) * _iw,
			(m10 * _v.x + m11 * _v.y + m12 * _v.z + m13) * _iw,
			(m20 * _v.x + m21 * _v.y + m22 * _v.z + m23) * _iw);
	}
	friend Vec3 operator * (const Vec3&_lhs, const Mat44& _rhs)
	{
		return _rhs.TransformProj(_lhs);
	}

	Mat44& Add(const Mat44& _rhs)
	{
		for (uint i = 0; i < 16; ++i)
			v[i] += _rhs.v[i];
		return *this;
	}
	Mat44 operator + (const Mat44& _rhs) const
	{
		return Copy().Add(_rhs);
	}
	Mat44& operator += (const Mat44& _rhs)
	{
		return Add(_rhs);
	}

	Mat44& Mul(float _rhs)
	{
		for (uint i = 0; i < 16; ++i)
			v[i] *= _rhs;
		return *this;
	}
	Mat44 operator * (float _rhs) const 
	{ 
		return Copy().Mul(_rhs); 
	}
	Mat44& operator *= (float _rhs)
	{ 
		return Mul(_rhs); 
	}

	Mat44& Mul(const Mat44& _rhs)
	{
		Mat44 _r;
		for (uint r = 0; r < 4; ++r)
		{
			for (uint c = 0; c < 4; ++c)
			{
				_r[r][c] = m[r][0] * _rhs[0][c] + m[r][1] * _rhs[1][c] + m[r][2] * _rhs[2][c] + m[r][3] * _rhs[3][c];
			}
		}
		return *this = _r;
	}
	Mat44 operator * (const Mat44& _rhs) const
	{
		return Copy().Mul(_rhs);
	}
	Mat44& operator *= (const Mat44& _rhs)
	{ 
		return Mul(_rhs);
	}

	bool IsAffine(void) const 
	{ 
		return m30 == 0 && m31 == 0 && m32 == 0 && m33 == 1;
	}

	Vec3 GetTranslation(void) const
	{
		return Vec3(m03, m13, m23); 
	}
	Mat44& SetTranslation(const Vec3& _t)
	{
		m03 = _t.x, m13 = _t.y, m23 = _t.z;
		return *this;
	}
	Mat44& CreateTranslation(const Vec3& _t)
	{
		return SetIdentity().SetTranslation(_t);
	}

	Vec3 GetScale(void) const
	{
		return Vec3(
			Sqrt(m00 * m00 + m10 * m10 + m20 * m20),
			Sqrt(m01 * m01 + m11 * m11 + m21 * m21),
			Sqrt(m02 * m02 + m12 * m12 + m22 * m22));
	}
	Mat44& SetScale(const Vec3& _s)
	{
		return SetRotationScale(GetRotation(), _s);
	}
	Mat44& CreateScale(const Vec3& _s)
	{
		SetZero();
		m[0][0] = _s.x;
		m[1][1] = _s.x;
		m[2][2] = _s.x;
		m[3][3] = 0;
		return *this;
	}

	Quat GetRotation(void) const
	{
		Vec3 _m0(m00, m10, m20);
		Vec3 _m1(m01, m11, m21);
		Vec3 _m2(m02, m12, m22);
		Vec3 _q0 = _m0.Copy().Normalize();
		Vec3 _q1 = (_m1 - _q0 * _q0.Dot(_m1)).Normalize();
		Vec3 _q2 = ((_m2 - _q0 * _q0.Dot(_m2)) - _q1 * _q1.Dot(_m2)).Normalize();
		float _det = _q0[0] * _q1[1] * _q2[2] + _q0[1] * _q1[2] * _q2[0] + _q0[2] * _q1[0] * _q2[1] - _q0[2] * _q1[1] * _q2[0] - _q0[1] * _q1[0] * _q2[2] - _q0[0] * _q1[2] * _q2[1];
		if (_det < 0)
			_q0 = -_q0, _q1 = -_q1, _q2 = -_q2;
		return Quat().FromRotationMatrix(*_q0, *_q1, *_q2);
	}
	Mat44& SetRotation(const Quat& _r)
	{
		return SetRotationScale(_r, GetScale());
	}
	Mat44& CreateRotation(const Quat& _r)
	{
		return SetIdentity().SetRotationScale(_r, 1);
	}

	Mat44& SetRotationScale(const Quat& _r, const Vec3& _s)
	{
		_r.ToRotationMatrix(m[0], m[1], m[2]);
		Row(0) *= _s, Row(1) *= _s, Row(2) *= _s;
		return *this;
	}
	Mat44& CreateRotationScale(const Quat& _r, const Vec3& _s)
	{
		return SetIdentity().SetRotationScale(_r, _s);
	}
	Mat44& CreateTransform(const Vec3& _t, const Quat& _r, const Vec3& _s = 1)
	{
		// Ordering: Scale, Rotate, Translate
		float _r0[3], _r1[3], _r2[3];
		_r.ToRotationMatrix(_r0, _r1, _r2);
		m00 = _s.x * _r0[0], m01 = _s.y * _r0[1], m02 = _s.z * _r0[2], m03 = _t.x;
		m10 = _s.x * _r1[0], m11 = _s.y * _r1[1], m12 = _s.z * _r1[2], m13 = _t.y;
		m20 = _s.x * _r2[0], m21 = _s.y * _r2[1], m22 = _s.z * _r2[2], m23 = _t.z;
		return *this;
	}
	Mat44& CreateInverseTransform(const Vec3& _t, const Quat& _r, const Vec3& _s = 1)
	{
		Vec3 _invs(1 / _s);
		Quat _invr = _r.Copy().Inverse();
		Vec3 _invt((-_t * _invr) * _invs);
		return CreateTransform(_invt, _invr, _invs);
	}
	
	Mat44& Inverse(void)
	{
		static const int dr0[4] = { 2, 2, 1, 1, };
		static const int dr1[4] = { 3, 3, 3, 2, };
		static const int dr2[4] = { 1, 0, 0, 0, };
		static const int dc0[6] = { 0, 0, 0, 1, 1, 2, };
		static const int dc1[6] = { 1, 2, 3, 2, 3, 3, };
		static const float ds[4] = { 1, -1, 1, -1, };
		static const int di[4][3] = { { 5, 4, 3, },{ 5, 2, 1, },{ 4, 2, 0, },{ 3, 1, 0, }, };

		Mat44 _m = *this;

		for (int r = 0; r < 4; ++r)
		{			
			int r0 = dr0[r]; // 2 2 1 1
			int r1 = dr1[r]; // 3 3 3 2
			int r2 = dr2[r]; // 1 0 0 0
			float s = ds[r];
			float s2 = -s;
			float t[6];

			/*
			t[0] = _m[r0][0] * _m[r1][1] - _m[r0][1] * _m[r1][0];
			t[1] = _m[r0][0] * _m[r1][2] - _m[r0][2] * _m[r1][0];
			t[2] = _m[r0][0] * _m[r1][3] - _m[r0][3] * _m[r1][0];
			t[3] = _m[r0][1] * _m[r1][2] - _m[r0][2] * _m[r1][1];
			t[4] = _m[r0][1] * _m[r1][3] - _m[r0][3] * _m[r1][1];
			t[5] = _m[r0][2] * _m[r1][3] - _m[r0][3] * _m[r1][2];
			*/
													  
			for (int i = 0; i < 6; ++i)
			{
				int c0 = dc0[i]; // 0 0 0 1 1 2
				int c1 = dc1[i]; // 1 2 3 2 3 3
				t[i] = _m[r0][c0] * _m[r1][c1] - _m[r0][c1] * _m[r1][c0];
			}

			/*
			m[0][r] = (t[5] * _m[r2][1] - t[4] * _m[r2][2] + t[3] * _m[r2][3]) * s;
			m[1][r] = (t[5] * _m[r2][0] - t[2] * _m[r2][2] + t[1] * _m[r2][3]) * s2;
			m[2][r] = (t[4] * _m[r2][0] - t[2] * _m[r2][1] + t[0] * _m[r2][3]) * s;
			m[3][r] = (t[3] * _m[r2][0] - t[1] * _m[r2][1] + t[0] * _m[r2][2]) * s2;
			*/

			for (int i = 0; i < 4; ++i)
			{
				int _i0 = di[i][0]; // 5 5 4 3
				int _i1 = di[i][1]; // 4 2 2 1
				int _i2 = di[i][2]; // 3 1 0 0
				int _r0 = dr0[i]; // 2 2 1 1
				int _r1 = dr1[i]; // 3 3 3 2
			    int _r2 = dr2[i]; // 1 0 0 0
				m[i][r] = (t[_i0] * _m[r2][_r2] - t[_i1] * _m[r2][_r0] + t[_i2] * _m[r2][_r1]) * s * ds[i];
			}
		}

		float _d = (m00 * _m[0][0] + m10 * _m[0][1] + m20 * _m[0][2] + m30 * _m[0][3]); // determinant

		return Mul(1 / _d);
	}



	/*Mat44& CreateProjection(float _left, float _right, float _bottom, float _top, float _znear, float _zfar, uint _flags)
	{
		return *this;
	}*/

	Mat44& CreateOrtho(float _left, float _right, float _bottom, float _top, float _znear, float _zfar)
	{
		SetIdentity();
		m00 = 2 / (_right - _left);
		m03 = (_right + _left) / (_left - _right);
		m11 = 2 / (_top - _bottom);
		m13 = (_top + _bottom) / (_bottom - _top);
		m22 = 1 / (_znear - _zfar);
		m23 = _znear / (_znear - _zfar);
		return *this;
	}

	Mat44& CreateOrtho2D(float _width, float _height)
	{
		SetIdentity();
		m00 = 2 / _width; // scale.x
		m03 = -1; // offset.x
		m11 = -2 / _height; // scale.y
		m13 = 1; // offset.y
		m22 = -0.5f;
		m23 = 0.5f;
		return *this;
	}

	union
	{
		float m[4][4]; // [row][column]
		float v[16];
		struct
		{
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
		};
	};
};

const Mat44 MAT44_ZERO(1);
const Mat44 MAT44_IDENTITY(1);

//----------------------------------------------------------------------------//
// Plane
//----------------------------------------------------------------------------//

struct Plane
{
	Vec3 pos;
	float d;
};

//----------------------------------------------------------------------------//
// Noise
//----------------------------------------------------------------------------//

inline uint IRand(int& _rseed)
{
	return (uint)(_rseed = 69069 * _rseed + 1);
}
inline float Rand(int& _rseed)
{
	return IRand(_rseed) * (1.0f / 0xffffffff);
}
inline float Noise2d(int _x, int _y, int _rseed)
{
	int _n = _x + _y * _rseed;
	_n = (_n << 13) ^ _n;
	return (1.0f - ((_n * (_n * _n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}
inline float SmoothedNoise2d(int _x, int _y, int _rseed)
{
	float _n = 0;
	_n += (Noise2d(_x - 1, _y - 1, _rseed) +
		Noise2d(_x + 1, _y - 1, _rseed) +
		Noise2d(_x - 1, _y + 1, _rseed) +
		Noise2d(_x + 1, _y + 1, _rseed)) * 0.0625f; // corners 1/16
	_n += (Noise2d(_x - 1, _y, _rseed) +
		Noise2d(_x + 1, _y, _rseed) +
		Noise2d(_x, _y - 1, _rseed) +
		Noise2d(_x, _y + 1, _rseed)) *0.125f; // sides 1/8
	_n += Noise2d(_x, _y, _rseed) * 0.25f; // center 1/4
	return _n;
}
inline float InterpolatedNoise2d(float _x, float _y, int _rseed)
{
	int _ix = (int)_x, _iy = (int)_y;
	float _fx = _x - _ix, _fy = _y - _iy;
	float _v0 = SmoothedNoise2d(_ix, _iy, _rseed);
	float _v1 = SmoothedNoise2d(_ix + 1, _iy, _rseed);
	float _v2 = SmoothedNoise2d(_ix, _iy + 1, _rseed);
	float _v3 = SmoothedNoise2d(_ix + 1, _iy + 1, _rseed);
	return Cerp(Cerp(_v0, _v1, _fx), Cerp(_v2, _v3, _fx), _fy);
}
inline float Perlin2d(float _x, float _y, int _rseed, uint _iterations = 4)
{
	float _n = 0, _f = 1, _a = 1;
	for (uint i = 0; i < _iterations; ++i)
	{
		_n += InterpolatedNoise2d(_x * _f, _y * _f, _rseed) * _a;
		_a *= 0.5f;
		_f *= 2;
	}
	return (1 - _n) * 0.5f;
}

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//



