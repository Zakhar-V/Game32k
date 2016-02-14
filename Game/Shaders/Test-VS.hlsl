#include "Constants.h"

struct VS_OUTPUT
{
	float4 pos : POSITION;
	float2 tc : TEXCOORD;
	float4 color : COLOR;
};

VS_OUTPUT main( float4 pos : POSITION, float4 sprite : TEXCOORD1, float4 color : COLOR, float4 tc : TEXCOORD0)
{
	float4 wpos = mul(pos + float4(sprite.x, sprite.y, 0, 0), WorldMatrix);
	//float4 wpos = pos;
	VS_OUTPUT _out;
	_out.pos = mul(wpos, ViewProjMatrix);
	_out.color = color;
	//_out.tc = tc;
	_out.tc = tc.xy;
	return _out;
}			  