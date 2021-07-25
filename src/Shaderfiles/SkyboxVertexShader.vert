#version 430 core

layout (location = 0) in vec3 vertexPosition_worldspace;

out vec3 TexCoords;

uniform mat4 P;
uniform mat4 V;

void main()
{
    TexCoords = vertexPosition_worldspace;
    vec4 pos =  P * V * vec4(vertexPosition_worldspace, 1.0);
    gl_Position = pos.xyww;
} 