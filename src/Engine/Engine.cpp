#include <iostream>
#include <utility>

#include "Engine.h"

#include "Exceptions/InitializationException.h"
#include "App/Renderables/TestEntity.h"
#include "Components/Shaders/ShaderGlobalData.h"
#include "DebugLib/Log.h"

#include "Renderer/OpenGL/Init.h"
#include "Renderer/OpenGL/Validation.h"

Engine::Engine()
	:
	props(*(new Properties())),
	comps(*this)
{
	// Initialize Debug Log
	LOG.init(true, true, true, true, true);

	try
	{
		// Start OpenGL Initialization Phase
		runPhaseRendererInit();
	}
	catch (InitializationException& e) {
		LOG.ERROR("Exception: %s\n Message: %s\n, Location: %s\n", e.get_type(), e.get_msg(), e.get_func());
		return;
	}

	// Start Engine Preparation Phase
	runPhaseEnginePrep();
}

void Engine::registerShader(Shader& shader)
{
	comps.shaderManager.registerShader(shader);
}

Shader* Engine::getShaderByName(std::string name)
{
	return comps.shaderManager.getShaderByName(std::move(name));
}

bool Engine::registerProcessable(IProcessable* processablePtr)
{
	comps.processableList.push_back(processablePtr);
	comps.processableQueue.pushProcessable(processablePtr);

	return true;
}

void Engine::registerCamera(Camera* cameraPtr)
{
	comps.cameraPtr = cameraPtr;
}

void Engine::registerRenderPass(RenderPass* renderPassPtr)
{
	comps.renderPassList.push_back(renderPassPtr);
}

void Engine::runPhaseRuntime()
{
	//
	// Verify Engine's readiness to start execution 
	//
	if (!verifySetupStatus())
		return;

	//
	// Runtime loop
	//
	while (!glfwWindowShouldClose(comps.engineWindowPtr))
	{
		//
		// Preprocess:
		//
		{
			// Dear ImGui: Start the frame
			ImGuiNewFrame();

			// Publish message bus
			comps.messageBus.publish();

			// Perform preprocess phase
			comps.processableQueue.preprocess();
		}

		//
		// Process:
		//
		{
			// Start process phase
			comps.processableQueue.process();

			//// Perform necessary side tasks, while waiting for processableQueue tasks to be completed

			// Update shaders uniforms with lights
			comps.shaderManager.updatePerFrame();

			// Wait for preRenderQueue to be finished
			// while waiting, start performing processable tasks, if there are some already waiting for execution
			while (!comps.processableQueue.ifFinished())
			{
				comps.processableQueue.processNext();
			}
		}

		//
		// Render:
		//
		{
			// Render all rendering tasks from processableQueue
			// comps.processableQueue.render();
			render();

			// Dear ImGui: Render
			ImGuiRender();
		}

		//
		// Frame finalize:
		//
		{
			// Swap buffers after the rendering is finished
			glfwSwapBuffers(comps.engineWindowPtr);

			// Tracy Profiler: Collect Gpu data
			TracyGpuCollect;

			// Process pending events
			glfwPollEvents();

			// Check for OpenGL rendering errors
			checkOglErrors(__FUNCTION__);

			// Tracy Profiler: End frame mark
			FrameMark;
		}
	}

	// Cleanup processable queue
	comps.processableQueue.cleanup();

	// Dear ImGui: Cleanup
	ImGuiCleanup();

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

bool Engine::verifySetupStatus()
{
	bool readyForExecution = true;

	// Check camera presence
	if (comps.cameraPtr == nullptr)
	{
		readyForExecution = true;
		LOG.ERROR("Engine verification: Uninitialized Camera");
	}


	return readyForExecution;
}

void Engine::render()
{
	// Render all render passes
	for (const auto& renderPass : comps.renderPassList)
	{
		renderPass->preRender();
		renderPass->render();
		renderPass->postRender();
	}
}

void Engine::runPhaseRendererInit()
{
	//
	// Initialize Glfw
	//
	comps.engineWindowPtr = initGlfw(
		props.consts.windowWidth,
		props.consts.windowHeight,
		props.consts.windowName,
		true
	);

	// Verify Glfw window
	if (comps.engineWindowPtr == nullptr) {
		glfwTerminate();
		throw InitializationException("Failed to open Glfw window", __FUNCTION__);
	}

	//
	// Initialize Glew
	//
	if (initGlew() == false)
	{
		glfwTerminate();
		throw InitializationException("Failed to initialize Glew", __FUNCTION__);
	}

	//
	// Initialize tracy context
	//
	TracyGpuContext;

	//
	// Set OpenGL Engine's parameters
	//
	setDefaultOglParameters();

	//
	// Set default properties of the Engine
	//
	setDefaultProperties();
}

void Engine::setDefaultOglParameters()
{
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(comps.engineWindowPtr, GLFW_STICKY_KEYS, GL_TRUE);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it is closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Disable Vsync (enabling requires time synchronization)
	glfwSwapInterval(1);

	// Cull triangles which normal is not towards the camera (should be turned on for optimization)
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	// Set default background color
	glClearColor(0.2f, 0.8f, 1.0f, 0.0f);
}

void Engine::setDefaultProperties()
{
	//
	// Set default time properties
	//
	props.time.lastPrintTimestamp = glfwGetTime();
	props.time.lastFrameTimestamp = props.time.lastPrintTimestamp;
	props.time.printInterval = 1.0;
	props.time.processedFramesNumber = 0;
	props.time.lastFrameDelta = 0;
}


void Engine::runPhaseEnginePrep()
{
	// Initialize Engine's components
	initComponents();

	// Dump environment properties after preparation
	dumpEnvironmentSetup();
}

void Engine::initComponents()
{
	// Dear ImGui: Setup context
	ImGuiInit(comps.engineWindowPtr);

	// Set worker threads in preRenderQueue
	comps.processableQueue.initWorkers(2);

	// Init shaderGlobalData
	comps.shaderGlobalData.init();
}

void Engine::dumpEnvironmentSetup()
{
	LOG.INFO("OGL version: %s\n", glGetString(GL_VERSION));

	int work_grp_cnt[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

	LOG.INFO("Max global (total) compute work group counts x:%i y:%i z:%i\n",
		work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);

	int work_grp_size[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);

	LOG.INFO("max local (in one shader) compute work group sizes x:%i y:%i z:%i\n",
		work_grp_size[0], work_grp_size[1], work_grp_size[2]);

	int work_grp_inv;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
	LOG.INFO("max local work group invocations %i\n", work_grp_inv);

	LOG.INFO("max global (total) compute work group counts x:%i y:%i z:%i\n",
		work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);
}
