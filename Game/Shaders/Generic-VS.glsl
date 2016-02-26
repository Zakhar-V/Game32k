
void main()
{
	OutInstanceID = gl_InstanceID;

	OutPos = InPos;
#if PARTICLES
	OutWorldPos = InPos;
#else
	OutWorldPos = WorldMat[gl_InstanceID] * InPos;
#endif

	OutColor = InColor;
	OutTexCoord = InTexCoord;

#if SPRITE
	OutSize = InSize;
	OutRot = InRot;
	OutTexCoord2 = InTexCoord2 - InTexCoord;
#else

#endif

#if SKINNED

#endif
	gl_Position =  ViewProjMat * OutWorldPos;
}
