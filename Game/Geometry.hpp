#pragma once

#include "Math.hpp"
#include "Object.hpp"
#include "Graphics.hpp"

//----------------------------------------------------------------------------//
// Utils
//----------------------------------------------------------------------------//

static const float QuadVertices[4][2] =
{
	{ -0.5f, -0.5f, }, // 0 lt
	{ -0.5f, 0.5f, }, // 1 lb
	{ 0.5f, 0.5f, }, // 2 rb
	{ 0.5f, -0.5f, }, // 3 rt
};

static const float QuadTexCoord[4][2] =
{
	{ 0, 0, }, // 0 lt
	{ 0, 1, }, // 1 lb
	{ 1, 1, }, // 2 rb
	{ 1, 0, }, // 3 rt
};

static const uint16 QuadIndices[6] =
{
	// lt lb rt  rt lb rb
	0, 1, 3, 3, 1, 2,
};

void AddSpritesToBatch(SpriteVertex* _dst, const SpriteVertex* _src, uint _count, bool _flipYTexcoord = false)
{
	SpriteVertex _v[4];
	while (_count--)
	{
		const SpriteVertex& _sprite = *_src++;
		Vec2 _tc2 = _sprite.texcoord2 - _sprite.texcoord;
		for (uint i = 0; i < 4; ++i)
		{
			_v[i] = _sprite;
			_v[i].size.x = _sprite.size.x * QuadVertices[i][0];
			_v[i].size.y = _sprite.size.y * QuadVertices[i][1];
			_v[i].texcoord.x = _sprite.texcoord.x + _tc2.x * QuadTexCoord[i][0];
			_v[i].texcoord.y = _sprite.texcoord.y + _tc2.y * QuadTexCoord[i][1];
			_v[i].texcoord2.x = 1;
			_v[i].texcoord2.y = 1;
		}

		if (_flipYTexcoord)
		{
			Swap(_v[0].texcoord.y, _v[1].texcoord.y);
			Swap(_v[2].texcoord.y, _v[3].texcoord.y);
		}

		memcpy(_dst, _v, sizeof(_v));
		_dst += 4;
	}
}

void AddQuadsToBatch(uint16* _dst, uint16 _base, uint _count)
{
	while (_count--)
	{
		for (uint i = 0; i < 6; ++i)
			*_dst++ = QuadIndices[i] + _base;
	}
}

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//



//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

class Mesh : public RefCounted
{
public:

protected:


};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//


