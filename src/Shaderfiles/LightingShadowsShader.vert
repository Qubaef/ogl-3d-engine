#version 430 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
} vs_out;

//// Uniforms
layout (std140, binding = 0) uniform GlobalData
{
	// Display mode
	int displayMode;

	// Camera and viewport
	vec3 viewPos;
	vec3 viewDir;
	float nearPlane;
	float farPlane;

	mat4 MVP;
	mat4 M;
	mat4 V;
	mat4 P;

	// Lights
	vec3 dirLightAmbient;
	vec3 dirLightDiffuse;
	vec3 dirLightSpecular;
	vec3 dirLightDirection;
} g;

void main() {
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  g.MVP * vec4(vertexPosition_modelspace, 1);

	vs_out.FragPos = vec3(g.M * vec4(vertexPosition_modelspace, 1.0));
	vs_out.Normal = normalize(transpose(inverse(mat3(g.M))) * vertexNormal_modelspace);
}