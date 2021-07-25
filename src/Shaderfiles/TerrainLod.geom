#version 430

//
// Uniforms
//

uniform mat4 matMvp;
uniform mat4 matMv;
uniform mat4 matM;
uniform mat4 matP;

uniform vec2 viewport;

//
// Inputs
//
layout(triangles) in;
in vec2 tes_terrainTexCoord[];
in float tes_tessLevel[];

in vec3 tes_normal[];
in vec3 tes_fragPos[];

//
// Outputs
//
layout(triangle_strip, max_vertices = 4) out;

out vec2 gs_terrainTexCoord;

out vec3 normal;
out vec3 fragPos;


void main(void)
{
	// Calculate edge distances for wireframe
	float ha, hb, hc;
	ha = hb = hc = 0.0;

	// Output verts
	for(int i = 0; i < gl_in.length(); ++i)
	{
		gl_Position = gl_in[i].gl_Position;
		gs_terrainTexCoord = tes_terrainTexCoord[i];
		EmitVertex();
	}

	// This closes the the triangle
	gl_Position = gl_in[0].gl_Position;
	gs_terrainTexCoord = tes_terrainTexCoord[0];

	normal = tes_normal[0];
	fragPos = tes_fragPos[0];

	EmitVertex();
	EndPrimitive();
}