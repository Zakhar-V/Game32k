#ifndef _ShaderCommon_h_
#define _ShaderCommon_h_

// VS/GS flags
#define INSTANCED_BIT 0x1
#define SKINNED_BIT 0x2

#define SPRITE_BIT 0x4
#define BILLBOARD_BIT 0x8
#define BILLBOARD_Y_BIT 0x10
#define PARTICLES_BIT 0x20

//#define TERRAIN_BIT 0x40
#define FSQUAD_BIT 0x80

// PS flags
#define TEXTURE_BIT 0x1

#define CEL_SHADE_BIT 0x2

#define LIGHTING_BIT 0x4
#define DIRECTIONAL_LIGHT_BIT 0x8
#define SPOT_LIGHT_BIT 0x10
#define POINT_LIGHT_BIT 0x20

// constants
#define MAX_INSTANCES 512
#define MAX_BONES 64

#define C_POLYGON_OFFSET 5e-4
#define C_SILHOUETTE_OFFSET 5e-4

// c++/glsl
#ifdef GLSL
#	define UBUFFER(Id, Name) layout(binding = Id, std140) uniform U##Name
#	define USAMPLER(Id, T, Name) layout(binding = Id) uniform T Name
#else
#	define ivec2 Vec2i
#	define vec2 Vec2
#	define vec3 Vec3
#	define vec4 Vec4
#	define mat3 Mat34
#	define mat2x4 Mat24
#	define mat3x4 Mat34
#	define mat4 Mat44
#	define UBUFFER(Id, Name) enum {U_##Name = Id}; struct U##Name
#	define USAMPLER(Id, T, Name) enum {U_##Name = Id};
#	define layout(...)
#endif // GLSL

#define ROWMAJOR layout(row_major)

UBUFFER(1, Camera)
{
	ROWMAJOR mat4 ViewMat;
	ROWMAJOR mat4 ProjMat;
	ROWMAJOR mat4 ViewProjMat;
	ROWMAJOR mat4 InvViewProjMat;
	ROWMAJOR mat3 NormMat; // inverse(mat3(ViewMat))
	vec4 CameraPos;
	vec4 Depth;	// near, far, C=constant, FC = 1.0/log(far*C + 1)
	ivec2 ScreenSize;
	vec2 InvScreenSize;
};

UBUFFER(2, InstanceMat)
{
	ROWMAJOR mat4 WorldMat[MAX_INSTANCES];
};
UBUFFER(5, InstanceMtl)
{
	mat2x4 InstanceMtl[MAX_INSTANCES];
};

UBUFFER(3, SkinMat)
{
	ROWMAJOR mat4 SkinMat[MAX_BONES];
};

UBUFFER(4, ClipPlane) // TODO: remove it
{
	vec4 ClipPlane[7];
	int NumClipPlanes;
};

UBUFFER(7, RasterizerParams)
{
	float DepthBias; // default is 0 or 1e-5
	float SilhouetteOffset;
};

#ifdef GLSL

#define VARGS(A)  #A

#ifndef COMPILE_VS
#define COMPILE_VS 0
#endif
#ifndef COMPILE_GS
#define COMPILE_GS 0
#endif
#ifndef COMPILE_FS
#define COMPILE_FS 0
#endif

#if COMPILE_GS
#define GS_IN_SUFFIX []
#else
#define GS_IN_SUFFIX
#endif

#define _INOUT(S, IO, T, P, N) layout(location = S) IO T P##N
#define IN(S, T, N) _INOUT(S, in, T, In, N) GS_IN_SUFFIX
#define IN_FLAT(S, T, N) _INOUT(S, flat in, T, In, N) GS_IN_SUFFIX 
#define OUT_FLAT(S, T, N) _INOUT(S, flat out, T, Out, N) 
#define OUT(S, T, N) _INOUT(S, out, T, Out, N)
#define INOUT(S, T, N) IN(S, T, N); OUT(S, T, N)
#define INOUT_FLAT(S, T, N) IN_FLAT(S, T, N); OUT_FLAT(S, T, N)

#define _PerVertex gl_PerVertex { vec4 gl_Position; }

#if COMPILE_VS
INOUT(0, vec4, Pos);
INOUT(1, vec2, TexCoord);
INOUT(2, vec4, Color);
INOUT(3, vec3, Normal);
INOUT(4, vec4, Tangent);
IN(5, vec4, Weights);
IN(6, vec4, Indices);
INOUT(7, vec2, TexCoord2);
INOUT(8, vec2, Size);
INOUT(9, float, Rot);
OUT(10, vec4, WorldPos);
OUT(11, vec3, Binormal);
OUT_FLAT(12, int, InstanceID);
OUT(13, float, LogZ);
out _PerVertex;
#endif  

#if COMPILE_GS
IN(0, vec4, Pos);
INOUT(1, vec2, TexCoord);
INOUT(2, vec4, Color);
INOUT(3, vec3, Normal);
INOUT(4, vec4, Tangent);
//IN(5, vec4, Weights);
//IN(6, vec4, Indices);
IN(7, vec2, TexSize);
IN(8, vec2, Size);
IN(9, float, Rot);
INOUT(10, vec4, WorldPos);
INOUT(11, vec3, Binormal);
INOUT_FLAT(12, int, InstanceID);
OUT(13, float, LogZ);
in _PerVertex gl_in[];
out _PerVertex;
#endif

#if COMPILE_FS
IN(1, vec2, TexCoord);
IN(2, vec4, Color);
IN(3, vec3, Normal);
IN(4, vec3, Tangent);
IN(10, vec4, WorldPos);
IN(11, vec3, Binormal);
IN_FLAT(12, int, InstanceID);
IN(13, float, LogZ);
OUT(0, vec4, Color);
OUT(1, vec4, Normal);
OUT(2, vec4, Material);
#endif

#define UNIT_X vec3(1, 0, 0)
#define UNIT_Y vec3(0, 1, 0)
#define UNIT_Z vec3(0, 0, 1)

#endif // GLSL

#endif//_ShaderCommon_h_