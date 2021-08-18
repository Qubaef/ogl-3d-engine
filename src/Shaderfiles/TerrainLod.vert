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
uniform vec2 terrainPos;

//
// Inputs
//
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;

//
// Outputs
//


void main() {
	// Calculate vertex position basing on node's parameters and
	//   output position of the vertex, in world space
	//   MVP * position used in .tesc in gl_in
	gl_Position = vec4(vertexPosition_modelspace.xyz * nodeSize + nodePos, 1.0);
}