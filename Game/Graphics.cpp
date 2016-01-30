#pragma once

#include "Graphics.hpp"
#include "Window.hpp"

//----------------------------------------------------------------------------//
// VertexBuffer
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
VertexBuffer::VertexBuffer(uint _size, BufferUsage _usage) :
	m_size(_size),
	m_handle(nullptr)
{
	HRESULT _r = gGraphicsDevice->CreateVertexBuffer(_size, _usage, 0, D3DPOOL_DEFAULT, &m_handle, nullptr);
	CHECK(_r >= 0, "Couldn't create vertex buffer");

	//gGraphicsDevice->CreateIn
}
//----------------------------------------------------------------------------//
VertexBuffer::~VertexBuffer(void)
{
	m_handle->Release();
}
//----------------------------------------------------------------------------//
void VertexBuffer::Write(const void* _data, uint _offset, uint _size)
{
	void* _dst = nullptr;
	m_handle->Lock(_offset, _size, &_dst, D3DLOCK_DISCARD);
	memcpy(_dst, _data, _size);
	m_handle->Unlock();
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Graphics
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Graphics::Graphics(void)
{
	LOG("Create Graphics");

	ASSERT(gWindow != nullptr); // the window must be created before it

	m_d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!m_d3d)
		Fatal("Couldn't create IDirect3D9");

	D3DPRESENT_PARAMETERS _pp; 
	memset(&_pp, 0, sizeof(_pp));
	_pp.Windowed = TRUE;
	_pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	_pp.BackBufferFormat = D3DFMT_UNKNOWN;

	HRESULT _r = m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, gWindow->Handle(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &_pp, &m_device);
	_CHECK(_r >= 0, "Couldn't create IDirect3D9Device");

	// create vertex types
	{	 
		DLOG("Create vertex declarations");

		D3DVERTEXELEMENT9 _elems[7];
		D3DVERTEXELEMENT9 _pos = { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }; // Vec3 position
		D3DVERTEXELEMENT9 _tc = { 0, sizeof(Vec3), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }; // Vec2 texcoord
		D3DVERTEXELEMENT9 _end = D3DDECL_END();

		memset(m_vertexTypes, 0, sizeof(m_vertexTypes));

		// Vertex
		_elems[0] = _pos;
		_elems[1] = _end;
		_r = m_device->CreateVertexDeclaration(_elems, &m_vertexTypes[VF_Base]);
		CHECK(_r >= 0, "Couldn't create vertex declaration");

		// SimpleVertex
		_elems[1] = _tc;
		_elems[2] = { 0, offsetof(SimpleVertex, color), D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 };
		_elems[3] = _end;
		_r = m_device->CreateVertexDeclaration(_elems, &m_vertexTypes[VF_Simple]);
		CHECK(_r >= 0, "Couldn't create vertex declaration");

		// StaticVertex
		_elems[2] = { 0, offsetof(StaticVertex, normal), D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 };
		_elems[3] = { 0, offsetof(StaticVertex, tangent), D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 };
		_elems[4] = _end;
		_r = m_device->CreateVertexDeclaration(_elems, &m_vertexTypes[VF_Static]);
		CHECK(_r >= 0, "Couldn't create vertex declaration");

		// SkinnedVertex
		_elems[4] = { 0, offsetof(SkinnedVertex, bones), D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 };
		_elems[5] = { 0, offsetof(SkinnedVertex, weights), D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 };
		_elems[6] = _end;
		_r = m_device->CreateVertexDeclaration(_elems, &m_vertexTypes[VF_Skinned]);
		CHECK(_r >= 0, "Couldn't create vertex declaration");
	}
}
//----------------------------------------------------------------------------//
void Graphics::BeginFrame(void)
{
	m_device->BeginScene();
}
//----------------------------------------------------------------------------//
void Graphics::EndFrame(void)
{
	m_device->EndScene();
	m_device->Present(nullptr, nullptr, nullptr, nullptr);
}
//----------------------------------------------------------------------------//
void Graphics::ClearFrameBuffers(uint _buffers, const Color& _color, float _depth, uint _stencil)
{
	m_device->Clear(0, nullptr, _buffers, _color.Argb(), _depth, _stencil);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
