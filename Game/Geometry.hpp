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

	Geometry(BufferUsage _usage = BU_Static);
	~Geometry(void);

	Array<Vertex>& Vertices(void) { return m_vertices; }
	Array<index_t>& Indices(void) { return m_indices; }
	uint GetVertexCount(void) { return m_vertices.Size(); }
	uint GetIndexCount(void) { return m_indices.Size(); }
	PrimitiveType GetPrimitiveType(void) { return m_type; }

	void Clear(void);

	void Merge(const Vertex* _vertices, uint _numVertices, const index_t* _indices, uint _numIndices);
	void Transform(const Mat44& _m);
	// Mirror
	// MakeAdjacency
	void ComputeNormals(void);
	// ComputeTangents
	// Load
	// Save
	AlignedBox ComputeBBox(void);

	void CreateCube(const Vec3& _size = 1, bool _smoothed = false) { CreateCube(AlignedBox().FromCenterExtends(VEC3_ZERO, _size * 0.5f), _smoothed); }
	void CreateCube(const AlignedBox& _box, bool _smoothed = false);
	void CreateSphere(float _radius = 1, int _segments = 16, int _rings = 8);
	void CreateGridXZ(uint _numSectors, float _sectorSize);


	VertexArrayPtr CreateVertexArray(BufferUsage _usage = BU_Static);
	void Upload(VertexArray* _dst);

protected:

	Array<Vertex> m_vertices;
	Array<index_t> m_indices;
	PrimitiveType m_type;
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//


