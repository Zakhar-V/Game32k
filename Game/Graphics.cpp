#pragma once

#include "Graphics.hpp"
#include "Device.hpp"
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
static const D3DVERTEXELEMENT9 VF_Sprite_Desc[] =
{
	{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, offsetof(SpriteVertex, color), D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
	{ 0, offsetof(SpriteVertex, texcoord), D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	//{ 0, offsetof(SpriteVertex, texscale), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
	{ 0, offsetof(SpriteVertex, size), D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
	//{ 0, offsetof(SpriteVertex, rotation), D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
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
VertexBuffer::VertexBuffer(VertexFormat _format, BufferUsage _usage) :
	m_usage(_usage),
	m_format(_format),
	m_size(0),
	m_handle(nullptr)
{
}
//----------------------------------------------------------------------------//
VertexBuffer::~VertexBuffer(void)
{
	if (m_handle)
		m_handle->Release();
}
//----------------------------------------------------------------------------//
void VertexBuffer::Realloc(uint _newSize)
{
	if (_newSize != m_size)
	{
		if (m_handle)
		{
			m_handle->Release();
			m_handle = nullptr;
		}
		m_size = _newSize;
		gGraphicsDevice->CreateVertexBuffer(_newSize, m_usage, 0, m_usage == BU_Static ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT, &m_handle, nullptr);
		CHECK(m_handle != nullptr, "Couldn't create vertex buffer");
		
		if (gGraphics->GetVertexBuffer() == this)
			gGraphicsDevice->SetStreamSource(0, m_handle, gGraphics->GetVertexBufferOffset(), VertexSize[m_format]);
	}
}
//----------------------------------------------------------------------------//
void VertexBuffer::Write(const void* _data, uint _offset, uint _size)
{
	ASSERT(m_handle != nullptr);
	ASSERT(_offset + _size <= m_size);
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
IndexBuffer::IndexBuffer(IndexFormat _format, BufferUsage _usage) :
	m_usage(_usage),
	m_format(_format),
	m_size(0),
	m_handle(nullptr)
{
}
//----------------------------------------------------------------------------//
IndexBuffer::~IndexBuffer(void)
{
	m_handle->Release();
}
//----------------------------------------------------------------------------//
void IndexBuffer::Realloc(uint _newSize)
{
	if (_newSize != m_size)
	{
		if (m_handle)
		{
			m_handle->Release();
			m_handle = nullptr;
		}
		m_size = _newSize;
		gGraphicsDevice->CreateIndexBuffer(m_size, m_usage, (D3DFORMAT)m_format, m_usage == BU_Static ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT, &m_handle, nullptr);
		CHECK(m_handle != nullptr, "Couldn't create index buffer");

		if (gGraphics->GetIndexBuffer() == this)
			gGraphicsDevice->SetIndices(m_handle);
	}
}
//----------------------------------------------------------------------------//
void IndexBuffer::Write(const void* _data, uint _offset, uint _size)
{
	ASSERT(m_handle != nullptr);
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
Texture::Texture(TextureType _type, PixelFormat _format, TextureUsage _usage, bool _useMipMaps) :
	m_type(_type),
	m_usage(_usage),
	m_format(_format),
	m_size(0),
	m_depth(0),
	m_lods(1),
	m_useMipMaps(_useMipMaps),
	m_handle(nullptr)
{
	if (m_usage == TU_RenderTarget)
		m_useMipMaps = false;
}
//----------------------------------------------------------------------------//
Texture::~Texture(void)
{
	_Destroy();
}
//----------------------------------------------------------------------------//
void Texture::SetSize(uint _width, uint _height, uint _depth)
{
	m_size.Set(_width, _height);
	m_depth = _depth;
	uint _size = Max(_width, _height);
	m_lods = (m_useMipMaps && _size) ? Log2i(_size) + 1 : 1;
	_Create();
}
//----------------------------------------------------------------------------//
void Texture::SetData(uint _lod, const void* _data)
{
	ASSERT(m_handle != nullptr);
	ASSERT(_data != nullptr);

	if (_lod >= m_lods)
		return;

	uint _bpp = BitsPerPixel(m_format);

	if (m_type == TT_2D || m_type == TT_2DMultisample)
	{
		D3DLOCKED_RECT _dst;
		RECT _rect;
		_rect.left = 0;
		_rect.right = m_size.x;
		_rect.top = 0;
		_rect.bottom = m_size.y;

		for (uint i = _lod; i--;)
		{
			if (_rect.right > 1)
				_rect.right >>= 1;
			if (_rect.bottom > 1)
				_rect.bottom >>= 1;
		}
			
		m_texture2d->LockRect(_lod, &_dst, &_rect, D3DLOCK_DISCARD);

		ASSERT(_dst.pBits != nullptr);

		if (m_format == PF_RGBA8 || m_format == PF_RGBX8)
			RgbaToArgb(_dst.pBits, _data, _rect.right * _rect.bottom);
		//else // compressed
		//	memcpy(_dst.pBits, _data, (m_size.x * m_size.y * _bpp) >> 3);

		m_texture2d->UnlockRect(_lod);
	}
}
//----------------------------------------------------------------------------//
void Texture::SetImage(Image* _img)
{
	ASSERT(_img != nullptr);
	if (m_usage == TU_RenderTarget)
		return;
	SetSize(_img->Width(), _img->Height());
	ImagePtr _lod = _img;
	for (uint i = 0; i < m_lods; ++i)
	{
		SetData(i, _lod->RawData());
		_lod = _lod->CreateLod();
	}
}
//----------------------------------------------------------------------------//
void Texture::UpdateRenderTarget(void)
{
	//m_handle->GenerateMipSubLevels();
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

#if _DEBUG
	LOG("Texture %dx%dx%d, %d lods", m_size.x, m_size.y, m_depth, m_handle->GetLevelCount());
#endif
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
void Texture::_Bind(uint _unit)
{
	//m_handle->SetLOD(2);
	gGraphicsDevice->SetTexture(_unit, m_handle);

	// lod bias, ...
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Sampler
//----------------------------------------------------------------------------//

const D3DTEXTUREADDRESS DXAddressing[] =
{
	D3DTADDRESS_WRAP, // TW_Repeat
	D3DTADDRESS_MIRROR, // TW_Mirror
	D3DTADDRESS_CLAMP, // TW_Clamp
};

const D3DTEXTUREFILTERTYPE DXMagFilter[] =
{
	D3DTEXF_ANISOTROPIC, // TF_Default
	D3DTEXF_POINT, // TF_Nearest
	D3DTEXF_LINEAR, // TF_Linear
	D3DTEXF_ANISOTROPIC, // TF_Bilinear
	D3DTEXF_ANISOTROPIC, // TF_Trilinear
};

const D3DTEXTUREFILTERTYPE DXMinFilter[] =
{
	D3DTEXF_ANISOTROPIC, // TF_Default
	D3DTEXF_POINT, // TF_Nearest
	D3DTEXF_LINEAR, // TF_Linear
	D3DTEXF_ANISOTROPIC, // TF_Bilinear
	D3DTEXF_ANISOTROPIC, // TF_Trilinear
};

const uint DXMipFilter[] =
{
	D3DTEXF_LINEAR, // TF_Default
	D3DTEXF_NONE, // TF_Nearest
	D3DTEXF_NONE, // TF_Linear
	D3DTEXF_POINT, // TF_Bilinear
	D3DTEXF_LINEAR, // TF_Trilinear
};

//----------------------------------------------------------------------------//
Sampler::Sampler(void)
{
}
//----------------------------------------------------------------------------//
Sampler::Sampler(SamplerID _id, const Desc& _desc, uint _hash)	:
	m_id(_id),
	m_desc(_desc),
	m_hash(_hash)
{
	m_filter = _desc.filter != TF_Default ? _desc.filter : gGraphics->GetDefaultTextureFilter();
	m_dxwrap[0] = DXAddressing[_desc.wrapU];
	m_dxwrap[1] = DXAddressing[_desc.wrapV];
	m_dxwrap[2] = DXAddressing[_desc.wrapW];
	m_dxfilter[0] = DXMagFilter[m_filter];
	m_dxfilter[1] = DXMinFilter[m_filter];
	m_dxfilter[2] = DXMipFilter[m_filter];
	m_aniso = _desc.aniso ? _desc.aniso : gGraphics->GetDefaultTextureAnisotropy();
	if (m_aniso > gGraphics->GetMaxTextureAnisotropy())
		m_aniso = gGraphics->GetMaxTextureAnisotropy();
}
//----------------------------------------------------------------------------//
Sampler::~Sampler(void)
{
}
//----------------------------------------------------------------------------//
Sampler& Sampler::operator = (const Sampler& _rhs)
{
	memcpy(this, &_rhs, sizeof(Sampler));
	return *this;
}
//----------------------------------------------------------------------------//
void Sampler::_SetDefaultFilter(TextureFilter _filter)
{
	if (m_desc.filter == TF_Default)
	{
		m_filter = _filter;
		m_dxfilter[0] = DXMagFilter[_filter];
		m_dxfilter[1] = DXMinFilter[_filter];
		m_dxfilter[2] = DXMipFilter[_filter];
	}
}
//----------------------------------------------------------------------------//
void Sampler::_SetDefaultAniso(uint _aniso)
{
	if (m_desc.aniso == 0)
	{
		m_aniso = _aniso;
	}
}
//----------------------------------------------------------------------------//
void Sampler::_Bind(uint _unit)
{
	gGraphicsDevice->SetSamplerState(_unit, D3DSAMP_ADDRESSU, m_dxwrap[0]);
	gGraphicsDevice->SetSamplerState(_unit, D3DSAMP_ADDRESSV, m_dxwrap[1]);
	gGraphicsDevice->SetSamplerState(_unit, D3DSAMP_ADDRESSW, m_dxwrap[2]);
	gGraphicsDevice->SetSamplerState(_unit, D3DSAMP_MAGFILTER, m_dxfilter[0]);
	gGraphicsDevice->SetSamplerState(_unit, D3DSAMP_MINFILTER, m_dxfilter[1]);
	gGraphicsDevice->SetSamplerState(_unit, D3DSAMP_MIPFILTER, m_dxfilter[2]);
	gGraphicsDevice->SetSamplerState(_unit, D3DSAMP_MAXANISOTROPY, m_aniso);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Shaders
//----------------------------------------------------------------------------//

const char* g_vertexShaderNames[] =
{
	"Test-VS.cso", // VS_Test
	nullptr,
};

const char* g_pixelShaderNames[] =
{
	"Test-PS.cso", // PS_Test
	nullptr,
};

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

	ASSERT(gDevice != nullptr); // the window must be created before it

	m_d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!m_d3d)
		Fatal("Couldn't create IDirect3D9");

	D3DPRESENT_PARAMETERS _pp; 
	memset(&_pp, 0, sizeof(_pp));
	_pp.Windowed = TRUE;
	_pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	_pp.BackBufferFormat = D3DFMT_UNKNOWN;

	m_device = 0;
	m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, gDevice->WindowHandle(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &_pp, &m_device);
	_CHECK(m_device, "Couldn't create IDirect3D9Device");

	// vertex formats
	{	 
		DLOG("Create vertex declarations");

		m_device->CreateVertexDeclaration(VF_Base_Desc, &m_vertexFormats[VF_Base]);
		CHECK(m_vertexFormats[VF_Base], "Couldn't create vertex declaration");
		m_device->CreateVertexDeclaration(VF_Simple_Desc, &m_vertexFormats[VF_Simple]);
		CHECK(m_vertexFormats[VF_Simple], "Couldn't create vertex declaration");
		m_device->CreateVertexDeclaration(VF_Sprite_Desc, &m_vertexFormats[VF_Sprite]);
		CHECK(m_vertexFormats[VF_Sprite], "Couldn't create vertex declaration");
		m_device->CreateVertexDeclaration(VF_Static_Desc, &m_vertexFormats[VF_Static]);
		CHECK(m_vertexFormats[VF_Static], "Couldn't create vertex declaration");
		m_device->CreateVertexDeclaration(VF_Skinned_Desc, &m_vertexFormats[VF_Skinned]);
		CHECK(m_vertexFormats[VF_Skinned], "Couldn't create vertex declaration");
	}

	// vertex source
	{
		m_vertexFormat = VF_Base;
		m_vertexBuffer = nullptr;
		m_vertexBufferOffset = 0;

		m_device->SetVertexDeclaration(m_vertexFormats[m_vertexFormat]);
	}

	// index source
	{
		m_indexBuffer = nullptr;
	}

	// vertex shaders
	for (uint i = 0; g_vertexShaderNames[i]; ++i)
	{
		ASSERT(i < MAX_VERTEX_SHADERS);

		const char* _name = g_vertexShaderNames[i];
		LOG("Load VertexShader \"%s\"", _name);

		RawData _bytecode = LoadFile(_name);
		HRESULT _r = gGraphicsDevice->CreateVertexShader((const DWORD*)*_bytecode, m_vertexShaders + i);
		CHECK(_r >= 0, "Couldn't create VertexShader");
	}

	// pixel shaders
	for (uint i = 0; g_pixelShaderNames[i]; ++i)
	{
		ASSERT(i < MAX_PIXEL_SHADERS);

		const char* _name = g_pixelShaderNames[i];
		LOG("Load PixelShader \"%s\"", _name);

		RawData _bytecode = LoadFile(_name);
		HRESULT _r = gGraphicsDevice->CreatePixelShader((const DWORD*)*_bytecode, m_pixelShaders + i);
		CHECK(_r >= 0, "Couldn't create PixelShader");
	}

	// samplers
	{
		m_maxTexAniso = 4; // todo:
		m_defaultTexAniso = m_maxTexAniso;
		m_defaultTexFilter = TF_Trilinear;
		m_numSamplers = 0;
		AddSampler(TF_Default);
	}
}
//----------------------------------------------------------------------------//
Graphics::~Graphics(void)
{
	LOG("Destroy Graphics");
}
//----------------------------------------------------------------------------//
void Graphics::SetDefaultTextureFilter(TextureFilter _filter)
{
	if (_filter == TF_Default)
		_filter = TF_Trilinear;
	if (_filter != m_defaultTexFilter)
	{
		m_defaultTexFilter = _filter;
		for (uint i = 0; i < m_numSamplers; ++i)
			m_samplers[i]._SetDefaultFilter(_filter);
	}
}
//----------------------------------------------------------------------------//
void Graphics::SetDefaultTextureAnisotropy(uint _value)
{
	if (_value == 0 || _value > m_maxTexAniso)
		_value = m_maxTexAniso;
	if (_value != m_defaultTexAniso)
	{
		m_defaultTexAniso = _value;
		for (uint i = 0; i < m_numSamplers; ++i)
			m_samplers[i]._SetDefaultAniso(_value);
	}
}
//----------------------------------------------------------------------------//
SamplerID Graphics::AddSampler(const Sampler::Desc& _desc)
{
	uint _hash = Hash(&_desc, sizeof(_desc));
	for (uint i = 0; i < m_numSamplers; ++i)
	{
		if (m_samplers[i].m_hash == _hash)
			return i;
	}
	SamplerID _id = m_numSamplers++;
	m_samplers[_id] = Sampler(_id, _desc, _hash);
	return _id;
}
//----------------------------------------------------------------------------//
void Graphics::BeginFrame(void)
{
	m_device->BeginScene();

	// reset state

	//m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW); // ccw == front face
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // ccw == front face

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
void Graphics::SetVertexBuffer(VertexBuffer* _buffer, uint _offset)
{
	ASSERT(_buffer != nullptr);
	ASSERT(_offset < _buffer->Size());

	if (m_vertexBuffer != _buffer || m_vertexBufferOffset != _offset)
	{
		if (m_vertexFormat != _buffer->Format())
		{
			m_vertexFormat = _buffer->Format();
			m_device->SetVertexDeclaration(m_vertexFormats[m_vertexFormat]);
		}
		m_vertexBuffer = _buffer;
		m_vertexBufferOffset = _offset;
		m_device->SetStreamSource(0, _buffer->Handle(), _offset, _buffer->Stride());
	}
}
//----------------------------------------------------------------------------//
void Graphics::SetIndexBuffer(IndexBuffer* _buffer)
{
	ASSERT(_buffer != nullptr);

	if (m_indexBuffer != _buffer)
	{
		m_indexBuffer = _buffer;
		m_device->SetIndices(m_indexBuffer->Handle());
	}
}
//----------------------------------------------------------------------------//
void Graphics::SetVertexShader(VertexShaderID _id)
{
	ASSERT(_id < MAX_VERTEX_SHADERS);
	ASSERT(m_vertexShaders[_id] != nullptr);

	m_device->SetVertexShader(m_vertexShaders[_id]);
}
//----------------------------------------------------------------------------//
void Graphics::SetPixelShader(PixelShaderID _id)
{
	ASSERT(_id < MAX_PIXEL_SHADERS);
	ASSERT(m_pixelShaders[_id] != nullptr);

	m_device->SetPixelShader(m_pixelShaders[_id]);
}
//----------------------------------------------------------------------------//
void Graphics::SetFloatUniformVS(uint _index, const void* _data, uint _num4)
{
	m_device->SetVertexShaderConstantF(_index, (const float*)_data, _num4);
}
//----------------------------------------------------------------------------//
void Graphics::SetFloatUniformPS(uint _index, const void* _data, uint _num4)
{
	m_device->SetPixelShaderConstantF(_index, (const float*)_data, _num4);
}
//----------------------------------------------------------------------------//
void Graphics::SetTexture(uint _unit, Texture* _texture)
{
	ASSERT(_unit < MAX_TEXTURE_UNITS);
	TextureUnit& _tu = m_texUnit[_unit];
	if (_tu.texture != _texture)
	{
		_tu.texture = _texture;
		if (_texture)
			_texture->_Bind(_unit);
		else
			m_device->SetTexture(_unit, nullptr);
	}
}
//----------------------------------------------------------------------------//
void Graphics::SetSampler(uint _unit, SamplerID _sampler)
{
	ASSERT(_unit < MAX_TEXTURE_UNITS);
	TextureUnit& _tu = m_texUnit[_unit];
	if (_tu.sampler != _sampler)
	{
		_tu.sampler = _sampler;
		m_samplers[_sampler]._Bind(_unit);
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
