#version 430 core
layout (location = 0) in vec3 vertexPosition_modelspace;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(vertexPosition_modelspace, 1.0);
}