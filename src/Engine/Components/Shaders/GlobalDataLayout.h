#pragma once

#include "Engine/Include/Common.h"

//
// Type definitions for STD140
// Note: Vec3 is deliberately treated as vec4 bc cpp compiler optimizes it in some cases
// Using Vec4 instead helps to map data the same way it is on Glsl side
// (there are no downsides to this approach - on the Glsl side vec3 still can be used)
//

#define STD140_INT		alignas(4) int
#define STD140_FLOAT	alignas(4) float
#define STD140_BOOL		alignas(4) bool
#define STD140_VEC3		alignas(16) glm::vec4
#define STD140_VEC4		alignas(16) glm::vec4
#define STD140_MAT4		alignas(16) glm::mat4

//
// Struct representing memory layout of engine's global data
// It preserves the exact layout of gpu uniform object memory, so it can be copied 1:1 without conversion
// It's representation of memory layout from which offsets can be calculated
// Actual setters of the global data are in ShaderGlobalData
//

struct GlobalDataLayout
{
	// Disable assignment operator
	GlobalDataLayout& operator=(const GlobalDataLayout&) = delete;

	// Display mode
	STD140_INT displayMode;

	// Camera and viewport
	STD140_VEC4	viewPos;
	STD140_VEC4	viewDir;
	STD140_FLOAT nearPlane;
	STD140_FLOAT farPlane;

	STD140_MAT4	MVP;
	STD140_MAT4	M;
	STD140_MAT4	V;
	STD140_MAT4	P;

	// Lights
	STD140_VEC4	dirLightAmbient;
	STD140_VEC4	dirLightDiffuse;
	STD140_VEC4	dirLightSpecular;
	STD140_VEC4	dirLightDirection;
};

#undef STD140_INT
#undef STD140_FLOAT
#undef STD140_BOOL
#undef STD140_VEC3
#undef STD140_VEC4
#undef STD140_MAT4
