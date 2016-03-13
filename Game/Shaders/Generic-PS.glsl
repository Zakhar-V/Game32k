USAMPLER(0, sampler2D, ColorMap);

void main()
{
	gl_FragDepth = log(InLogZ)*(Depth.w - DepthBias);

	vec4 mtlColor = InstanceMtl[InInstanceID][0];
	vec4 mtlParams = InstanceMtl[InInstanceID][1]; // shininess, emission

#if TEXTURE
	OutColor = mtlColor * texture(ColorMap, InTexCoord);
#else
	OutColor = mtlColor;
#endif
	OutNormal = vec4(InNormal, 1);
	OutMaterial = mtlParams;
}