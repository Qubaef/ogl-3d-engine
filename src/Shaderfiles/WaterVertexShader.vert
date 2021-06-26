#version 430 core

layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform mat3 M_inverted;

#define PI 3.14159265358979323
#define waves_number 3
uniform vec4 wave[waves_number];
uniform float time;
uniform vec4 test;

out vec3 normal;
out vec3 frag_pos;

vec3 gerstner_wave(vec4 wave, vec3 p);

// values to calculate normals
vec3 tangent;
vec3 binormal;

void main() {

	// Output position of the vertex, in clip space : MVP * position
	tangent = vec3(0,0,0);
	binormal = vec3(0,0,0);

	vec3 new_vert_pos_modelspace = gerstner_wave(wave[0], vertexPosition_modelspace);
	new_vert_pos_modelspace += gerstner_wave(wave[1], vertexPosition_modelspace);
	new_vert_pos_modelspace += gerstner_wave(wave[2], vertexPosition_modelspace);
	gl_Position =  MVP * vec4(new_vert_pos_modelspace,1);

	// Calculate data for fragment shader
	frag_pos = vec3(M * vec4(vertexPosition_modelspace, 1.0));
	
	normal = M_inverted * normalize(cross(binormal, tangent));
}

vec3 gerstner_wave(vec4 wave, vec3 p)
{
	float k = 2 * PI / wave.w;
	float c = sqrt(9.8 / k);
	vec2 d = vec2(wave.x, wave.y);
	float f = k * (dot(d, vec2(p.x, p.z)) - c * time);
	float a = wave.z / k;
	float cosf = cos(f);
	float sinf = sin(f);

	tangent += vec3(
	1-d.x * d.x * (wave.z * sinf),
	d.x * (wave.z * cosf),
	-d.x * d.y * (wave.z * sinf));

	binormal += vec3(
	-d.x * d.y * (wave.z * sinf),
	d.y * (wave.z * cosf),
	1-d.y * d.y * (wave.z * sinf)
	);

	return vec3(
		p.x + d.x * (a *cosf),
		a * sin(f),
		p.z + d.y * (a *cosf));
}