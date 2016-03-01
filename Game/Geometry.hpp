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

	void Clear(void);
	void Begin(PrimitiveType _type);
	void End(void);

	void Merge(const Vertex* _vertices, uint _numVertices, const index_t* _indices, uint _numIndices);
	// Transform
	// Mirror
	// MakeAdjacency
	// ComputeNormals
	// ComputeTangents
	// Load
	// Save

	void CreateGridXZ(uint _numSectors, float _sectorSize);

	void Upload(void);
	Buffer* GetVertexBuffer(void) { return m_vertexBuffer; }
	uint GetVertexCount(void) { return m_vertices.Size(); }
	Buffer* GetIndexBuffer(void) { return m_indexBuffer; }
	uint GetIndexCount(void) { return m_indices.Size(); }
	PrimitiveType GetPrimitiveType(void) { return m_type; }

	//void GetRenderOp(RenderOp&) {}

protected:

	Array<Vertex> m_vertices;
	Array<index_t> m_indices;
	PrimitiveType m_type;
	BufferUsage m_usage;
	BufferPtr m_vertexBuffer;
	BufferPtr m_indexBuffer;
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//


