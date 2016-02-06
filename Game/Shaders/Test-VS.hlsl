#include "Constants.h"

struct VS_OUTPUT
{
	float4 pos : POSITION;
	float4 tc : TEXCOORD;
	float4 color : COLOR;
};

VS_OUTPUT main( float4 pos : POSITION, float4 color : COLOR, float4 tc : TEXCOORD)
{
	VS_OUTPUT _out;
	_out.pos = mul(pos, ViewProjMatrix);
	_out.color = color;
	_out.tc = tc;
	return _out;
}			  