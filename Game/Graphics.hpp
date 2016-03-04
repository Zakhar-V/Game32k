#pragma once

#include "Math.hpp"
#include "Object.hpp"
#include "Shaders/Common.h"

#include <Windows.h>
#include <GL/gl.h>
#include <GL/glext.h>

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

#define gGraphics Graphics::Get()

#if defined(_DEBUG) || 0
#define _DEBUG_OUTPUT
#endif
#if defined(_DEBUG) || 0
#define _DEBUG_DRAW
#endif

#define MAX_SHADER_OBJECTS 64
#define MAX_SAMPLER_OBJECTS 16

#define MAX_TEXTURE_UNITS 15 // 16 - reserved
#define MAX_RENDER_TARGETS 4

enum FrameBufferType : uint16
{
	FBT_Color = GL_COLOR_BUFFER_BIT,
	FBT_Depth = GL_DEPTH_BUFFER_BIT,
	FBT_Stencil = GL_STENCIL_BUFFER_BIT,
	FBT_DepthStencil = FBT_Depth | FBT_Stencil,
	FBT_All = FBT_Color | FBT_DepthStencil,
};

/*enum IndexFormat : uint8
{
	IF_UShort = GL_UNSIGNED_SHORT,
	IF_UInt = GL_UNSIGNED_INT,
};*/

typedef struct Vertex vertex_t;
typedef uint16 index_t;

enum PrimitiveType : uint16
{
	PT_Points = 1,
	PT_Lines = 2,
	PT_Triangles = 3,
};

//----------------------------------------------------------------------------//
// DepthStencilState
//----------------------------------------------------------------------------//

enum CompareFunc : uint16
{
	CF_Never = GL_NEVER,
	CF_Less = GL_LESS,
	CF_LessEqual = GL_LEQUAL,
	CF_Equal = GL_EQUAL,
	CF_NotEqual = GL_NOTEQUAL,
	CF_GreaterEqual = GL_GEQUAL,
	CF_Greater = GL_GREATER,
	CF_Always = GL_ALWAYS,
};

class DepthStencilState
{

};

//----------------------------------------------------------------------------//
// BlendState
//----------------------------------------------------------------------------//

class BlendState
{

};

//----------------------------------------------------------------------------//
// RasterizerState
//----------------------------------------------------------------------------//

class RasterizerState
{

};

//----------------------------------------------------------------------------//
// Buffer
//----------------------------------------------------------------------------//

enum BufferUsage : uint16
{
	BU_Static = GL_STATIC_DRAW,
	BU_Dynamic = GL_DYNAMIC_DRAW,
};

enum LockMode : uint16
{
	LM_ReadOnly = GL_MAP_READ_BIT,
	LM_WriteDiscard = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT,
	LM_WriteNoOverwrite = GL_MAP_WRITE_BIT,
};

typedef Ptr<class Buffer> BufferPtr;

class Buffer : public RefCounted
{
public:
	Buffer(BufferUsage _usage = BU_Static);
	~Buffer(void);
	BufferUsage Usage(void) { return m_usage; }
	uint Size(void) { return m_size; }
	uint Handle(void) { return m_handle; }
	void Realloc(uint _newSize, const void* _data = nullptr);
	uint8* Map(LockMode _mode, uint _offset = 0, int _size = -1);
	void Unmap(void);
	void Write(const void* _src, uint _offset, uint _size);
	void Copy(Buffer* _src, uint _srcOffset, uint _dstOffset, uint _size);

protected:

	BufferUsage m_usage;
	uint m_size;
	uint m_handle;
};

//----------------------------------------------------------------------------//
// Vertex
//----------------------------------------------------------------------------//

#define USE_HALF_FLOAT_TEXCOORD 1
#define USE_BYTE_NORMALS 1


struct Vertex // generic vertex, 36(40) bytes 
{
	Vec3 position; // 0
#if USE_HALF_FLOAT_TEXCOORD
	uint16 texcoord[2]; // 1
#else
	float texcoord[2]; // 1
#endif
	uint8 color[4]; // 2

	union
	{
		struct
		{
#if USE_BYTE_NORMALS
			uint8 normal[4]; // 3
#else
			float normal[3]; // 3
#endif
			uint8 tangent[4]; // 4
			uint8 weights[4]; // 5
			uint8 indices[4]; // 6
		};

		struct
		{
#if USE_HALF_FLOAT_TEXCOORD
			uint16 texcoord2[2]; // 7
#else
			float texcoord2[2]; // 7
#endif
			float size[2]; // 8
			float rotation; // 9

		};
	};

#if USE_HALF_FLOAT_TEXCOORD
	Vertex& SetTexCoord(const Vec2& _tc) { texcoord[0] = FloatToHalf(_tc.x), texcoord[1] = FloatToHalf(_tc.y); return *this; };
	Vec2 GetTexCoord(void) const { return Vec2(HalfToFloat(texcoord[0]), HalfToFloat(texcoord[1])); }
	Vertex& SetTexCoord2(const Vec2& _tc) { texcoord2[0] = FloatToHalf(_tc.x), texcoord2[1] = FloatToHalf(_tc.y); return *this; };
	Vec2 GetTexCoord2(void) const { return Vec2(HalfToFloat(texcoord2[0]), HalfToFloat(texcoord2[1])); }
#else
	Vertex& SetTexCoord(const Vec2& _tc) { texcoord[0] = _tc.x, texcoord[1] = _tc.y; return *this; };
	Vec2 GetTexCoord(void) const { return *(Vec2*)texcoord; }
	Vertex& SetTexCoord2(const Vec2& _tc) { texcoord2[0] = _tc.x, texcoord2[1] = _tc.y; return *this; };
	Vec2 GetTexCoord2(void) const { return *(Vec2*)texcoord2; }
#endif

	Vertex& SetColor(const Color& _c) { color[0] = _c.r, color[1] = _c.g, color[2] = _c.b, color[3] = _c.a; return *this; }

	Vertex& SetSize(const Vec2& _s) { size[0] = _s.x, size[1] = _s.y; return *this; }
	Vertex& SetRotation(float _r) { rotation = _r; return *this; }

#if USE_BYTE_NORMALS
	Vec3 GetNormal(void) const { return Vec3(SByteToFloat(normal[0]), SByteToFloat(normal[1]), SByteToFloat(normal[2])); }
	Vertex& SetNormal(const Vec3& _n) { normal[0] = FloatToSByte(_n[0]), normal[1] = FloatToSByte(_n[1]), normal[2] = FloatToSByte(_n[2]); return *this; }
#else
	Vec3 GetNormal(void) const { return *(const Vec3*)normal; }
	Vertex& SetNormal(const Vec3& _n) { normal[0] = _n[0], normal[1] = _n[1], normal[2] = _n[2]; return *this; }
#endif
	// ...
};

//----------------------------------------------------------------------------//
// VertexArray
//----------------------------------------------------------------------------//

typedef Ptr<class VertexArray> VertexArrayPtr;

class VertexArray : public RefCounted
{
public:

	VertexArray(PrimitiveType _type, bool _indexed, BufferUsage _usage = BU_Static);
	~VertexArray(void);

	PrimitiveType GetType(void) { return m_type; }

	Buffer* GetVertexBuffer(void) { return m_vertexBuffer; }
	uint GetVertexCount(void) { return m_vertexBuffer->Size() / sizeof(Vertex); }

	Buffer* GetIndexBuffer(void) { return m_indexBuffer; }
	uint GetIndexCount(void) { return m_indexBuffer ? (m_indexBuffer->Size() / sizeof(index_t)) : 0; }

	void Bind(void);
	void Draw(uint _start, uint _count, uint _numInstances = 1);
	

protected:

	PrimitiveType m_type;
	BufferPtr m_vertexBuffer;
	BufferPtr m_indexBuffer;
};

//----------------------------------------------------------------------------//
// Texture
//----------------------------------------------------------------------------//

enum PixelFormat : uint16
{
	PF_RGB8,
	PF_RGBA8,
	PF_RGB10A2,
	PF_R32F,
	PF_RG32F,
	PF_RGB32F,
	PF_RGBA32F,
	PF_RGBA16F,
	PF_RG11B10F,
	PF_D24S8,
	PF_DXT1,
	PF_DXT5,
};

uint BitsPerPixel(PixelFormat _format);
bool IsCompressed(PixelFormat _format);

enum TextureType : uint16
{
	TT_2D,
	TT_3D,
	TT_Array,
	TT_Cube,
};

typedef Ptr<class Texture> TexturePtr;

class Texture : public RefCounted
{
public:

	Texture(TextureType _type = TT_2D, PixelFormat _format = PF_DXT5, bool _mipmap = true);
	~Texture(void);

	TextureType Type(void) { return m_type; }
	PixelFormat Format(void) { return m_format; }
	bool HasMipmaps(void) { return m_mipmap; }
	uint Width(void) { return m_width; }
	uint Height(void) { return m_height; }
	uint Depth(void) { return m_depth; }
	uint Handle(void) { return m_handle; }

	void Realloc(uint _width, uint _height, uint _depth = 0);
	void Write(PixelFormat _format, const void* _src);
	void Read(PixelFormat _format, void* _dst);
	void GenerateMipmap(void);

protected:

	TextureType m_type;
	PixelFormat m_format;
	bool m_mipmap;
	uint m_width;
	uint m_height;
	uint m_depth;
	uint m_handle;
};

//----------------------------------------------------------------------------//
// RenderBuffer
//----------------------------------------------------------------------------//

class RenderBuffer : public NonCopyable
{
public:

	RenderBuffer(PixelFormat _format);
	~RenderBuffer(void);

	PixelFormat Format(void) { return m_format; }
	uint Width(void) { return m_width; }
	uint Height(void) { return m_height; }
	uint Samples(void) { return m_samples; }
	uint Handle(void) { return m_handle; }

	void Realloc(uint _width, uint _height, uint _samples = 0);
	void CopyToTexture(Texture* _texture, uint _z = 0);	// todo: filter

protected:
	friend class Graphics;

	static void _BindRenderTargetTexture(uint _framebuffer, uint _attachment, Texture* _texture, uint _z);

	PixelFormat m_format;
	uint16 m_iformat;
	uint m_width;
	uint m_height;
	uint m_samples;
	uint m_handle;
};

//----------------------------------------------------------------------------//
// Sampler
//----------------------------------------------------------------------------//

enum TextureWrap : uint16
{

};

//----------------------------------------------------------------------------//
// Shader
//----------------------------------------------------------------------------//

enum ShaderType	: uint16
{
	ST_Vertex,
	ST_Fragment,
	ST_Geometry,
};

enum ShaderID
{
	VS_StaticModel,
	VS_SkinnedModel,
	VS_Sprite,
	VS_Particles,
	VS_Terrain,

	GS_Sprite,
	GS_Billboard,
	GS_BillboardY,
	GS_Particles,
	GS_Terrain,

	FS_Texture,
	FS_NoTexture,
};

class Shader : public NonCopyable
{
public:

	ShaderType Type(void) { return m_type; }
	uint Handle(void) { return m_handle; }

protected:
	friend class Graphics;

	Shader(void);
	~Shader(void);
	void _Init(ShaderType _type, const char* _common, const char* _src, uint16 _flags = 0);

	ShaderType m_type;
	uint16 m_flags;
	uint m_handle;
};

//----------------------------------------------------------------------------//
// Graphics
//----------------------------------------------------------------------------//

class Graphics : public Singleton<Graphics>
{
public:

	Graphics(void);
	~Graphics(void);

	void BeginFrame(void);
	void EndFrame(void);

	void EnableRenderTargets(bool _enabled = true);
	bool IsRenderTargetsEnabled(void) { return m_drawToFramebuffer; }
	void ResetRenderTargets(void);
	void SetRenderBuffer(uint _slot, RenderBuffer* _target);
	void SetDepthStencilBuffer(RenderBuffer* _target);
	void SetRenderTargetTexture(uint _slot, Texture* _target, uint _z = 0);
	void SetDepthStencilTargetTexture(Texture* _target, uint _z = 0);

	void ResetTextures(void);
	void SetTexture(uint _slot, Texture* _texture);

	void ClearFrameBuffers(uint _buffers, const Color& _color = 0x000000ff, float _depth = 1.0, uint _stencil = 0xff);

	void SetVertexBuffer(Buffer* _buffer);
	void SetIndexBuffer(Buffer* _buffer);
	void SetUniformBuffer(uint _slot, Buffer* _buffer, uint _base = 0);	///!<\note redundant state is not ignored

	Shader* GetShader(ShaderID _id) { return &m_shaders[_id]; }
	void SetShader(ShaderID _id) { SetShader(m_shaders[_id].Type(), &m_shaders[_id]); }
	void SetShader(ShaderType _type, Shader* _shader);

	void Draw(PrimitiveType _type, uint _start, uint _count, uint _numInstances = 1);
	void DrawIndexed(PrimitiveType _type, uint _baseVertex, uint _start, uint _count, uint _numInstances = 1);


protected:
	friend class RenderBuffer;

	void _ResetState(void);
	uint _GetFramebuffer(void) { return m_framebuffer; }
	uint _SrcCopyFramebuffer(void) { return m_copyFramebuffer[0]; }
	uint _DstCopyFramebuffer(void) { return m_copyFramebuffer[1]; }

	HDC m_dc;
	HGLRC m_rc;

	// shaders

	Shader m_shaders[MAX_SHADER_OBJECTS];
	uint m_numShaders;
	Shader* m_currentShader[3];
	uint m_shaderPipeline;

	// geometry

	uint m_vertexArray;
	Buffer* m_currentVertexBuffer;
	Buffer* m_currentIndexBuffer;

	// framebuffer

	struct FramebufferAttachment
	{
		Texture* texture;
		RenderBuffer* buffer;
		uint textureZ;
	};

	uint m_framebuffer;
	uint m_copyFramebuffer[2];
	bool m_drawToFramebuffer;
	FramebufferAttachment m_renderTargets[MAX_RENDER_TARGETS];
	FramebufferAttachment m_depthStencilTarget;

	// textures

	Texture* m_textures[MAX_TEXTURE_UNITS];
	class Sampler* m_samplers[MAX_TEXTURE_UNITS];
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
