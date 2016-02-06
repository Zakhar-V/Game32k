
struct PS_INPUT
{
	float4 tc : TEXCOORD;
	float4 color : COLOR;
};

sampler2D tex;

float4 main(PS_INPUT _in) : COLOR
{
	//return _in.color.wzyx /* tex2D(tex, _in.tc * 4)*/;

	return _in.color.wzyx * tex2D(tex, _in.tc);
}