
void main()
{
#if TEXTURE
	//OutColor = texture(ColorMap, InTexCoord) * InColor;
	OutColor = vec4(InTexCoord, InLogZ, 1);
#else
	//OutColor = InColor;

	OutColor = vec4(InWorldPos * 2);
#endif

	//gl_FragDepth = InLogZ;
	//gl_FragDepth = InLogZ;
	gl_FragDepth = log(InLogZ)*Depth.w;
	OutColor = vec4(InWorldPos.y, InWorldPos.y, gl_FragDepth, 1);
}