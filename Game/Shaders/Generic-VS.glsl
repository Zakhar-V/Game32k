
void main()
{
	OutInstanceID = gl_InstanceID;
	OutPos = InPos;
	OutWorldPos = InPos;
#if !PARTICLES
	OutWorldPos = WorldMat[gl_InstanceID] * OutWorldPos;
#endif

	OutColor = InColor;
	OutTexCoord = InTexCoord;

	OutNormal = normalize(InNormal*NormMat);

#if SPRITE
	OutSize = InSize;
	OutRot = InRot;
	OutTexCoord2 = (InTexCoord2 - InTexCoord);
#else

#endif

#if SKINNED

#endif
	gl_Position =  ViewProjMat * OutWorldPos;
	OutLogZ = gl_Position.w*Depth.z + 1;
}  
