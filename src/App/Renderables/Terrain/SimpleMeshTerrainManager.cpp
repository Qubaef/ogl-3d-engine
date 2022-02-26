#include "SimpleMeshTerrainManager.h"
#include "Engine/Components/Camera/Camera.h"

#include "FlatTerrain.h"
#include "RandomTerrain.h"
#include "WaterTerrain.h"
#include "SimplexTerrainChunk.h"

using namespace glm;

void SimpleMeshTerrainManager::terrainInit(int x, int y, int pointsPerLine)
{
	depthShader = engine.getShaderByName("DepthShader");
	lightingShadowsShader = engine.getShaderByName("LightingShadowsShader");

	// Initialize terrain object
	switch (terrainType)
	{
	case TERRAIN_TYPE::FLAT:
		terrainPtr = reinterpret_cast<Terrain*>(new FlatTerrain(x, y, pointsPerLine));
		break;
	case TERRAIN_TYPE::RANDOM:
		terrainPtr = reinterpret_cast<Terrain*>(new RandomTerrain(x, y, pointsPerLine));
		break;
	case TERRAIN_TYPE::WATER:
		terrainPtr = reinterpret_cast<Terrain*>(new WaterTerrain(x, y, pointsPerLine, lightingShadowsShader));
		break;
	case TERRAIN_TYPE::SIMPLEX:
		terrainPtr = reinterpret_cast<Terrain*>(new SimplexTerrainChunk(x, y, pointsPerLine));
		break;
	}

	terrainPtr->initialize();
}

SimpleMeshTerrainManager::SimpleMeshTerrainManager(Engine& engine, TERRAIN_TYPE terrainType) :
	IProcessable(engine), terrainType(terrainType)
{
	ZoneScoped;

	// Compile shader program
	// shaderPtr = new Shader(vertexShaderPath, fragmentShaderPath);

	terrainInit(x, y, pointsPerLine);
}

void SimpleMeshTerrainManager::preprocess()
{
	ZoneScoped;

	ImGui::Begin("Terrain size");

	static int x = 500;
	static int y = 500;
	static int pointsPerLine = 501;

	ImGui::SliderInt("x", &x, 1, 2000);
	ImGui::SliderInt("y", &y, 1, 2000);
	ImGui::SliderInt("pointsPerLine", &pointsPerLine, 2, 2001);

	if (ImGui::Button("Apply"))
	{
		delete terrainPtr;
		terrainInit(x, y, pointsPerLine);
	}

	ImGui::End();

	if (terrainType == TERRAIN_TYPE::SIMPLEX)
	{
		SimplexTerrainChunk* simplexTerrainPtr = dynamic_cast<SimplexTerrainChunk*>(terrainPtr);
		SimplexNoiseWrapper simplexNoiseParams = simplexTerrainPtr->getSimplexNoise();

		bool updated = false;

		ImGui::Begin("SimplexNoise params");

		updated = updated | ImGui::SliderFloat("amplitude", &simplexNoiseParams.amplitude, 1, 1000);
		updated = updated | ImGui::SliderFloat("frequency", &simplexNoiseParams.frequency, 0, 0.1);
		updated = updated | ImGui::SliderFloat("lacunarity", &simplexNoiseParams.lacunarity, 0, 10);
		updated = updated | ImGui::SliderFloat("persistance", &simplexNoiseParams.persistance, 0, 5);
		updated = updated | ImGui::SliderInt("terrainLayers", &simplexNoiseParams.terrainLayers, 1, 10);
		updated = updated | ImGui::SliderFloat("noiseOffsetX", &simplexNoiseParams.noiseOffsetX, -1000, 1000);
		updated = updated | ImGui::SliderFloat("noiseOffsetZ", &simplexNoiseParams.noiseOffsetZ, -1000, 1000);

		if (updated)
		{
			simplexTerrainPtr->updateSimplexNoise(simplexNoiseParams);
		}

		ImGui::End();
	}
}

void SimpleMeshTerrainManager::process()
{
	ZoneScoped;

	// Update terrain
	terrainPtr->update();
}

void SimpleMeshTerrainManager::setupDepthRender()
{
	// Render scene from light's point of view
	depthShader->use();
}

void SimpleMeshTerrainManager::setupDefaultRender()
{
	// Render scene from camera's point of view
	lightingShadowsShader->use();

	lightingShadowsShader->setVec3("material.ambient", vec3(0.0215, 0.1745, 0.0215));
	lightingShadowsShader->setVec3("material.diffuse", vec3(0.07568, 0.61424, 0.07568));
	lightingShadowsShader->setVec3("material.specular", vec3(0.633, 0.727811, 0.633));
	lightingShadowsShader->setFloat("material.shininess", 2);
}

void SimpleMeshTerrainManager::render()
{
	if ((engine.getShaderGlobalData().data.displayMode & ShaderGlobalData::DisplayMode::DEPTH) > 0)
	{
		setupDepthRender();
		terrainPtr->sendAndRender(depthShader);
	}
	else if ((engine.getShaderGlobalData().data.displayMode & ShaderGlobalData::DisplayMode::DEFAULT) > 0)
	{
		setupDefaultRender();
		terrainPtr->sendAndRender(lightingShadowsShader);
	}
}
