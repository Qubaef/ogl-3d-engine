#version 430 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform mat3 M_inverted;

out vec3 normal;
out vec3 frag_pos;

void main() {

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);

	// Calculate data for fragment shader
	frag_pos = vec3(M * vec4(vertexPosition_modelspace, 1.0));
	normal = M_inverted * vertexNormal_modelspace;
}