
void main()
{
	OutInstanceID = gl_InstanceID;

	OutPos = InPos;
#if PARTICLES || TERRAIN
	OutWorldPos = InPos;
#else
	OutWorldPos = WorldMat[gl_InstanceID] * InPos;
#endif

	OutColor = InColor;
	OutTexCoord = InTexCoord;

#if SPRITE || TERRAIN
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
