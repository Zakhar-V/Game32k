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

#define SC_CameraVS 0
#define SC_ObjectVS 4

CBUFFER(CameraParamsVS, SC_CameraVS)
{
};

#ifdef HLSL
uniform matrix<float, 4, 4> ViewProjMatrix : register(c0);
#endif

CBUFFER(ObjectParamsVS, SC_ObjectVS)
{
	float4x4 WorldMatrix;
};

#endif//__ShaderConstants_h__
