#version 430

//
// Uniforms
//
uniform mat4 matMvp;
uniform mat4 matMv;
uniform mat4 matM;
uniform mat4 matP;

uniform sampler2D terrainHeight;
// uniform sampler2D terrainHeight2;
uniform float terrainHeightOffset;

//
// Inputs
//
layout(quads, fractional_even_spacing) in;

patch in float gl_TessLevelOuter[4];
patch in float gl_TessLevelInner[2];

in vec2 tcs_terrainTexCoord[];
in float tcs_tessLevel[];

in vec3 normal[];
in vec3 fragPos[];

//
// Outputs
//
out vec2 tes_terrainTexCoord;
out float tes_tessLevel;

out vec3 tes_normal;
out vec3 tes_fragPos;

vec4 interpolate(in vec4 v0, in vec4 v1, in vec4 v2, in vec4 v3)
{
	vec4 a = mix(v0, v1, gl_TessCoord.x);
	vec4 b = mix(v3, v2, gl_TessCoord.x);
	return mix(a, b, gl_TessCoord.y);
}

vec2 interpolate2(in vec2 v0, in vec2 v1, in vec2 v2, in vec2 v3)
{
	vec2 a = mix(v0, v1, gl_TessCoord.x);
	vec2 b = mix(v3, v2, gl_TessCoord.x);
	return mix(a, b, gl_TessCoord.y);
}

void main()
{
	// Calculate the vertex position using the four original points and interpolate depneding on the tessellation coordinates.	
	gl_Position = interpolate(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position, gl_in[3].gl_Position);

	// Terrain heightmap coords
	vec2 terrainTexCoord = interpolate2(tcs_terrainTexCoord[0], tcs_terrainTexCoord[1], tcs_terrainTexCoord[2], tcs_terrainTexCoord[3]);

	// Sample the heightmap and offset y position of vertex
	vec4 samp = texture(terrainHeight, terrainTexCoord);
	// vec4 samp2 = texture(terrainHeight2, terrainTexCoord);
	gl_Position.y = samp[0] * terrainHeightOffset;

	// Project the vertex to clip space and send it along
	gl_Position = matP * matMv * gl_Position;

	tes_terrainTexCoord = terrainTexCoord;
	tes_tessLevel = tcs_tessLevel[0];
	tes_normal = normal[0];
	tes_fragPos = fragPos[0];
}