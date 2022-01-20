#version 430 core

layout (location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * vec4(vertexPosition_modelspace, 1.0);
}