#ifndef __ShaderConstants_h__
#define __ShaderConstants_h__

#ifndef HLSL // in cpp
#	define cbuffer struct
#	define float4x4 Mat44
#	define REGISTER(T, I)
#	define CBUFFER(Name, Index) enum : unsigned { SP_##Name = Index }; struct Name
#else // in hlsl
#	define REGISTER(T, I) : register (T[I])
#	define CBUFFER(Name, Index) cbuffer Name : register(c[Index])
#endif

CBUFFER(UCameraVS, 0)
{
	float4x4 ViewMatrix;
	float4x4 ProjMatrix;
};

#ifdef HLSL
uniform float4x4 ViewProjMatrix : register(c0);
uniform float4x4 WorldMatrix : register(c4);
#endif

/*CBUFFER(UObjectVS, 4)
{
	float4x4 WorldMatrix;
};*/

#endif//__ShaderConstants_h__
