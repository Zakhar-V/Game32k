#pragma once

#include "Base.hpp"
#include <math.h>

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
	__asm fstp st
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
	return log2f(_x);
#else
	float _r;
	__asm fld1
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

typedef uint16_t float16_t;
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
// Noise
//----------------------------------------------------------------------------//

inline uint IRand(int& _rseed) { return (uint)(_rseed = 69069 * _rseed + 1); }
inline float Rand(int& _rseed) { return IRand(_rseed) * (1.0f / 0xffffffff); }
float Noise2d(int _x, int _y, int _rseed);
float SmoothedNoise2d(int _x, int _y, int _rseed);
float InterpolatedNoise2d(float _x, float _y, int _rseed);
float Perlin2d(float _x, float _y, int _rseed, uint _iterations = 4);

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
	bool operator != (const Vec3& _rhs) const { return !(*this == _rhs); }

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

	bool operator < (const Vec3& _rhs) const { return x < _rhs.x && y < _rhs.y && z < _rhs.z; }
	bool operator > (const Vec3& _rhs) const { return x > _rhs.x && y > _rhs.y && z > _rhs.z; }
	bool operator <= (const Vec3& _rhs) const { return !(*this > _rhs); }
	bool operator >= (const Vec3& _rhs) const { return !(*this < _rhs); }

	float x, y, z;
};

const Vec3 VEC3_ZERO(0);
const Vec3 VEC3_ONE(1);
const Vec3 VEC3_UNIT_X(1, 0, 0);
const Vec3 VEC3_UNIT_Y(0, 1, 0);
const Vec3 VEC3_UNIT_Z(0, 0, 1);

//----------------------------------------------------------------------------//
// Vec4
//----------------------------------------------------------------------------//

struct Vec4
{
	Vec4(void) { }
	Vec4(float _s) : x(_s), y(_s), z(_s), w(_s) { }
	Vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) { }

	Vec4 Copy(void) const { return *this; }

	const float operator [] (uint _index) const { return (&x)[_index]; }
	float& operator [] (uint _index) { return (&x)[_index]; }
	const float* operator * (void) const { return &x; }
	float* operator * (void) { return &x; }

	Vec4& Set(float _x, float _y, float _z, float _w) { x = _x, y = _y, z = _z, w = _w; return *this; }
	Vec4& Set(float _s) { x = _s, y = _s, z = _s, w = _s; return *this; }
	Vec4& Set(const Vec3& _xyz, float _w) { return Set(_xyz.x, _xyz.y, _xyz.z, _w); }
	Vec4& Set(const Vec4& _other) { return *this = _other; }

	union
	{
		struct { float r, g, b, a; };
		struct { float x, y, z, w; };
		struct { float bw[2]; uint bi[2]; }; // for skinning
	};
};

const Vec4 VEC4_ZERO(0);
const Vec4 VEC4_ONE(1);
const Vec4 VEC4_IDENTITY(0, 0, 0, 1);

//----------------------------------------------------------------------------//
// Color
//----------------------------------------------------------------------------//

struct Color
{
	Color(void) { }
	Color(uint _rgba) : r((_rgba >> 24) & 0xff), g((_rgba >> 16) & 0xff), b((_rgba >> 8) & 0xff), a((_rgba >> 0) & 0xff) { }
	//Color(uint _rgba) : rgba(_rgba) { }
	Color(uint8 _r, uint8 _g, uint8 _b, uint8 _a = 0xff) : r(_r), g(_g), b(_b), a(_a) { }
	Color(const Vec4& _v) : r(FloatToByte(_v.r)), g(FloatToByte(_v.g)), b(FloatToByte(_v.b)), a(FloatToByte(_v.a)) { }
	operator Vec4 (void) const { return Vec4(ByteToFloat(r), ByteToFloat(g), ByteToFloat(b), ByteToFloat(a)); }

	uint Argb(void) const { return (rgba >> 8) | (a << 24); }
	uint Abgr(void) const { return (a << 24) | (b << 16) | (g << 8) | r; }

	Color& Blend(const Color& _c1, const Color& _c2, const Color& _c3, const Color& _c4)
	{
		r = (uint8)(uint(_c1.r + _c2.r + _c3.r + _c4.r) >> 2);
		g = (uint8)(uint(_c1.g + _c2.g + _c3.g + _c4.g) >> 2);
		b = (uint8)(uint(_c1.b + _c2.b + _c3.b + _c4.b) >> 2);
		a = (uint8)(uint(_c1.a + _c2.a + _c3.a + _c4.a) >> 2);
		return *this;
	}
	Color& Blend(const Color& _c1, const Color& _c2)
	{
		r = (uint8)(uint(_c1.r + _c2.r) >> 1);
		g = (uint8)(uint(_c1.g + _c2.g) >> 1);
		b = (uint8)(uint(_c1.b + _c2.b) >> 1);
		a = (uint8)(uint(_c1.a + _c2.a) >> 1);
		return *this;
	}

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
	Quat(void) { }

	Quat(float _x, float _z, float _y, float _w) : x(_x), y(_y), z(_z), w(_w) { }
	Quat(float _w) : x(0), y(0), z(0), w(_w) { }
	Quat Copy(void) const { return *this; }

	Quat& Set(const Quat& _other) { return *this = _other; }
	Quat& Set(float _x, float _y, float _z, float _w) { x = _x, y = _y, z = _z, w = _w; return *this; }
	Quat& Set(float _w) { x = 0, y = 0, z = 0, w = _w; return *this; }
	
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
	friend Vec3 operator * (const Vec3& _lhs, const Quat& _rhs) { return _rhs.Transform(_lhs); }
	
	Quat& Mul(const Quat& _rhs);
	Vec3 Transform(const Vec3& _v) const;
	float Dot(const Quat& _q) const { return x * _q.x + y * _q.y + z * _q.z + w * _q.w; }
	Quat& Normalize(void);
	Quat& Inverse(void);
	Quat& UnitInverse(void) { x = -x, y = -y, z = -z; return *this; }
	Quat Nlerp(const Quat& _q, float _t, bool _shortestPath = true) const;
	Quat Slerp(const Quat& _q, float _t, bool _shortestPath = true) const;
	void ToRotationMatrix(float* _r0, float* _r1, float* _r2) const;
	Quat& FromRotationMatrix(const float* _r0, const float* _r1, const float* _r2);
	Quat& FromAxisAngle(const Vec3& _axis, float _angle);

	float x, y, z, w;
};

inline Quat Mix(const Quat& _a, const Quat& _b, float _t)
{
	return _a.Slerp(_b, _t);
}

const Quat QUAT_ZERO(0);
const Quat QUAT_IDENTITY(1);

//----------------------------------------------------------------------------//
// Mat33
//----------------------------------------------------------------------------//

struct Mat33
{
	Vec3 r[3];
};

//----------------------------------------------------------------------------//
// Mat34
//----------------------------------------------------------------------------//

struct Mat34
{
	Vec4 r[3];
};

//----------------------------------------------------------------------------//
// Mat44
//----------------------------------------------------------------------------//

/// Row-major matrix 4x4
struct Mat44
{
	Mat44(void) { }
	explicit Mat44(float _val) { SetIdentity(_val); }
	explicit Mat44(const float* _m44) { FromPtr(_m44); }
	Mat44(float _00, float _01, float _02, float _03, float _10, float _11, float _12, float _13, float _20, float _21, float _22, float _23, float _30, float _31, float _32, float _33);

	Mat44 Copy(void) const { return *this; }
	Mat44& FromPtr(const float* _m44);
	Mat44& Set(const Mat44& _other) { return *this = _other; }
	Mat44& Set(float _00, float _01, float _02, float _03, float _10, float _11, float _12, float _13, float _20, float _21, float _22, float _23, float _30, float _31, float _32, float _33);
	Mat44& SetDiagonal(float _val) { m00 = _val, m11 = _val, m22 = _val, m33 = _val; return *this; }
	Mat44& SetZero(void) { for (uint i = 0; i < 16; ++i) v[i] = 0; return *this; }
	Mat44& SetIdentity(float _val = 1) { return SetZero().SetDiagonal(_val); }

	Vec3& Row(uint _r) { return *(Vec3*)(m[_r]); }
	const Vec3& Row(uint _r) const { return *(Vec3*)(m[_r]); }

	float* operator [] (uint _r) { return m[_r]; }
	const float* operator [] (uint _r) const { return m[_r]; }
	const float* operator * (void) const { return v; }
	float* operator * (void) { return v; }

	Vec3 Translate(const Vec3& _v) const { return Vec3(_v.x + m03, _v.y + m13, _v.z + m23); }
	Vec3 Transform(const Vec3& _v) const;
	Vec3 TransformVectorAbs(const Vec3& _v) const;
	Vec3 TransformVector(const Vec3& _v) const;
	Vec3 TransformProj(const Vec3& _v) const;
	Vec4 Transform(const Vec4& _v) const;
	friend Vec3 operator * (const Vec3&_lhs, const Mat44& _rhs) { return _rhs.TransformProj(_lhs); }
	friend Vec4 operator * (const Vec4&_lhs, const Mat44& _rhs) { return _rhs.Transform(_lhs); }

	Mat44& Add(const Mat44& _rhs) { for (uint i = 0; i < 16; ++i) v[i] += _rhs.v[i]; return *this; }
	Mat44 operator + (const Mat44& _rhs) const { return Copy().Add(_rhs); }
	Mat44& operator += (const Mat44& _rhs) { return Add(_rhs); }

	Mat44& Mul(float _rhs) { for (uint i = 0; i < 16; ++i) v[i] *= _rhs; return *this; }
	Mat44 operator * (float _rhs) const { return Copy().Mul(_rhs); }
	Mat44& operator *= (float _rhs) { return Mul(_rhs); }

	Mat44& Mul(const Mat44& _rhs);
	Mat44 operator * (const Mat44& _rhs) const { return Copy().Mul(_rhs); }
	Mat44& operator *= (const Mat44& _rhs) { return Mul(_rhs); }

	bool IsAffine(void) const { return m30 == 0 && m31 == 0 && m32 == 0 && m33 == 1; }

	Vec3 GetTranslation(void) const { return Vec3(m03, m13, m23); }
	Mat44& SetTranslation(const Vec3& _t) { m03 = _t.x, m13 = _t.y, m23 = _t.z; return *this; }
	Mat44& CreateTranslation(const Vec3& _t) { return SetIdentity().SetTranslation(_t); }

	Vec3 GetScale(void) const;
	Mat44& SetScale(const Vec3& _s) { return SetRotationScale(GetRotation(), _s); }
	Mat44& CreateScale(const Vec3& _s);

	Quat GetRotation(void) const;
	Mat44& SetRotation(const Quat& _r) { return SetRotationScale(_r, GetScale()); }
	Mat44& CreateRotation(const Quat& _r) { return SetIdentity().SetRotationScale(_r, 1); }

	Mat44& SetRotationScale(const Quat& _r, const Vec3& _s);
	Mat44& CreateRotationScale(const Quat& _r, const Vec3& _s) { return SetIdentity().SetRotationScale(_r, _s); }
	Mat44& CreateTransform(const Vec3& _t, const Quat& _r, const Vec3& _s = 1);
	Mat44& CreateInverseTransform(const Vec3& _t, const Quat& _r, const Vec3& _s = 1);
	
	Mat44& Inverse(void);
	Mat44& Transpose(void)
	{
		Swap(m[0][1], m[1][0]);
		Swap(m[0][2], m[2][0]);
		Swap(m[1][2], m[2][1]);
		Swap(m[1][3], m[3][1]);
		Swap(m[2][3], m[3][2]);
		return *this;
	}

	/*Mat44& CreateProjection(float _left, float _right, float _bottom, float _top, float _znear, float _zfar, uint _flags)
	{
		return *this;
	}*/

	/*Mat44& CreateLookAt()
	{

	}*/

	Mat44& CreatePerspective(float _fov, float _aspect, float _near, float _far, bool _reversed = false)
	{
		if (_aspect != _aspect)
			_aspect = 1; // NaN
		if (_far == _near)
			_far = _near + EPSILON;
		float _h = 1 / Tan(_fov * 0.5f);
		float _w = _h / _aspect;
		float _d = (_far - _near);
		float _q = -(_far + _near) / _d;
		float _qn = -2 * (_far * _near) / _d;
		if (_reversed)
			Swap(_q, _qn);
		SetZero();
		m00 = _w;
		m11 = _h;
		m22 = _q;
		m23 = _qn;
		m32 = -1;
		return *this;
	}

	Mat44& CreateOrtho(float _left, float _right, float _bottom, float _top, float _znear, float _zfar);
	Mat44& CreateOrtho2D(float _width, float _height);

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
// Ray 
//----------------------------------------------------------------------------//

struct Ray
{
	Ray(void) : origin(VEC3_ZERO), dir(VEC3_UNIT_Z) { }
	Ray(const Vec3& _origin, const Vec3& _dir) : origin(_origin), dir(_dir) { }
	Vec3 Point(float _t) const { return origin + dir * _t; }
	Vec3 operator * (float _t) const { return Point(_t); }
	friend Vec3 operator * (float _t, const Ray& _ray) { return _ray.Point(_t); }

	Vec3 origin, dir;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

inline Vec3 TriangleNormal(const Vec3& _v0, const Vec3& _v1, const Vec3& _v2)
{
	return (_v1 - _v0).Cross(_v2 - _v0);
}

//----------------------------------------------------------------------------//
// Plane
//----------------------------------------------------------------------------//

struct Plane
{
	Plane(void) : normal(VEC3_ZERO), dist(0) { }
	Plane(float _nx, float _ny, float _nz, float _d) : normal(_nx, _ny, _nz), dist(_d) { }
	Plane(const Vec3& _n, float _d) : normal(_n), dist(_d) { }

	Plane Copy(void) const { return *this; }

	Plane& Set(const Plane& _other) { return *this = _other; }
	Plane& Set(const Vec4& _v) { return Set(_v.x, _v.y, _v.z, Abs(_v.w)); }
	Plane& Set(float _nx, float _ny, float _nz, float _d) { normal.Set(_nx, _ny, _nz); dist = _d; return *this; }
	Plane& Set(const Vec3& _normal, float _distance) { normal = _normal, dist = _distance; return *this; }

	Plane& FromNormalPoint(const Vec3& _normal, const Vec3& _point) { return Set(_normal, -_normal.AbsDot(_point)); }
	Plane& FromTriangle(const Vec3& _v0, const Vec3& _v1, const Vec3& _v2) { return FromNormalPoint(TriangleNormal(_v0, _v1, _v2), _v0); }

	Vec4& AsVec4(void) { return *(Vec4*)&normal; }
	const Vec4& AsVec4(void) const { return *(const Vec4*)&normal; }

	Plane& Normalize(void)
	{
		float _length = normal.LengthSq();
		if (_length > EPSILON2)
		{
			_length = 1 / _length;
			normal *= _length;
			dist *= _length;
		}
		return *this;
	}
	Plane& Transform(const Mat44& _m) { return Set(AsVec4() * _m.Copy().Inverse().Transpose()); }

	Plane operator * (const Mat44& _m) const { return Copy().Transform(_m); }
	Plane& operator *= (const Mat44& _m) { return Transform(_m); }

	float Distance(const Vec3& _point) const { return normal.Dot(_point) + dist; }
	float Distance(const Vec3& _center, float _radius) const
	{
		float _d = normal.Dot(_center) + dist;
		float _md = normal.AbsDot(_radius);
		return (_d < -_md ? _d + _md : (_d > _md ? _d - _md : 0));
	}

	Mat44 GetReflectionMatrix(void) const 
	{
		Vec3 _n = -2 * normal;
		Mat44 _m;
		_m.SetIdentity();
		for (uint r = 0; r < 3; ++r)
		{
			for (uint c = 0; c < 4; ++r)
			{
				_m[r][c] += _n[r] * AsVec4()[c];
			}
		}
		return _m;
	}

	Vec3 normal;
	float dist;
};

//----------------------------------------------------------------------------//
// AlignedBox
//----------------------------------------------------------------------------//

const uint16 BOX_LINE_INDICES[24] = { 0, 1, 1, 2, 2, 3, 3, 0, 3, 5, 2, 4, 1, 7, 0, 6, 4, 5, 5, 6, 6, 7, 7, 4 };
const uint16 BOX_QUAD_INDICES[24] = { 5, 3, 2, 4, 4, 2, 1, 7, 7, 1, 0, 6, 6, 0, 3, 5, 6, 5, 4, 7, 2, 3, 0, 1 };
const uint16 BOX_TRIANLGE_INDICES[36] = { 5, 3, 2, 5, 2, 4, 4, 2, 1, 4, 1, 7, 7, 1, 0, 7, 0, 6, 6, 0, 3, 6, 3, 5, 6, 5, 4, 6, 4, 7, 3, 0, 1, 3, 1, 2 };

enum BoxCorner : unsigned int
{
	BC_LeftBottomFar = 0,// min
	BC_RightBottomFar,
	BC_RightBottomNear,
	BC_LeftBottomNear,
	BC_RightTopNear, // max
	BC_LeftTopNear,
	BC_LeftTopFar,
	BC_RightTopFar,
};

struct AlignedBox
{
	AlignedBox(void) : mn(999999.9f), mx(-999999.9f) { }
	AlignedBox(const Vec3& _min, const Vec3& _max) : mn(_min), mx(_max) { }

	AlignedBox Copy(void) const { return *this; }

	AlignedBox& Set(const AlignedBox& _b) { return *this = _b; }
	AlignedBox& Set(const Vec3& _min, const Vec3& _max) { mn.Set(_min), mx.Set(_max); return *this; }
	AlignedBox& SetMinMax(const Vec3& _a, const Vec3& _b) { mn.SetMin(_a, _b), mx.SetMax(_a, _b); return *this; }
	AlignedBox& SetZero(void) { mn = VEC3_ZERO, mx = VEC3_ZERO; return *this; }
	AlignedBox& FromCenterExtends(const Vec3& _center, const Vec3& _extends) { return Set(_center - _extends, _center + _extends); }
	AlignedBox& FromViewProjMatrix(const Mat44& _m) { return *this = AlignedBox(-1, 1).TransformProj(_m); }

	AlignedBox& Reset(const Vec3& _pt) { return Set(_pt, _pt); }
	AlignedBox& Reset(void) { mn = 999999.9f, mx = -999999.9f; return *this; }
	AlignedBox& AddPoint(const Vec3& _pt) { mn.SetMin(_pt), mx.SetMax(_pt); return *this; }
	AlignedBox& AddVertices(const void* _src, uint _count, size_t _stride = 0, size_t _offset = 0);

	bool IsZero(void) const { return mn == mx && mn == VEC3_ZERO; }
	bool IsFinite(void) const { return mn.x <= mx.x && mn.y <= mx.y && mn.z <= mx.z; }

	Vec3 Size(void) const { return mx - mn; }
	Vec3 Extends(void) const { return (mx - mn) * 0.5f; }
	Vec3 Center(void) const { return (mx + mn) * 0.5f; }
	//operator Sphere (void) const { return Sphere(Center(), Radius()); }
	float Diagonal(void) const { return (mx - mn).Length(); }
	float DiagonalSq(void) const { return (mx - mn).LengthSq(); }
	float Radius(void) const { return Diagonal() * 0.5f; }
	float Width(void) const { return mx.x - mn.x; }
	float Height(void) const { return mx.y - mn.y; }
	float Depth(void) const { return mx.z - mn.z; }
	float Volume(void) const { return (mx - mn).LengthSq(); }
	void GetAllCorners(const void* _dst, size_t _stride = 0, size_t _offset = 0) const;

	AlignedBox TransformAffine(const Mat44& _rhs) { return FromCenterExtends(Center() * _rhs, _rhs.TransformVectorAbs(Extends())); }
	AlignedBox TransformProj(const Mat44& _rhs);

	AlignedBox operator + (const Vec3& _point) const { return Copy().AddPoint(_point); }
	AlignedBox operator + (const AlignedBox& _box) const { return Copy().AddPoint(_box.mn).AddPoint(_box.mx); }
	AlignedBox& operator += (const Vec3& _point) { return AddPoint(_point); }
	AlignedBox& operator += (const AlignedBox& _box) { return AddPoint(_box.mn).AddPoint(_box.mx); }
	AlignedBox& operator *= (const Mat44& _rhs) { return TransformAffine(_rhs); }
	AlignedBox operator * (const Mat44& _rhs) const { return Copy().TransformAffine(_rhs); }

	bool Contains(const Vec3& _point) const { return _point >= mn && _point <= mx; }
	bool Contains(const AlignedBox& _box) const { return _box.mx <= mx && _box.mn >= mn; }
	bool Intersects(const AlignedBox& _box) const { return _box.mx >= mn && _box.mn <= mx; }

	Vec3 mn, mx;
};

//----------------------------------------------------------------------------//
// OrientedBox
//----------------------------------------------------------------------------//

struct OrientedBox
{

};

//----------------------------------------------------------------------------//
// Sphere
//----------------------------------------------------------------------------//

struct Sphere
{

};

//----------------------------------------------------------------------------//
// Frustum
//----------------------------------------------------------------------------//

enum FrustumPlane : uint16
{
	FP_Left,
	FP_Right,
	FP_Bottom,
	FP_Top,
	FP_Near,
	FP_Far,
};

struct Frustum
{
	Frustum(void) { }

	Frustum& FromCameraMatrices(const Mat44& _view, const Mat44& _proj);

	bool Intersects(const Vec3& _point) const;
	bool Intersects(const Vec3& _center, float _radius) const;
	bool Intersects(const AlignedBox& _box, bool* _contains = nullptr) const;
	bool Intersects(const Frustum& _frustum, bool* _contains = nullptr) const;

	float Distance(const Vec3& _point) const { return origin.Distance(_point); }
	float Distance(const Vec3& _center, float _radius) const { float _d = origin.Distance(_center); return _d < _radius ? 0 : _d - _radius; }
	float Distance(const AlignedBox& _box) const { return Distance(_box.Center(), _box.Radius()); }


	static void GetPlanes(Plane* _planes, const Mat44& _m);

	Plane planes[6];
	Vec3 corners[8];
	Vec3 origin;
	AlignedBox box;
};

//----------------------------------------------------------------------------//
// DbvtNode
//----------------------------------------------------------------------------//

struct DbvTreeNode
{
	DbvTreeNode* parent;
	AlignedBox box;
	union
	{
		DbvTreeNode* child[2];
		struct
		{
			void* object;
			bool leaf;
		};
	};
};

//----------------------------------------------------------------------------//
// DbvTree
//----------------------------------------------------------------------------//

class DbvTree
{
public:

protected:

	DbvTreeNode* m_root;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

