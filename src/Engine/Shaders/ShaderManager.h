#pragma once
#ifndef LIGHTSMANAGER_H
#define LIGHTSMANAGER_H

#include <vector>
#include <iostream>
#include <map>

#include "../Include/Common.h"
#include "../Lights/LightDirectional.h"
#include "../Lights/LightPoint.h"
#include "Shader.h"
#include "Engine/MessageBus/IMessanger.h"

using namespace glm;

class Engine;

class ShaderManager : public IMessanger
{
private:
	Engine* enginePtr;

	// Collection of all instanced Shaders with their names
	std::map<std::string, Shader*> shaderCollection;

	// List of shaders which will be used in the program
	std::vector<Shader*> usedShadersList;

	// Lights variables names int fragment shader
	const std::string directionalLightName = "dir_light";
	const std::string pointLightsName = "point_lights";
	const std::string pointLightsCountName = "point_lights_count";

	//// Configurable lights
	// Directional lights
	LightDirectional* directionalLight = nullptr;

	// Point lights
	std::vector<LightPoint*> pointLightsList;
	int actualPointLightsNumber = 0;

	// Initialize properties of all used user-defined lights
	void initializeLights();

	// Update lights positions/colors/directions
	void updateLights();

	// Update lights to shaders uniforms
	void updateShaders();

	// Update uniforms of stored shaders
	void updateDirectionalLight(Shader* shaderPtr) const;
	void updatePointLights(Shader* shaderPtr) const;

public:
	ShaderManager(Engine* enginePtr);

	// Set directional light
	void setDirectionalLight(LightDirectional* directionalLight);

	// Add point light
	void addPointLight(LightPoint* pointLight);


	// Get directional light
	LightDirectional getDirectionalLight();

	// Get list of point lights
	std::vector<LightPoint*> getPointLightsList();


	// Add shader to shaderManager, so that engine will be able to process light uniforms for it
	void registerShader(Shader& shader);

	// Get shader OpenGL name
	Shader* getShaderByName(std::string name);

	// Update lights position
	void updatePerFrame();
};

#endif
