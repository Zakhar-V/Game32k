#pragma once

#include "Math.hpp"
#include "Object.hpp"
#include "Shaders/Constants.h"
#include <d3d9.h>

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

#define gGraphics Graphics::Get()
#define gGraphicsDevice gGraphics->Device()

#define MAX_VERTEX_SHADERS 256
#define MAX_PIXEL_SHADERS 256

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

class VertexBuffer : public RefCounted
{
public:

	VertexBuffer(uint _size, BufferUsage _usage = BU_Static);
	~VertexBuffer(void);

	uint Size(void) { return m_size; }
	IDirect3DVertexBuffer9* Handle(void) { return m_handle; }

	void Write(const void* _data, uint _offset, uint _size);

protected:

	uint m_size;
	IDirect3DVertexBuffer9*	m_handle;
};

class IndexBuffer : public NonCopyable
{
public:

	IndexBuffer(uint _size, IndexFormat _format, BufferUsage _usage = BU_Static);
	~IndexBuffer(void);

	uint Size(void) { return m_size; }
	IDirect3DIndexBuffer9* Handle(void) { return m_handle; }

	void Write(const void* _data, uint _offset, uint _size);

protected:

	uint m_size;
	IDirect3DIndexBuffer9*	m_handle;
};

//----------------------------------------------------------------------------//
// Texture
//----------------------------------------------------------------------------//

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
	TU_Default = D3DUSAGE_AUTOGENMIPMAP,
	TU_Dynamic = D3DUSAGE_AUTOGENMIPMAP | D3DUSAGE_DYNAMIC,
	TU_RenderTarget = D3DUSAGE_AUTOGENMIPMAP | D3DUSAGE_RENDERTARGET,
};

enum TextureFormat : uint
{
	TF_RGBA8 = D3DFMT_A8R8G8B8,
	TF_D24S8 = D3DFMT_D24S8,
	TF_DXT1 = D3DFMT_DXT1,
	TF_DXT5 = D3DFMT_DXT5,
};

class Texture : public RefCounted
{
public:

	Texture(TextureType _type, TextureFormat _format, TextureUsage _usage = TU_Default);
	~Texture(void);

	void SetSize(uint _width, uint _height, uint _depth = 1, uint _lods = 0);
	void SetData(const void* _data);
	void GenerateLods(void);

	IDirect3DBaseTexture9* Handle(void) { return m_handle; }

protected:

	void _Create(void);
	void _Destroy(void);

	TextureType m_type;
	TextureUsage m_usage;
	TextureFormat m_format;
	Vec2i m_size;
	uint m_depth;
	uint m_lods;
	uint m_desiredLods;
	union
	{
		IDirect3DBaseTexture9* m_handle;
		IDirect3DTexture9* m_texture2d;
		IDirect3DVolumeTexture9* m_texture3d;
		IDirect3DCubeTexture9* m_textureCube;
	};
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

	void LoadVertexShaders(const char** _assets);
	void LoadPixelShaders(const char** _assets);

	void BeginFrame(void);
	void EndFrame(void);

	void ClearFrameBuffers(uint _buffers, const Color& _color = 0x000000ff, float _depth = 1.0, uint _stencil = 0xff);

	void SetVertexFormat(VertexFormat _format);
	void SetVertexBuffer(VertexBuffer* _buffer, uint _offset, uint _stride);

	void SetVertexShader(uint _id);
	void SetPixelShader(uint _id);


protected:

	IDirect3D9* m_d3d;
	IDirect3DDevice9* m_device;
	IDirect3DVertexDeclaration9* m_vertexFormats[MAX_VERTEX_FORMATS];
	IDirect3DVertexShader9*	m_vertexShaders[MAX_VERTEX_SHADERS];
	IDirect3DPixelShader9* m_pixelShaders[MAX_PIXEL_SHADERS];
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
