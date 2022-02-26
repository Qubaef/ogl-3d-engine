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
#include "Renderables/InteriorNoShading.h"
#include "Renderables/LodTerrain/SingleMeshLodTerrain.h"
#include "Renderables/GuiEntityManager/EntityManager.h"
#include "RenderPasses/DefaultRenderPass.h"
#include "RenderPasses/DepthRenderPass.h"
#include "RenderPasses/ShadowsRenderPass.h"

void loadTerrainScene(Engine& engine, InputManager* inputManager);
void loadInteriorScene(Engine& engine, InputManager* inputManager);

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
	Engine engine = Engine();
	InputManager* inputManager = new InputManager(engine);

	/* Register shaders*/
	Shader lightingShader = Shader("LightingShader",
		"src/Shaderfiles/LightingVertexShader.vert",
		"src/Shaderfiles/LightingFragmentShader.frag");
	engine.registerShader(lightingShader);

	Shader lightingShadowsShader = Shader("LightingShadowsShader",
		"src/Shaderfiles/LightingShadowsShader.vert",
		"src/Shaderfiles/LightingShadowsShader.frag");
	engine.registerShader(lightingShadowsShader);

	Shader oldLightingShadowsShader = Shader("OldLightingShadowsShader",
		"src/Shaderfiles/OldLightingShadowsShader.vert",
		"src/Shaderfiles/OldLightingShadowsShader.frag");
	engine.registerShader(oldLightingShadowsShader);

	Shader depthShader = Shader("DepthShader",
		"src/Shaderfiles/DepthShader.vert",
		"src/Shaderfiles/DepthShader.frag",
		nullptr,
		nullptr,
		"src/Shaderfiles/DepthShader.geom");
	engine.registerShader(depthShader);

	Shader waterShader = Shader("WaterShader",
		"src/Shaderfiles/WaterVertexShader.vert",
		"src/Shaderfiles/LightingFragmentShader.frag");
	engine.registerShader(waterShader);

	Shader skyboxShader = Shader("SkyboxShader",
		"src/Shaderfiles/SkyboxVertexShader.vert",
		"src/Shaderfiles/SkyboxFragmentShader.frag");
	engine.registerShader(skyboxShader);

	Shader terrainLodShader = Shader("TerrainLod",
		"src/Shaderfiles/TerrainLod.vert",
		"src/Shaderfiles/TerrainLod.frag",
		"src/Shaderfiles/TerrainLod.tesc",
		"src/Shaderfiles/TerrainLod.tese");
	engine.registerShader(terrainLodShader);

	Shader heightmapShader = Shader("HeightmapShader",
		"src/Shaderfiles/HeightmapGenerationShader.comp");
	engine.registerShader(heightmapShader);

	Shader framebufferShader = Shader("FramebufferShader",
		"src/Shaderfiles/FramebufferShader.vert",
		"src/Shaderfiles/FramebufferShader.frag");
	engine.registerShader(framebufferShader);

	switch (2)
	{
	case 1:
		loadTerrainScene(engine, inputManager);
		break;
	case 2:
		loadInteriorScene(engine, inputManager);
		break;
	default: 
		return -1;
	}

	//// Register render pipeline
	//engine.registerRenderPass(new DefaultRenderPass(engine));

	DepthRenderPass* depthRenderPass = new DepthRenderPass(engine);
	ShadowsRenderPass* shadowsRenderPass = new ShadowsRenderPass(engine, *(depthRenderPass));
	engine.registerRenderPass(depthRenderPass);
	engine.registerRenderPass(shadowsRenderPass);

	// Start the Engine
	engine.runPhaseRuntime();

	return 0;
}

void loadInteriorScene(Engine& engine, InputManager* inputManager)
{
	/* Register camera manager */
	CameraManager* cameraManager = new CameraManager(engine, inputManager);
	cameraManager->addCamera(new MenuUseCameraController(engine, inputManager));
	cameraManager->addCamera(new FirstPersonCameraController(engine, inputManager));
	cameraManager->addCamera(new OverviewCameraController(engine, inputManager));
	engine.registerProcessable(cameraManager);

	engine.registerProcessable(new EntityManager(engine));

	// Ported
	engine.registerProcessable(new SimpleMeshTerrainManager(engine, SimpleMeshTerrainManager::TERRAIN_TYPE::SIMPLEX));
	engine.registerProcessable(new Sphere(engine, vec3(5, 5, 5)));
	engine.registerProcessable(new Skybox(engine));
	// engine.registerProcessable(new InteriorNoShading(engine));

	// engine.registerProcessable(new Interior(engine));
	// engine.registerProcessable(reinterpret_cast<IProcessable*>(new Framebuffer(engine)));
	engine.registerProcessable(new BaseGui(engine));
}

void loadTerrainScene(Engine& engine, InputManager* inputManager)
{
	/* Register camera manager */
	CameraManager* cameraManager = new CameraManager(engine, inputManager);
	cameraManager->addCamera(new MenuUseCameraController(engine, inputManager));
	cameraManager->addCamera(new FirstPersonCameraController(engine, inputManager));
	cameraManager->addCamera(new OverviewCameraController(engine, inputManager));
	engine.registerProcessable(cameraManager);

	engine.registerProcessable(new SingleMeshLodTerrain(engine, 5000, 5000, 8));
	// engine.registerProcessable(reinterpret_cast<IProcessable*>(new SimpleMeshTerrainManager(engine, SimpleMeshTerrainManager::SIMPLEX)));
	engine.registerProcessable(new Skybox(engine));
	engine.registerProcessable(new EntityManager(engine));

	// engine.registerProcessable(reinterpret_cast<IProcessable*>(new Framebuffer(engine)));
	engine.registerProcessable(new BaseGui(engine));
}

