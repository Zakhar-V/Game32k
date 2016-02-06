
struct PS_INPUT
{
	float4 color : COLOR;
};

float4 main(PS_INPUT _in) : COLOR
{
	// in  rgba
	// clr argb
	// shd yzwx
	return _in.color.yzwx;
}