#include "LightsManager.h"

void LightsManager::initializeLights()
{
	// Initialize directional light
	directionalLight = new LightDirectional(
		vec3(0.3f, 0.3f, 0.3f),
		vec3(0.7f, 0.7f, 0.7f),
		vec3(1.f, 1.f, 1.f),
		vec3(10.f, -1.f, 10.f));

	// Set the number of point lights
	int usedPointLightsNumber = 0;

	// Generate chosen amount of point lights
	int pointLightsNumberLimited = min(usedPointLightsNumber, enginePtr->getConstProperties().maxPointLightsNumber);
	for(int i = 0; i < pointLightsNumberLimited; i++)
	{
		pointLightsList.push_back(new LightPoint());
	}

	// Set properties of generated lights
	
	// pointLightsList[0] = new LightPoint(
	// 	vec3(0.2f, 0.2f, 0.2f),
	// 	vec3(0.5f, 0.5f, 0.5f),
	// 	vec3(1.f, 1.f, 1.f),
	// 	vec3(5.f, 2.f, 5.f),
	// 	0.5f,
	// 	0.09,
	// 	0.032
	// );
	// 
	// pointLightsList[0] = new LightPoint(
	// 	vec3(0.2f, 0.2f, 0.2f),
	// 	vec3(0.5f, 0.5f, 0.5f),
	// 	vec3(1.f, 1.f, 1.f),
	// 	vec3(64.f, 2.f, 64.f),
	// 	1.0f,
	// 	0.09,
	// 	0.032
	// );
}

LightsManager::LightsManager(Engine* enginePtr) :
	enginePtr(enginePtr)
{
	initializeLights();
}

void LightsManager::updateLightsPerFrame()
{
	// Empty
}

LightDirectional LightsManager::getDirectionalLight()
{
	return *directionalLight;
}

std::vector<LightPoint*> LightsManager::getPointLightsList()
{
	return pointLightsList;
}