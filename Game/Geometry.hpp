#pragma once

#include "Math.hpp"
#include "Object.hpp"
#include "Graphics.hpp"

//----------------------------------------------------------------------------//
// Geometry
//----------------------------------------------------------------------------//

typedef Ptr<class Geometry> GeometryPtr;

class Geometry : public RefCounted
{
public:

	// Merge
	// Transform
	// Mirror
	// MakeAdjacency
	// ComputeNormals
	// ComputeTangents
	// Load
	// Save

	void MakeRenderable(bool _forceUpdate = false) { }

protected:

	Array<Vertex> m_vertices;
	Array<index_t> m_indices;


	//BufferPtr m_vertexBuffer;
	//BufferPtr m_indexBuffer;
};

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

struct Sprite
{
	Sprite(void) { }
	Sprite(const Vec3& _position, const Vec2& _size, float _scale, float _rotation, const Vec2& _texcoord1, const Vec2& _texcoord2, const Color& _color) :
		position(_position), size(_size), scale(_scale), rotation(_rotation), texcoord1(_texcoord1), texcoord2(_texcoord2), color(_color) { }

	Vec3 position;
	Vec2 size;
	Color color;
	Vec2 texcoord1;
	Vec2 texcoord2;
	float scale; // xy
	float rotation; // z

	/*static void AddToBatch(uint16* _idx, uint _baseIndex, SpriteVertex* _dst, const Sprite* _src, uint _count, bool _flipY = false)
	{
		while (_count--)
		{
			const Sprite& _sprite = *_src++;
			Vec2 _ts = _sprite.texcoord2 - _sprite.texcoord1;
			for (uint i = 0; i < 4; ++i)
			{
				_dst[i].position = _sprite.position;
				_dst[i].color = _sprite.color;
				_dst[i].rotation = _sprite.rotation;
				_dst[i].size.x = _sprite.size.x * _sprite.scale * QuadVertices[i][0];
				_dst[i].size.y = _sprite.size.y * _sprite.scale * QuadVertices[i][1];
				_dst[i].texcoord.x = _sprite.texcoord1.x + _ts.x * QuadTexCoord[i][0];
				_dst[i].texcoord.y = _sprite.texcoord1.y + _ts.y * QuadTexCoord[i][1];
			}

			if (_flipY)
			{
				Swap(_dst[0].texcoord.y, _dst[1].texcoord.y);
				Swap(_dst[2].texcoord.y, _dst[3].texcoord.y);
			}

			_dst += 4;

			for (uint i = 0; i < 6; ++i)
				*_idx++ = QuadIndices[i] + _baseIndex;
		}
	} */

};


//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//



//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//


