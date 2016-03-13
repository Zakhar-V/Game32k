USAMPLER(0, sampler2D, ColorMap);
USAMPLER(1, sampler2D, NormalMap);
USAMPLER(2, sampler2D, MaterialMap);
USAMPLER(3, sampler2D, DepthMap);

void main()
{
	float depth = texture(DepthMap, InTexCoord).r;
	vec4 color = texture(ColorMap, InTexCoord);
	vec4 normal = texture(NormalMap, InTexCoord);
	vec4 material = texture(MaterialMap, InTexCoord); // shininess, emission

#if DIRECTIONAL_LIGHT
	// restore depth
	// restore position
	// resore normal
	vec4 light = vec4(color); // TODO
	//OutColor = mix(light, color, material.y);
	OutColor = color;
#elif CEL_SHADE
	// edge detect
	// do saturate normals
	OutColor = color;
#endif

}
