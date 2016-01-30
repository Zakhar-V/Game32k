#pragma once

#include "Base.hpp"
#include <math.h>

const float BYTE2FLOAT = 1.0f / 255.0f;
const float FLOAT2BYTE = 255.0f;

inline float ByteToFloat(uint8 _x) { return _x * BYTE2FLOAT; }
inline uint8 FloatToByte(float _x) { return (uint8)(_x * FLOAT2BYTE); }

struct Vec2
{
	Vec2(void) { }
	Vec2(float _s) : x(_s), y(_s) { }
	Vec2(float _x, float _y) : x(_x), y(_y) { }
	Vec2& Set(const Vec2& _v) { return *this = _v; }
	Vec2& Set(float _s) { x = _s, y = _s; return *this; }
	Vec2& Set(float _x, float _y) { x = _x, y = _y; return *this; }
	float x, y;
};

struct Vec2i
{
	Vec2i(void) { }
	Vec2i(int _s) : x(_s), y(_s) { }
	Vec2i(int _x, int _y) : x(_x), y(_y) { }
	Vec2i(const Vec2& _v) : x((int)_v.x), y((int)_v.y) { }
	operator Vec2 (void) const { return Vec2((float)x, (float)y); }
	Vec2i& Set(const Vec2i& _v) { return *this = _v; }
	Vec2i& Set(int _s) { x = _s, y = _s; return *this; }
	Vec2i& Set(int _x, int _y) { x = _x, y = _y; return *this; }

	int x, y;
};

struct Vec3
{
	Vec3(void) { }
	Vec3(float _s) : x(_s), y(_s), z(_s) { }
	Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }

	float x, y, z;
};

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

struct Color
{
	Color(void) { }
	//Color(uint _rgba) : r((_rgba >> 24) & 0xff), g((_rgba >> 16) & 0xff), b((_rgba >> 8) & 0xff), a((_rgba >> 0) & 0xff) { }
	Color(uint _rgba) : rgba(_rgba) { }
	Color(uint8 _r, uint8 _g, uint8 _b, uint8 _a = 0xff) : r(_r), g(_g), b(_b), a(_a) { }
	Color(const Vec4& _v) : r(FloatToByte(_v.r)), g(FloatToByte(_v.g)), b(FloatToByte(_v.b)), a(FloatToByte(_v.a)) { }
	operator Vec4 (void) const { return Vec4(ByteToFloat(r), ByteToFloat(g), ByteToFloat(b), ByteToFloat(a)); }

	uint Argb(void) const { return (rgba >> 8) | (a << 24); }

	union
	{
		//uint rgba;
		struct { uint8 r, g, b, a; };
		struct { uint8 x, y, z, w; };
		uint8 v[4];
		uint32 rgba;
	};
};

struct Mat34
{
	Vec4 r0, r1, r2;
};

struct Mat44
{
	Vec4 r0, r1, r2, r3;
};
