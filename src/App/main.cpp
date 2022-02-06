#include "Engine/Engine.h"
#include "CameraControllers/CameraManager.h"
#include "CameraControllers/OverviewCameraController.h"
#include "CameraControllers/FirstPersonCameraController.h"
#include "CameraControllers/MenuUseCameraController.h"

#include "Renderables/Terrain/SimpleMeshTerrainManager.h"
#include "Renderables/BaseGui.h"
#include "Renderables/Skybox.h"
#include "Renderables/Sphere.h"
#include "Renderables/Framebuffer.h"
#include "Renderables/Interior.h"
#include "Renderables/LodTerrain/SingleMeshLodTerrain.h"
#include "Renderables/GuiEntityManager/EntityManager.h"

void loadTerrainScene(Engine* enginePtr, InputManager* inputManager);
void loadInteriorScene(Engine* enginePtr, InputManager* inputManager);

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

	Shader lightingShadowsShader = Shader("LightingShadowsShader",
		"src/Shaderfiles/LightingShadowsShader.vert",
		"src/Shaderfiles/LightingShadowsShader.frag");
	enginePtr->registerShader(lightingShadowsShader);

	Shader mainShader = Shader("MainShader",
		"src/Shaderfiles/MainShader.vert",
		"src/Shaderfiles/MainShader.frag");
	enginePtr->registerShader(mainShader);

	Shader depthShader = Shader("DepthShader",
		"src/Shaderfiles/DepthShader.vert",
		"src/Shaderfiles/DepthShader.frag");
	enginePtr->registerShader(depthShader);

	Shader shadowMappingShader = Shader("ShadowMappingShader",
		"src/Shaderfiles/ShadowMappingShader.vert",
		"src/Shaderfiles/ShadowMappingShader.frag",
		nullptr,
		nullptr,
		"src/Shaderfiles/ShadowMappingShader.geom");
	enginePtr->registerShader(shadowMappingShader);

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
		"src/Shaderfiles/TerrainLod.tese");
	enginePtr->registerShader(terrainLodShader);

	Shader heightmapShader = Shader("HeightmapShader",
		"src/Shaderfiles/HeightmapGenerationShader.comp");
	enginePtr->registerShader(heightmapShader);

	Shader framebufferShader = Shader("FramebufferShader",
		"src/Shaderfiles/FramebufferShader.vert",
		"src/Shaderfiles/FramebufferShader.frag");
	enginePtr->registerShader(framebufferShader);

	switch (2)
	{
	case 1:
		loadTerrainScene(enginePtr, inputManager);
		break;
	case 2:
		loadInteriorScene(enginePtr, inputManager);
		break;
	default: ;
	}

	// Start the Engine
	enginePtr->run();

	return 0;
}

void loadInteriorScene(Engine* enginePtr, InputManager* inputManager)
{
	/* Register camera manager */
	CameraManager* cameraManager = new CameraManager(enginePtr, inputManager);
	cameraManager->addCamera(new MenuUseCameraController(enginePtr, inputManager));
	cameraManager->addCamera(new FirstPersonCameraController(enginePtr, inputManager));
	cameraManager->addCamera(new OverviewCameraController(enginePtr, inputManager));
	enginePtr->registerProcessable(cameraManager);

	// enginePtr->registerProcessable(new SimpleMeshTerrainManager(enginePtr, SimpleMeshTerrainManager::TERRAIN_TYPE::FLAT));
	// enginePtr->registerProcessable(new Sphere(enginePtr, vec3(5, 5, 5)));
	enginePtr->registerProcessable(new Skybox(enginePtr));
	enginePtr->registerProcessable(new EntityManager(enginePtr));
	enginePtr->registerProcessable(new Interior(enginePtr));

	// enginePtr->registerProcessable(reinterpret_cast<IProcessable*>(new Framebuffer(enginePtr)));
	enginePtr->registerProcessable(new BaseGui(enginePtr));
}

void loadTerrainScene(Engine* enginePtr, InputManager* inputManager)
{
	/* Register camera manager */
	CameraManager* cameraManager = new CameraManager(enginePtr, inputManager);
	cameraManager->addCamera(new MenuUseCameraController(enginePtr, inputManager));
	cameraManager->addCamera(new FirstPersonCameraController(enginePtr, inputManager));
	cameraManager->addCamera(new OverviewCameraController(enginePtr, inputManager));
	enginePtr->registerProcessable(cameraManager);

	enginePtr->registerProcessable(new SingleMeshLodTerrain(enginePtr, 5000, 5000, 8));
	// enginePtr->registerProcessable(reinterpret_cast<IProcessable*>(new SimpleMeshTerrainManager(enginePtr, SimpleMeshTerrainManager::SIMPLEX)));
	enginePtr->registerProcessable(new Skybox(enginePtr));
	enginePtr->registerProcessable(new EntityManager(enginePtr));

	// enginePtr->registerProcessable(reinterpret_cast<IProcessable*>(new Framebuffer(enginePtr)));
	enginePtr->registerProcessable(new BaseGui(enginePtr));
}

