#pragma once

#include "Geometry.hpp"

//----------------------------------------------------------------------------//
// Geometry
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Geometry::Geometry(BufferUsage _usage) :
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
void Geometry::Merge(const Vertex* _vertices, uint _numVertices, const index_t* _indices, uint _numIndices)
{
	ASSERT(_vertices || !_numVertices);

	index_t _baseVertex = (index_t)m_vertices.Size();
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
void Geometry::Transform(const Mat44& _m)
{
	Mat44 _nm = _m.Copy().Transpose();
	for (uint i = 0; i < m_vertices.Size(); ++i)
	{
		Vertex& _v = m_vertices[i];
		_v.position = _m.Transform(_v.position);
		_v.SetNormal(_nm.TransformVector(_v.GetNormal()).Normalize());
	}
}
//----------------------------------------------------------------------------//
void Geometry::ComputeNormals(void)
{
	if (m_type != PT_Triangles)
		return;

	Vec3 _n;
	Array<Vec3> _normals;
	_normals.Resize(m_vertices.Size(), VEC3_ZERO);

	if (m_indices.Size())
	{
		for (uint i = 0; i + 3 <= m_indices.Size(); i += 3)
		{
			index_t _idx0 = m_indices[i + 0];
			index_t _idx1 = m_indices[i + 1];
			index_t _idx2 = m_indices[i + 2];
			_n = TriangleNormal(m_vertices[_idx0].position, m_vertices[_idx1].position, m_vertices[_idx2].position);
			_normals[_idx0] += _n;
			_normals[_idx1] += _n;
			_normals[_idx2] += _n;
		}
		for (uint i = 0; i < _normals.Size(); ++i)
		{
			m_vertices[i].SetNormal(_normals[i].Normalize());
		}
	}
	else if(m_indices.Size() > 3)
	{
		for (uint i = 0; i + 3 <= m_vertices.Size(); i += 3)
		{
			_n = TriangleNormal(m_vertices[i + 0].position, m_vertices[i + 1].position, m_vertices[i + 2].position).Normalize();
			m_vertices[i + 0].SetNormal(_n);
			m_vertices[i + 1].SetNormal(_n);
			m_vertices[i + 2].SetNormal(_n);
		}
	}
}
//----------------------------------------------------------------------------//
AlignedBox Geometry::ComputeBBox(void)
{
	return AlignedBox().AddVertices(m_vertices.Ptr(), m_vertices.Size(), sizeof(Vertex));
}
//----------------------------------------------------------------------------//
void Geometry::CreateCube(const AlignedBox& _box, bool _smoothed)
{
	static const float _tc[4][2] =
	{
		{ 0, 0 }, // 0 lt
		{ 0, 1 }, // 1 lb
		{ 1, 1 }, // 2 rb
		{ 1, 0 }, // 3 rt
	};

	Clear();
	m_type = PT_Triangles;

	Vertex _v;
	Vertex _corners[8];
	_box.GetAllCorners(_corners, sizeof(Vertex));

	if (_smoothed)
	{
		// todo: texcoord
		m_vertices.Push(_corners, 8);
		m_indices.Push(BOX_TRIANLGE_INDICES, 36);
	}
	else
	{
		m_vertices.Reserve(24);
		m_indices.Reserve(36);
		for (uint i = 0; i < 24; i += 4)
		{
			// lt lb rb rt 
			for (uint j = 0; j < 4; ++j)
			{
				_v = _corners[BOX_QUAD_INDICES[i + j]];
				_v.SetTexCoord({ _tc[j][0], _tc[j][1] });
				m_vertices.Push(_v);
			}

			// 012 023
			m_indices.Push(i + 0).Push(i + 1).Push(i + 2);
			m_indices.Push(i + 0).Push(i + 2).Push(i + 3);
		}
	}
}
//----------------------------------------------------------------------------//
void Geometry::CreateSphere(float _radius, int _segments, int _rings)
{
	//http://www.ogre3d.org/tikiwiki/ManualSphereMeshes
	Clear();
	m_type = PT_Triangles;

	m_vertices.Resize((_segments + 1) * (_rings + 1));
	m_indices.Resize((_segments + 1) * _rings * 6);

	Vertex* _v = m_vertices.Ptr();
	index_t* _i = m_indices.Ptr();

	float _xz, _x, _y, _z;
	float _invS = 1.0f / _segments;
	float _invR = 1.0f / _rings;
	float _rAngle = PI * _invR;
	float _sAngle = 2 * PI * _invS;
	index_t _idx = 0;
	Vec3 _n;

	for (int _ring = 0; _ring <= _rings; _ring++)
	{
		SinCos(_ring * _rAngle, _xz, _y);
		for (int _seg = 0; _seg <= _segments; _seg++)
		{
			SinCos(_seg * _sAngle, _x, _z);

			_n.Set(_x * _xz, _y, _z * _xz);
			_v->position.Set(_n * _radius);
			_v->SetNormal(_n);
			_v->SetTexCoord({ _seg * _invS, _ring * _invR });
			++_v;

			if (_ring != _rings)
			{
				*_i++ = _idx + _segments + 1;
				*_i++ = _idx;
				*_i++ = _idx + _segments;
				*_i++ = _idx + _segments + 1;
				*_i++ = _idx + 1;
				*_i++ = _idx;
				_idx++;
			}
		}
	}
}
//----------------------------------------------------------------------------//
void Geometry::CreateGridXZ(uint _numSectors, float _sectorSize)
{
	ASSERT((_numSectors + 1) * (_numSectors + 1) < (index_t)-1); // 254 for uint16_t

	Clear();
	m_type = PT_Triangles;

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
VertexArrayPtr Geometry::CreateVertexArray(BufferUsage _usage)
{
	VertexArrayPtr _va = new VertexArray(m_type, m_indices.Size() != 0, _usage);
	Upload(_va);
	/*if (m_vertices.Size())
		_va->GetVertexBuffer()->Realloc(m_vertices.Size() * sizeof(Vertex), m_vertices.Ptr());
	if (m_indices.Size())
		_va->GetIndexBuffer()->Realloc(m_indices.Size() * sizeof(index_t), m_indices.Ptr());*/
	return _va;
}
//----------------------------------------------------------------------------//
void Geometry::Upload(VertexArray* _dst)
{
	ASSERT(_dst != nullptr);
	ASSERT(_dst->GetVertexBuffer() != nullptr);

	_dst->GetVertexBuffer()->Realloc(m_vertices.Size() * sizeof(Vertex), m_vertices.Ptr());

	if (m_indices.Size())
	{
		if (!_dst->GetIndexBuffer())
			_dst->SetIndexBuffer(new Buffer(_dst->GetVertexBuffer()->Usage()));
		_dst->GetIndexBuffer()->Realloc(m_indices.Size() * sizeof(index_t), m_indices.Ptr());
	}
	else if (!m_indices.Size())
	{
		_dst->SetIndexBuffer(nullptr);
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

