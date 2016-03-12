USAMPLER(0, 2D, ColorMap);

void main()
{
	gl_FragDepth = log(InLogZ)*(Depth.w - DepthBias);

	vec4 mtlColor = InstanceMtl[InInstanceID][0];
	vec4 mtlParams = InstanceMtl[InInstanceID][1]; // shininess, emission


#if TEXTURE
	//OutColor = mtlColor * texture(ColorMap, InTexCoord);
	OutColor = mtlColor * texture(ColorMap, InTexCoord);
	//OutColor = InColor;
#else
	OutColor = mtlColor;

	//gl_FragDepth = log(InLogZ)*(Depth.w - 0.001);
	//gl_FragDepth = log(InLogZ)*(Depth.w - DepthBias);
	//OutColor = vec4(InWorldPos * 2);
	//OutColor = vec4(InWorldPos.y, InWorldPos.y, gl_FragDepth, 1);
	//OutColor = vec4(gl_FragDepth, gl_FragDepth, 0, 1);
	//OutColor = vec4(InTexCoord, InLogZ, 1);
	//OutColor = vec4(InNormal, 1);
#endif
	OutNormal = vec4(InNormal, 1);
	OutMaterial = mtlParams;
	//OutOverlay = OutColor * (1 - mtlParams.y * 1 - OutColor.a);

	//gl_FragDepth = InLogZ;
	//gl_FragDepth = InLogZ;
}