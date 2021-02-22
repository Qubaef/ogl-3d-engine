#pragma once
#ifndef LIGHTSMANAGER_H
#define LIGHTSMANAGER_H

// Library includes
#include <vector>

// Own includes
#include "LightDirectional.h"
#include "LightPoint.h"
#include "Engine.h"

class Engine;

class LightsManager
{
private:
	Engine* enginePtr;
	
	// Configurable lights
	LightDirectional* directionalLight = nullptr;
	std::vector<LightPoint*> pointLightsList;

	// Initialize properties of all used user-defined lights
	void initializeLights();
	
public:
	LightsManager(Engine* enginePtr);

	// Update lights positions/colors/directions using user-defined pattern
	// Leaving this function empty, causes lights not to change their behaviour in time
	void updateLightsPerFrame();

	// Get directional light
	LightDirectional getDirectionalLight();

	// Get list of point lights
	std::vector<LightPoint*> getPointLightsList();
};

#endif
