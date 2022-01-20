#version 430 core

layout (location = 0) in vec3 vertexPosition_modelspace;
layout (location = 1) in vec3 vertexNormal_modelspace;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
    vs_out.FragPos = vec3(model * vec4(vertexPosition_modelspace, 1.0));
    vs_out.Normal = transpose(inverse(mat3(model))) * vertexNormal_modelspace;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    gl_Position = projection * view * vec4(vs_out.FragPos, 1.0);
}

// layout(location = 0) in vec3 vertexPosition_modelspace;
// layout(location = 1) in vec3 vertexNormal_modelspace;

// uniform mat4 MVP;
// uniform mat4 V;
// uniform mat4 M;
// uniform mat3 M_inverted;

// out vec3 normal;
// out vec3 frag_pos;

// void main() {

// 	// Output position of the vertex, in clip space : MVP * position
// 	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);

// 	// Calculate data for fragment shader
// 	frag_pos = vec3(M * vec4(vertexPosition_modelspace, 1.0));
// 	normal = M_inverted * vertexNormal_modelspace;
// }