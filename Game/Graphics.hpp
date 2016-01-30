#pragma once

#include "Base.hpp"
#include "Math.hpp"
#include "Shaders/Constants.h"
#include <d3d9.h>

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

#define gGraphics Graphics::Get()
#define gGraphicsDevice gGraphics->Device()

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
	VF_MaxFormats,
};

struct Vertex
{
	Vec3 pos;
};

struct SimpleVertex : Vertex
{
	Vec2 texcoord;
	Color color;
};

struct StaticVertex : Vertex
{
	Vec2 texcoord;
	Color normal;
	Color tangent;
};

struct SkinnedVertex : StaticVertex
{
	Color bones;
	Color weights;
};

//----------------------------------------------------------------------------//
// Buffer
//----------------------------------------------------------------------------//

enum BufferUsage : uint16
{
	BU_Static = 0,
	BU_Dynamic = D3DUSAGE_DYNAMIC,
};

class VertexBuffer : public NonCopyable
{
public:

	VertexBuffer(uint _size, BufferUsage _usage = BU_Static);
	~VertexBuffer(void);
	void Write(const void* _data, uint _offset, uint _size);

protected:

	uint m_size;
	IDirect3DVertexBuffer9*	m_handle;
};

//----------------------------------------------------------------------------//
// ShaderBytecode
//----------------------------------------------------------------------------//

class ShaderBytecode : public NonCopyable
{
public:

	ShaderBytecode(void);
	~ShaderBytecode(void);
	void Realloc(uint _newSize);
	//uint8*

protected:
	uint8* m_bytecode;
	uint m_size;
};

//----------------------------------------------------------------------------//
// Graphics
//----------------------------------------------------------------------------//

class Graphics : public Singleton<Graphics>
{
public:

	Graphics(void);

	IDirect3D9* Direct3D(void) { return m_d3d; }
	IDirect3DDevice9* Device(void) { return m_device; }

	void BeginFrame(void);
	void EndFrame(void);

	void ClearFrameBuffers(uint _buffers, const Color& _color = 0x000000ff, float _depth = 1.0, uint _stencil = 0xff);

protected:

	IDirect3D9* m_d3d;
	IDirect3DDevice9* m_device;
	IDirect3DVertexDeclaration9* m_vertexTypes[VF_MaxFormats];
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
