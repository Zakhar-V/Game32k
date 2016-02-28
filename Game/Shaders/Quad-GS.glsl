layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

void Vertex(in vec3 offset, in vec2 coord)
{
	OutInstanceID = InInstanceID[0];
	OutTexCoord = InTexCoord[0] + InTexSize[0] * coord;
	OutWorldPos = vec4(InPos[0].xyz + offset, 1);
#if TERRAIN
	OutWorldPos.y += (InRot[0] * (texture(HeightMap, OutTexCoord).r * 2 - 1));
#endif
#if !PARTICLES
	OutWorldPos = WorldMat[InInstanceID[0]] * OutWorldPos;
#endif
	OutColor = InColor[0];
	gl_Position = ViewProjMat * OutWorldPos;
	OutLogZ = gl_Position.w*Depth.z + 1;
	EmitVertex();
}

void main()
{
#if TERRAIN
#define XY xzy
	mat3 mat = mat3(UNIT_X * InSize[0].x, UNIT_Y, UNIT_Z * InSize[0].y);
#else
#define XY xyz
	mat3 mat = mat3(UNIT_X * InSize[0].x, UNIT_Y * InSize[0].y, UNIT_Z);
#endif

#if BILLBOARD_Y
	mat = mat3(NormMat[0], UNIT_Y, UNIT_Z) * mat;
#elif BILLBOARD || PARTICLES
	mat = NormMat * mat;
#endif	

	Vertex(mat * vec3(.5, .5, 0).XY, vec2(1, 1)); // rb
	Vertex(mat * vec3(.5, -.5, 0).XY, vec2(1, 0)); // rt
	Vertex(mat * vec3(-.5, .5, 0).XY, vec2(0, 1)); // lb
	Vertex(mat * vec3(-.5, -.5, 0).XY, vec2(0, 0)); // lt

	EndPrimitive();
}