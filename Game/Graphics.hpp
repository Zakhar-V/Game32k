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
	Vertex(const Vec3& _pos) : pos(_pos) { }
	Vec3 pos;
};

struct SimpleVertex : Vertex
{
	SimpleVertex(void) { }
	SimpleVertex(const Vec3& _pos, const Vec2& _texcoord, const Color& _color) :
		Vertex(_pos), texcoord(_texcoord), color(_color) { }
	Color color;
	Vec2 texcoord;
};

struct SpriteVertex : SimpleVertex
{
	SpriteVertex(void) { }
	SpriteVertex(const Vec3& _pos, const Vec2& _size, float _scale, float _rotation, const Vec2& _texcoord, const Vec2& _texcoord2, const Color& _color) :
		SimpleVertex(_pos, _texcoord, _color), size(_size), scale(_scale), rotation(_rotation), texcoord2(_texcoord2) { }
	
	Vec2 texcoord2; // in shader: TEXCOORD0.zw (texture scaling for point-sprites)
	Vec2 size; // in shader: float4 sprite:TEXCOORD1 {size.x, size.y, scale, rotate}
	float scale; // in shader: TEXCOORD1.z
	float rotation;	// in shader: TEXCOORD1.w
};

struct StaticVertex : Vertex
{
	StaticVertex(void) { }
	StaticVertex(const Vec3& _pos, const Vec2& _texcoord, const Color& _normal, const Color& _tangent) :
		Vertex(_pos), texcoord(_texcoord), normal(_normal), tangent(_tangent) { }
	Vec2 texcoord;
	Color normal;
	Color tangent;
};

struct SkinnedVertex : StaticVertex
{
	SkinnedVertex(void) { }
	SkinnedVertex(const Vec3& _pos, const Vec2& _texcoord, const Color& _normal, const Color& _tangent, const Color& _bones, const Color& _weights) :
		StaticVertex(_pos, _texcoord, _normal,_tangent), bones(_bones), weights(_weights) { }
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

typedef Ptr<class VertexBuffer> VertexBufferPtr;

class VertexBuffer : public RefCounted
{
public:

	VertexBuffer(VertexFormat _format, BufferUsage _usage = BU_Static);
	~VertexBuffer(void);

	void Realloc(uint _newSize);
	uint Size(void) { return m_size; }
	IDirect3DVertexBuffer9* Handle(void) { return m_handle; }
	VertexFormat Format(void) { return m_format; }
	uint Stride(void) { return VertexSize[m_format]; }

	void Write(const void* _data, uint _offset, uint _size);

protected:

	BufferUsage m_usage;
	VertexFormat m_format;
	uint m_size;
	IDirect3DVertexBuffer9*	m_handle;
};

typedef Ptr<class IndexBuffer> IndexBufferPtr;

class IndexBuffer : public RefCounted
{
public:

	IndexBuffer(IndexFormat _format = IF_UShort, BufferUsage _usage = BU_Static);
	~IndexBuffer(void);

	void Realloc(uint _newSize);
	uint Size(void) { return m_size; }
	IDirect3DIndexBuffer9* Handle(void) { return m_handle; }
	uint Stride(void) { return m_format == IF_UShort ? 2 : 4; }

	void Write(const void* _data, uint _offset, uint _size);

protected:

	BufferUsage m_usage;
	IndexFormat m_format;
	uint m_size;
	IDirect3DIndexBuffer9*	m_handle;
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

	void SetVertexBuffer(VertexBuffer* _buffer, uint _offset = 0);
	VertexBuffer* GetVertexBuffer(void) { return m_vertexBuffer; }
	uint GetVertexBufferOffset(void) { return m_vertexBufferOffset; }

	void SetIndexBuffer(IndexBuffer* _buffer);
	IndexBuffer* GetIndexBuffer(void) { return m_indexBuffer; }

	void SetVertexShader(VertexShaderID _id);
	void SetPixelShader(PixelShaderID _id);

	void SetFloatUniformVS(uint _index, const void* _data, uint _num4);
	void SetFloatUniformPS(uint _index, const void* _data, uint _num4);

	void SetTexture(uint _unit, Texture* _texture);
	void SetSampler(uint _unit, SamplerID _sampler);


	//void DrawSprites();

protected:

	IDirect3D9* m_d3d;
	IDirect3DDevice9* m_device;
	IDirect3DVertexDeclaration9* m_vertexFormats[MAX_VERTEX_FORMATS];
	IDirect3DVertexShader9*	m_vertexShaders[MAX_VERTEX_SHADERS];
	IDirect3DPixelShader9* m_pixelShaders[MAX_PIXEL_SHADERS];

	VertexFormat m_vertexFormat;
	VertexBuffer* m_vertexBuffer;
	uint m_vertexBufferOffset;

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
