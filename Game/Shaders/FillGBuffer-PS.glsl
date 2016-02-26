
void main()
{
#if TEXTURE
	OutColor = texture(ColorMap, InTexCoord) * InColor;
#else
	//OutColor = InColor;

	OutColor = vec4(InWorldPos * 2);
#endif
}