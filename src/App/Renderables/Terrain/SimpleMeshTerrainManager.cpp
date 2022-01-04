#include "SimpleMeshTerrainManager.h"
#include "Engine/Camera/Camera.h"

#include "FlatTerrain.h"
#include "RandomTerrain.h"
#include "WaterTerrain.h"
#include "SimplexTerrainChunk.h"

void SimpleMeshTerrainManager::terrainInit(int x, int y, int pointsPerLine)
{
	// Initialize terrain object
	switch (terrainType)
	{
	case TERRAIN_TYPE::FLAT:
		shaderPtr = enginePtr->getShaderByName("LightingShader");
		terrainPtr = reinterpret_cast<Terrain*>(new FlatTerrain(x, y, 1, 1, pointsPerLine, shaderPtr));
		break;
	case TERRAIN_TYPE::RANDOM:
		shaderPtr = enginePtr->getShaderByName("LightingShader");
		terrainPtr = reinterpret_cast<Terrain*>(new RandomTerrain(x, y, 1, 1, pointsPerLine, shaderPtr));
		break;
	case TERRAIN_TYPE::WATER:
		shaderPtr = enginePtr->getShaderByName("WaterShader");
		terrainPtr = reinterpret_cast<Terrain*>(new WaterTerrain(x, y, 1, 1, pointsPerLine, shaderPtr));
		break;
	case TERRAIN_TYPE::SIMPLEX:
		shaderPtr = enginePtr->getShaderByName("LightingShader");
		terrainPtr = reinterpret_cast<Terrain*>(new SimplexTerrainChunk(x, y, 1, 1, pointsPerLine, shaderPtr));
		break;
		//shaderPtr = enginePtr->getShaderByName("LightingShader");
		//terrainPtr = reinterpret_cast<Terrain*>(new LodTerrain(x, y, 1, 1, pointsPerLine, shaderPtr));
		//break;
	}

	// Get Shader from initialized terrain
	shaderPtr = terrainPtr->getShader();

	terrainPtr->initialize();

	// Initialize view matrices
	shaderPtr->use();

	MvpMatrixID = glGetUniformLocation(shaderPtr->get_ID(), "MVP");
	ViewMatrixID = glGetUniformLocation(shaderPtr->get_ID(), "V");
	ModelMatrixID = glGetUniformLocation(shaderPtr->get_ID(), "M");
}

SimpleMeshTerrainManager::SimpleMeshTerrainManager(Engine* enginePtr, TERRAIN_TYPE terrainType) :
	IProcessable(enginePtr), terrainType(terrainType)
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
	//ZoneScopedN("SimpleMeshTerrainManager - ProcessPerFrame");

	// Update terrain
	terrainPtr->update();
}

void SimpleMeshTerrainManager::render()
{
	ZoneScoped

	Camera* cameraPtr = enginePtr->getCamera();

	// Get pointers to matrices
	glm::mat4* mvp = cameraPtr->getMVPMatrix();
	glm::mat4* V = cameraPtr->getViewMatrix();
	glm::mat4* M = cameraPtr->getModelMatrix();

	shaderPtr->use();

	// Send transformation to the currently bound shader, in the "MVP" uniform
	// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
	glUniformMatrix4fv(MvpMatrixID, 1, GL_FALSE, &(*mvp)[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &(*V)[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &(*M)[0][0]);

	// Send M_inverted for optimization purposes (it is better to calculate it on cpu)
	shaderPtr->set_mat3("M_inverted", mat3(transpose(inverse(*M))));

	// Send view position for specular component
	shaderPtr->set_vec3("view_pos", cameraPtr->getPosition());

	terrainPtr->sendAndRender();
}