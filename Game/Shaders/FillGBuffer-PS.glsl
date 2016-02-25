
void main()
{
#if TEXTURE
	OutColor = texture(ColorMap, InTexCoord) * InColor;
#else
	//OutColor = InColor;
	OutColor = vec4(1);
#endif
}