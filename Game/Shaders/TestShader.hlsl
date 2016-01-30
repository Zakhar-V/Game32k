#include "Constants.h"

float4 main( float4 pos : POSITION, float4 color : COLOR, float4 tc : TEXCOORD) : SV_POSITION
{
	return mul(pos, WorldMat);
}			  