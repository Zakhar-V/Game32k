#ifndef _ShaderCommon_h__
#define _ShaderCommon_h__


// VS/GS flags
#define INSTANCED_BIT 0x1
#define SKINNED_BIT 0x2
#define SPRITE_BIT 0x4
#define BILLBOARD_BIT 0x8
#define TERRAIN_BIT 0x10


// PS flags
#define TEXTURE_BIT 0x1


#ifdef GLSL
#define INSTANCED (FLAGS & INSTANCED_BIT)
#define SKINNED (FLAGS & SKINNED_BIT)
#define SPRITE (FLAGS & SPRITE_BIT)
#define BILLBOARD (FLAGS & BILLBOARD_BIT)
#define TERRAIN (FLAGS & TERRAIN_BIT)
#endif


// constants
#define MAX_INSTANCES 1000 // 1000 * sizeof(mat4) = 47kb (max = 64kb)
#define MAX_BONES 64


// c++/glsl
#ifdef GLSL
#	define UBUFFER(Id, Name) layout(binding = Id, std140) uniform U##Name
#else
#	define vec2 Vec2
#	define vec3 Vec3
#	define vec4 Vec4
#	define mat3 Mat33
#	define mat4 Mat44
#	define UBUFFER(Id, Name) enum {U_##Name = Id}; struct U##Name
#	define layout(...)
#endif // GLSL

UBUFFER(1, Camera)
{
	layout(row_major) mat4 ViewMat;
	layout(row_major) mat4 ProjMat;
	layout(row_major) mat4 ViewProjMat;
	//mat3 NormMat; // inverse(mat3(ViewMat))
	//vec2 Depth;
};

UBUFFER(2, InstanceMat)
{
	layout(row_major) mat4 WorldMat[MAX_INSTANCES];
};

UBUFFER(3, SkinMat)
{
	layout(row_major) mat4 SkinMat[MAX_BONES];
};

UBUFFER(4, ClipPlane)
{
	vec4 ClipPlane[6];
	int NumClipPlanes;
};


#ifdef GLSL

#ifndef COMPILE_VS
#	define COMPILE_VS 0
#endif
#ifndef COMPILE_GS
#	define COMPILE_GS 0
#endif
#ifndef COMPILE_FS
#	define COMPILE_FS 0
#endif
#define USE_GS (FLAGS & (SPRITE | BILLBOARD | TERRAIN))

#if COMPILE_GS
#	define GS_IN_SUFFIX []
#else
#	define GS_IN_SUFFIX
#endif
#define IN(S, T, N) layout(location = S) in T In##N GS_IN_SUFFIX
#define IN_FLAT(S, T, N) layout(location = S) flat in T In
#define OUT(S, T, N) layout(location = S) out T Out##N
#define INOUT(S, T, N) IN(S, T, N); OUT(S, T, N);

#if COMPILE_VS
INOUT(0, vec4, Pos);
INOUT(1, vec2, TexCoord);
INOUT(2, vec2, Color);
INOUT(3, vec3, Normal);
INOUT(4, vec4, Tangent);
IN(5, vec4, Weights);
IN(6, vec4, Indices);
INOUT(7, vec2, TexCoord2);
INOUT(8, vec2, Size);
INOUT(9, float, Rot);

OUT(10, vec4, WorldPos);
OUT(11, vec3, Binormal);

OUT(12, int, InstanceID);

out gl_PerVertex
{
	vec4 gl_Position;
	//float gl_PointSize;
	float gl_ClipDistance[];
};
#endif  

#if COMPILE_GS
IN(0, vec4, Pos);
INOUT(1, vec2, TexCoord);
INOUT(2, vec2, Color);
INOUT(3, vec3, Normal);
INOUT(4, vec4, Tangent);
//IN(5, vec4, Weights);
//IN(6, vec4, Indices);
IN(7, vec2, TexCoord2);
IN(8, vec2, Size);
IN(9, float, Rot);

INOUT(10, vec4, WorldPos);
INOUT(11, vec3, Binormal);

INOUT(12, int, InstanceID);

in gl_PerVertex
{
	vec4 gl_Position;
	//float gl_PointSize;
	float gl_ClipDistance[];
} gl_in[]; 

out gl_PerVertex
{
	vec4 gl_Position;
	//float gl_PointSize;
	float gl_ClipDistance[];
};
#endif

#if COMPILE_FS
IN(1, vec2, TexCoord);
IN(2, vec2, Color);
IN(3, vec3, Normal);
IN(4, vec3, Tangent);
IN(10, vec4, WorldPos);
IN(11, vec3, Binormal);
IN_FLAT(12, int, InstanceID);

OUT(0, vec4, Color);
#endif

#endif // GLSL
#endif//_ShaderCommon_h__