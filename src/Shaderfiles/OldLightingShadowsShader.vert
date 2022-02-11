#version 430 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform mat3 M_inverted;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
} vs_out;


void main() {

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(vertexPosition_modelspace, 1);

	vs_out.FragPos = vec3(M * vec4(vertexPosition_modelspace, 1.0));
	vs_out.Normal = normalize(M_inverted * vertexNormal_modelspace);
}