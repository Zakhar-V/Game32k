#pragma once

#include "Graphics.hpp"
#include "Device.hpp"
#include "File.hpp"

#include <GL/wglext.h>

#include "OpenGL.inl"


enum
{
	PF_RGB8,
	PF_RGB10A2,
	PF_R32F,
	PF_RGBA16F,
	PF_RG11B10F,
};

struct GLPixelFormat
{
	uint8 bpp;
	bool compressed;
	uint16 iformat;
	uint16 format;
	uint16 type;
};
GLPixelFormat GLPixelFormats[] =
{
	{ 24, false, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE }, //PF_RGB8
	{ 32, false, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE }, //PF_RGBA8
	{ 32, false, GL_RGB10_A2, GL_RGBA, GL_UNSIGNED_INT_2_10_10_10_REV }, //PF_RGB10A2
	{ 32, false, GL_R32F, GL_RED, GL_FLOAT }, //PF_R32F
	{ 64, false, GL_RGBA16F, GL_RGBA, GL_FLOAT }, //PF_RGBA16F
	{ 32, false, GL_R11F_G11F_B10F, GL_RGB, GL_UNSIGNED_INT_10F_11F_11F_REV }, //PF_RG11B10F
	{ 32, false, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8 }, //PF_D24S8
	{ 4, true, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, 0, 0 }, //PF_DXT1
	{ 8, true, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, 0, 0 }, //PF_DXT5
};

class _Texture
{

};

class _Shader
{
public:

protected:
};


//----------------------------------------------------------------------------//
// Buffer
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Buffer::Buffer(BufferUsage _usage) :
	m_usage(_usage),
	m_size(0),
	m_handle(0)
{
	glGenBuffers(1, &m_handle);
	glBindBuffer(GL_ARRAY_BUFFER, m_handle);
}
//----------------------------------------------------------------------------//
Buffer::~Buffer(void)
{
	glDeleteBuffers(1, &m_handle);
}
//----------------------------------------------------------------------------//
void Buffer::Realloc(uint _newSize, const void* _data)
{
	if (m_size != _newSize)
	{
		glNamedBufferDataEXT(m_handle, _newSize, _data, m_usage);
		m_size = _newSize;
	}
}
//----------------------------------------------------------------------------//
uint8* Buffer::Map(LockMode _mode, uint _offset, uint _size)
{
	ASSERT(_offset + _size <= m_size);
	return (uint8*)glMapNamedBufferRangeEXT(m_handle, _offset, _size, _mode);
}
//----------------------------------------------------------------------------//
void Buffer::Unmap(void)
{
	glUnmapNamedBufferEXT(m_handle);
}
//----------------------------------------------------------------------------//
void Buffer::Write(const void* _src, uint _offset, uint _size)
{
	ASSERT(_offset + _size <= m_size);
	ASSERT(_src || !_size);
	glNamedBufferSubDataEXT(m_handle, _offset, _size, _src);
}
//----------------------------------------------------------------------------//
void Buffer::Copy(Buffer* _src, uint _srcOffset, uint _dstOffset, uint _size)
{
	ASSERT(_src != nullptr);
	ASSERT(_dstOffset + _size <= m_size);
	ASSERT(_srcOffset + _size <= _src->m_size);
	glNamedCopyBufferSubDataEXT(_src->m_handle, m_handle, _srcOffset, _dstOffset, _size);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Buffer
//----------------------------------------------------------------------------//

struct GLVertexAttrib
{
	uint16 format;
	uint16 components;
	bool integer;
	bool normalized;
	uint offset;
}
const GLVertexAttribs[] = 
{
	{ GL_FLOAT, 3, false, false, offsetof(Vertex, position) },
	{ GL_HALF_FLOAT, 2, false, false, offsetof(Vertex, texcoord) },
	{ GL_UNSIGNED_BYTE, 4, false, true, offsetof(Vertex, color) },
	{ GL_UNSIGNED_BYTE, 4, false, true, offsetof(Vertex, normal) },
	{ GL_UNSIGNED_BYTE, 4, false, true, offsetof(Vertex, tangent) },
	{ GL_UNSIGNED_BYTE, 4, false, true, offsetof(Vertex, weights) },
	{ GL_UNSIGNED_BYTE, 4, true, false, offsetof(Vertex, indices) },
	{ GL_HALF_FLOAT, 2, false, false, offsetof(Vertex, texcoord2) },
	{ GL_FLOAT, 2, false, false, offsetof(Vertex, size) },
	{ GL_FLOAT, 1, false, false, offsetof(Vertex, rotation) },
};

//----------------------------------------------------------------------------//
// Shader
//----------------------------------------------------------------------------//

const char* ShaderPrefix[] =
{
	"VS", // ST_Vertex
	"FS", // ST_Fragment
	"GS", // ST_Geometry
};

const uint16 GLShaderTypes[] =
{
	GL_VERTEX_SHADER, // ST_Vertex
	GL_FRAGMENT_SHADER, // ST_Fragment
	GL_GEOMETRY_SHADER, // ST_Geometry
};

struct ShaderName
{
	ShaderType type;
	const char* name;
	uint flags;
}
const g_shaderNames[] =
{
	{ ST_Vertex, "Generic-VS.glsl", INSTANCED_BIT }, // VS_StaticModel
	{ ST_Vertex, "Generic-VS.glsl", SKINNED_BIT }, // VS_SkinnedModel
	{ ST_Vertex, "Generic-VS.glsl", SPRITE_BIT }, // VS_Sprite
	{ ST_Vertex, "Generic-VS.glsl", BILLBOARD_BIT }, // VS_Billboard
	{ ST_Vertex, "Generic-VS.glsl", TERRAIN_BIT }, // VS_Terrain

	//{ ST_Geometry, "Sprite-GS.glsl", 0 }, // GS_Sprite
	//{ ST_Geometry, "Terrain-GS.glsl", 0 }, // GS_Terrain

	{ ST_Fragment, "FillGBuffer-PS.glsl", TEXTURE_BIT }, // FS_Texture
	{ ST_Fragment, "FillGBuffer-PS.glsl", 0 }, // FS_NoTexture

};
//----------------------------------------------------------------------------//
Shader::Shader(void) :
	m_type(ST_Vertex),
	m_flags(0),
	m_handle(0)
{
}
//----------------------------------------------------------------------------//
Shader::~Shader(void)
{
	//if (m_handle)
	//	glDeleteProgram(m_handle);
}
//----------------------------------------------------------------------------//
void Shader::_Init(ShaderType _type, const char* _common, const char* _src, uint _flags)
{
	ASSERT(m_handle == 0);
	ASSERT(_common != nullptr);
	ASSERT(_src != nullptr);

	m_type = _type;
	m_flags = _flags;

	char _header[256]; 
	sprintf(_header, 
		"#version 330 core\n"
		"#extension GL_ARB_enhanced_layouts:enable\n"
		"#extension GL_ARB_separate_shader_objects:enable\n"
		"#define FLAGS 0x%08x\n"
		"#define COMPILE_%s 1\n"
		"#define GLSL 330\n",
		_flags ,
		ShaderPrefix[m_type]);

	String _srcs = _header;
	_srcs += _common;
	_srcs += "\n#line 0";
	_srcs += _src;

	const char* _srcp = _srcs;

	m_handle = glCreateShaderProgramv(GLShaderTypes[m_type], 1, &_srcp);

#ifdef _DEBUG_OUTPUT
	int _length;
	glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &_length);
	if (_length > 1)
	{
		Array<char> _log(_length + 1);
		glGetProgramInfoLog(m_handle, _length, &_length, *_log);
		_log[_length] = 0;
		LOG("%s\n", *_log);
	}
#endif

	int _status;
	glGetProgramiv(m_handle, GL_LINK_STATUS, &_status);
	_CHECK(_status, "Couldn't compile shader");
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Graphics
//----------------------------------------------------------------------------//

uint16 GLPrimitiveType[] =
{
	0,
	GL_POINTS, // PT_Points = 1,
	GL_LINES, // PT_Lines = 2,
	GL_TRIANGLES, // PT_Triangles = 3,
};

//----------------------------------------------------------------------------//
Graphics::Graphics(void)
{
	LOG("Create Graphics");

	ASSERT(gDevice != nullptr); // the window must be created before it

	// create context
	{
		PIXELFORMATDESCRIPTOR _pfd;
		memset(&_pfd, 0, sizeof(_pfd));
		_pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		_pfd.nVersion = 1;
		_pfd.nSize = sizeof(_pfd);
		_pfd.cColorBits = 32;
		_pfd.cDepthBits = 24;
		_pfd.cStencilBits = 8;

		m_dc = GetDC(gDevice->WindowHandle());
		int _pf = ChoosePixelFormat(m_dc, &_pfd);
		SetPixelFormat(m_dc, _pf, &_pfd);

		HGLRC _tempRc = wglCreateContext(m_dc);
		_CHECK(_tempRc, "Couldn't create temporary OpenGL render context");
		wglMakeCurrent(m_dc, _tempRc);

		PFNWGLCREATECONTEXTATTRIBSARBPROC _wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		_CHECK(_wglCreateContextAttribsARB, "OpenGL 3.x is not supported");

		int _attribs[] =
		{ 
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
#ifdef _DEBUG_OUTPUT
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB | WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | WGL_CONTEXT_DEBUG_BIT_ARB,
#else
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB | WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
#endif
			0, 0
		};

		m_rc = _wglCreateContextAttribsARB(m_dc, 0, _attribs);
		_CHECK(m_rc, "Couldn't create OpenGL render context");

		wglMakeCurrent(m_dc, m_rc);
		wglDeleteContext(_tempRc);
	}

	LOG("OpenGL %s on %s, %s", glGetString(GL_VERSION), glGetString(GL_RENDERER), glGetString(GL_VENDOR));

	// load functions
	{
		LoadOpenGL();

		int _p[4];
		glGetIntegerv(GL_MAJOR_VERSION, _p);
		glGetIntegerv(GL_MINOR_VERSION, _p + 1);
		int _ver = _p[0] * 10 + _p[1];

		CHECK(HasOpenGLExtension("GL_ARB_separate_shader_objects") || _ver >= 41, "No GL_ARB_separate_shader_objects extension");
		CHECK(HasOpenGLExtension("GL_ARB_vertex_attrib_binding") || _ver >= 43, "No GL_ARB_vertex_attrib_binding extension");
		CHECK(HasOpenGLExtension("GL_ARB_enhanced_layouts") || _ver >= 44, "No GL_ARB_enhanced_layouts extension");
		CHECK(HasOpenGLExtension("GL_EXT_direct_state_access"), "No GL_EXT_direct_state_access extension");
	}

	//shaders
	{
		RawData _commonFile = LoadFile("Common.h");
		_commonFile.Push('\0');
		const char* _common = (const char*)*_commonFile;
		for (m_numShaders = 0; m_numShaders < sizeof(g_shaderNames) / sizeof(g_shaderNames[0]); ++m_numShaders)
		{

			const ShaderName& _info = g_shaderNames[m_numShaders];

			LOG("Load Shader \"%s\" 0x%08x", _info.name, _info.flags);

			RawData _file = LoadFile(_info.name);
			_file.Push('\0');
			const char* _src = (const char*)*_file;
			m_shaders[m_numShaders]._Init(_info.type, _common, _src, _info.flags);
		}

		//memset(m_currentShader, 0, sizeof(m_currentShader));
		uint m_shaderPipeline = 0;
		glGenProgramPipelines(1, &m_shaderPipeline);
		//glBindProgramPipeline(m_shaderPipeline);
	}



	_ResetState();
}
//----------------------------------------------------------------------------//
Graphics::~Graphics(void)
{
	LOG("Destroy Graphics");
}
//----------------------------------------------------------------------------//
void Graphics::BeginFrame(void)
{
	_ResetState();
}
//----------------------------------------------------------------------------//
void Graphics::EndFrame(void)
{
	SwapBuffers(m_dc);
}
//----------------------------------------------------------------------------//
void Graphics::ClearFrameBuffers(uint _buffers, const Color& _color, float _depth, uint _stencil)
{
	int _colorMask[4];
	if (_buffers & FBT_Color)
	{
		Vec4 _clr = _color;
		glGetIntegerv(GL_COLOR_WRITEMASK, _colorMask);
		glColorMask(1, 1, 1, 1);
		glClearColor(_clr.x, _clr.y, _clr.z, _clr.w);
	}

	int _depthMask;
	if (_buffers & FBT_Depth)
	{
		glGetIntegerv(GL_DEPTH_WRITEMASK, &_depthMask);
		glDepthMask(1);
		glClearDepth(_depth);
	}

	int _stencilMask;
	if (_buffers & FBT_Stencil)
	{
		glGetIntegerv(GL_STENCIL_WRITEMASK, &_stencilMask);
		glStencilMask(_stencil);
		glClearStencil(_stencil);
	}

	int _viewport[4], _scissor[4];
	glGetIntegerv(GL_VIEWPORT, _viewport);
	glGetIntegerv(GL_SCISSOR_BOX, _scissor);

	int _scissorEnabled = glIsEnabled(GL_SCISSOR_TEST);
	glEnable(GL_SCISSOR_TEST);
	glScissor(_viewport[0], _viewport[1], _viewport[2], _viewport[3]);

	// disable blendig ?

	glClear(_buffers);

	glScissor(_scissor[0], _scissor[1], _scissor[2], _scissor[3]);

	if(!_scissorEnabled)
		glDisable(GL_SCISSOR_TEST);

	if (_buffers & FBT_Stencil)
		glStencilMask(_stencilMask);

	if (_buffers & FBT_Depth)
		glDepthMask(_depthMask);

	if (_buffers & FBT_Color)
		glColorMask(_colorMask[0], _colorMask[1], _colorMask[2], _colorMask[3]);
}
//----------------------------------------------------------------------------//
void Graphics::SetVertexBuffer(Buffer* _buffer)
{
	if (m_currentVertexBuffer != _buffer)
	{
		glBindVertexBuffer(0, _buffer ? _buffer->Handle() : 0, 0, sizeof(Vertex));
		m_currentVertexBuffer = _buffer;
	}
}
//----------------------------------------------------------------------------//
void Graphics::SetIndexBuffer(Buffer* _buffer)
{
	if (m_currentIndexBuffer != _buffer)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer ? _buffer->Handle() : 0);
		m_currentIndexBuffer = _buffer;
	}
}
//----------------------------------------------------------------------------//
void Graphics::SetUniformBuffer(uint _slot, Buffer* _buffer, uint _base)
{
	ASSERT(_buffer != nullptr);
	ASSERT(_base <= _buffer->Size());
	// todo: check alignment

	glBindBufferRange(GL_UNIFORM_BUFFER, _slot, _buffer->Handle(), _base, _buffer->Size() - _base);
}
//----------------------------------------------------------------------------//
void Graphics::SetShader(ShaderType _type, Shader* _shader)
{
	if (m_currentShader[_type] != _shader)
	{
		glUseProgramStages(m_shaderPipeline, 1 << _type, _shader ? _shader->Handle() : 0);
		m_currentShader[_type] = _shader;
	}
}
//----------------------------------------------------------------------------//
void Graphics::Draw(PrimitiveType _type, uint _start, uint _count, uint _numInstances)
{
	ASSERT(m_currentVertexBuffer != nullptr);

	glDrawArraysInstanced(GLPrimitiveType[_type], _start, _count, _numInstances);
}
//----------------------------------------------------------------------------//
void Graphics::DrawIndexed(PrimitiveType _type, uint _baseVertex, uint _start, uint _count, uint _numInstances)
{
	ASSERT(m_currentVertexBuffer != nullptr);
	ASSERT(m_currentIndexBuffer != nullptr);

	glDrawElementsInstancedBaseVertex(GLPrimitiveType[_type], _count, GL_UNSIGNED_SHORT, (const void*)(_start * sizeof(index_t)), _numInstances, _baseVertex);
}
//----------------------------------------------------------------------------//
void Graphics::_ResetState(void)
{
	// shaders
	memset(m_currentShader, 0, sizeof(m_currentShader));
	glBindProgramPipeline(m_shaderPipeline);
	glUseProgramStages(m_shaderPipeline, GL_VERTEX_SHADER_BIT | GL_FRAGMENT_SHADER_BIT | GL_GEOMETRY_SHADER_BIT, 0);

	for (uint i = 0; i < sizeof(GLVertexAttribs) / sizeof(GLVertexAttribs[0]); ++i)
	{
		const GLVertexAttrib& _attrib = GLVertexAttribs[i];
		glVertexAttribBinding(i, 0);
		glEnableVertexAttribArray(i);
		if (_attrib.integer)
			glVertexAttribIFormat(i, _attrib.components, _attrib.format, _attrib.offset);
		else
			glVertexAttribFormat(i, _attrib.components, _attrib.format, _attrib.normalized, _attrib.offset);
	}

	m_currentVertexBuffer = nullptr;
	m_currentIndexBuffer = nullptr;
	
	//glPrimitiveRestartIndex(0xffff);

}
//----------------------------------------------------------------------------//

#if 0

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
// old Direct3D9 renderer
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

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

#endif
