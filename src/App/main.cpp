#include "Engine/Engine.h"
#include "CameraControllers/OverviewCameraController.h"
#include "CameraControllers/FirstPersonCameraController.h"
#include "Renderables/Terrain/TerrainManager.h"
#include "Renderables/BaseGui.h"
#include "Renderables/Skybox.h"
#include "Renderables/Sphere.h"
#include "Renderables/LodTerrain/LodTerrain.h"


int main()
{
	ZoneScoped;

	// Setup environment
#ifdef TRACY_PROFILE_PARALLEL
	// TODO: Add log message
	system("start tracy\\profiler\\build\\win32\\x64\\Debug\\Tracy.exe");
#elif TRACY_PROFILE_COLLECT
	// TODO: Add log message
	system("start cmd /c tracy\\capture\\build\\win32\\x64\\Debug\\capture.exe -o output.tracy -f");
#endif

	// Initialize the Engine
	Engine* enginePtr = new Engine();

	InputManager* inputManager = new InputManager(enginePtr);

	/* Register shaders*/
	Shader lightingShader = Shader("LightingShader",
		"src/Shaderfiles/LightingVertexShader.vert",
		"src/Shaderfiles/LightingFragmentShader.frag");
	enginePtr->registerShader(lightingShader);

	Shader waterShader = Shader("WaterShader",
		"src/Shaderfiles/WaterVertexShader.vert",
		"src/Shaderfiles/LightingFragmentShader.frag");
	enginePtr->registerShader(waterShader);

	Shader skyboxShader = Shader("SkyboxShader",
		"src/Shaderfiles/SkyboxVertexShader.vert",
		"src/Shaderfiles/SkyboxFragmentShader.frag");
	enginePtr->registerShader(skyboxShader);

	Shader terrainLodShader = Shader("TerrainLod",
		"src/Shaderfiles/TerrainLod.vert",
		"src/Shaderfiles/TerrainLod.frag",
		"src/Shaderfiles/TerrainLod.tesc",
		"src/Shaderfiles/TerrainLod.tese",
		"src/Shaderfiles/TerrainLod.geom");
	enginePtr->registerShader(terrainLodShader);

	Shader heightmapShader = Shader("HeightmapShader",
		"src/Shaderfiles/HeightmapGenerationShader.comp");
	enginePtr->registerShader(heightmapShader);

	/* Register camera */
	// enginePtr->registerCamera(reinterpret_cast<Camera*>(new FirstPersonCameraController(enginePtr, new InputManager(enginePtr))));
	enginePtr->registerCamera(reinterpret_cast<Camera*>(new OverviewCameraController(enginePtr, new InputManager(enginePtr))));

	/* Register processing and rendering tasks */
	// enginePtr->registerTask(reinterpret_cast<Renderable*>(new TerrainManager(enginePtr, TerrainManager::FLAT)));
	// enginePtr->registerTask(reinterpret_cast<Renderable*>(new TerrainManager(enginePtr, TerrainManager::WATER)));
	enginePtr->registerProcessable(reinterpret_cast<Processable*>(new Sphere(enginePtr)));
	//enginePtr->registerProcessable(reinterpret_cast<Processable*>(new LodTerrain(enginePtr)));
	enginePtr->registerProcessable(reinterpret_cast<Processable*>(new TerrainManager(enginePtr, TerrainManager::SIMPLEX)));
	enginePtr->registerProcessable(reinterpret_cast<Processable*>(new Skybox(enginePtr)));
	enginePtr->registerProcessable(reinterpret_cast<Processable*>(new BaseGui(enginePtr)));

	// Start the Engine
	enginePtr->run();

	return 0;
}