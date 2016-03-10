#pragma once

#include "Graphics.hpp"
#include "Device.hpp"
#include "File.hpp"

#include <GL/wglext.h>

#include "OpenGL.inl"

//----------------------------------------------------------------------------//
// DebugOutput
//----------------------------------------------------------------------------//

#ifdef _DEBUG_OUTPUT

bool g_DebugOutputEnabled = false;

struct GLDebugPoint : public NonCopyable
{
	GLDebugPoint(const char* _func, const char* _file, int _line) :
		func(_func),
		file(_file),
		line(_line),
		prev(Top)
	{
		Top = this;
	}
	~GLDebugPoint(void)
	{
		Top = prev;
	}
	const char* func;
	const char* file;
	int line;
	GLDebugPoint* prev;
	static GLDebugPoint* Top;
};

GLDebugPoint* GLDebugPoint::Top = nullptr;

#define GL_DEBUG_POINT() GLDebugPoint _gldbg_(__FUNCTION__, __FILE__, __LINE__)

//----------------------------------------------------------------------------//
void __stdcall _DebugProc(uint _source, uint _type, uint _id, uint _severity, int _length, const char* _message, const void* _ud)
{
	const char* _src;
	const char* _typeStr;
	const char* _levelStr;
	int  _level = 0;
	const char* _func = nullptr;
	const char* _file = nullptr;
	int _line = 0;

	switch (_source)
	{
	case GL_DEBUG_SOURCE_API_ARB:
		_src = "OpenGL";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
		_src = "Window system";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
		_src = "Shader compiler";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
		_src = "Third party";
		break;
	case GL_DEBUG_SOURCE_APPLICATION_ARB:
		_src = "Application";
		break;
	case GL_DEBUG_SOURCE_OTHER_ARB:
		_src = "Other";
		break;
	default:
		return;
	}

	switch (_type)
	{
	case GL_DEBUG_TYPE_ERROR_ARB:
		_typeStr = "Error";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
		_typeStr = "Deprecated behavior";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
		_typeStr = "Undefined behavior";
		break;
	case GL_DEBUG_TYPE_PORTABILITY_ARB:
		_typeStr = "Portability";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE_ARB:
		_typeStr = "Performance";
		break;
	case GL_DEBUG_TYPE_OTHER_ARB:
		_typeStr = "Message";
		break;
#if 0
	case GL_DEBUG_TYPE_MARKER:
		_dtype = "Marker";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		_dtype = "Push group";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		_dtype = "Pop group";
		break;
#endif
	default:
		return;
	}

	switch (_severity)
	{
	case GL_DEBUG_SEVERITY_HIGH_ARB:
		_levelStr = "high";
		_level = 1;
		break;
	case GL_DEBUG_SEVERITY_MEDIUM_ARB:
		_levelStr = "medium";
		_level = 0;
		break;
	case GL_DEBUG_SEVERITY_LOW_ARB:
		_levelStr = "low";
		_level = -1;
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		_levelStr = "notification";
		_level = -2;
		break;
	default:
		return;
	}

	if (_level >= 0 && g_DebugOutputEnabled)
	{
		if (GLDebugPoint::Top)
			LOG("%s(%d)[%s]: %s: %s(%s): %s", GLDebugPoint::Top->file, GLDebugPoint::Top->line, GLDebugPoint::Top->func, _src, _typeStr, _levelStr, _message);
		else
			LOG("%s: %s(%s): %s", _src, _typeStr, _levelStr, _message);
	}
}
//----------------------------------------------------------------------------//

#else

#define GL_DEBUG_POINT()

#endif // _DEBUG_OUTPUT

//----------------------------------------------------------------------------//
// Utils
//----------------------------------------------------------------------------//

void glEnableEx(uint _state, bool _enabled)
{
	if (_enabled)
		glEnable(_state);
	else
		glDisable(_state);
}

//----------------------------------------------------------------------------//
// DepthStencilState
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
DepthStencilState::DepthStencilState(void) :
	m_hash(0)
{
}
//----------------------------------------------------------------------------//
DepthStencilState::~DepthStencilState(void)
{
}
//----------------------------------------------------------------------------//
void DepthStencilState::_Init(uint _hash, const Desc& _desc)
{
	m_hash = _hash;
	m_desc = _desc;
}
//----------------------------------------------------------------------------//
void DepthStencilState::_Bind(uint _ref)
{
	GL_DEBUG_POINT();

	glEnableEx(GL_DEPTH_TEST, m_desc.depthTest);
	glDepthFunc(m_desc.depthFunc);
	glDepthMask(m_desc.depthWrite);

	if (m_desc.stencilTest)
	{
		glEnable(GL_STENCIL_TEST);
		glStencilMask(m_desc.stencilWrite);
		glStencilFuncSeparate(GL_FRONT, m_desc.stencilFront.func, _ref, m_desc.stencilRead);
		glStencilFuncSeparate(GL_BACK, m_desc.stencilBack.func, _ref, m_desc.stencilRead);
		glStencilOpSeparate(GL_FRONT, m_desc.stencilFront.stencilFail, m_desc.stencilFront.depthFail, m_desc.stencilFront.pass);
		glStencilOpSeparate(GL_BACK, m_desc.stencilBack.stencilFail, m_desc.stencilBack.depthFail, m_desc.stencilBack.pass);
	}
	else
	{
		glDisable(GL_STENCIL_TEST);
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Buffer
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Buffer::Buffer(BufferUsage _usage) :
	m_usage(_usage),
	m_used(0),
	m_size(0),
	m_handle(0)
{
	GL_DEBUG_POINT();

	glGenBuffers(1, &m_handle);
	glBindBuffer(GL_ARRAY_BUFFER, m_handle);
}
//----------------------------------------------------------------------------//
Buffer::~Buffer(void)
{
	GL_DEBUG_POINT();

	glDeleteBuffers(1, &m_handle);
}
//----------------------------------------------------------------------------//
void Buffer::Realloc(uint _newSize, const void* _data)
{
	GL_DEBUG_POINT();

	if (!_newSize && _data)
		_data = nullptr;

	if (m_size < _newSize || (m_usage == BU_Static && m_size != _newSize))
	{
		glNamedBufferDataEXT(m_handle, _newSize, _data, m_usage);
		m_size = _newSize;
	}
	else if (_data)
	{
		glNamedBufferSubDataEXT(m_handle, 0, _newSize, _data);
	}
	m_used = _newSize;
}
//----------------------------------------------------------------------------//
uint8* Buffer::Map(LockMode _mode, uint _offset, int _size)
{
	GL_DEBUG_POINT();
	ASSERT(_offset <= m_used);
	if (_size < 0)
		_size = m_used - _offset;
	ASSERT((uint)(_offset + _size) <= m_size);

	return (uint8*)glMapNamedBufferRangeEXT(m_handle, _offset, _size, _mode);
}
//----------------------------------------------------------------------------//
void Buffer::Unmap(void)
{
	GL_DEBUG_POINT();

	glUnmapNamedBufferEXT(m_handle);
}
//----------------------------------------------------------------------------//
void Buffer::Write(const void* _src, uint _offset, uint _size)
{
	GL_DEBUG_POINT();
	ASSERT(_offset + _size <= m_used);
	ASSERT(_src || !_size);

	glNamedBufferSubDataEXT(m_handle, _offset, _size, _src);
}
//----------------------------------------------------------------------------//
void Buffer::Copy(Buffer* _src, uint _srcOffset, uint _dstOffset, uint _size)
{
	GL_DEBUG_POINT();
	ASSERT(_src != nullptr);
	ASSERT(_dstOffset + _size <= m_used);
	ASSERT(_srcOffset + _size <= _src->m_used);

	glNamedCopyBufferSubDataEXT(_src->m_handle, m_handle, _srcOffset, _dstOffset, _size);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Vertex
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
#if USE_HALF_FLOAT_TEXCOORD
	{ GL_HALF_FLOAT, 2, false, false, offsetof(Vertex, texcoord) },
#else
	{ GL_FLOAT, 2, false, false, offsetof(Vertex, texcoord) },
#endif
	{ GL_UNSIGNED_BYTE, 4, false, true, offsetof(Vertex, color) },
#if USE_BYTE_NORMALS
	{ GL_BYTE, 4, false, true, offsetof(Vertex, normal) },
#else
	{ GL_FLOAT, 3, false, false, offsetof(Vertex, normal) },
#endif
	{ GL_UNSIGNED_BYTE, 4, false, true, offsetof(Vertex, tangent) },
	{ GL_UNSIGNED_BYTE, 4, false, true, offsetof(Vertex, weights) },
	{ GL_UNSIGNED_BYTE, 4, true, false, offsetof(Vertex, indices) },
#if USE_HALF_FLOAT_TEXCOORD
	{ GL_HALF_FLOAT, 2, false, false, offsetof(Vertex, texcoord2) },
#else
	{ GL_FLOAT, 2, false, false, offsetof(Vertex, texcoord2) },
#endif
	{ GL_FLOAT, 2, false, false, offsetof(Vertex, size) },
	{ GL_FLOAT, 1, false, false, offsetof(Vertex, rotation) },
};

//----------------------------------------------------------------------------//
// VertexArray
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
VertexArray::VertexArray(PrimitiveType _type, bool _indexed, BufferUsage _usage) :
	m_type(_type)
{
	m_vertexBuffer = new Buffer(_usage);
	if (_indexed)
		m_indexBuffer = new Buffer(_usage);
}
//----------------------------------------------------------------------------//
VertexArray::~VertexArray(void)
{
}
//----------------------------------------------------------------------------//
void VertexArray::Bind(void)
{
	gGraphics->SetVertexBuffer(m_vertexBuffer);
	gGraphics->SetIndexBuffer(m_indexBuffer);
}
//----------------------------------------------------------------------------//
void VertexArray::Draw(uint _start, uint _count, uint _numInstances)
{
	if (m_indexBuffer)
		gGraphics->DrawIndexed(m_type, 0, _start, _count, _numInstances);
	else
		gGraphics->Draw(m_type, _start, _count, _numInstances);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Texture
//----------------------------------------------------------------------------//

struct GLPixelFormat
{
	uint16 bpp;
	uint16 iformat;
	uint16 format;
	uint16 type;
};
const GLPixelFormat GLPixelFormats[] =
{
	{ 24, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE }, //PF_RGB8
	{ 32, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE }, //PF_RGBA8
	{ 32, GL_RGB10_A2, GL_RGBA, GL_UNSIGNED_INT_2_10_10_10_REV }, //PF_RGB10A2
	{ 32, GL_R32F, GL_RED, GL_FLOAT }, //PF_R32F
	{ 64, GL_RG32F, GL_RG, GL_FLOAT }, //PF_RG32F
	{ 96, GL_RGB32F, GL_RGB, GL_FLOAT }, //PF_RGB32F
	{ 128, GL_RGBA32F, GL_RGBA, GL_FLOAT }, //PF_RGBA32F
	{ 64, GL_RGBA16F, GL_RGBA, GL_FLOAT }, //PF_RGBA16F
	{ 32, GL_R11F_G11F_B10F, GL_RGB, GL_UNSIGNED_INT_10F_11F_11F_REV }, //PF_RG11B10F
	{ 32, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8 }, //PF_D24S8
	{ 4, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, 0, 0 }, //PF_DXT1
	{ 8, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, 0, 0 }, //PF_DXT5
};

//----------------------------------------------------------------------------//
uint BitsPerPixel(PixelFormat _format)
{
	return GLPixelFormats[_format].bpp;
}
//----------------------------------------------------------------------------//
bool IsCompressed(PixelFormat _format)
{
	return _format >= PF_DXT1; //_format == PF_DXT1 || _format == PF_DXT5;
}
//----------------------------------------------------------------------------//

const uint16 GLTextureType[] =
{
	GL_TEXTURE_2D, // TT_2D
	GL_TEXTURE_3D, // TT_3D
	GL_TEXTURE_2D_ARRAY, // TT_Array
	GL_TEXTURE_CUBE_MAP, // TT_Cube
};

const uint16 MaxTextureDepth[] =
{
	1, // TT_2D
	4096, // TT_3D
	4096, // TT_Array
	6, // TT_Cube
};
const uint16 MinTextureDepth[] =
{
	1, // TT_2D
	1, // TT_3D
	1, // TT_Array
	6, // TT_Cube
};

//----------------------------------------------------------------------------//
Texture::Texture(TextureType _type, PixelFormat _format, bool _mipmap) :
	m_type(_type),
	m_format(_format),
	m_mipmap(_mipmap),
	m_width(0),
	m_height(0),
	m_depth(0),
	m_handle(0)
{
}
//----------------------------------------------------------------------------//
Texture::~Texture(void)
{
	GL_DEBUG_POINT();

	if(m_handle)
		glDeleteTextures(1, &m_handle);
}
//----------------------------------------------------------------------------//
void Texture::Realloc(uint _width, uint _height, uint _depth)
{
	GL_DEBUG_POINT();

	if (!_depth)
		_depth = m_depth;
	_depth = Clamp<uint>(_depth, MinTextureDepth[m_type], MaxTextureDepth[m_type]);

	if (m_width == _width && m_height == _height && m_depth == _depth) // no changes
		return;

	uint _target = GLTextureType[m_type];

	if(m_handle)
		glDeleteTextures(1, &m_handle);
	glGenTextures(1, &m_handle);
	glBindMultiTextureEXT(GL_TEXTURE0 + MAX_TEXTURE_UNITS, _target, m_handle);


	m_width = _width;
	m_height = _height;
	m_depth = _depth;

	uint _side = Max(m_width, m_height);
	uint _lods = (m_mipmap && _side) ? Log2i(_side) + 1 : 1;
	uint _iformat = GLPixelFormats[m_format].iformat;
	if (m_type == TT_2D || m_type == TT_Cube)
		glTextureStorage2DEXT(m_handle, _target, _lods, _iformat, m_width, m_height);
	else
		glTextureStorage3DEXT(m_handle, _target, _lods, _iformat, m_width, m_height, m_depth);
}
//----------------------------------------------------------------------------//
void Texture::Write(PixelFormat _format, const void* _src)
{
	GL_DEBUG_POINT();
	ASSERT(_src != nullptr);

	const GLPixelFormat& _pf = GLPixelFormats[_format];
	bool _compressed = IsCompressed(_format);
	uint _bpl = (m_width * m_height * _pf.bpp) >> 3;
	uint _bpi = _bpl * m_depth;
	uint _target = GLTextureType[m_type];

	if (m_type == TT_2D)
	{
		if (_compressed)
			glCompressedTextureSubImage2DEXT(m_handle, _target, 0, 0, 0, m_width, m_height, _pf.iformat, _bpi, _src);
		else
			glTextureSubImage2DEXT(m_handle, _target, 0, 0, 0, m_width, m_height, _pf.format, _pf.type, _src);
	}
	else if (m_type == TT_3D || m_type == TT_Array)
	{
		if (_compressed)
			glCompressedTextureSubImage3DEXT(m_handle, _target, 0, 0, 0, 0, m_width, m_height, m_depth, _pf.iformat, _bpi, _src);
		else
			glTextureSubImage3DEXT(m_handle, _target, 0, 0, 0, 0, m_width, m_height, m_depth, _pf.format, _pf.type, _src);
	}
	else // TT_Cube
	{
		for (uint i = 0; i < 6; ++i)
		{
			_target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
			const void* _img = ((const uint8*)_src) + _bpl * i;
			if (_compressed)
				glCompressedTextureSubImage2DEXT(m_handle, _target, 0, 0, 0, m_width, m_height, _pf.iformat, _bpl, _img);
			else
				glTextureSubImage2DEXT(m_handle, _target, 0, 0, 0, m_width, m_height, _pf.format, _pf.type, _img);
		}
	}
}
//----------------------------------------------------------------------------//
void Texture::Read(PixelFormat _format, void* _dst)
{
	GL_DEBUG_POINT();
	ASSERT(_dst != nullptr);

	const GLPixelFormat& _pf = GLPixelFormats[_format];
	bool _compressed = IsCompressed(_format);
	uint _target = GLTextureType[m_type];

	ASSERT(!_compressed || m_format == _format, "Pixel format is mismatched");

	if (m_type == TT_Cube)
	{
		uint _bpl = (m_width * m_height * BitsPerPixel(_format)) >> 3;
		for (uint i = 0; i < 6; ++i)
		{
			_target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
			void* _img = ((uint8*)_dst) + _bpl * i;
			if (_compressed)
				glGetCompressedTextureImageEXT(m_handle, _target, 0, _img);
			else
				glGetTextureImageEXT(m_handle, _target, 0, _pf.format, _pf.type, _img);
		}
	}
	else
	{
		if (_compressed)
			glGetCompressedTextureImageEXT(m_handle, _target, 0, _dst);
		else
			glGetTextureImageEXT(m_handle, _target, 0, _pf.format, _pf.type, _dst);
	}
}
//----------------------------------------------------------------------------//
void Texture::GenerateMipmap(void)
{
	if (m_mipmap && m_handle)
		glGenerateTextureMipmapEXT(m_handle, GLTextureType[m_type]);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// RenderBuffer
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
RenderBuffer::RenderBuffer(PixelFormat _format)	:
	m_format(_format),
	m_iformat(GLPixelFormats[_format].iformat),
	m_width(0),
	m_height(0),
	m_samples(0),
	m_handle(0)
{
	glGenRenderbuffers(1, &m_handle);
	glBindRenderbuffer(GL_RENDERBUFFER, m_handle);
}
//----------------------------------------------------------------------------//
RenderBuffer::~RenderBuffer(void)
{
	glDeleteRenderbuffers(1, &m_handle);
}
//----------------------------------------------------------------------------//
void RenderBuffer::Realloc(uint _width, uint _height, uint _samples)
{
	if (!_samples)
		_samples = m_samples;
	_samples = Clamp(_samples, 1u, 16u);

	if (m_width == _width && m_height == _height && m_samples == _samples)
		return;

	m_width = _width;
	m_height = _height;
	m_samples = _samples;
	glNamedRenderbufferStorageMultisampleEXT(m_handle, m_samples, m_iformat, m_width, m_height);
}
//----------------------------------------------------------------------------//
void RenderBuffer::CopyToTexture(Texture* _texture, uint _z)
{
	ASSERT(_texture != nullptr);
	ASSERT(_z < _texture->Depth());
	ASSERT(m_format != PF_D24S8 || _texture->Format() == PF_D24S8);

	uint _src = gGraphics->_SrcCopyFramebuffer();
	uint _dst = gGraphics->_SrcCopyFramebuffer();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _dst);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _src);

	uint _buffers;
	if (m_format == PF_D24S8)
	{
		_buffers = GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
		glNamedFramebufferRenderbufferEXT(_src, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_handle);
		glNamedFramebufferRenderbufferEXT(_src, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_handle);
		_BindRenderTargetTexture(_dst, GL_DEPTH_ATTACHMENT, _texture, _z);
		_BindRenderTargetTexture(_dst, GL_STENCIL_ATTACHMENT, _texture, _z);
	}
	else
	{
		_buffers = GL_COLOR_BUFFER_BIT;
		glNamedFramebufferRenderbufferEXT(_src, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_handle);
		_BindRenderTargetTexture(_dst, GL_COLOR_ATTACHMENT0, _texture, _z);
	}

	glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, _texture->Width(), _texture->Height(), _buffers, GL_NEAREST);
	_texture->GenerateMipmap();

	glBindFramebuffer(GL_FRAMEBUFFER, gGraphics->IsRenderTargetsEnabled() ? gGraphics->_GetFramebuffer() : 0);
}
//----------------------------------------------------------------------------//
void RenderBuffer::_BindRenderTargetTexture(uint _framebuffer, uint _attachment, Texture* _texture, uint _z)
{
	if (_texture)
	{
		uint _target = _texture->Type() == TT_Cube ? GL_TEXTURE_CUBE_MAP_POSITIVE_X + _z : GLTextureType[_texture->Type()];
		if (_texture->Type() == TT_Array || _texture->Type() == TT_3D)
			glNamedFramebufferTexture3DEXT(_framebuffer, _attachment, _target, _texture->Handle(), 0, _z);
		else
			glNamedFramebufferTexture2DEXT(_framebuffer, _attachment, _target, _texture->Handle(), 0);
	}
	else
		glNamedFramebufferTextureEXT(_framebuffer, _attachment, 0, 0);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Sampler
//----------------------------------------------------------------------------//

const uint16 GLTextureWrap[] =
{
	GL_REPEAT, // TW_Repeat
	GL_CLAMP_TO_EDGE, // TW_Clamp
};
const uint16 GLTextureMinFilter[] =
{
	GL_NEAREST, // TF_Nearest
	GL_LINEAR, // TF_Linear
	GL_NEAREST_MIPMAP_NEAREST, // TF_Bilinear
	GL_LINEAR_MIPMAP_LINEAR, // TF_Trilinear
};
const uint16 GLTextureMagFilter[] =
{
	GL_NEAREST, // TF_Nearest
	GL_LINEAR, // TF_Linear
	GL_LINEAR, // TF_Bilinear
	GL_LINEAR, // TF_Trilinear
};

//----------------------------------------------------------------------------//
Sampler::Sampler(void) :
	m_hash(0),
	m_handle(0)
{
}
//----------------------------------------------------------------------------//
Sampler::~Sampler(void)
{
	// glDeleteSamplers(1, &m_handle);
}
//----------------------------------------------------------------------------//
void Sampler::_Init(uint _hash, const Desc& _desc)
{
	GL_DEBUG_POINT();
	ASSERT(m_handle == 0); // init once

	m_hash = _hash;
	m_desc = _desc;

	glGenSamplers(1, &m_handle);
	glBindSampler(MAX_TEXTURE_UNITS, m_handle);
	glSamplerParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GLTextureMinFilter[m_desc.filter]);
	glSamplerParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GLTextureMagFilter[m_desc.filter]);
	glSamplerParameteri(m_handle, GL_TEXTURE_WRAP_S, GLTextureWrap[m_desc.wrap]);
	glSamplerParameteri(m_handle, GL_TEXTURE_WRAP_T, GLTextureWrap[m_desc.wrap]);
	glSamplerParameteri(m_handle, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_desc.anisotropy == TA_Max ? TA_X16 : m_desc.anisotropy);
	if (m_desc.depthFunc != CF_Never)
	{
		glSamplerParameteri(m_handle, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glSamplerParameteri(m_handle, GL_TEXTURE_COMPARE_FUNC, m_desc.depthFunc);
	}
}
//----------------------------------------------------------------------------//

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
	ShaderID id;
	const char* name;
	uint16 flags;
}
const g_shaderNames[] =
{
	{ ST_Vertex, VS_StaticModel, "Generic-VS.glsl", INSTANCED_BIT },
	{ ST_Vertex, VS_SkinnedModel, "Generic-VS.glsl", SKINNED_BIT },
	{ ST_Vertex, VS_Sprite, "Generic-VS.glsl", SPRITE_BIT },
	{ ST_Vertex, VS_Particles, "Generic-VS.glsl", SPRITE_BIT | PARTICLES_BIT },
	//{ ST_Vertex, VS_Terrain, "Generic-VS.glsl", TERRAIN_BIT },

	{ ST_Geometry, GS_Sprite, "Quad-GS.glsl", SPRITE_BIT },
	{ ST_Geometry, GS_Billboard, "Quad-GS.glsl", SPRITE_BIT | BILLBOARD_BIT },
	{ ST_Geometry, GS_BillboardY, "Quad-GS.glsl", SPRITE_BIT | BILLBOARD_Y_BIT },
	{ ST_Geometry, GS_Particles, "Quad-GS.glsl", SPRITE_BIT | PARTICLES_BIT },
	//{ ST_Geometry, GS_Terrain, "Quad-GS.glsl", SPRITE_BIT | TERRAIN_BIT },

	{ ST_Fragment, FS_Texture, "Generic-PS.glsl", TEXTURE_BIT },
	{ ST_Fragment, FS_NoTexture, "Generic-PS.glsl", 0 },

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
	//GL_DEBUG_POINT();

	//if (m_handle)
	//	glDeleteProgram(m_handle);
}
//----------------------------------------------------------------------------//
void Shader::_Init(ShaderType _type, const char* _common, const char* _src, uint16 _flags)
{
	GL_DEBUG_POINT();
	ASSERT(m_handle == 0); // init once
	ASSERT(_common != nullptr);
	ASSERT(_src != nullptr);

	m_type = _type;
	m_flags = _flags;

	char _header[256]; 
	sprintf(_header, 
		"#version 330 core\n"
		//"#extension GL_ARB_enhanced_layouts:enable\n"
		"#extension GL_ARB_shading_language_420pack:enable\n"
		"#extension GL_ARB_separate_shader_objects:enable\n"
		"#define FLAGS 0x%08x\n"
		"#define COMPILE_%s 1\n"
		"#define GLSL 330\n",
		_flags,
		ShaderPrefix[m_type]);

	String _srcs = _header;
	_srcs += _common;
	_srcs += "\n";
	_srcs += "\n#line 1 1\n";
	_srcs += _src;
	_srcs += "\n";

	const char* _srcp = _srcs;

	m_handle = glCreateShaderProgramv(GLShaderTypes[m_type], 1, &_srcp);

	//if(!HasOpenGLExtension("GL_ARB_shading_language_420pack") && GLVersion < 42) // no explicit binding
	//glUniformBlockBinding(m_handle, glGetUniformBlockIndex(m_handle, "UCamera"), U_Camera);
	//glUniformBlockBinding(m_handle, glGetUniformBlockIndex(m_handle, "UInstanceMat"), U_InstanceMat);

#ifdef _DEBUG_OUTPUT
	int _length;
	glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &_length);
	if (_length > 1)
	{
		Array<char> _log(_length + 1);
		glGetProgramInfoLog(m_handle, _length, &_length, *_log);
		_log[_length] = 0;
		LOG("%s", *_log);
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
		//CHECK(HasOpenGLExtension("GL_ARB_enhanced_layouts") || _ver >= 44, "No GL_ARB_enhanced_layouts extension"); // for layout(offset=n, align=n)
		CHECK(HasOpenGLExtension("GL_ARB_shading_language_420pack") || _ver >= 42, "No GL_ARB_shading_language_420pack extension"); // for layout(binding=n)
		CHECK(HasOpenGLExtension("GL_ARB_texture_storage") || _ver >= 42, "No GL_ARB_texture_storage extension");
		//CHECK(HasOpenGLExtension("GL_ARB_texture_storage_multisample") || _ver >= 43, "No GL_ARB_texture_storage_multisample extension");
		CHECK(HasOpenGLExtension("GL_EXT_direct_state_access"), "No GL_EXT_direct_state_access extension");
	}

	// debug output
#ifdef _DEBUG_OUTPUT
	if(glDebugMessageCallback)
	{
		g_DebugOutputEnabled = true;
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(&_DebugProc, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, false);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, true);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, true);
	}
#endif

	// depthstencil
	{
		m_numDepthStencilState = 0;
		m_currentDepthStencilState = 0;
		m_currentStencilRef = 0;
		AddDepthStencilState(DepthStencilState::Desc());
	}

	// sampler
	{
		m_numSamplers = 0;
		memset(m_currentSamplers, 0, sizeof(m_currentSamplers));
		AddSampler(Sampler::Desc()); // add default sampler
	}

	// shader
	{
		GL_DEBUG_POINT();

		RawData _commonFile = LoadFile("Common.h");
		_commonFile.Push('\0');
		const char* _common = (const char*)*_commonFile;
		for (m_numShaders = 0; m_numShaders < sizeof(g_shaderNames) / sizeof(g_shaderNames[0]); ++m_numShaders)
		{
			const ShaderName& _info = g_shaderNames[m_numShaders];

			LOG("Load Shader %d \"%s\" 0x%08x", _info.id, _info.name, _info.flags);

			RawData _file = LoadFile(_info.name);
			_file.Push('\0');
			const char* _src = (const char*)*_file;
			m_shaders[_info.id]._Init(_info.type, _common, _src, _info.flags);
		}

		//memset(m_currentShader, 0, sizeof(m_currentShader));
		m_shaderPipeline = 0;
		glGenProgramPipelines(1, &m_shaderPipeline);
		glBindProgramPipeline(m_shaderPipeline);
	}

	// geometry
	{
		m_vertexArray = 0;
		glGenVertexArrays(1, &m_vertexArray);
		glBindVertexArray(m_vertexArray);
	}

	// framebuffer
	{
	
		m_drawToFramebuffer = false;

		glGenFramebuffers(1, &m_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

		glGenFramebuffers(2, m_copyFramebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_copyFramebuffer[0]);
		glBindFramebuffer(GL_FRAMEBUFFER, m_copyFramebuffer[1]);

		memset(m_renderTargets, 0, sizeof(m_renderTargets));
		memset(&m_depthStencilTarget, 0, sizeof(m_depthStencilTarget));
	}

	// texture
	{
		memset(m_currentTextures, 0, sizeof(m_currentTextures));
	}

	_ResetState();
}
//----------------------------------------------------------------------------//
Graphics::~Graphics(void)
{
	LOG("Destroy Graphics");

	// delete vao
	// delete framebuffers
	// delete shaders
	// delete samplers
	// delete context
}
//----------------------------------------------------------------------------//
void Graphics::BeginFrame(void)
{
	_ResetState();
}
//----------------------------------------------------------------------------//
void Graphics::EndFrame(void)
{
#ifdef _DEBUG_OUTPUT
	bool _dbg = g_DebugOutputEnabled;
	g_DebugOutputEnabled = false;
#endif

	SwapBuffers(m_dc);

#ifdef _DEBUG_OUTPUT
	g_DebugOutputEnabled = _dbg;
#endif
}
//----------------------------------------------------------------------------//
DepthStencilStateID Graphics::AddDepthStencilState(const DepthStencilState::Desc& _desc)
{
	GL_DEBUG_POINT();

	uint _hash = Hash(&_desc, sizeof(_desc));
	for (uint i = 0; i < m_numDepthStencilState; ++i)
	{
		if (m_depthStencilState[i].m_hash == _hash)
			return i;
	}

	ASSERT(m_numDepthStencilState < MAX_DEPTHSTENCIL_STATE_OBJECTS);

	DepthStencilStateID _id = m_numDepthStencilState++;
	m_depthStencilState[_id]._Init(_hash, _desc);
	return _id;
}
//----------------------------------------------------------------------------//
void Graphics::SetDepthStencilState(DepthStencilStateID _state, uint _stencilRef)
{
	GL_DEBUG_POINT();
	ASSERT(_state < m_numDepthStencilState);

	if (m_currentDepthStencilState != _state || m_currentStencilRef != _stencilRef)
	{
		m_currentDepthStencilState = _state;
		m_currentStencilRef = _stencilRef;
		m_depthStencilState[_state]._Bind(_stencilRef);
	}
}
//----------------------------------------------------------------------------//
SamplerID Graphics::AddSampler(const Sampler::Desc& _desc)
{
	GL_DEBUG_POINT();

	uint _hash = Hash(&_desc, sizeof(_desc));
	for (uint i = 0; i < m_numSamplers; ++i)
	{
		if (m_samplers[i].m_hash == _hash)
			return i;
	}

	ASSERT(m_numSamplers < MAX_SAMPLER_OBJECTS);
	
	SamplerID _id = m_numSamplers++;
	m_samplers[_id]._Init(_hash, _desc);
	return _id;
}
//----------------------------------------------------------------------------//
void Graphics::SetSampler(uint _slot, SamplerID _sampler)
{
	GL_DEBUG_POINT();
	ASSERT(_slot < MAX_SAMPLER_OBJECTS);
	ASSERT(_sampler < m_numSamplers);

	if (m_currentSamplers[_slot] != _sampler)
	{
		m_currentSamplers[_slot] = _sampler;
		glBindSampler(_slot, m_samplers[_sampler].m_handle);
	}
}
//----------------------------------------------------------------------------//
void Graphics::EnableRenderTargets(bool _enabled)
{
	GL_DEBUG_POINT();

	if (m_drawToFramebuffer != _enabled)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _enabled ? m_framebuffer : 0);
		m_drawToFramebuffer = _enabled;
	}
}
//----------------------------------------------------------------------------//
void Graphics::ResetRenderTargets(void)
{
	GL_DEBUG_POINT();

	SetDepthStencilBuffer(nullptr);
	SetDepthStencilTargetTexture(nullptr);
	for (uint i = 0; i < MAX_RENDER_TARGETS; ++i)
	{
		SetRenderBuffer(i, nullptr);
		SetRenderTargetTexture(i, nullptr);
	}
}
//----------------------------------------------------------------------------//
void Graphics::SetRenderBuffer(uint _slot, RenderBuffer* _target)
{
	GL_DEBUG_POINT();
	ASSERT(_slot < MAX_RENDER_TARGETS);

	if (m_renderTargets[_slot].buffer != _target)
	{
		glNamedFramebufferRenderbufferEXT(m_framebuffer, GL_COLOR_ATTACHMENT0 + _slot, GL_RENDERBUFFER, _target ? _target->Handle() : 0);
		m_renderTargets[_slot].buffer = _target;
	}
}
//----------------------------------------------------------------------------//
void Graphics::SetDepthStencilBuffer(RenderBuffer* _target)
{
	GL_DEBUG_POINT();
	ASSERT(!_target || _target->Format() == PF_D24S8);

	if (m_depthStencilTarget.buffer != _target)
	{
		uint _handle = _target ? _target->Handle() : 0;
		glNamedFramebufferRenderbufferEXT(m_framebuffer, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _handle);
		glNamedFramebufferRenderbufferEXT(m_framebuffer, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _handle);
		m_depthStencilTarget.buffer = _target;
	}
}
//----------------------------------------------------------------------------//
void Graphics::SetRenderTargetTexture(uint _slot, Texture* _target, uint _z)
{
	GL_DEBUG_POINT();
	ASSERT(_slot < MAX_RENDER_TARGETS);

	FramebufferAttachment& _dst = m_renderTargets[_slot];
	if (_dst.texture != _target || _dst.textureZ != _z)
	{
		RenderBuffer::_BindRenderTargetTexture(m_framebuffer, GL_COLOR_ATTACHMENT0 + _slot, _target, _z);
		_dst.texture = _target; 
		_dst.textureZ = _z;
	}
}
//----------------------------------------------------------------------------//
void Graphics::SetDepthStencilTargetTexture(Texture* _target, uint _z)
{
	GL_DEBUG_POINT();
	ASSERT(!_target || _target->Format() == PF_D24S8);

	if (m_depthStencilTarget.texture != _target || m_depthStencilTarget.textureZ != _z)
	{
		RenderBuffer::_BindRenderTargetTexture(m_framebuffer, GL_DEPTH_ATTACHMENT, _target, _z);
		RenderBuffer::_BindRenderTargetTexture(m_framebuffer, GL_STENCIL_ATTACHMENT, _target, _z);
		m_depthStencilTarget.texture = _target;
		m_depthStencilTarget.textureZ = _z;
	}
}
//----------------------------------------------------------------------------//
void Graphics::ResetTextures(void)
{
	for (uint i = 0; i < MAX_TEXTURE_UNITS; ++i)
		SetTexture(i, nullptr);
}
//----------------------------------------------------------------------------//
void Graphics::SetTexture(uint _slot, Texture* _texture)
{
	GL_DEBUG_POINT();
	ASSERT(_slot < MAX_TEXTURE_UNITS);

	if (m_currentTextures[_slot] != _texture)
	{
		uint _target = GL_TEXTURE_2D, _handle = 0;
		if (_texture)
			_target = GLTextureType[_texture->Type()], _handle = _texture->Handle();
		glBindMultiTextureEXT(GL_TEXTURE0 + _slot, _target, _handle);
		m_currentTextures[_slot] = _texture;
	}
}
//----------------------------------------------------------------------------//
void Graphics::ClearFrameBuffers(uint _buffers, const Color& _color, float _depth, uint _stencil)
{
	GL_DEBUG_POINT();

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
	GL_DEBUG_POINT();

	if (m_currentVertexBuffer != _buffer)
	{
		glBindVertexBuffer(0, _buffer ? _buffer->Handle() : 0, 0, sizeof(Vertex));
		m_currentVertexBuffer = _buffer;
	}
}
//----------------------------------------------------------------------------//
void Graphics::SetIndexBuffer(Buffer* _buffer)
{
	GL_DEBUG_POINT();

	if (m_currentIndexBuffer != _buffer)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer ? _buffer->Handle() : 0);
		m_currentIndexBuffer = _buffer;
	}
}
//----------------------------------------------------------------------------//
void Graphics::SetUniformBuffer(uint _slot, Buffer* _buffer, uint _base)
{
	GL_DEBUG_POINT();
	ASSERT(_buffer != nullptr);
	ASSERT(_base <= _buffer->Size());
	// todo: check alignment

	glBindBufferRange(GL_UNIFORM_BUFFER, _slot, _buffer->Handle(), _base, _buffer->Size() - _base);
}
//----------------------------------------------------------------------------//
void Graphics::SetShader(ShaderType _type, Shader* _shader)
{
	GL_DEBUG_POINT();

	if (m_currentShader[_type] != _shader)
	{
		glUseProgramStages(m_shaderPipeline, 1 << _type, _shader ? _shader->Handle() : 0);
		m_currentShader[_type] = _shader;
	}
}
//----------------------------------------------------------------------------//
void Graphics::Draw(PrimitiveType _type, uint _start, uint _count, uint _numInstances)
{
	GL_DEBUG_POINT();
	ASSERT(m_currentVertexBuffer != nullptr);

	/*glBindProgramPipeline(m_shaderPipeline);
	glValidateProgramPipeline(m_shaderPipeline);
	int _status;
	glGetProgramPipelineiv(m_shaderPipeline, GL_VALIDATE_STATUS, &_status);
	if (!_status)
	{
		int _length;
		glGetProgramPipelineiv(m_shaderPipeline, GL_INFO_LOG_LENGTH, &_length);
		if (_length > 1)
		{
			Array<char> _log(_length + 1);
			glGetProgramPipelineInfoLog(m_shaderPipeline, _length, &_length, *_log);
			_log[_length] = 0;
			LOG("%s", *_log);
		}
	} */

	glDrawArraysInstanced(GLPrimitiveType[_type], _start, _count, _numInstances);
}
//----------------------------------------------------------------------------//
void Graphics::DrawIndexed(PrimitiveType _type, uint _baseVertex, uint _start, uint _count, uint _numInstances)
{
	GL_DEBUG_POINT();
	ASSERT(m_currentVertexBuffer != nullptr);
	ASSERT(m_currentIndexBuffer != nullptr);

	glDrawElementsInstancedBaseVertex(GLPrimitiveType[_type], _count, GL_UNSIGNED_SHORT, (const void*)(_start * sizeof(index_t)), _numInstances, _baseVertex);
}
//----------------------------------------------------------------------------//
void Graphics::_ResetState(void)
{
	GL_DEBUG_POINT();

	// depthstencil
	m_currentDepthStencilState = 0;
	m_depthStencilState[0]._Bind(0);

	// sampler

	// shaders
	memset(m_currentShader, 0, sizeof(m_currentShader));
	glBindProgramPipeline(m_shaderPipeline);
	glUseProgramStages(m_shaderPipeline, GL_VERTEX_SHADER_BIT | GL_FRAGMENT_SHADER_BIT | GL_GEOMETRY_SHADER_BIT, 0);

	// geometry
	glBindVertexArray(m_vertexArray);
	m_currentVertexBuffer = nullptr;
	m_currentIndexBuffer = nullptr;

	// vertex format
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

	// framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ResetRenderTargets();

	for (uint i = 0; i < 2; ++i)
	{
		glNamedFramebufferTextureEXT(m_copyFramebuffer[i], GL_COLOR_ATTACHMENT0, 0, 0);
		glNamedFramebufferTextureEXT(m_copyFramebuffer[i], GL_DEPTH_ATTACHMENT, 0, 0);
		glNamedFramebufferTextureEXT(m_copyFramebuffer[i], GL_STENCIL_ATTACHMENT, 0, 0);
	}

	// textures
	ResetTextures();
	//ResetSamplers();

	//glPrimitiveRestartIndex(0xffff);

}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
