#pragma once

#include "Math.hpp"
#include "Object.hpp"
#include "Image.hpp"
#include "Shaders/Constants.h"
#include <d3d9.h>

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

#define gGraphics Graphics::Get()
#define gGraphicsDevice gGraphics->Device()

#define MAX_VERTEX_SHADERS 256
#define MAX_PIXEL_SHADERS 256
#define MAX_SAMPLERS 256
#define MAX_TEXTURE_UNITS 8

enum FrameBufferType : uint
{
	FBT_Color = D3DCLEAR_TARGET,
	FBT_Depth = D3DCLEAR_ZBUFFER,
	FBT_Stencil = D3DCLEAR_STENCIL,
	FBT_DepthStencil = FBT_Depth | FBT_Stencil,
	FBT_All = FBT_Color | FBT_DepthStencil,
};

enum IndexFormat : uint8
{
	IF_UShort = D3DFMT_INDEX16,
	IF_UInt = D3DFMT_INDEX32,
};

enum PrimitiveType : uint
{
	PT_Points = 1,
	PT_Lines = 2,
	PT_Triangles = 3,
};

//----------------------------------------------------------------------------//
// Vertex
//----------------------------------------------------------------------------//

enum VertexFormat : uint8
{
	VF_Base,
	VF_Simple,
	VF_Sprite,
	VF_Static,
	VF_Skinned,
	MAX_VERTEX_FORMATS,
};

struct Vertex
{
	Vertex(void) { }
	Vertex(const Vec3& _position) : position(_position) { }
	Vec3 position;
};

struct SimpleVertex : Vertex
{
	SimpleVertex(void) { }
	SimpleVertex(const Vec3& _position, const Vec2& _texcoord, const Color& _color) :
		Vertex(_position), texcoord(_texcoord), color(_color) { }
	Color color;
	Vec2 texcoord;
};

struct SpriteVertex : SimpleVertex
{
	SpriteVertex(void) { }
	SpriteVertex(const Vec3& _position, const Vec2& _size, float _rotation, const Vec2& _texcoord, const Color& _color) :
		SimpleVertex(_position, _texcoord, _color), size(_size), rotation(_rotation) { }

	Vec2 size; // in shader: float4 sprite:TEXCOORD1 {size.x, size.y, rotate}
	float rotation;	// in shader: TEXCOORD1.z
};

struct StaticVertex : Vertex
{
	StaticVertex(void) { }
	StaticVertex(const Vec3& _position, const Vec2& _texcoord, const Color& _normal, const Color& _tangent) :
		Vertex(_position), texcoord(_texcoord), normal(_normal), tangent(_tangent) { }
	Vec2 texcoord;
	Color normal;
	Color tangent;
};

struct SkinnedVertex : StaticVertex
{
	SkinnedVertex(void) { }
	SkinnedVertex(const Vec3& _position, const Vec2& _texcoord, const Color& _normal, const Color& _tangent, const Color& _bones, const Color& _weights) :
		StaticVertex(_position, _texcoord, _normal, _tangent), bones(_bones), weights(_weights) { }
	Color bones;
	Color weights;
};

static const uint VertexSize[] =
{
	sizeof(Vertex),
	sizeof(SimpleVertex),
	sizeof(SpriteVertex),
	sizeof(StaticVertex),
	sizeof(SkinnedVertex),
};

//----------------------------------------------------------------------------//
// Buffer
//----------------------------------------------------------------------------//

enum BufferType : uint16
{
	BT_Vertex,
	BT_Index,
	BT_Constant,
};
enum BufferUsage : uint16
{
	BU_Static = 0,
	BU_Dynamic = D3DUSAGE_DYNAMIC,
};

enum LockMode : uint
{
	LM_ReadOnly = D3DLOCK_READONLY,
	LM_WriteDiscard = D3DLOCK_DISCARD,
	LM_WriteNoOverwrite = D3DLOCK_NOOVERWRITE,
};

class VertexBuffer : public NonCopyable
{
public:

	VertexBuffer(VertexFormat _format, BufferUsage _usage = BU_Static);
	~VertexBuffer(void);

	void Realloc(uint _newSize);
	uint Size(void) { return m_size; }
	uint ElementCount(void) { return m_size / ElementSize(); }
	uint ElementSize(void) { return VertexSize[m_format]; }
	IDirect3DVertexBuffer9* Handle(void) { return m_handle; }
	VertexFormat Format(void) { return m_format; }

	uint8* Lock(LockMode _mode, uint _offset, uint _size);
	void Unlock(void);
	void Write(const void* _src, uint _offset, uint _size);

protected:

	BufferUsage m_usage;
	VertexFormat m_format;
	uint m_size;
	IDirect3DVertexBuffer9*	m_handle;
};

class IndexBuffer : public NonCopyable
{
public:

	IndexBuffer(IndexFormat _format = IF_UShort, BufferUsage _usage = BU_Static);
	~IndexBuffer(void);

	void Realloc(uint _newSize);
	uint Size(void) { return m_size; }
	uint ElementCount(void) { return m_size / ElementSize(); }
	uint ElementSize(void) { return m_format == IF_UShort ? 2 : 4; }
	IDirect3DIndexBuffer9* Handle(void) { return m_handle; }

	uint8* Lock(LockMode _mode, uint _offset, uint _size);
	void Unlock(void);
	void Write(const void* _src, uint _offset, uint _size);

protected:

	BufferUsage m_usage;
	IndexFormat m_format;
	uint m_size;
	IDirect3DIndexBuffer9*	m_handle;
};

//----------------------------------------------------------------------------//
// RenderModel
//----------------------------------------------------------------------------//

typedef Ptr<class RenderModel> RenderModelPtr;

class RenderModel : public RefCounted
{
public:

	RenderModel(VertexFormat _format, PrimitiveType _type, BufferUsage _usage = BU_Static);
	~RenderModel(void);

	PrimitiveType Type(void) { return m_type; }
	VertexFormat Format(void) { return m_vertexBuffer.Format(); }

	void Bind(void);
	void Draw(uint _baseVertex, uint _start, uint _count);

	uint NumVertices(void) { return m_numVertices; }
	void ReallocVertices(uint _count);
	Vertex* LockVertices(LockMode _mode, uint _offset, uint _count, bool _realloc = false, uint _newSize = 0);
	void UnlockVertices(void);
	void SetVertices(const void* _data, uint _count);

	uint NumIndices(void) { return m_numIndices; }
	void ReallocIndices(uint _count);
	uint16* LockIndices(LockMode _mode, uint _offset, uint _count, bool _realloc = false, uint _newSize = 0);
	void UnlockIndices(void);
	void SetIndices(const void* _data, uint _count);

protected:

	PrimitiveType m_type;
	uint m_numVertices;
	uint m_numIndices;
	uint m_vertexSize;
	uint m_indexSize;
	VertexBuffer m_vertexBuffer;
	IndexBuffer m_indexBuffer;
};

//----------------------------------------------------------------------------//
// Texture
//----------------------------------------------------------------------------//

typedef Ptr<class Texture> TexturePtr;

enum TextureType
{
	TT_2D,
	TT_2DMultisample,
	TT_3D,
	TT_Cube,
};

/*enum
{
MAX_TEXTURE_2D_SIZE = 2048,
MAX_TEXTURE_3D_SIZE = 128,
MAX_TEXTURE_CUBE_SIZE = 1024,
}; */

enum TextureUsage : uint
{
	TU_Default = /*D3DUSAGE_AUTOGENMIPMAP*/0,
	TU_Dynamic = /*D3DUSAGE_AUTOGENMIPMAP |*/ D3DUSAGE_DYNAMIC,
	TU_RenderTarget = /*D3DUSAGE_AUTOGENMIPMAP |*/ D3DUSAGE_RENDERTARGET, // use as render-target, no mipmaps
};


class Texture : public RefCounted
{
public:

	Texture(TextureType _type, PixelFormat _format, TextureUsage _usage = TU_Default, bool _useMipMaps = true);
	~Texture(void);

	void SetSize(uint _width, uint _height, uint _depth = 1);
	void SetData(uint _lod, const void* _data);
	void SetImage(Image* _img);
	void UpdateRenderTarget(void);

	IDirect3DBaseTexture9* Handle(void) { return m_handle; }

protected:
	friend class Graphics;

	void _Create(void);
	void _Destroy(void);
	void _Bind(uint _unit);

	TextureType m_type;
	TextureUsage m_usage;
	PixelFormat m_format;
	Vec2i m_size;
	uint m_depth;
	uint m_lods;
	bool m_useMipMaps;
	//uint m_desiredLods;
	union
	{
		IDirect3DBaseTexture9* m_handle;
		IDirect3DTexture9* m_texture2d;
		IDirect3DVolumeTexture9* m_texture3d;
		IDirect3DCubeTexture9* m_textureCube;
	};
};

//----------------------------------------------------------------------------//
// Sampler
//----------------------------------------------------------------------------//

enum TextureWrap : uint
{
	TW_Repeat = 0,
	TW_Mirror,
	TW_Clamp,
};

enum TextureFilter : uint
{
	TF_Default = 0,
	TF_Nearest,
	TF_Linear,
	TF_Bilinear,
	TF_Trilinear,
};

typedef uint SamplerID;

class Sampler : public NonCopyable
{
public:

	struct Desc
	{
		Desc(TextureFilter _filter = TF_Default, TextureWrap _u = TW_Repeat, TextureWrap _v = TW_Repeat, TextureWrap _w = TW_Repeat, uint _aniso = 0) :
			wrapU(_u), wrapV(_v), wrapW(_w), filter(_filter), aniso(_aniso) { }
		TextureWrap wrapU;
		TextureWrap wrapV;
		TextureWrap wrapW;
		TextureFilter filter;
		uint aniso;
	};

protected:
	friend class Graphics;

	Sampler(void);
	Sampler(SamplerID _id, const Desc& _desc, uint _hash);
	~Sampler(void);
	Sampler& operator = (const Sampler& _rhs);

	void _SetDefaultFilter(TextureFilter _filter);
	void _SetDefaultAniso(uint _aniso);
	void _Bind(uint _unit);

	SamplerID m_id;
	Desc m_desc;
	uint m_hash;
	TextureFilter m_filter;
	uint m_dxwrap[3]; // u v w
	uint m_dxfilter[3];	// mag, min, mip
	uint m_aniso;
};

//----------------------------------------------------------------------------//
// Shaders
//----------------------------------------------------------------------------//

enum VertexShaderID
{
	VS_Test,
};

enum PixelShaderID
{
	PS_Test,
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

class Material
{
public:

protected:
};

//----------------------------------------------------------------------------//
// Graphics
//----------------------------------------------------------------------------//

class Graphics : public Singleton<Graphics>
{
public:

	Graphics(void);
	~Graphics(void);

	IDirect3D9* Direct3D(void) { return m_d3d; }
	IDirect3DDevice9* Device(void) { return m_device; }

	TextureFilter GetDefaultTextureFilter(void) { return m_defaultTexFilter; }
	void SetDefaultTextureFilter(TextureFilter _filter);

	uint GetMaxTextureAnisotropy(void) { return m_maxTexAniso; }
	uint GetDefaultTextureAnisotropy(void) { return m_defaultTexAniso; }
	void SetDefaultTextureAnisotropy(uint _value);

	SamplerID AddSampler(const Sampler::Desc& _desc);


	void BeginFrame(void);
	void EndFrame(void);

	void ClearFrameBuffers(uint _buffers, const Color& _color = 0x000000ff, float _depth = 1.0, uint _stencil = 0xff);

	void SetVertexBuffer(VertexBuffer* _buffer);
	VertexBuffer* GetVertexBuffer(void) { return m_vertexBuffer; }

	void SetIndexBuffer(IndexBuffer* _buffer);
	IndexBuffer* GetIndexBuffer(void) { return m_indexBuffer; }

	void SetVertexShader(VertexShaderID _id);
	void SetPixelShader(PixelShaderID _id);

	void SetFloatUniformVS(uint _index, const void* _data, uint _num4);
	void SetFloatUniformPS(uint _index, const void* _data, uint _num4);

	void SetTexture(uint _unit, Texture* _texture);
	void SetSampler(uint _unit, SamplerID _sampler);

	void Draw(PrimitiveType _type, uint _start, uint _count);
	void DrawIndexed(PrimitiveType _type, uint _baseVertex, uint _start, uint _count);

	//void DrawSprites();

protected:

	IDirect3D9* m_d3d;
	IDirect3DDevice9* m_device;
	IDirect3DVertexDeclaration9* m_vertexFormats[MAX_VERTEX_FORMATS];
	IDirect3DVertexShader9*	m_vertexShaders[MAX_VERTEX_SHADERS];
	IDirect3DPixelShader9* m_pixelShaders[MAX_PIXEL_SHADERS];

	VertexFormat m_vertexFormat;
	VertexBuffer* m_vertexBuffer;
	IndexBuffer* m_indexBuffer;

	TextureFilter m_defaultTexFilter;
	uint m_defaultTexAniso;
	uint m_maxTexAniso;
	Sampler m_samplers[MAX_SAMPLERS];
	uint m_numSamplers;

	struct TextureUnit
	{
		Texture* texture = nullptr;
		SamplerID sampler = 0;
	};

	TextureUnit m_texUnit[MAX_TEXTURE_UNITS];
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//


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
	{ 0, offsetof(SpriteVertex, size), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
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
			gGraphicsDevice->SetStreamSource(0, m_handle, 0, VertexSize[m_format]);
	}
}
//----------------------------------------------------------------------------//
uint8* VertexBuffer::Lock(LockMode _mode, uint _offset, uint _size)
{
	ASSERT(m_handle != nullptr);
	ASSERT(_offset + _size <= m_size);
	void* _dst = nullptr;
	m_handle->Lock(_offset, _size, &_dst, _mode);
	return (uint8*)_dst;
}
//----------------------------------------------------------------------------//
void VertexBuffer::Unlock(void)
{
	m_handle->Unlock();
}
//----------------------------------------------------------------------------//
void VertexBuffer::Write(const void* _src, uint _offset, uint _size)
{
	memcpy(Lock(LM_WriteDiscard, _offset, _size), _src, _size);
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
uint8* IndexBuffer::Lock(LockMode _mode, uint _offset, uint _size)
{
	ASSERT(m_handle != nullptr);
	ASSERT(_offset + _size <= m_size);
	void* _dst = nullptr;
	m_handle->Lock(_offset, _size, &_dst, _mode);
	return (uint8*)_dst;
}
//----------------------------------------------------------------------------//
void IndexBuffer::Unlock(void)
{
	m_handle->Unlock();
}
//----------------------------------------------------------------------------//
void IndexBuffer::Write(const void* _src, uint _offset, uint _size)
{
	memcpy(Lock(LM_WriteDiscard, _offset, _size), _src, _size);
	m_handle->Unlock();
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// RenderModel
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
RenderModel::RenderModel(VertexFormat _format, PrimitiveType _type, BufferUsage _usage) :
	m_type(_type),
	m_numVertices(0),
	m_numIndices(0),
	m_vertexBuffer(_format, (BufferUsage)_usage),
	m_indexBuffer(IF_UShort, (BufferUsage)_usage)
{
	m_vertexSize = m_vertexBuffer.ElementSize();
	m_indexSize = m_indexBuffer.ElementSize();
}
//----------------------------------------------------------------------------//
RenderModel::~RenderModel(void)
{
}
//----------------------------------------------------------------------------//
void RenderModel::Bind(void)
{
	gGraphics->SetVertexBuffer(&m_vertexBuffer);
	gGraphics->SetIndexBuffer(&m_indexBuffer);
}
//----------------------------------------------------------------------------//
void RenderModel::Draw(uint _baseVertex, uint _start, uint _count)
{
	if (m_numIndices)
		gGraphics->DrawIndexed(m_type, _baseVertex, _start, _count);
	else
		gGraphics->Draw(m_type, _baseVertex + _start, _count);
}
//----------------------------------------------------------------------------//
void RenderModel::ReallocVertices(uint _count)
{
	if (m_vertexBuffer.ElementCount() < _count)
	{
		m_vertexBuffer.Realloc(_count * m_vertexSize);
		m_numVertices = _count;
	}
}
//----------------------------------------------------------------------------//
Vertex* RenderModel::LockVertices(LockMode _mode, uint _offset, uint _count, bool _realloc, uint _newSize)
{
	if (_realloc)
		ReallocVertices(_newSize);
	return (Vertex*)m_vertexBuffer.Lock(_mode, _offset * m_vertexSize, _count * m_vertexSize);
}
//----------------------------------------------------------------------------//
void RenderModel::UnlockVertices(void)
{
	m_vertexBuffer.Unlock();
}
//----------------------------------------------------------------------------//
void RenderModel::SetVertices(const void* _data, uint _count)
{
	ReallocVertices(_count);
	m_vertexBuffer.Write(_data, 0, _count * m_vertexSize);
}
//----------------------------------------------------------------------------//
void RenderModel::ReallocIndices(uint _count)
{
	if (m_indexBuffer.ElementCount() < _count)
	{
		m_indexBuffer.Realloc(_count * m_indexSize);
		m_numIndices = _count;
	}
}
//----------------------------------------------------------------------------//
uint16* RenderModel::LockIndices(LockMode _mode, uint _offset, uint _count, bool _realloc, uint _newSize)
{
	if (_realloc)
		ReallocIndices(_newSize);
	return (uint16*)m_indexBuffer.Lock(_mode, _offset * m_indexSize, _count * m_indexSize);
}
//----------------------------------------------------------------------------//
void RenderModel::UnlockIndices(void)
{
	m_indexBuffer.Unlock();
}
//----------------------------------------------------------------------------//
void RenderModel::SetIndices(const void* _data, uint _count)
{
	ReallocVertices(_count);
	m_indexBuffer.Write(_data, 0, _count * m_indexSize);
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
Sampler::Sampler(SamplerID _id, const Desc& _desc, uint _hash) :
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

D3DPRIMITIVETYPE DXPrimitiveType[] =
{
	D3DPT_FORCE_DWORD,
	D3DPT_POINTLIST, // PT_Points = 1,
	D3DPT_LINELIST, // PT_Lines = 2,
	D3DPT_TRIANGLELIST, // PT_Triangles = 3,
};

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

	// geometry source
	{
		m_vertexFormat = VF_Base;
		m_vertexBuffer = nullptr;
		m_indexBuffer = nullptr;

		m_device->SetVertexDeclaration(m_vertexFormats[m_vertexFormat]);
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
void Graphics::SetVertexBuffer(VertexBuffer* _buffer)
{
	ASSERT(_buffer != nullptr);

	if (m_vertexBuffer != _buffer)
	{
		if (m_vertexFormat != _buffer->Format())
		{
			m_vertexFormat = _buffer->Format();
			m_device->SetVertexDeclaration(m_vertexFormats[m_vertexFormat]);
		}
		m_vertexBuffer = _buffer;
		m_device->SetStreamSource(0, _buffer->Handle(), 0, _buffer->ElementSize());
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
void Graphics::Draw(PrimitiveType _type, uint _start, uint _count)
{
	ASSERT(m_vertexBuffer != nullptr);
	ASSERT(_start + _count <= m_vertexBuffer->ElementCount());

	m_device->DrawPrimitive(DXPrimitiveType[_type], _start, _count / _type);
}
//----------------------------------------------------------------------------//
void Graphics::DrawIndexed(PrimitiveType _type, uint _baseVertex, uint _start, uint _count)
{
	ASSERT(m_vertexBuffer != nullptr);
	ASSERT(_baseVertex <= m_vertexBuffer->ElementCount());
	ASSERT(m_indexBuffer != nullptr);
	ASSERT(_start + _count <= m_indexBuffer->ElementCount());

	m_device->DrawIndexedPrimitive(DXPrimitiveType[_type], _baseVertex, 0, m_vertexBuffer->ElementCount() - _baseVertex, _start, _count / _type);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
