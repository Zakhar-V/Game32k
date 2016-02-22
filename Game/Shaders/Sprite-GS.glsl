layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

void Vertex(in vec2 basis, in vec2 coord)
{
	OutInstanceID = InInstanceID[0];
	//OutWorldPos = InWorldPos
	//OutWorldPos = WorldMat[InInstanceID[0]] * vec4(InPos[0].xyz * vec3(InSize[0] * (coord - 0.5), 1.0), 1.0);
	OutWorldPos = WorldMat[InInstanceID[0]] * vec4(InPos[0].xyz + vec3(InSize[0] * coord, 0.0), 1.0);
	OutTexCoord = InTexCoord[0] + InTexCoord2[0] * coord;
	OutColor = InColor[0];
	gl_Position = ViewProjMat * OutWorldPos;
	EmitVertex();
}

void main()
{
	vec2 basis;
	Vertex(basis, vec2(1.0, 1.0)); // rb
	Vertex(basis, vec2(1.0, 0.0)); // rt
	Vertex(basis, vec2(0.0, 1.0)); // lb
	Vertex(basis, vec2(0.0, 0.0)); // lt  

	EndPrimitive();
}
