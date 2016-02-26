#pragma once

#include "Geometry.hpp"

//----------------------------------------------------------------------------//
// Geometry
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Geometry::Geometry(void)
{

}
//----------------------------------------------------------------------------//
Geometry::~Geometry(void)
{

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

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

