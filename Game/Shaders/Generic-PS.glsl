
void main()
{
	gl_FragDepth = log(InLogZ)*(Depth.w - DepthBias);

#if TEXTURE
	//OutColor = texture(ColorMap, InTexCoord).rrra * 0.7 + vec4(InTexCoord, InLogZ, 1) * 0.3;
	OutColor = InColor;
#else
	//OutColor = InColor;

	//gl_FragDepth = log(InLogZ)*(Depth.w - 0.001);
	//gl_FragDepth = log(InLogZ)*(Depth.w - DepthBias);
	//OutColor = vec4(InWorldPos * 2);
	//OutColor = vec4(InWorldPos.y, InWorldPos.y, gl_FragDepth, 1);
	//OutColor = vec4(gl_FragDepth, gl_FragDepth, 0, 1);
	//OutColor = vec4(InTexCoord, InLogZ, 1);
	OutColor = vec4(InNormal, 1);
#endif

	//gl_FragDepth = InLogZ;
	//gl_FragDepth = InLogZ;
}