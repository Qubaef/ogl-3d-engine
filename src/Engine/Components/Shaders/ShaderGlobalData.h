#pragma once

#include "GlobalDataLayout.h"
#include "Renderer/OpenGL/Objects/GLUbo.h"

//
// ShaderGlobalData provides interface to initialize and set shaders' global data kept in UBO.
// It's no generic and configurable dynamically due to strong connection to the shaders' data layout.
// Any change in data layout of this class should be followed by updates of data layouts in shader files.
//
class ShaderGlobalData
{
public:

	enum class DisplayMode
	{
		DEFAULT = 0,
		WIREFRAME = 1,
		NORMALS = 2,
		DEPTH = 3,
		CASCADES_DEBUG = 4
	};

	// Cpu global data, which can be updated
	GlobalDataLayout data;

	ShaderGlobalData(): data()
	{
	}

	void init()
	{
		// Ensure that global data is initialized only once
		if (!initialized)
		{
			globalDataBuffer.init(sizeof(data));
			initialized = true;
		}
	}

	void updateToGpu()
	{
		globalDataBuffer.setData(0, sizeof(GlobalDataLayout), &data);
	}

	void bind(int bindingIndex) const
	{
		globalDataBuffer.bind(bindingIndex);
	}

//	//
//	// Generate setters for all the GlobalDataLayout properties
//	//
//#define PROPERTY_SETTER(type, name) void set_##name(type name) const {									\
//	globalDataBuffer.setData(offsetof(GlobalDataLayout, name), sizeof(GlobalDataLayout::name), &name);	\
//	}
//
//	PROPERTY_SETTER(int, displayMode)
//
//	PROPERTY_SETTER(glm::vec4, viewPos)
//	PROPERTY_SETTER(glm::vec4, viewDir)
//	PROPERTY_SETTER(float, nearPlane)
//	PROPERTY_SETTER(float, farPlane)
//
//	PROPERTY_SETTER(glm::mat4, MVP)
//	PROPERTY_SETTER(glm::mat4, M)
//	PROPERTY_SETTER(glm::mat4, M_inv)
//	PROPERTY_SETTER(glm::mat4, V)
//	PROPERTY_SETTER(glm::mat4, P)
//
//	PROPERTY_SETTER(glm::vec4, dirLightAmbient)
//	PROPERTY_SETTER(glm::vec4, dirLightDiffuse)
//	PROPERTY_SETTER(glm::vec4, dirLightSpecular)
//	PROPERTY_SETTER(glm::vec4, dirLightDirection)
//
//#undef SET_UNIFORM_PROPERTY

private:
	bool initialized = false;
	GLUbo globalDataBuffer;
};
