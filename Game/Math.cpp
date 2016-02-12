#pragma once

#include "Math.hpp"

//----------------------------------------------------------------------------//
// Noise
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
float Noise2d(int _x, int _y, int _rseed)
{
	int _n = _x + _y * _rseed;
	_n = (_n << 13) ^ _n;
	return (1.0f - ((_n * (_n * _n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}
//----------------------------------------------------------------------------//
float SmoothedNoise2d(int _x, int _y, int _rseed)
{
	float _n = 0;
	_n += (Noise2d(_x - 1, _y - 1, _rseed) +
		Noise2d(_x + 1, _y - 1, _rseed) +
		Noise2d(_x - 1, _y + 1, _rseed) +
		Noise2d(_x + 1, _y + 1, _rseed)) * 0.0625f; // corners 1/16 
	_n += (Noise2d(_x - 1, _y, _rseed) +
		Noise2d(_x + 1, _y, _rseed) +
		Noise2d(_x, _y - 1, _rseed) +
		Noise2d(_x, _y + 1, _rseed)) * 0.125f; // sides 1/8
	_n += Noise2d(_x, _y, _rseed) * 0.25f; // center 1/4
	return _n;
}
//----------------------------------------------------------------------------//
float InterpolatedNoise2d(float _x, float _y, int _rseed)
{
	int _ix = (int)_x, _iy = (int)_y;
	float _tx = _x - _ix, _ty = _y - _iy;
	float _v0 = SmoothedNoise2d(_ix, _iy, _rseed);
	float _v1 = SmoothedNoise2d(_ix + 1, _iy, _rseed);
	float _v2 = SmoothedNoise2d(_ix, _iy + 1, _rseed);
	float _v3 = SmoothedNoise2d(_ix + 1, _iy + 1, _rseed);
	return Mix(Mix(_v0, _v1, _tx), Mix(_v2, _v3, _tx), _ty);
}
//----------------------------------------------------------------------------//
float Perlin2d(float _x, float _y, int _rseed, uint _iterations)
{
	float _n = 0, _f = 1, _w = 1;
	for (uint i = 0; i < _iterations; ++i)
	{
		_n += InterpolatedNoise2d(_x * _f, _y * _f, _rseed) / _w;
		_w += _w;
		_f *= 2;
	}
	return (1 - _n) * 0.5f;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Quat
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Quat& Quat::Mul(const Quat& _rhs)
{
	return Set(w * _rhs.x + x * _rhs.w + y * _rhs.z - z * _rhs.y,
		w * _rhs.y + y * _rhs.w + z * _rhs.x - x * _rhs.z,
		w * _rhs.z + z * _rhs.w + x * _rhs.y - y * _rhs.x,
		w * _rhs.w - x * _rhs.x - y * _rhs.y - z * _rhs.z);
}
//----------------------------------------------------------------------------//
Vec3& Quat::Transform(const Vec3& _v) const
{
	const Vec3& _q = *(const Vec3*)(&x);
	Vec3 _uv(_q.Cross(_v));
	return _v + _uv * 2 * w + _q.Cross(_uv) * 2;
}
//----------------------------------------------------------------------------//
Quat& Quat::Normalize(void)
{
	float _l = x * x + y * y + z * z + w * w;
	if (_l > EPSILON2)
		*this *= RSqrt(_l);
	return *this;
}
//----------------------------------------------------------------------------//
Quat& Quat::Inverse(void)
{
	float _d = Dot(*this);
	if (_d > 0)
		x *= -_d, y *= -_d, z *= -_d, w *= _d;
	else
		x = 0, y = 0, z = 0, w = 1;
	return *this;
}
//----------------------------------------------------------------------------//
Quat Quat::Nlerp(const Quat& _q, float _t, bool _shortestPath) const
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
//----------------------------------------------------------------------------//
Quat Quat::Slerp(const Quat& _q, float _t, bool _shortestPath) const
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
//----------------------------------------------------------------------------//
void Quat::ToRotationMatrix(float* _r0, float* _r1, float* _r2) const
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
//----------------------------------------------------------------------------//
Quat& Quat::FromRotationMatrix(const float* _r0, const float* _r1, const float* _r2)
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
//----------------------------------------------------------------------------//
Quat& Quat::FromAxisAngle(const Vec3& _axis, float _angle)
{
	float _s, _c;
	SinCos(_angle * 0.5f, _s, _c);
	return Set(_axis.x * _s, _axis.y * _s, _axis.z * _s, _c);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Mat44
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Mat44::Mat44(float _00, float _01, float _02, float _03,
	float _10, float _11, float _12, float _13,
	float _20, float _21, float _22, float _23,
	float _30, float _31, float _32, float _33) :
	m00(_00), m01(_01), m02(_02), m03(_03),
	m10(_10), m11(_11), m12(_12), m13(_13),
	m20(_20), m21(_21), m22(_22), m23(_23),
	m30(_30), m31(_31), m32(_32), m33(_33)
{
}
//----------------------------------------------------------------------------//
Mat44& Mat44::FromPtr(const float* _m44)
{
	ASSERT(_m44 != nullptr);
	memcpy(v, _m44, sizeof(float) * 16);
	return *this;
}
//----------------------------------------------------------------------------//
Mat44& Mat44::Set(float _00, float _01, float _02, float _03,
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
//----------------------------------------------------------------------------//
Vec3 Mat44::Transform(const Vec3& _v) const
{
	return Vec3(
		m00 * _v.x + m01 * _v.y + m02 * _v.z + m03,
		m10 * _v.x + m11 * _v.y + m12 * _v.z + m13,
		m20 * _v.x + m21 * _v.y + m22 * _v.z + m23);
}
//----------------------------------------------------------------------------//
Vec3 Mat44::TransformVectorAbs(const Vec3& _v) const
{
	return Vec3(
		Abs(m00) * _v.x + Abs(m01) * _v.y + Abs(m02) * _v.z,
		Abs(m10) * _v.x + Abs(m11) * _v.y + Abs(m12) * _v.z,
		Abs(m20) * _v.x + Abs(m21) * _v.y + Abs(m22) * _v.z);
}
//----------------------------------------------------------------------------//
Vec3 Mat44::TransformVector(const Vec3& _v) const
{
	return Vec3(
		m00 * _v.x + m01 * _v.y + m02 * _v.z,
		m10 * _v.x + m11 * _v.y + m12 * _v.z,
		m20 * _v.x + m21 * _v.y + m22 * _v.z);
}
//----------------------------------------------------------------------------//
Vec3 Mat44::TransformProj(const Vec3& _v) const
{
	float _iw = 1 / (m30 * _v.x + m31 * _v.y + m32 * _v.z + m33);
	return Vec3(
		(m00 * _v.x + m01 * _v.y + m02 * _v.z + m03) * _iw,
		(m10 * _v.x + m11 * _v.y + m12 * _v.z + m13) * _iw,
		(m20 * _v.x + m21 * _v.y + m22 * _v.z + m23) * _iw);
}
//----------------------------------------------------------------------------//
Mat44& Mat44::Mul(const Mat44& _rhs)
{
	Mat44 _r;
	for (uint r = 0; r < 4; ++r)
	{
		for (uint c = 0; c < 4; ++c)
			_r[r][c] = m[r][0] * _rhs[0][c] + m[r][1] * _rhs[1][c] + m[r][2] * _rhs[2][c] + m[r][3] * _rhs[3][c];
	}
	return *this = _r;
}
//----------------------------------------------------------------------------//
Vec3 Mat44::GetScale(void) const
{
	return Vec3(
		Sqrt(m00 * m00 + m10 * m10 + m20 * m20),
		Sqrt(m01 * m01 + m11 * m11 + m21 * m21),
		Sqrt(m02 * m02 + m12 * m12 + m22 * m22));
}
//----------------------------------------------------------------------------//
Mat44& Mat44::CreateScale(const Vec3& _s)
{
	SetZero();
	m00 = _s.x;
	m11 = _s.y;
	m22 = _s.z;
	m33 = 1;
	return *this;
}
//----------------------------------------------------------------------------//
Quat Mat44::GetRotation(void) const
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
//----------------------------------------------------------------------------//
Mat44& Mat44::SetRotationScale(const Quat& _r, const Vec3& _s)
{
	_r.ToRotationMatrix(m[0], m[1], m[2]);
	Row(0) *= _s, Row(1) *= _s, Row(2) *= _s;
	return *this;
}
//----------------------------------------------------------------------------//
Mat44& Mat44::CreateTransform(const Vec3& _t, const Quat& _r, const Vec3& _s)
{
	// Ordering: Scale, Rotate, Translate
	float _r0[3], _r1[3], _r2[3];
	_r.ToRotationMatrix(_r0, _r1, _r2);
	m00 = _s.x * _r0[0], m01 = _s.y * _r0[1], m02 = _s.z * _r0[2], m03 = _t.x;
	m10 = _s.x * _r1[0], m11 = _s.y * _r1[1], m12 = _s.z * _r1[2], m13 = _t.y;
	m20 = _s.x * _r2[0], m21 = _s.y * _r2[1], m22 = _s.z * _r2[2], m23 = _t.z;
	m30 = 0, m31 = 0, m32 = 0, m33 = 1;
	return *this;
}
//----------------------------------------------------------------------------//
Mat44& Mat44::CreateInverseTransform(const Vec3& _t, const Quat& _r, const Vec3& _s)
{
	Vec3 _invs(1 / _s);
	Quat _invr = _r.Copy().Inverse();
	Vec3 _invt((-_t * _invr) * _invs);
	return CreateTransform(_invt, _invr, _invs);
}
//----------------------------------------------------------------------------//
Mat44& Mat44::Inverse(void)
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
//----------------------------------------------------------------------------//
Mat44& Mat44::CreateOrtho(float _left, float _right, float _bottom, float _top, float _znear, float _zfar)
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
//----------------------------------------------------------------------------//
Mat44& Mat44::CreateOrtho2D(float _width, float _height)
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
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
