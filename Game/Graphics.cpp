#pragma once

#include "Graphics.hpp"
#include "Window.hpp"
#include "File.hpp"

//----------------------------------------------------------------------------//
// Vertex
//----------------------------------------------------------------------------//

static const D3DVERTEXELEMENT9 VF_Base_Desc[] =
{
	{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	D3DDECL_END(),
};

static const D3DVERTEXELEMENT9 VF_Simple_Desc[] =
{
	{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, offsetof(SimpleVertex, texcoord), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	{ 0, offsetof(SimpleVertex, color), D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
	D3DDECL_END(),
};

static const D3DVERTEXELEMENT9 VF_Static_Desc[] =
{
	{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, offsetof(StaticVertex, texcoord), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	{ 0, offsetof(StaticVertex, normal), D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
	{ 0, offsetof(StaticVertex, tangent), D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
	D3DDECL_END(),
};

static const D3DVERTEXELEMENT9 VF_Skinned_Desc[] =
{
	{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, offsetof(SkinnedVertex, texcoord), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	{ 0, offsetof(SkinnedVertex, normal), D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
	{ 0, offsetof(SkinnedVertex, tangent), D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
	{ 0, offsetof(SkinnedVertex, bones), D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
	{ 0, offsetof(SkinnedVertex, weights), D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
	D3DDECL_END(),
};

//----------------------------------------------------------------------------//
// VertexBuffer
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
VertexBuffer::VertexBuffer(uint _size, BufferUsage _usage) :
	m_size(_size),
	m_handle(nullptr)
{
	HRESULT _r = gGraphicsDevice->CreateVertexBuffer(_size, _usage, 0, _usage == BU_Static ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT, &m_handle, nullptr);
	CHECK(_r >= 0, "Couldn't create vertex buffer");
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
// IndexBuffer
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
IndexBuffer::IndexBuffer(uint _size, IndexFormat _format, BufferUsage _usage) :
	m_size(_size),
	m_handle(nullptr)
{
	HRESULT _r = gGraphicsDevice->CreateIndexBuffer(_size, _usage, (D3DFORMAT)_format, _usage == BU_Static ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT, &m_handle, nullptr);
	CHECK(_r >= 0, "Couldn't create index buffer");
}
//----------------------------------------------------------------------------//
IndexBuffer::~IndexBuffer(void)
{
	m_handle->Release();
}
//----------------------------------------------------------------------------//
void IndexBuffer::Write(const void* _data, uint _offset, uint _size)
{
	void* _dst = nullptr;
	m_handle->Lock(_offset, _size, &_dst, D3DLOCK_DISCARD);
	memcpy(_dst, _data, _size);
	m_handle->Unlock();
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Texture
//----------------------------------------------------------------------------//

const D3DFORMAT DXPixelFormat[] = 
{
	D3DFMT_X8R8G8B8, // PF_RGB8
	D3DFMT_A8R8G8B8, // PF_RGBA8
	D3DFMT_D24S8, // PF_D24S8
	D3DFMT_DXT1, // PF_DXT1
	D3DFMT_DXT5, // PF_DXT5
};

//----------------------------------------------------------------------------//
Texture::Texture(TextureType _type, PixelFormat _format, TextureUsage _usage) :
	m_type(_type),
	m_usage(_usage),
	m_format(_format),
	m_size(0),
	m_depth(0),
	m_lods(0),
	m_desiredLods(0),
	m_handle(nullptr)
{
}
//----------------------------------------------------------------------------//
Texture::~Texture(void)
{
	_Destroy();
}
//----------------------------------------------------------------------------//
void Texture::SetSize(uint _width, uint _height, uint _depth, uint _lods)
{
	m_size.Set(_width, _height);
	m_depth = _depth;
	m_desiredLods = _lods;
	_Create();
}
//----------------------------------------------------------------------------//
void Texture::SetData(const void* _data)
{
	ASSERT(m_handle != nullptr);
	ASSERT(_data != nullptr);

	uint _bpp = BitsPerPixel(m_format);

	if (m_type == TT_2D || m_type == TT_2DMultisample)
	{
		D3DLOCKED_RECT _dst;
		RECT _rect;
		_rect.left = 0;
		_rect.right = m_size.x;
		_rect.top = 0;
		_rect.bottom = m_size.y;
			
		m_texture2d->LockRect(0, &_dst, &_rect, D3DLOCK_DISCARD);

		if (m_format == PF_RGBA8)
			Image::RgbaToArgb(_dst.pBits, _data, m_size.x * m_size.y);
		else if (m_format == PF_RGB8)
			Image::RgbToXrgb(_dst.pBits, _data, m_size.x * m_size.y);
		else
			memcpy(_dst.pBits, _data, (m_size.x * m_size.y * _bpp) >> 3);

		m_texture2d->UnlockRect(0);
	}
}
//----------------------------------------------------------------------------//
void Texture::GenerateLods(void)
{
	m_handle->GenerateMipSubLevels();
}
//----------------------------------------------------------------------------//
void Texture::_Create(void)
{
	_Destroy();

	D3DFORMAT _format = DXPixelFormat[m_format];
	D3DPOOL _pool = m_usage == TU_Default ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT;
	uint _usage = m_usage;
	if (m_format == PF_D24S8)
		_usage |= D3DUSAGE_DEPTHSTENCIL;

	uint _lods = 1;
	for (int i = FirstPow2(Max(m_size.x, m_size.y)); i > 1; i >>= 1)
		++_lods;
	if (m_desiredLods > 0 && _lods > m_desiredLods)
		_lods = m_desiredLods;
	m_lods = _lods;

	switch (m_type)
	{
	case TT_2D:
	case TT_2DMultisample:
		gGraphicsDevice->CreateTexture(m_size.x, m_size.y, m_lods, _usage, _format, _pool, &m_texture2d, nullptr);
		break;

	default:
		break;
	}

	if (!m_handle)
		Fatal("Couldn't create texture");
}
//----------------------------------------------------------------------------//
void Texture::_Destroy(void)
{
	if (m_handle)
		m_handle->Release();
	m_handle = nullptr;
	//if (m_target)
	//	m_target->Release();
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Graphics
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Graphics::Graphics(void)
{
	LOG("Create Graphics");

#ifdef _DEBUG
	memset(m_vertexFormats, 0, sizeof(m_vertexFormats));
	memset(m_vertexShaders, 0, sizeof(m_vertexShaders));
	memset(m_pixelShaders, 0, sizeof(m_pixelShaders));
#endif

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

		_r = m_device->CreateVertexDeclaration(VF_Base_Desc, &m_vertexFormats[VF_Base]);
		CHECK(_r >= 0, "Couldn't create vertex declaration");
		_r = m_device->CreateVertexDeclaration(VF_Simple_Desc, &m_vertexFormats[VF_Simple]);
		CHECK(_r >= 0, "Couldn't create vertex declaration");
		_r = m_device->CreateVertexDeclaration(VF_Static_Desc, &m_vertexFormats[VF_Static]);
		CHECK(_r >= 0, "Couldn't create vertex declaration");
		_r = m_device->CreateVertexDeclaration(VF_Skinned_Desc, &m_vertexFormats[VF_Skinned]);
		CHECK(_r >= 0, "Couldn't create vertex declaration");
	}
}
//----------------------------------------------------------------------------//
Graphics::~Graphics(void)
{
	LOG("Destroy Graphics");
}
//----------------------------------------------------------------------------//
void Graphics::LoadVertexShaders(const char** _assets)
{
	ASSERT(m_vertexShaders[0] == nullptr);

	for (uint i = 0; *_assets; ++i)
	{
		ASSERT(i < MAX_VERTEX_SHADERS);

		const char* _name = *_assets++;
		LOG("Load VertexShader \"%s\"", _name);

		RawData _bytecode = LoadFile(_name);
		HRESULT _r = gGraphicsDevice->CreateVertexShader((const DWORD*)*_bytecode, m_vertexShaders + i);
		CHECK(_r >= 0, "Couldn't create VertexShader");
	}
}
//----------------------------------------------------------------------------//
void Graphics::LoadPixelShaders(const char** _assets)
{
	ASSERT(m_pixelShaders[0] == nullptr);

	for (uint i = 0; *_assets; ++i)
	{
		ASSERT(i < MAX_PIXEL_SHADERS);

		const char* _name = *_assets++;
		LOG("Load PixelShader \"%s\"", _name);

		RawData _bytecode = LoadFile(_name);
		HRESULT _r = gGraphicsDevice->CreatePixelShader((const DWORD*)*_bytecode, m_pixelShaders + i);
		CHECK(_r >= 0, "Couldn't create PixelShader");
	}
}
//----------------------------------------------------------------------------//
void Graphics::BeginFrame(void)
{
	m_device->BeginScene();

	// reset state

	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW); // ccw == front face

	// ...
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
void Graphics::SetVertexFormat(VertexFormat _format)
{
	m_device->SetVertexDeclaration(m_vertexFormats[_format]);
}
//----------------------------------------------------------------------------//
void Graphics::SetVertexBuffer(VertexBuffer* _buffer, uint _offset, uint _stride)
{
	ASSERT(_buffer != nullptr);
	ASSERT(_offset < _buffer->Size());

	m_device->SetStreamSource( 0, _buffer->Handle(), _offset, _stride);
}
//----------------------------------------------------------------------------//
void Graphics::SetVertexShader(uint _id)
{
	ASSERT(_id < MAX_VERTEX_SHADERS);
	ASSERT(m_vertexShaders[_id] != nullptr);

	m_device->SetVertexShader(m_vertexShaders[_id]);
}
//----------------------------------------------------------------------------//
void Graphics::SetPixelShader(uint _id)
{
	ASSERT(_id < MAX_PIXEL_SHADERS);
	ASSERT(m_pixelShaders[_id] != nullptr);

	m_device->SetPixelShader(m_pixelShaders[_id]);
}
//----------------------------------------------------------------------------//
void Graphics::SetTexture(uint _stage, Texture* _tex)
{
	m_device->SetTexture(_stage, _tex ? _tex->Handle() : nullptr);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
