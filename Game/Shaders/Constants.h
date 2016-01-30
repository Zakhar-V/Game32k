#ifndef __ShaderConstants_h__
#define __ShaderConstants_h__

#ifndef HLSL // in cpp
#	define cbuffer struct
#	define float4x4 Mat44
#	define REGISTER(T, I)
#	define CBUFFER(Name, Index) enum : unsigned { CB_##Name = Index }; struct Name
#else // in hlsl
#	define REGISTER(T, I) : register (T[I])
#	define CBUFFER(Name, Index) cbuffer Name : register(c[Index])
#endif


CBUFFER(PerFrame, 0)
{
	float4x4 WorldMat;
};

#endif//__ShaderConstants_h__
