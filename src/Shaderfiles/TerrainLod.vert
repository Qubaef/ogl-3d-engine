#version 430 core

//
// Uniforms
//
uniform mat4 matMvp;
uniform mat4 matMv;
uniform mat4 matM;
uniform mat4 matP;

uniform vec3 nodePos;
uniform float nodeSize;

//
// Inputs
//
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;

//
// Outputs
//
out vec3 normal;
out vec3 fragPos;

out vec2 vs_terrainTexCoord;

vec2 calcTerrainTexCoord(in vec4 pos)
{
	return vec2(abs(pos.x) / nodeSize, abs(pos.z) / nodeSize);
}

void main() {
	// Calculate vertex position basing on node's parameters
	vec3 vertexPosition_scaled = vertexPosition_modelspace * nodeSize + nodePos;

	// Output position of the vertex, in clip space : MVP * position
	gl_Position = matMvp * vec4(vertexPosition_scaled, 1);

	vec4 p = matM * vec4(vertexPosition_scaled, 1.0);
	vs_terrainTexCoord = calcTerrainTexCoord(p);

	// Calculate data for fragment shader
	fragPos = vec3(matM * vec4(vertexPosition_scaled, 1.0));
	normal = mat3(transpose(inverse(matM))) * vertexNormal_modelspace;
}