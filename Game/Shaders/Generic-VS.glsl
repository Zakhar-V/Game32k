
void main()
{
	OutInstanceID = gl_InstanceID;

	OutPos = InPos;
	OutWorldPos = WorldMat[gl_InstanceID] * InPos;

	OutColor = InColor;
	OutTexCoord = InTexCoord;

#if SPRITE || BILLBOARD
	OutSize = InSize;
	OutRot = InRot;
	OutTexCoord2 = InTexCoord2;
#else

#endif

#if SKINNED

#endif
	gl_Position =  ViewProjMat * vec4(InPos.xyz, 1.0);// *OutWorldPos;
}
