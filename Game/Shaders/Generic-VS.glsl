
void main()
{
	OutInstanceID = gl_InstanceID;
	OutPos = InPos;

#if SKINNED
	OutWorldPos = InPos; // TODO:
#elif !PARTICLES
	OutWorldPos = WorldMat[gl_InstanceID] * vec4((InPos.xyz + InNormal * SilhouetteOffset), 1);
#else
	OutWorldPos = InPos;
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

	gl_Position =  ViewProjMat * OutWorldPos;
	OutLogZ = gl_Position.w*Depth.z + 1;
}  
