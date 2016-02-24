layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

void Vertex(in vec3 offset, in vec2 coord)
{
	OutInstanceID = InInstanceID[0];

	OutWorldPos = vec4(InPos[0].xyz + offset, 1.0);
#if !PARTICLES
	OutWorldPos = WorldMat[InInstanceID[0]] * OutWorldPos;
#endif
	OutTexCoord = InTexCoord[0] + InTexSize[0] * coord;
	OutColor = InColor[0];
	gl_Position = ViewProjMat * OutWorldPos;
	EmitVertex();
}

void main()
{
	vec3 up, right;
	vec3 scale = vec3(InSize[0].xy, 1.0) * 0.5;
#if BILLBOARD_Y
	up = vec3(0.0, 1.0, 0.0);
	right = cross(normalize(CameraPos.xyz - InWorldPos[0].xyz), up);
#elif BILLBOARD || PARTICLES
	up = ViewMat[1].xyz;
	right = ViewMat[0].xyz;
#else
	up = vec3(0.0, 1.0, 0.0);
	right = vec3(1.0, 0.0, 0.0);
#endif

	Vertex((right + up) * scale, vec2(1.0, 1.0)); // rb
	Vertex((right - up) * scale, vec2(1.0, 0.0)); // rt
	Vertex((-right + up) * scale, vec2(0.0, 1.0)); // lb
	Vertex((-right - up) * scale, vec2(0.0, 0.0)); // lt  

	EndPrimitive();
}
