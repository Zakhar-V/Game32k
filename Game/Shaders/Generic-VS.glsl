
void main()
{
	OutPos = InPos;
	OutWorldPos = WorldMat[gl_InstanceID] * InPos;

#if SKINNED

	
#endif
	gl_Position = ViewProjMat * OutWorldPos;
}
