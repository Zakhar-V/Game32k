#pragma once

#include "Geometry.hpp"

//----------------------------------------------------------------------------//
// Geometry
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Geometry::Geometry(BufferUsage _usage) :
	m_usage(_usage),
	m_type(PT_Points)
{
}
//----------------------------------------------------------------------------//
Geometry::~Geometry(void)
{
}
//----------------------------------------------------------------------------//
void Geometry::Clear(void)
{
	m_vertices.Clear();
	m_indices.Clear();
}
//----------------------------------------------------------------------------//
void Geometry::Begin(PrimitiveType _type)
{
	Clear();
	m_type = _type;
}
//----------------------------------------------------------------------------//
void Geometry::End(void)
{
	Upload();
}
//----------------------------------------------------------------------------//
void Geometry::Merge(const Vertex* _vertices, uint _numVertices, const index_t* _indices, uint _numIndices)
{
	ASSERT(_vertices || !_numVertices);

	uint16 _baseVertex = (uint16)m_vertices.Size();
	m_vertices.Push(_vertices, _numVertices);

	if (_numIndices)
	{
		ASSERT(_indices);
		m_indices.Upsize(_numIndices);
		for (uint i = 0; i < _numIndices; ++i)
			m_indices.Push(_indices[i] + _baseVertex);
	}
}
//----------------------------------------------------------------------------//
void Geometry::CreateGridXZ(uint _numSectors, float _sectorSize)
{
	ASSERT((_numSectors + 1) * (_numSectors + 1) < (index_t)-1); // 254 for uint16_t

	Clear();
	//m_type = PT_Triangles;

	uint _numVertices = _numSectors + 1;
	uint _numQuads = _numSectors - 1;
	float _size = _sectorSize * _numSectors;
	float _start = _size * -0.5f;
	float _texStep = 1.0f / _numSectors;
	Vec2 _tc(0);
	Vec2 _pt(_start);
	Vertex _v;
	//_v.SetColor({ 0xff, 0xff, 0xff, 0xff });
	// normal
	// tangent
	for (uint y = 0, iy = 0; y <= _numSectors; ++y, iy += _numVertices)
	{
		_pt.x = _start;
		_tc.x = 0;
		for (uint x = 0; x <= _numSectors; ++x)
		{
			_v.position.Set(_pt.x, 0, _pt.y);
			_v.SetTexCoord(_tc);
			m_vertices.Push(_v);

			if (x < _numQuads && y < _numQuads)
			{
				/*
				0--1
				|  |
				|  |
				3--2
				*/
				uint _lt = iy + x; // 0
				uint _rt = _lt + 1; // 1
				uint _lb = _lt + _numVertices; // 3
				uint _rb = _lb + 1;	// 2
				m_indices.Push(_rt).Push(_lt).Push(_lb); // 103
				m_indices.Push(_rt).Push(_lb).Push(_rb); // 132
				//m_indices.Push(_rt).Push(_lt).Push(_rb); // 102
				//m_indices.Push(_lb).Push(_rb).Push(_lt); // 320
			}

			_pt.x += _sectorSize;
			_tc.x += _texStep;
		}
		_pt.y += _sectorSize;
		_tc.y += _texStep;
	}
}
//----------------------------------------------------------------------------//
void Geometry::Upload(void)
{
	if (m_vertices.Size())
	{
		if (!m_vertexBuffer)
			m_vertexBuffer = new Buffer(m_usage);

		uint _size = m_vertices.Size() * sizeof(Vertex);

		if (m_vertexBuffer->Size() < _size)
			m_vertexBuffer->Realloc(_size, m_vertices.Ptr());
		else
			m_vertexBuffer->Write(m_vertices.Ptr(), 0, _size);
	}

	if (m_indices.Size())
	{
		if (!m_indexBuffer)
			m_indexBuffer = new Buffer(m_usage);

		uint _size = m_indices.Size() * sizeof(index_t);

		if (m_indexBuffer->Size() < _size)
			m_indexBuffer->Realloc(_size, m_indices.Ptr());
		else
			m_indexBuffer->Write(m_indices.Ptr(), 0, _size);
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

