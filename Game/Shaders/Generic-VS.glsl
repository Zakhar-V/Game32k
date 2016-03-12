
void main()
{
	OutInstanceID = gl_InstanceID;
	OutPos = InPos;

#if SKINNED
	OutWorldPos = InPos; // TODO:
#elif PARTICLES || FSQUAD
	OutWorldPos = InPos;
#else
	OutWorldPos = WorldMat[gl_InstanceID] * vec4((InPos.xyz + InNormal * SilhouetteOffset), 1);
#endif

	OutColor = InColor;
	OutTexCoord = InTexCoord;

#if SPRITE
	OutSize = InSize;
	OutRot = InRot;
	OutTexCoord2 = (InTexCoord2 - InTexCoord);
#else
	OutNormal = normalize(InNormal*NormMat);
	// TODO: tangent, binormal
#endif

#if FSQUAD
	gl_Position = OutWorldPos;
#else
	gl_Position = ViewProjMat * OutWorldPos;
#endif
	OutLogZ = gl_Position.w*Depth.z + 1;
}  
