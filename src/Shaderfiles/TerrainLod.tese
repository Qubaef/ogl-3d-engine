#version 430

//
// Uniforms
//
uniform mat4 matMvp;
uniform mat4 matMv;
uniform mat4 matM;
uniform mat4 matP;

uniform sampler2D terrainHeight;
uniform float terrainHeightOffset;
uniform float terrainDensity;
uniform float terrainSize;

//
// Inputs
//
layout(quads, fractional_even_spacing, cw) in;
// layout(quads, fractional_odd_spacing, cw) in;
// layout(quads, equal_spacing, cw) in;

patch in float gl_TessLevelOuter[4];
patch in float gl_TessLevelInner[2];

//
// Outputs
//
out float steepness;

out vec3 tese_normal;
out vec3 tese_fragPos;


vec4 interpolate4(in vec4 v0, in vec4 v1, in vec4 v2, in vec4 v3)
{
	vec4 a = mix(v0, v1, gl_TessCoord.x);
	vec4 b = mix(v2, v3, gl_TessCoord.x);
	return mix(a, b, gl_TessCoord.y);
}

vec3 interpolate3(in vec3 v0, in vec3 v1, in vec3 v2, in vec3 v3)
{
    vec3 a = mix(v0, v1, gl_TessCoord.x);
	vec3 b = mix(v3, v2, gl_TessCoord.x);
	return mix(a, b, gl_TessCoord.y);
}

vec2 interpolate2(in vec2 v0, in vec2 v1, in vec2 v2, in vec2 v3)
{
	vec2 a = mix(v0, v1, gl_TessCoord.x);
	vec2 b = mix(v3, v2, gl_TessCoord.x);
	return mix(a, b, gl_TessCoord.y);
}

vec3 pt_q(in vec3 p0, in vec3 p1, in vec3 p2, in vec3 p3, in float u, in float v)
{
	return (1.0-u)*(1.0-v)*p0 + u*(1.0-v)*p1 + (1.0-u)*v*p2 + u*v*p3;
}

vec3 pt_pi(in vec3 q, in vec3 p, in vec3 n)
{
	return q - dot(q - p, n) * n;
}


float getHeight(vec2 p)
{
	return texture(terrainHeight, vec2(p.x, p.y) / terrainSize + 0.5).x + terrainHeightOffset;
}

vec3 getPosWHeight(vec3 p)
{
	return vec3(p.x, texture(terrainHeight, vec2(p.x, p.z) / terrainSize + 0.5).x + terrainHeightOffset, p.z);
}


void main()
{
	// Calculate the vertex position using the four original points and interpolate depneding on the tessellation coordinates.	
	gl_Position = interpolate4(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position, gl_in[3].gl_Position);

	// Sample the heightmap and offset y position of vertex
	gl_Position.y = getHeight(vec2(gl_Position.x, gl_Position.z));

	// Store worldspace position in separate variable
	vec3 p = gl_Position.xyz;

	// Calculate fragPos
	tese_fragPos = vec3(matM * gl_Position);

	// Project the vertex to clip space and send it along
	gl_Position = matP * matMv * gl_Position;

	// Calculate normal vector 
	tese_normal = vec3(0);
	float s = terrainSize / terrainDensity;
	tese_normal += cross(getPosWHeight(vec3(p.x - s, 0, p.z - s)) - p, getPosWHeight(vec3(p.x, 0, p.z - s)) - p); // top-left and top
	tese_normal += cross(getPosWHeight(vec3(p.x + s, 0, p.z - s)) - p, getPosWHeight(vec3(p.x + s, 0, p.z)) - p); // top-right and right
	tese_normal += cross(getPosWHeight(vec3(p.x + s, 0, p.z + s)) - p, getPosWHeight(vec3(p.x, 0, p.z + s)) - p); // bot-right and bot
	tese_normal += cross(getPosWHeight(vec3(p.x - s, 0, p.z + s)) - p, getPosWHeight(vec3(p.x - s, 0, p.z)) - p); // bot left and left

	// Calculate angle for steepness calculation
	float angle = degrees(acos(dot(vec3(0, 1, 0), normalize(tese_normal))));

	tese_normal = mat3(transpose(inverse(matM))) * normalize(tese_normal);

	// calculate steepness factor for color calculation
	float minAngle = 140;
	float maxAngle = 155;

	steepness = clamp((angle - minAngle) / (maxAngle - minAngle), 0, 1);
}